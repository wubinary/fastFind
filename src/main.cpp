#include "common/log.h"

#include <iostream>

#include <getopt.h>

// ============================================================================================== //
// OPT DEFINITIONS AND USAGES_HINT
// ============================================================================================== //
#include "optUsage.def"

static const char *optsStr = "p:d:vh::";
static const struct option optsLong[] = {
    {"pattern", 0, NULL, 'p'},
    {"root",    0, NULL, 'r'},
    {"version", 0, NULL, 'v'},
    {"help",    0, NULL, 'h'},
};



////////////////////////////////////
#include <glob.h>
#include <filesystem>

#include <vector>

int main(int argc, char *argv[]) {
    std::string pattern;
    std::string root;

    // (1) Option arguments
    int c;
    while ((c = getopt_long(argc, argv, optsStr, optsLong, NULL)) != -1) {
        switch (c) {
            case 'p':
                pattern = std::string(optarg);
                break;
            case 'r':
                root = std::string(optarg);
                break;
            case 'v':
                LOG(INFO) << VERSION_TXT;
                exit(EXIT_SUCCESS);
            case 'h':
            case '?':
            default:
                LOG(INFO) << USAGE_TXT;
                exit(EXIT_SUCCESS);
        }
    }

    // (2) Regular arguments
    if (optind + 2 == argc) {
        pattern = argv[optind];
        root    = argv[optind + 1];
    }


    // GetConfig

    // search Config
    std::filesystem::path folder(root);
    if (!std::filesystem::is_directory(folder)) {
        throw std::runtime_error(folder.string() + " is not a folder");
    }

    std::vector<std::string> file_list;

    for (const auto& entry : std::filesystem::directory_iterator(folder)) {
        const auto fullName = entry.path().string();

        std::cout << fullName << std::endl;
        //if (entry.is_regular_file()) {

        //}
    }


    std::cout << " Pattern:" << pattern
              << " Root:"    << root
              << std::endl;

    return 0;
}
