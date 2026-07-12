/**
 * @file main.cpp
 * @author Ernest Barron
 * @brief Driver program for the Load Balancer simulation.
 * 
 * The program initializes the router, processing, and streaming
 * load balancers, generates randomized wen requests, executes 
 * the simulation for a user-defined number of clock cycles, and records simulation
 * statistics in a log file.
 */

#include <iostream>
#include <fstream> 
#include <cstdlib>
#include <ctime>
#include <vector>
#include <tuple>

#include "LoadBalancer.h"
#include "Request.h"

const int MIN_TASK_TIME = 10;
const int MAX_TASK_TIME = 60;

/**
 * @brief Generates a randomized networmk request.
 * 
 * Creats a random source and destination IP address,
 * assigns a random processing time, and randomly selects 
 * either a processing or streaming job type.
 * 
 * @return Randomly generated Request object.
 */
Request generateRandomRequest() {

    std::string ipIn = 
        std::to_string(rand() % 256) + "." +
        std::to_string(rand() % 256) + "." +
        std::to_string(rand() % 256) + "." +
        std::to_string(rand() % 256);

    std::string ipOut = "10.0.0." + std::to_string(rand() % 256);



    int processingTime = rand() % (MAX_TASK_TIME - MIN_TASK_TIME + 1) + MIN_TASK_TIME;

    char type = (rand() % 2 == 0) ? 'p' : 's';

    return Request(ipIn, ipOut, processingTime, type);
}

/**
 * @brief Executes the load balancer simulation.
 *
 * Initializes the simulation environment, creates the
 * initial request workload, runs the simulation clock,
 * dynamically manages server resources, records system
 * statistics, and writes a complete simulation log.
 *
 * @return Returns 0 upon successful completion.
 */
