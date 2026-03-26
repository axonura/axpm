#include <iostream>
#include <vector>
#include <string>
#include "pkgparser.h"

using namespace std;

// Print package names (can extend later to print more info)
void printPackageInfo(const string& pakg) {
    if (!isPKGExist(pakg)) {
        cout << pakg << " Named Package Is Not Found" << endl << endl;
        return;
    }

    package pkg = getPackageInfo(pakg);

    cout << "Information Of " << pakg << " Package" << endl;
    cout << "Name: " << pkg.name << endl;
    cout << "Version: " << pkg.version << endl;

    cout << "Author: ";
    for (size_t i = 0; i < pkg.author.size(); i++) {
        cout << pkg.author[i];
        if (i != pkg.author.size() - 1)
            cout << ", ";
    }
    cout << endl;

    cout << "Root Access: " << (pkg.requiresRoot ? "Required" : "Optional") << endl;
    cout << "Dependencies: " << endl;
    for(package_dep dependency : pkg.depedencies) {
        cout << "   " << dependency.name << " " << dependency.requiredVersion << endl;
    }
    cout << endl;
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        if (string(argv[1]) == "info") {   // Compare as std::string
            if (argc > 2) {
                for (int i = 2; i < argc; i++) {
                    printPackageInfo(argv[i]);
                }
            } else {
                cout << "No packages specified after 'info'" << endl;
            }
        } else {
            cout << "Unknown command: " << argv[1] << endl;
        }
    } else {
        cout << "Usage: " << argv[0] << " info <package1> <package2> ..." << endl;
    }

    return 0;
}