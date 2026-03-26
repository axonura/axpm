#ifndef PKGPARSER_H
#define PKGPARSER_H

#include <string>
#include <vector>

// Dependency structure
struct package_dep {
    std::string name;
    std::string requiredVersion;
};

// Package structure
struct package {
    std::string name;
    std::string packagePath;
    std::string packageName;
    std::vector<std::string> author;
    std::string version;
    std::vector<package_dep> depedencies; // keep spelling consistent with your code
    bool requiresRoot;
};

extern bool isPathOrNot(const std::string& s);
extern bool isPKGExist(std::string pakg);
extern package getPackageInfo(std::string pakg);

#endif // PKGPARSER_H