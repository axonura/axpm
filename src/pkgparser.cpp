#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <pkgparser.h>
#include <archive.h>
#include <archive_entry.h>
#include <jsoncpp/json/json.h>

using namespace std;
namespace fs = std::filesystem;
bool isPathOrNot(const std::string& s) {
    try {
        fs::path p(s);

        // If the string contains a directory separator or an extension, consider it a path
        if (p.has_parent_path()) 
            return true;

        if (p.has_extension()) 
            return true;

        // Optional: detect absolute paths
        if (p.is_absolute()) 
            return true;

        return false;
    } catch (...) {
        return false;
    }
}

bool isPKGExist(string pakg) {
    if(isPathOrNot(pakg)) {
        if(fs::exists(pakg))
            return true;
        else
            return false;
    }
    else {
        std::string home = std::getenv("HOME") ? std::getenv("HOME") : "";
        std::string sysPath = "/etc/axpm/" + pakg + "/manifest.json";

        if(fs::exists(sysPath) || fs::exists(home + "/.axroot" + sysPath))
            return true;
        else
            return false;
    }
}

package getPackageInfo(string pakg) {
    struct archive *a;
    struct archive_entry *entry;

    Json::CharReaderBuilder readerBuilder;
    Json::Value root;
    std::string errs;
    std::string buffer;

    package pkg;

    // =============================
    // CASE 1: PACKAGE IS ARCHIVE
    // =============================
    if (isPathOrNot(pakg)) {
        if (!fs::exists(pakg))
            throw runtime_error(pakg + " does not exist.");

        pkg.packagePath = pakg;

        a = archive_read_new();
        archive_read_support_filter_all(a);
        archive_read_support_format_all(a);

        if (archive_read_open_filename(a, pakg.c_str(), 10240) != ARCHIVE_OK) {
            string errMsg = "Failed to open archive: ";
            errMsg += archive_error_string(a);
            archive_read_free(a);
            throw runtime_error(errMsg);
        }

        while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
            string entryPath = archive_entry_pathname(entry);
            size_t pos = entryPath.find("manifest.json");
            if (pos != string::npos && pos + 13 == entryPath.length()) {
                size_t size = archive_entry_size(entry);

                buffer.resize(size);
                archive_read_data(a, buffer.data(), size);
                break;
            }
            archive_read_data_skip(a);
        }

        archive_read_close(a);
        archive_read_free(a);

        if (buffer.empty()) {
            throw runtime_error("manifest.json not found in archive.");
        }
    }

    // =============================
    // CASE 2: INSTALLED PACKAGE
    // =============================
    else {
        std::string home = std::getenv("HOME") ? std::getenv("HOME") : "";
        std::string sysPath = "/etc/axpm/" + pakg + "/manifest.json";

        std::string manifestPath;

        if (fs::exists(home + "/.axroot" + sysPath)) {
            manifestPath = home + "/.axroot" + sysPath;
            pkg.packagePath = home + "/.axroot" + "/etc/axpm/" + pakg;
        } else if (fs::exists(sysPath)) {
            manifestPath = sysPath;
            pkg.packagePath = "/etc/axpm/" + pakg;
        } else {
            throw runtime_error("Package not found: " + pakg);
        }

        ifstream file(manifestPath, ios::in | ios::binary);
        if (!file)
            throw runtime_error("Failed to open " + manifestPath);

        std::ostringstream contents;
        contents << file.rdbuf();
        buffer = contents.str();
    }

    // =============================
    // JSON PARSING
    // =============================
    std::istringstream s(buffer);

    if (!Json::parseFromStream(readerBuilder, s, &root, &errs)) {
        throw runtime_error("JSON parse error: " + errs);
    }

    // =============================
    // FILL STRUCT
    // =============================
    pkg.name = root["name"].asString();
    pkg.packageName = root["package"].asString();
    pkg.version = root["version"].asString();

    if (root["author"].isArray()) {
        for (const auto& author : root["author"]) {
            pkg.author.push_back(author.asString());
        }
    }

    // dependencies
    const Json::Value deps = root["dependencies"];
    for (const auto& key : deps.getMemberNames()) {
        package_dep dep;
        dep.name = key;
        dep.requiredVersion = deps[key].asString();
        pkg.depedencies.push_back(dep);
    }

    return pkg;
}