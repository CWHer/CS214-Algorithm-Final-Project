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
    scheduler.sched_type = Scheduler::NETWORK_NECK;
    scheduler.neck_type = Scheduler::SAME_NEXT;
    Simulator sim;

    dag.init(graph);
    scheduler.initGraph(graph);
    sim.updateGraph(graph);
    // sim.printStatus();
    int task_cnt = 0;

    int TASK_THRESHOLD = 4;
    double TIME_THRESHOLD = 0.2;
    double pre_time = 0;
    // read settings from file
    std::ifstream fin;
    fin.open("net_neck_settings.txt");
    if (fin.is_open())
        fin >> TASK_THRESHOLD >> TIME_THRESHOLD;

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

            // if (!sched.empty())
            // {
            //     std::cout << sim.getTime() << std::endl;
            //     for (const auto &it : sched)
            //         std::cout << it.first << ' '
            //                   << it.second.first << ' '
            //                   << it.second.second << '\n';
            //     std::cout << '\n'
            //               << std::endl;
            // }
        }

        // sim.forwardTime();
        sim.tickTime(0.01);

        // std::cout << sim.getTime() << "\n";
        auto finished = sim.getFinished();
        // sim.printStatus();

        // if (!finished.empty())
        // {
        //     for (const auto &it : finished)
        //         std::cout << it.first << ' ';
        //     std::cout << std::endl;
        // }

        task_cnt += finished.size();
        dag.updateDAG(finished);
    }
    std::cout << "NETWORK NECK: " << sim.getTime() << "\n";
    // schedule cause
    std::cout << "TIME: " << TIME_cnt << ' '
              << "TASK: " << TASK_cnt << std::endl;
    graph->printStatistics("network_neck.log");
    graph->printFinishTime("network_neck.txt");
    // graph->printFinishTime();
    graph->printData("network_neck_data.txt");
    std::cout << std::endl;
    return 0;
}