int main() {
    srand(time(nullptr));

    int numServers;
    int runtime;

    std::cout << "Initial Servers: ";
    std::cin >> numServers;
    std::cout << "Simulation Cycles: ";
    std::cin >> runtime;


    int maxServers = numServers * 3;

    std::ofstream log("simulation_log.txt");

    LoadBalancer router(Role::ROUTER, 0, 0);
    LoadBalancer processing(Role::PROCESSING, numServers, maxServers);
    LoadBalancer streaming(Role::STREAMING, numServers, maxServers);

    router.setBalancers(&processing, &streaming);

    int startingLoad = numServers * 100;
    int processing_generated = 0;
    int streaming_generated = 0;

    for (int i = 0; i < startingLoad; i++) {
        Request req = generateRandomRequest();
        router.addRequest(req);
        if (req.getJobType() == 'p') {
            processing_generated++;
        } else {
            streaming_generated++;
        }
    }


    int startingProcessingQueue = processing.getNumRequests();
    int startingStreamingQueue = streaming.getNumRequests();

    int startingProcessingServers = processing.getNumServers();
    int startingStreamingServers = streaming.getNumServers();

    int processing_added = 0;
    int processing_removed = 0;
    int streaming_added = 0;
    int streaming_removed = 0;

    log << "===============================\n";
    log << " Load Balancer Simulation Log\n";
    log << "===============================\n\n";

    log << "Simulation Lenght: " << runtime << " cycles\n";
    
    log << "Task Time Range: " << MIN_TASK_TIME << " - " << MAX_TASK_TIME << " cycles\n";

    log << "Starting Queue Sizes:\n";
    log << "Processing: " << startingProcessingQueue << " requests\n";
    log << "Streaming: " << startingStreamingQueue << " requests\n\n";

    log << "Starting Server Counts:\n";
    log << "Processing: " << startingProcessingServers << " servers\n";
    log << "Streaming: " << startingStreamingServers << " servers\n\n";

    for (int cycle = 0; cycle < runtime; cycle++) {

        std::vector<std::tuple<int, Request>> procSuccess = processing.processCycle().serverIdCompleted;
        std::vector<std::tuple<int, Request>> streamSuccess = streaming.processCycle().serverIdCompleted;

        for (const auto& tup : procSuccess) {
            int serverId = std::get<0>(tup);
            const Request& req = std::get<1>(tup);
            log << "[Cycle " << cycle << "] Processing Server " << serverId 
                << " completed request: " << req.getIpIn() << " -> " 
                << req.getIpOut() << ", Processing Time: " 
                << req.getProcessingTime() << " cycles\n";
        }
        for (const auto& tup : streamSuccess) {
            int serverId = std::get<0>(tup);
            const Request& req = std::get<1>(tup);
            log << "[Cycle " << cycle << "] Streaming Server " << serverId 
                << " completed request: " << req.getIpIn() << " -> " 
                << req.getIpOut() << ", Processing Time: " 
                << req.getProcessingTime() << " cycles\n";
        }

        if (rand() % 100 < 90) {

            Request req = generateRandomRequest();
            router.addRequest(req);

            if (req.getJobType() == 'p') {
                processing_generated++;
            } else {
                streaming_generated++;
            }

            log << "[Cycle " << cycle << "] Generated "
                << (req.getJobType() == 'p' ? "Processing\n" : "Streaming\n")
                << " Request: " << req.getIpIn() << " -> " << req.getIpOut()
                << ", Processing Time: " << req.getProcessingTime() << " cycles\n";
        }

        if (cycle % 50 == 0) {
            CycleResult proc = processing.adjustServerPool();
            CycleResult stream = streaming.adjustServerPool();

            if (proc.serversAdded) {
                processing_added += proc.serversAdded;
                log << "[Cycle " << cycle << "] Added " << proc.serversAdded << " Processing Servers.\n";
            }
            if (stream.serversAdded) {
                streaming_added += stream.serversAdded;
                log << "[Cycle " << cycle << "] Added " << stream.serversAdded << " Streaming Servers.\n";
            }
            if (proc.serversRemoved) {
                processing_removed += proc.serversRemoved;
                log << "[Cycle " << cycle << "] Removed " << proc.serversRemoved << " Processing Servers.\n";
            }
            if (stream.serversRemoved) {
                streaming_removed += stream.serversRemoved;
                log << "[Cycle " << cycle << "] Removed " << stream.serversRemoved << " Streaming Servers.\n";
            }
        }

        if (cycle % 100 == 0) {
            log << "\n===============================\n";
            log << " Cycle " << cycle << " Summary\n";
            log << "===============================\n";

            log << "Processing Queue Size: " << processing.getNumRequests() << "\n";
            log << "Streaming Queue Size: " << streaming.getNumRequests() << "\n";

            log << "Processing Server Count: " << processing.getNumServers() << "\n";
            log << "Streaming Server Count: " << streaming.getNumServers() << "\n\n";
        }
    }

    log << "\n===============================\n";
    log << " Simulation Summary\n";
    log << "===============================\n\n";

    log << "Processing Load Balancer\n";
    log << "-------------------------------\n";
    log << "Starting Queue Size: " << startingProcessingQueue << "\n";
    log << "Ending Queue Size: " << processing.getNumRequests() << "\n";
    log << "Starting Server Count: " << startingProcessingServers << "\n";
    log << "Ending Server Count: " << processing.getNumServers() << "\n";
    log << "Servers Added: " << processing_added << "\n";
    log << "Servers Removed: " << processing_removed << "\n";
    log << "--------------------------------\n\n";

    log << "Streaming Load Balancer\n";
    log << "-------------------------------\n";
    log << "Starting Queue Size: " << startingStreamingQueue << "\n";
    log << "Ending Queue Size: " << streaming.getNumRequests() << "\n";
    log << "Starting Server Count: " << startingStreamingServers << "\n";
    log << "Ending Server Count: " << streaming.getNumServers() << "\n";
    log << "Servers Added: " << streaming_added << "\n";
    log << "Servers Removed: " << streaming_removed << "\n";
    log << "--------------------------------\n\n";

    log << "Router Statistics\n";
    log << "-------------------------------\n";
    log << "Processing Requests Routed: " << processing_generated << "\n";
    log << "Streaming Requests Routed: " << streaming_generated << "\n";
    log << "Total Requests Routed: " << processing_generated + streaming_generated << "\n";

    log.close();

    std::cout << "Simulation Completed\n";

    return 0;
}