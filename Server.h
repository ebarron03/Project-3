/**
 * @file Server.h
 * @author Ernest Barron
 * @brief Header file for the Server class.
 * 
 * Defines the interface for a web server that can process requests.
 */
#ifndef SERVER_H
#define SERVER_H

#include "Request.h"

/**
 * @class Server
 * @brief Represents a web server that can process requests.
 * 
 * The Server class manages the processing of requests, keeping 
 * track of its busy state and the time required to process each request.
 */
class Server {
    
    public:

        /**
         * @brief Constructs a Server object with a specified server ID.
         * @param serverId Unique identifier for the server.
         */
        Server(int serverId);

        /**
         * @brief Starts processing a new request.
         * @param req The request to be processed.
         */
        void startReq(const Request& req);

        /**
         * @brief Advances the server's processing by one clock cycle.
         * @return True if the server is now idle, false if it is still busy.
         * 
         * Decrements the time remaining to process the current request. 
         * If the processing time reaches zero, the server becomes idle.
         */
        bool tick();

        /**
         * @brief Returns the server's unique identifier.
         * @return Server ID.
         */
        int getServerId() const;

        /**
         * @brief Checks if the server is currently busy processing a request.
         * @return True if the server is busy, false otherwise.
         */
        bool isBusy() const;

        /**
         * @brief Returns the request currently being processed by the server.
         * @return Reference to the current request.
         */
        const Request& getRequest() const;

    private:
        int serverId_;
        Request currRequest_;
        int timeToProcess_;
        bool busy_;
};

#endif