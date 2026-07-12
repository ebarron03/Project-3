/** 
 * @file LoadBalancer.h
 * @author Ernest Barron
 * @brief Defines the LoadBalancer class used to simulate a web server load balancing system.
 * 
 * This file declares the LoadBalancer class, which is responsible for managing incoming requests 
 * and distributing them to available servers based on their job type. The LoadBalancer can operate 
 * in different roles, including ROUTER, PROCESSING, and STREAMING, and it dynamically adjusts 
 * the number of servers based on the current load.
 */

#ifndef LOADBALANCER_H
#define LOADBALANCER_H


#include <vector>
#include <fstream>

#include "RequestQueue.h"
#include "Server.h"

/** 
 * @enum Role
 * @brief Represents the function of a load balancer instance.
 */
enum class Role { 
    ROUTER,      /** Routes requests to specialized load balancers */
    PROCESSING,  /** Handles processing-intensive tasks */
    STREAMING    /** Manages streaming requests */
};

/**
 * @struct CycleResult
 * @brief Stores the results of a simulation cycle.
 * 
 * Indicates whether servers were added or removed during the cycle or a 
 * server completed processing a request.
 */
struct CycleResult {
    int serversAdded = 0;
    int serversRemoved = 0;
    std::vector<std::tuple<int, Request>> serverIdCompleted;
};

/**
 * @class LoadBalancer
 * @brief Manages incoming requests and distributes them to available servers.
 * 
 * A LoadBalancer maintains a queue of requests and a collection of web servers. It distributes 
 * requests to available servers and dynamically adjusts server capacity based on the queue utilization.
 */
class LoadBalancer {
    
    public:

        /** @brief Constructs a LoadBalancer instance.
         * @param role The role of the load balancer.
         * @param numServers The initial number of servers.
         * @param maxServers The maximum number of servers.
         */
        LoadBalancer(Role role, int numServers, int maxServers);

        /** 
         * @brief Destructs a LoadBalancer instance.
         */
        ~LoadBalancer();

        /**
         * @brief Sets the processing and streaming load balancers.
         * @param processing The processing load balancer.
         * @param streaming The streaming load balancer.
         */
        void setBalancers (LoadBalancer* processing, LoadBalancer* streaming);
        
        /**
         * @brief Adds a request to the load balancer.
         * @param request The request to add.
         */
        void addRequest(const Request& request);

        /**
         * @brief Processes a simulation cycle.
         * @return The result of the cycle.
         */
        CycleResult processCycle();

        /**
         * @brief Gets the number of servers.
         * @return The number of servers.
         */
        int getNumServers();

        /**
         * @brief Gets the number of requests.
         * @return The number of requests.
         */
        int getNumRequests();

        /**
         * @brief Adjusts the server pool based on the current load.
         * @return The result of the adjustment.
         */
        CycleResult adjustServerPool();

    private:
        
        Role role_;
        int initialNumServers_;
        int maxServers_;
        int nextServerId_;
        RequestQueue requestQueue_;
        std::vector<Server*> servers_;
        

        LoadBalancer* processingBalancer_;
        LoadBalancer* streamingBalancer_;

};

#endif 