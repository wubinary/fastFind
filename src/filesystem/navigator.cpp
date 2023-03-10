#include "common/utils.h"
#include "filesystem/navigator.h"

#include <string_view>

namespace filesystem {

namespace fs = std::filesystem;

void Navigator::Search() {
    const auto IsDirectory = [&](const auto& rootPath) {
        try {
            if (fs::is_directory(rootPath)) {
                return true;
            }
        } catch (fs::filesystem_error& e) {
            LOG(WARN) << "SKIPPED (filesystem_error) : " << rootPath << std::endl;
        }
        return false;
    };

    // ****************************************************************************************** //
    if (UNLIKELY(!IsDirectory(cfg.rootPath))) {
        LOG(WARN) << "given path is not a directory : " << cfg.rootPath << std::endl;
        return;
    }

    // Init thread pool
    ThreadPool::Start();

    // Enque root path as first job, DFS search and enque new jobs.
    ThreadPool::EnqueJob(std::bind(&Navigator::SearchFolder, this, cfg.rootPath));

    // Wait jobs finish
    ThreadPool::WaitFinish();

    // Destroy thread pool
    ThreadPool::Stop();
}

void Navigator::SearchFolder(const Path& pathStr) {
    const auto PushMatchesToResults = [&](const auto& matches) {
        {
            std::unique_lock<std::mutex> lock(results_mutex);
            for (const auto& match : matches) {
                results.emplace_back(match);
            }
        }
    };

    const auto IsAccessable = [&](const auto& path) {
        try {
            if (fs::exists(path) && (fs::is_regular_file(path) || fs::is_directory(path))) {
                return true;
            }
        } catch (fs::filesystem_error& e) {
            LOG(WARN) << "SKIPPED (filesystem_error) : " << path << std::endl;
        }
        return false;
    };

    const auto IsDirectory = [&](const auto& path) {
        try {
            if (fs::is_directory(path)) {
                return true;
            }
        } catch (fs::filesystem_error& e) {
            LOG(WARN) << "SKIPPED (filesystem_error) : " << path << std::endl;
        }
        return false;
    };

    const auto IsMatchSearchPattern = [&](const auto& entryStr, const auto& pattern,
            bool isRegex, auto& left, auto& right) {
        // match pattern = entry.substr[left, right]
        if (isRegex) {
            return true;
        } else {
            auto pos = entryStr.find(pattern);
            left  = pos;
            right = pos + pattern.size() - 1;
            return (pos != std::string::npos);
        }
    };

    // ****************************************************************************************** //
    int left, right;

    fs::path folder(pathStr);

    std::vector<PathMatch> matches;
    for (const auto& entry : fs::directory_iterator(folder,
            fs::directory_options::skip_permission_denied)) {
        const auto& entryStr = entry.path().string();

        // If directory then enque a new search job.
        if (IsDirectory(entry.path())) {
            ThreadPool::EnqueJob(std::bind(&Navigator::SearchFolder, this, entryStr), true/*dfs*/);
        }

        // Append matched entry to matches.
        if (IsMatchSearchPattern(entryStr, cfg.pattern, cfg.isRegex, left, right)) {
            matches.emplace_back(entryStr, left, right);
        }
    }

    // Append matches to results & enque show job.
    PushMatchesToResults(matches);
    ThreadPool::EnqueJob(std::bind(&Navigator::ShowResult, this));
}

void Navigator::ShowResult() {
    std::vector<PathMatch> showResults;
    {
        std::unique_lock<std::mutex> lock(results_mutex);
        showResults.reserve(results.size());
        while (!results.empty()) {
            showResults.emplace_back(std::move(results.front()));
            results.pop_front();
        }
    }

    {
        std::unique_lock<std::mutex> showLock(show_mutex);
        for (const auto&[path, left, right] : showResults) {
            const auto colorized = color::rize(path.substr(left, right-left+1),
                    "Black", "Light Yellow", "Default");

            std::string_view sv(path);
            std::string_view prefix(sv.substr(0, std::max(0, left)));
            std::string_view postfix(sv.substr(right+1, std::max(0, (int)path.size() - right - 1)));

            SHOW() << "- " << prefix << colorized << postfix << std::endl;
        }
        SHOW() << std::flush;
    }
}

}  // namespace filesystem
