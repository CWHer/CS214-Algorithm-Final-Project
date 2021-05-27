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
    scheduler.sched_type = Scheduler::NETWORK_SUM;
    Simulator sim;

    dag.init(graph);
    scheduler.initGraph(graph);
    sim.updateGraph(graph);
    // sim.printStatus();
    int task_cnt = 0;

    int TASK_THRESHOLD = 4;
    double TIME_THRESHOLD = 0.2;
    // read settings from file
    std::ifstream fin;
    fin.open("net_sum_settings.txt");
    if (fin.is_open())
        fin >> TASK_THRESHOLD >> TIME_THRESHOLD;

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
        sim.tickTime(0.01);

        // std::cout << sim.getTime() << "\n";
        auto finished = sim.getFinished();
        // sim.printStatus();

        // for (const auto &it : finished)
        //     std::cout << it.first << ' ';
        // std::cout << std::endl;

        task_cnt += finished.size();
        dag.updateDAG(finished);
    }
    std::cout << "NETWORK SUM: " << sim.getTime() << "\n";
    // schedule cause
    std::cout << "TIME: " << TIME_cnt << ' '
              << "TASK: " << TASK_cnt << std::endl;
    graph->printStatistics("network_sum.log");
    graph->printFinishTime("network_sum.txt");
    graph->printData("network_sum_data.txt");
    std::cout << std::endl;
    return 0;
}
