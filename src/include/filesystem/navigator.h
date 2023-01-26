#pragma once

#include "config/config.h"
#include "threadpool/threadpool.h"

#include <deque>
#include <filesystem>
#include <mutex>
#include <string>
#include <vector>

#include <glob.h>

__BEGIN_DECLS

namespace filesystem {

/*
 * @Briefly : Navigator is a class that search candid files for a given search config.
 */
class Navigator final : public threadpool::ThreadPool {
    using Config = config::Config;
    using Path = std::string;
    using PathVec = std::vector<Path>;
    using PathMatch = std::tuple<Path, int, int>;

public:
    static std::unique_ptr<Navigator> Create(const Config& cfg) {
        std::unique_ptr<Navigator> navigator(new Navigator(cfg));
        return std::move(navigator);
    }

private:
    Navigator() = default;
    Navigator(const Config& cfg_) : cfg(cfg_) {};

public:
    void Search();

private:
    void SearchFolder(const Path& pathStr);

    void ShowResult();

    void InsertFolders(const PathVec& newFolders);

    void InsertResults(const PathVec& newResults);

    const PathVec& GetFolders();

    const PathVec& GetFolders(int nums);

    const PathVec& GetResults();

    const PathVec& GetResults(int nums);

private:
    const Config cfg;

#ifndef DEQUE_MUTEX_CONDVAR
#define DEQUE_MUTEX_CONDVAR(type, var) \
    std::deque<type> var; \
    std::mutex var##_mutex; \
    std::condition_variable var##_mutex_condition;
#endif

    DEQUE_MUTEX_CONDVAR(PathMatch, results);

    std::mutex show_mutex;  // Prevent cout results out-of-order.
};


}  // namespace filesystem

__END_DECLS
