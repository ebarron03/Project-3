/**
 * @file Request.h
 * @author Ernest Barron
 * @brief Declares the Request class.
 * 
 * Defines constructors and accessor methods used to
 * represent individual web requests containing source
 * and destination IP addresses, processing time,
 * and request type.
 */

#ifndef REQUEST_H
#define REQUEST_H

#include <string>

/**
 * @class Request
 * @brief Represents a web request.
 * 
 * The Request class is used to represent a web request containing
 * source and destination IP addresses, processing time, and request type.
 */
class Request {

    public:

        /** 
         * @brief Constructs a default Request object. 
         */
        Request();

        /** 
         * @brief Constructs a Request object with specified parameters.
         * 
         * @param ipIn Source IP address.
         * @param ipOut Destination IP address.
         * @param processingTime Number of clock cycles required.
         * @param jobType Request type ('p' or 's').
         */
        Request(std::string ipIn, std::string ipOut, int processingTime, char jobType);

        /** 
         * @brief Returns the source IP address.
         * @return Source IP.
         */
        std::string getIpIn() const;

        /** 
         * @brief Returns the destination IP address.
         * @return Destination IP.
         */
        std::string getIpOut() const;

        /** 
         * @brief Returns the processing time.
         * @return Processing time.
         */
        int getProcessingTime() const;

        /** 
         * @brief Returns the request type.
         * @return Request type.
         */
        char getJobType() const;

    private:
        std::string ipIn_;
        std::string ipOut_;
        int processingTime_;
        char jobType_;

};

#endif