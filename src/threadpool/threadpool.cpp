#include "threadpool/threadpool.h"

namespace threadpool {

void ThreadPool::Start() {
    const uint32_t numThds = std::thread::hardware_concurrency();  // Max # of threads support.
    threads.reserve(numThds);
    for (uint32_t i = 0; i < numThds; ++i) {
        threads.emplace_back(std::thread(&ThreadPool::ThreadLoop, this));
    }
}

void ThreadPool::ThreadLoop() {
    while (true) {
        Job job;
        {
            std::unique_lock<std::mutex> lock(jobs_mutex);
            jobs_condition.wait(lock, [&] { return !jobs.empty() || shouldTerminate; });
            if (shouldTerminate) {
                return;
            }

            job = std::move(jobs.front());
            jobs.pop_front();

            runningThread++;
        }
        job();

        {
            std::unique_lock<std::mutex> lock(jobs_mutex);
            runningThread--;
        }
        finish_condition.notify_one();
    }
}

void ThreadPool::Stop() {
    {
        std::unique_lock<std::mutex> lock(jobs_mutex);
        shouldTerminate = true;
    }
    jobs_condition.notify_all();

    for (auto& thd : threads) {
        thd.join();
    }
    threads.clear();
}

void ThreadPool::EnqueJob(const Job& job, bool dfs) {
    {
        std::unique_lock<std::mutex> lock(jobs_mutex);
        if (dfs) {
            jobs.push_front(job);
        } else {
            jobs.push_back(job);
        }
    }
    jobs_condition.notify_one();
}

void ThreadPool::WaitFinish() {
    {
        std::unique_lock<std::mutex> lock(jobs_mutex);
        finish_condition.wait(lock, [this] { return jobs.empty() && runningThread == 0; });
    }
}

}  // namespace threadpool
