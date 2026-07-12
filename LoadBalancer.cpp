/**
 * @file LoadBalancer.cpp
 * @author Ernest Barron
 * @brief Implementation of the LoadBalancer class.
 * 
 * Provides the functionality required to route requests,
 * process cycles, distribute requests among servers,
 * and dynamically adjust the server pool based on load.
 */

#include "LoadBalancer.h"
#include <iostream>
#include <cmath>
#include <algorithm>

LoadBalancer::LoadBalancer(Role role, int numServers, int maxServers)
    :
    role_(role),
    initialNumServers_(numServers),
    maxServers_(maxServers),
    nextServerId_(1),
    processingBalancer_(nullptr),
    streamingBalancer_(nullptr) {
        
        if (role_ != Role::ROUTER) {
            for (int i = 0; i < numServers; ++i) {
                servers_.push_back(new Server(nextServerId_++));
            }
        }
    }

LoadBalancer::~LoadBalancer() {
    for (Server* server : servers_) {
        delete server;
    }

    servers_.clear();
}

void LoadBalancer::setBalancers(LoadBalancer* processing, LoadBalancer* streaming) {
    processingBalancer_ = processing;
    streamingBalancer_ = streaming;
}

void LoadBalancer::addRequest(const Request& request) {
    if (role_ == Role::ROUTER) {

        if (request.getJobType() == 'p') {
            
            if (processingBalancer_) {
                processingBalancer_->addRequest(request);
            }

        } 
        
        else if (request.getJobType() == 's') {

            if (streamingBalancer_) {
                streamingBalancer_->addRequest(request);
            }

        }
    } 
    
    else {
        requestQueue_.enqueue(request);
    }
}

CycleResult LoadBalancer::processCycle() {

    CycleResult result;
    
    if (role_ == Role::ROUTER) {
        return {};
    }

    for (Server* server : servers_) {
        if (server->tick()) {
            result.serverIdCompleted.push_back(std::make_tuple(server->getServerId(), server->getRequest()));
        }
    }

    for (Server* server : servers_) {
        if (!server->isBusy() && !requestQueue_.empty()) {
            server->startReq(requestQueue_.dequeue());
        }
    }

    return result;
}

CycleResult LoadBalancer::adjustServerPool() {

    CycleResult result;

    int currentServers = servers_.size();
    if(currentServers == 0) {
        return result;
    }

    const int TARGET_QUEUE_PER_SERVER = 25;
    const int MAX_CHANGE_PER_CYCLE = 2;

    int desiredServers = static_cast<int>(
        std::ceil(static_cast<double>(requestQueue_.size()) /
        TARGET_QUEUE_PER_SERVER)
    );
    
    desiredServers = std::clamp(
        desiredServers,
        initialNumServers_,
        maxServers_
    );

    if (desiredServers > currentServers) {
        int toAdd = std::min(MAX_CHANGE_PER_CYCLE, desiredServers - currentServers);
        for (int i = 0; i < toAdd; ++i) {
            servers_.push_back(new Server(nextServerId_++));
            result.serversAdded++;
        }
    }
    
    else if (desiredServers < currentServers) {
        int toRemove = std::min(MAX_CHANGE_PER_CYCLE, currentServers - desiredServers);
        for (auto it = servers_.begin(); it != servers_.end() && result.serversRemoved < toRemove; ){
            if (!(*it)->isBusy()) {
                delete *it;
                it = servers_.erase(it);
                result.serversRemoved++;
            } else {
                ++it;
            }
        }
        
    }
    return result;
}

int LoadBalancer::getNumRequests() {
    return requestQueue_.size();
}

int LoadBalancer::getNumServers() {
    return servers_.size();
}


