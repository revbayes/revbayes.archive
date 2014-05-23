/* 
 * File:   WebClient.h
 * Author: johadunf
 *
 * Created on May 20, 2014, 10:49 PM
 */

#ifndef HTTPCLIENT_H
#define	HTTPCLIENT_H

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <vector>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class HttpRequest {
public:

    HttpRequest() {
        method = "GET";
        path = "";
        host = "";
        accept = "*/*";
        protocol = "http";
    }

    std::string method;
    std::string path;
    std::string host;
    std::string accept;
    std::string protocol; 

};

class HttpResponse {
public:

    int code;
    std::string body;
    std::string header;
    std::string exception;
    std::vector<std::string> data;
};

class HttpClient {
public:

    static HttpResponse makeRequest(HttpRequest httpRequest) {
        HttpResponse httpResponse;

        try {

            boost::asio::io_service io_service;

            // Get a list of endpoints corresponding to the server name.
            tcp::resolver resolver(io_service);
            tcp::resolver::query query(httpRequest.host, httpRequest.protocol);
            tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

            // Try each endpoint until we successfully establish a connection.
            tcp::socket socket(io_service);
            boost::asio::connect(socket, endpoint_iterator);

            // Form the request. We specify the "Connection: close" header so that the
            // server will close the socket after transmitting the response. This will
            // allow us to treat all data up until the EOF as the content.
            boost::asio::streambuf request;
            std::ostream request_stream(&request);

            request_stream << httpRequest.method << " " << httpRequest.path << " HTTP/1.0\r\n";
            request_stream << "Host: " << httpRequest.host << "\r\n";
            request_stream << "Accept: " << httpRequest.accept << "\r\n";
            //request_stream << "User-Agent: curl/7.21.4 (universal-apple-darwin11.0) libcurl/7.21.4 OpenSSL/0.9.8y" << "\r\n"; // some servers are picky...
            request_stream << "Connection: close\r\n";
            request_stream << "\r\n";

            // Send the request.
            boost::asio::write(socket, request);

            // Read the response status line. The response streambuf will automatically
            // grow to accommodate the entire line. The growth may be limited by passing
            // a maximum size to the streambuf constructor.
            boost::system::error_code error;
            boost::asio::streambuf response;
            boost::asio::read_until(socket, response, "\r\n");

            // Check that response is OK.
            std::istream response_stream(&response);
            std::string http_version;
            response_stream >> http_version;
            unsigned int status_code;
            response_stream >> status_code;
            std::string status_message;
            std::getline(response_stream, status_message);


            if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
                //std::cout << "Invalid response\n";
            }
            if (status_code != 200) {
                //std::cout << "Response returned with status code " << status_code << "\n";
            }            

            httpResponse.code = status_code;


            // Read the response headers, which are terminated by a blank line.
            boost::asio::read_until(socket, response, "\r\n\r\n");

            // Process the response headers.
            std::string header;
            while (std::getline(response_stream, header) && header != "\r") {
                httpResponse.header.append(header).append("\n");
            }

            std::stringstream body;
            // Write whatever content we already have to output.
            if (response.size() > 0) {
                body << &response;
            }

            // Read until EOF, writing data to output as we go.
            while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error)) {
                body << &response;
            }
            
            httpResponse.body = body.str();            
            body.flush();
            
            if (error != boost::asio::error::eof) {
                throw boost::system::system_error(error);
            }
        } catch (std::exception& e) {            
            httpResponse.exception = e.what();            
        }

        return httpResponse;

    }

};

#endif	/* HTTPCLIENT_H */

