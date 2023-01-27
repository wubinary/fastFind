#include "common/log.h"
#include "common/utils.h"
#include "config/config.h"
#include "filesystem/navigator.h"

#include <iostream>

int main(int argc, char *argv[]) {
    using Config = config::Config;
    using Navigator = filesystem::Navigator;

    { SCOPED_TIMER()

    // CreateConfig
    auto cfg = Config::CreateConfig(argc, argv);

    // CreateNavigator
    auto navigator = Navigator::Create(cfg);

    // Start DFS search
    navigator->Search();

    } SCOPED_TIMER_SUMMARY();

    return 0;
}
