#pragma once

#include "nlohmann/json.hpp"
#include "common/log.h"

#include <string>
#include <vector>

#include <getopt.h>

__BEGIN_DECLS

namespace config {

// ============================================================================================== //
// OPT DEFINITIONS AND USAGES_HINT
// ============================================================================================== //
#include "optUsage.def"

static const char *optsStr = "p:d:vh::";
static const struct option optsLong[] = {
    {"pattern",  0, NULL, 'p'},
    {"rootpath", 0, NULL, 'r'},
    {"version",  0, NULL, 'v'},
    {"help",     0, NULL, 'h'},
};

struct Config {
    static Config CreateConfig(int argc, char *argv[]) {
        // ====================================================================================== //
        // Parse .ff.conf.json
        // ====================================================================================== //
        std::vector<std::string> skipFiles;

        // ====================================================================================== //
        // Parse runtime args
        // ====================================================================================== //
        std::string pattern;
        std::string rootPath;

        // Case-(1), Option arguments, Ex: ff -h -v
        int c;
        while ((c = getopt_long(argc, argv, optsStr, optsLong, NULL)) != -1) {
            switch (c) {
                case 'p':
                    pattern = std::string(optarg);
                    break;
                case 'r':
                    rootPath = std::string(optarg);
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

        // Case-(2), Regular arguments, Ex: ff <Pattern> <Root>
        if (optind + 2 == argc) {
            pattern  = argv[optind];
            rootPath = argv[optind + 1];
        }
        if (optind + 1 == argc) {
            pattern  = argv[optind];
            rootPath = kCurrentDirPath;
        }

        // ====================================================================================== //
        // Construct config
        // ====================================================================================== //
        Config cfg = {
            .pattern  = std::move(pattern),
            .rootPath = std::move(rootPath),
        };
        return cfg;
    }

    static inline const std::string kCurrentDirPath = "./";

    std::string pattern;   // File pattern that we are going to find.
    std::string rootPath;  // Root path that we start DFS search from.
    std::vector<std::string> skipFiles;  // Skip files patterns.
    bool isRegex;  // If we are using normal substring matching or regex matching.
};

}  // namespace config

__END_DECLS
