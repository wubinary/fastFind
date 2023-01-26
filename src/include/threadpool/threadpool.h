#pragma once

#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <thread>

__BEGIN_DECLS

namespace threadpool {

class ThreadPool {
    using Job = std::function<void()>;

protected:
    void Start();

    void ThreadLoop();

    void EnqueJob(const Job& job, bool dfs = false);

    void WaitFinish();

    void Stop();

private:
    std::vector<std::thread> threads;

    std::deque<Job> jobs;

    std::mutex jobs_mutex;  // Prevent race conditions to jobs que

    std::condition_variable jobs_condition;  // Allows threads to wait on new jobs or terminate

    std::condition_variable finish_condition;  // Allows wait finish to wait all jobs finish

    int runningThread = 0;  // Counts of current threads that are at running state.

    bool shouldTerminate = false;  // Tells thread to stop looking for jobs
};


}  // namespace threadpool

__END_DECLS
