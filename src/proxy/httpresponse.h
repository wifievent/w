#pragma once

#include "stdafx.h"
#include "http.h"

class HTTPResponse
{
    Protocol protocol;
    size_t status_code;
    string reason_phrase;
    vector<pair<string, string>> headers;
    string body;
    string response_packet;

	public:
		HTTPResponse();
		~HTTPResponse();

		void printResponse();
		void addResponsePacket(char* buf, int& len);
	
		int setProtocol(Protocol arg_protocol);
		Protocol getProtocol();
		int setStatusCode(size_t arg_status_code);
		size_t getStatusCode();
		int setReasonPhrase();
		string getReasonPhrase();
		int setHTTPHeader(string name, string content);
		string getHTTPHeader(string name);
		int setHTTPHeaderVector(vector<pair<string, string>>* header_vector);
		vector<pair<string, string>>* getHTTPHeaderVector();
		int setResponseBody(string& arg_body);
		string* getResponseBody();

		int makeResponse();
		int parseResponsePacket();
		size_t getResponseSize();
		string* getResponseData();
        string updateCursor(size_t& cursor_begin, size_t& cursor_end, string target, string obj, size_t next);
};