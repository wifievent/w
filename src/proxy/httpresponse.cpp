#include "httpresponse.h"

HTTPResponse::HTTPResponse()
{
    body = "";
    response_packet = "";
}

HTTPResponse::~HTTPResponse()
{
}

void HTTPResponse::printResponse()
{
	cout << "     ===Request Begin===" << endl;
    cout << response_packet << endl;
    cout << "     ====Request End====" << endl;
}

void HTTPResponse::addResponsePacket(char *buf, int& len)
{
	response_packet.append(buf, len);
}

int HTTPResponse::setProtocol(Protocol arg_protocol)
{
	protocol = arg_protocol;
	return 0;
}

Protocol HTTPResponse::getProtocol()
{
	return protocol;
}

int HTTPResponse::setStatusCode(size_t arg_status_code)
{
	status_code = arg_status_code;
	return 0;
}

size_t HTTPResponse::getStatusCode()
{
	return status_code;
}

int HTTPResponse::setReasonPhrase()
{
	switch(status_code)
    {
		case 200:
			reason_phrase = "OK";
			break;
		case 201:
			reason_phrase = "Created";
			break;
		case 400:
			reason_phrase = "Bad Request";
			break;
		case 403:
			reason_phrase = "Forbidden";
			break;
		case 404:
			reason_phrase = "Not Found";
			break;
		case 411:
			reason_phrase = "Length Required";
			break;
		case 500:
			reason_phrase = "Internal Server Error";
			break;
		case 501:
			reason_phrase = "Not Implemented";
			break;
		case 502:
			reason_phrase = "Bad Gateway";
			break;
		case 505:
			reason_phrase = "HTTP Version Not Supported";
			break;
		default:
			return -1;
			break;
	}

	return 0;
}

string HTTPResponse::getReasonPhrase()
{
	return reason_phrase;
}

int HTTPResponse::setHTTPHeader(string name, string content)
{
	headers.push_back(make_pair(name, content));
	return 0;
}

string HTTPResponse::getHTTPHeader(string name)
{
    vector<pair<string, string>>::iterator iter;
	for(iter = headers.begin(); iter != headers.end(); iter++)
    {
		if((*iter).first == name)
        {
			return (*iter).second;
		}
	}
	return "There is no header name " + name;
}

int HTTPResponse::setHTTPHeaderVector(vector<pair<string, string>>* header_vector)
{
    vector<pair<string, string>>::iterator iter;
	for(iter = header_vector->begin(); iter!=header_vector->end(); iter++)
    {
		setHTTPHeader((*iter).first, (*iter).second);
	}
	return 0;
}

vector<pair<string, string>>* HTTPResponse::getHTTPHeaderVector()
{
	return &headers;
}

int HTTPResponse::setResponseBody(string& arg_body)
{
    body = arg_body;
	return 0;
}

string* HTTPResponse::getResponseBody()
{
	return &body;
}

int HTTPResponse::makeResponse()
{
	string http_protocol;
	string tmp_packet;

	switch(protocol){
		case HTTP1_0:
			http_protocol = "HTTP/1.0";
			break;
		case HTTP1_1:
			http_protocol = "HTTP/1.1";
			break;
		default:
			return -1;
			break;
	}

	tmp_packet += http_protocol + " " + to_string(status_code) + " " + reason_phrase + CRLF;

    vector<pair<string, string>>::iterator iter;
	for(iter = headers.begin(); iter != headers.end(); iter++)
    {
		tmp_packet += (*iter).first + ": " + (*iter).second + CRLF;
	}
	tmp_packet += CRLF + body;
	response_packet = tmp_packet;
	return 0;
}

int HTTPResponse::parseResponsePacket()
{
	size_t cursor_begin = 0, cursor_end = 0;
	size_t header_cursor_begin, header_cursor_end;
	string http_protocol, http_status_code, header;
	string name, content;
	
	//protocol
    http_protocol = updateCursor(cursor_begin, cursor_end, " ", response_packet, 1);
	if(http_protocol == "HTTP/1.0")
    {
		protocol = HTTP1_0;
	}
    else if(http_protocol == "HTTP/1.1")
    {
		protocol = HTTP1_1;
	}
    else
    {
		protocol = HTTP_UNSUPPORTED;
		return 0;
	}

	//status code
    http_status_code = updateCursor(cursor_begin, cursor_end, " ", response_packet, 1);
    status_code = atoi(http_status_code.c_str());

	//reason phrase
    reason_phrase = updateCursor(cursor_begin, cursor_end, CRLF, response_packet, 1);
	
    cursor_begin++; //CRLF

	//header parse start here
	while(1)
    {
        header = updateCursor(cursor_begin, cursor_end, CRLF, response_packet, 1);
        //separate header line by line

		header_cursor_begin = 0;
        header_cursor_end = 0;

        //name: content
        name = updateCursor(header_cursor_begin, header_cursor_end, ":", header, 2);
        content = updateCursor(header_cursor_begin, header_cursor_end, CRLF, header, 0);

		setHTTPHeader(name, content);
		cursor_begin++; //CRLF
		if(response_packet.substr(cursor_begin, 2) == CRLF) //one more CRLF
        {
            break;
        }
	}

	cursor_begin+=2;
	body = response_packet.substr(cursor_begin);
	
	return 0;
}

size_t HTTPResponse::getResponseSize(void )
{
	return response_packet.length();
}

string* HTTPResponse::getResponseData()
{
	return &response_packet;
}

string HTTPResponse::updateCursor(size_t& cursor_begin, size_t& cursor_end, string target, string obj, size_t next)
{
	string result;
	cursor_end = obj.find_first_of(target, cursor_begin);
	result = obj.substr(cursor_begin, cursor_end - cursor_begin);
	cursor_begin = cursor_end + next;
	return result;
}