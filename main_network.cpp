#include "includes/common.hpp"
#include "includes/DAG.hpp"
#include "includes/scheduler.hpp"
#include "includes/simulator.hpp"

int main()
{
    shared_ptr<Graph> graph;
    graph = make_shared<Graph>();
    init_data(graph);
    DAG dag;
    Scheduler scheduler;
    scheduler.sched_type = scheduler.NETWORK;
    Simulator sim;

    dag.init(graph);
    scheduler.initGraph(graph);
    sim.updateGraph(graph);
    // sim.printStatus();
    int task_cnt = 0;

    // 8 tasks or 2 seconds
    const int TASK_THRESHOLD = 10;
    const double TIME_THRESHOLD = 1;
    double pre_time = 0;

    // schedule causes
    int TASK_cnt = 0;
    int TIME_cnt = 0;

    while (!dag.if_finished())
    {

        scheduler.sumbitTasks(dag.getSubmit());

        // schedule when there are sufficient jobs
        //  or after certain time
        if (scheduler.taskSize() >= TASK_THRESHOLD ||
            sim.getTime() - pre_time >= TIME_THRESHOLD)
        {
            if (scheduler.taskSize() >=
                TASK_THRESHOLD)
                TASK_cnt++;
            if (sim.getTime() - pre_time >=
                TIME_THRESHOLD)
                TIME_cnt++;

            pre_time = sim.getTime();
            auto sched = scheduler.getScheduled();
            sim.updateScheduled(sched);
        }

        // sim.forwardTime();
        sim.tickTime(0.1);

        // std::cout << sim.getTime() << "\n";
        auto finished = sim.getFinished();
        // sim.printStatus();

        // for (const auto &it : finished)
        //     std::cout << it.first << ' ';
        // std::cout << std::endl;

        task_cnt += finished.size();
        dag.updateDAG(finished);
    }
    std::cout << "NETWORK: " << sim.getTime() << "\n";
    // schedule cause
    std::cout << "TIME: " << TIME_cnt << ' '
              << "TASK: " << TASK_cnt << std::endl;
    graph->printStatistics("network.log");
    graph->printFinishTime("network.txt");
    graph->printData("network_data.txt");
    std::cout << std::endl;
    return 0;
}
