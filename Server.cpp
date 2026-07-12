/**
 * @file Server.cpp
 * @author Ernest Barron
 * @brief Implementation of the Server class.
 * 
 * This file contains the definitions of the member functions 
 * of the Server class, which manages the processing of requests 
 * in a web server context.
 */

#include "Server.h"

Server::Server(int serverId) : serverId_(serverId), timeToProcess_(0), busy_(false) {}

void Server::startReq(const Request& req) {
    currRequest_ = req;
    timeToProcess_ = req.getProcessingTime();
    busy_ = true;
}

bool Server::tick() {
    if (busy_) {
        timeToProcess_--;
        if (timeToProcess_ <= 0) {
            busy_ = false;
        }
    }
    return !busy_;
}

int Server::getServerId() const {
    return serverId_;
}

bool Server::isBusy() const {
    return busy_;
}

const Request& Server::getRequest() const {
    return currRequest_;
}