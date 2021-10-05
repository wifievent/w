#pragma once

#include "stdafx.h"
#include "http.h"

class HTTPRequest
{
    Method method;
    string url;
    Protocol protocol;
    string hostname;
    string useragent;

    vector<pair<string, string>> headers;
    string body;
    string request_packet;

    public:
        HTTPRequest();
        ~HTTPRequest();

        void printRequest();
        void addRequestPacket(char* buf, int& len);
        void addRequestBody(string& str);

        int setMethod(Method arg_method);
        Method getMethod();
        int setURL(string arg_url);
        string getURL();
        int setProtocol(Protocol arg_protocol);
		Protocol getProtocol();
		int setUserAgent(string arg_useragent);
		string getUserAgent();
		int setHTTPHeader(string name, string content);
		string getHTTPHeader(string name);
		int setHTTPHeaderVector(vector<pair<string, string>>* header_vector);
		vector<pair<string, string>>* getHTTPHeaderVector();
        int setRequestBody(string& arg_body);
		string* getRequestBody();

        int parseRequestPacket();
		int makeRequest();
		size_t getRequestSize();
		string* getRequestData();
        string updateCursor(size_t& cursor_begin, size_t& cursor_end, string target, string obj, size_t next);
};