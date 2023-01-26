#include "common/log.h"
#include "config/config.h"
#include "filesystem/navigator.h"

#include <iostream>

int main(int argc, char *argv[]) {
    using Config = config::Config;
    using Navigator = filesystem::Navigator;

    // CreateConfig
    auto cfg = Config::CreateConfig(argc, argv);

    // CreateNavigator
    auto navigator = Navigator::Create(cfg);

    // Start DFS search
    navigator->Search();

    std::cout << " Pattern:" << cfg.pattern
              << " RootPath:" << cfg.rootPath
              << std::endl;

    return 0;
}
