#include "handler.h"

TaskHandler::TaskHandler(int numThreads) : m_state(THState::NOT_INITIALIZED) {
    m_numThreads = numThreads;
    m_threads.reserve(m_numThreads);
}

#include <iostream>
void TaskHandler::executer() {
    std::unique_lock<std::mutex> lk(m_mutex);
    while (true) {
        m_cv.wait(lk, [this] {
            return m_state == THState::STOPPED || 
                   (m_state == THState::RUNNING && !m_tasks.empty());
        });
        switch (m_state) {
            case THState::RUNNING:
                if (!m_tasks.empty()) {
                    std::shared_ptr<task_t> t = m_tasks.front();
                    m_tasks.pop();
                    lk.unlock();
                    if (t && t->execute) {
                        t->execute();
                    }
                    lk.lock();
                }
                break;
            case THState::STOPPED:
                return;
            case THState::NOT_INITIALIZED:
                break;
        }
    }
}

void TaskHandler::start() {
    for (int i = 0; i < m_numThreads; ++i) {
        m_threads.push_back(std::thread(&TaskHandler::executer, this));
    }
    std::lock_guard<std::mutex> lk(m_mutex);
    m_state = THState::RUNNING;
    m_cv.notify_all();
}

void TaskHandler::stop() {
    std::lock_guard<std::mutex>lk(m_mutex);
    m_state = THState::STOPPED;
    m_cv.notify_all();
}

void TaskHandler::addTask(std::shared_ptr<task_t> task) {
    std::lock_guard<std::mutex> lk(m_mutex);
    m_tasks.push(task);
    m_cv.notify_all();

}

void TaskHandler::wait() {
    for (int i = 0; i < m_threads.size(); ++i) {
        m_threads[i].join();
    }
}
