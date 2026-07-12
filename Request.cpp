/**
 * @file Request.cpp
 * @author Ernest Barron
 * @brief Implements the Request class.
 *
 * Defines constructors and accessor methods used to
 * represent individual web requests containing source
 * and destination IP addresses, processing time,
 * and request type.
 */

 #include "Request.h"

 
Request::Request() {
    ipIn_ = "";
    ipOut_ = "";
    processingTime_ = 0;
    jobType_ = 'p';
}

Request::Request(std::string ipIn, std::string ipOut, int processingTime, char jobType) {
    ipIn_ = ipIn;
    ipOut_ = ipOut;
    processingTime_ = processingTime;
    jobType_ = jobType;
}

std::string Request::getIpIn() const{
    return ipIn_;
}

std::string Request::getIpOut() const {
    return ipOut_;
}

int Request::getProcessingTime() const {
    return processingTime_;
}

char Request::getJobType() const {
    return jobType_;
}