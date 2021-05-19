#include "common.hpp"
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

shared_ptr<Graph> graph;

void init_data();

int main(){
    init_data();
    return 0;
}

void init_data(){
    graph = graph = make_shared<Graph>();
    //
    //initialize run_time
    //
    json job;
    std::ifstream job_file("./ToyData/job_list.json");
    job_file>>job;

    int num_of_jobs = job["job"].size();
    for(int i = 0;i<num_of_jobs;++i){
        int num_of_task = job["job"][i]["task"].size();
        for(int j = 0;j<num_of_task;++j){
            graph->run_time[job["job"][i]["task"][j]["name"]] = job["job"][i]["task"][j]["time"];
        }
    }

    //Debug code

    // for(int i = 0;i<num_of_jobs;++i){
    //     int num_of_task = job["job"][i]["task"].size();
    //     for(int j = 0;j<num_of_task;++j){
    //         std::cout<<job["job"][i]["task"][j]["name"]<<":" <<graph->run_time[job["job"][i]["task"][j]["name"]];
    //         std::cout<<"\n";
    //     }
    // }

    //
    //initialize graph->resources
    //
    json DC;
    std::ifstream DC_file("./ToyData/DC.json");
    DC_file>>DC;
    
    int num_of_dc = DC["DC"].size();
    for(int i = 0;i<num_of_dc;++i){
        int num_of_resource = DC["DC"][i]["data"].size();
        for(int j = 0;j<num_of_resource;++j)
            graph->resource_loc[DC["DC"][i]["data"][j]] = DC["DC"][i]["name"];
    }

    //Debug code

    // for(int i = 0;i<num_of_dc;++i){
    //     int num_of_resource = DC["DC"][i]["data"].size();
    //     for(int j = 0;j<num_of_resource;++j)
    //         std::cout<<DC["DC"][i]["data"][j]<<":"<<graph->resource_loc[DC["DC"][i]["data"][j]]<<'\n';
    // }


    //
    //initialize edges
    //

    json link;
    std::ifstream link_file("./ToyData/link.json");
    link_file >> link;

    for(int i = 0;i<num_of_dc;++i){
        for(int j = 0;j<num_of_dc;++j){
            if(link["link"][i]["bandwidth"][j]==-1)
                graph->edges[link["link"][i]["start"]][link["link"][j]["start"]] = 0xFFFF;
            else 
                graph->edges[link["link"][i]["start"]][link["link"][j]["start"]] = 1/double(link["link"][i]["bandwidth"][j]);
        }
    }

    //Floyd
    for(int i = 0;i<num_of_dc;++i){
        for(int j = 0;j<num_of_dc;++j){
            for(int k = 0;k<num_of_dc;++k){
                auto dc_i = DC["DC"][i]["name"];
                auto dc_j = DC["DC"][j]["name"];
                auto dc_k = DC["DC"][k]["name"];
                if(graph->edges[dc_i][dc_j]>graph->edges[dc_i][dc_k]+graph->edges[dc_k][dc_j]){
                    graph->edges[dc_i][dc_j]=graph->edges[dc_i][dc_k]+graph->edges[dc_k][dc_j];
                }
            }
        }
    }

    //Debug code

    // for(int i = 0;i<num_of_dc;++i){
    //     for(int j = 0;j<num_of_dc;++j){
    //          std::cout<<int(1/graph->edges[DC["DC"][i]["name"]][DC["DC"][j]["name"]])<<"\t";
    //     }
    //     std::cout<<std::endl;
    // }

    //
    //initialize slots
    //
    for(int i = 0;i<num_of_dc;++i){
        graph->slots[DC["DC"][i]["name"]].first = DC["DC"][i]["size"];
    }

    //Debug code
    // for(int i = 0;i<num_of_dc;++i){
    //     std::cout<<graph->slots[DC["DC"][i]["name"]].first<<"\t";
    // }
    
}