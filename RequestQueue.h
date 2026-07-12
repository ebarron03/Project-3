/**
 * @file RequestQueue.h
 * @author Ernest Barron
 * @brief Declares the RequestQueue class.
 * 
 * Defines methods for managing a queue of web requests.
 */

#ifndef REQUESTQUEUE_H
#define REQUESTQUEUE_H

#include <queue>
#include "Request.h"

/**
 * @class RequestQueue
 * @brief Manages a queue of web requests.
 * 
 * Provides methods to enqueue, dequeue, check if the queue is empty, and get the size of the queue.
 */
class RequestQueue {
    public:

        /** 
         * @brief Adds a request to the queue.
         * @param request The Request object to be added.
         */
        void enqueue(Request request);

        /** 
         * @brief Removes and returns the request at the front of the queue.
         * @return The Request object at the front of the queue.
         */
        Request dequeue();

        /** 
         * @brief Checks if the queue is empty.
         * @return True if the queue is empty, false otherwise.
         */
        bool empty();

        /** 
         * @brief Returns the size of the queue.
         * @return The number of requests in the queue.
         */
        int size();

    private:
        std::queue<Request> requests;

};

#endif
