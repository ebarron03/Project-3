/**
 * @file RequestQueue.cpp
 * @author Ernest Barron
 * @brief Implements the RequestQueue class.
 * 
 * Defines methods for managing a queue of web requests.
 */
#include "RequestQueue.h"

void RequestQueue::enqueue(Request request) {
    requests.push(request);
}

Request RequestQueue::dequeue() {
    Request request = requests.front();
    requests.pop();
    return request;
}

bool RequestQueue::empty() {
    return requests.empty();
}

int RequestQueue::size() {
    return requests.size();
}