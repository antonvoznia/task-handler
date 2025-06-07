#include "handler/handler.h"

#include <iostream>
#include <format>

constexpr int ts = 4;

void task_f(int job_id) {
    std::string prnt = std::format("TID: {}, Executing job {}\n",
        (std::hash<std::thread::id>{}(std::this_thread::get_id()))%ts, job_id);
    std::cout << prnt;
}

int main(int argc, char **argv) {
    TaskHandler thandler(ts);
    constexpr int numJobs = 100;
    for (int i = 0; i < numJobs; ++i) {
        std::shared_ptr<task_t> job = std::make_shared<task_t>();
        job->id = i;
        job->name = "task_f()";
        job->execute = [i] { task_f(i); };
        thandler.addTask(job);
    }
    thandler.start();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    thandler.stop();
    thandler.wait();

    return 0;
}