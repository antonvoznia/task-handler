#ifndef _HANDLER_H
#define _HANDLER_H

#include <thread>
#include <queue>
#include <functional>
#include <memory>
#include <mutex>
#include <condition_variable>

struct task_t {
    int id;
    std::string name;
    std::function<void()> execute;
};

class TaskHandler {
public:
    void executer();
    TaskHandler() = delete;
    TaskHandler(int numThreads);

    void stop();
    void start();
    void addTask(std::shared_ptr<task_t> task);
    void wait();
    enum class THState {
        RUNNING,
        STOPPED,
        NOT_INITIALIZED
    };
private:
    int m_numThreads;
    std::queue<std::shared_ptr<task_t>> m_tasks;
    std::vector<std::thread> m_threads;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    THState m_state = THState::NOT_INITIALIZED;
};

#endif