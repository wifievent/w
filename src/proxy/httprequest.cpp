#include "httprequest.h"

HTTPRequest::HTTPRequest()
{
    body = "";
    request_packet = "";
}

HTTPRequest::~HTTPRequest()
{
}

void HTTPRequest::printRequest(void )
{
	cout << "     ===Request Begin===" << endl;
    cout << request_packet << endl;
    cout << "     ====Request End====" << endl;
}

void HTTPRequest::addRequestPacket(char* buf, int& len)
{
	request_packet.append(buf, len);
}

void HTTPRequest::addRequestBody(string& str)
{
	body += str;
}

//set, get func begin

int HTTPRequest::setMethod(Method arg_method)
{
	method = arg_method;
	return 0;
}

Method HTTPRequest::getMethod()
{
	return method;
}

int HTTPRequest::setURL(string arg_url)
{
    url = arg_url;
	return 0;
}

string HTTPRequest::getURL()
{
	return url;
}

int HTTPRequest::setProtocol(Protocol arg_protocol)
{
	protocol = arg_protocol;
	return 0;
}

Protocol HTTPRequest::getProtocol()
{
	return protocol;
}

int HTTPRequest::setUserAgent(string arg_useragent)
{
	useragent = arg_useragent;
	return 0;
}

string HTTPRequest::getUserAgent()
{
	return useragent;
}

int HTTPRequest::setHTTPHeader(string name, string content)
{
	headers.push_back(make_pair(name, content));
	return 0;
}

string HTTPRequest::getHTTPHeader(string name)
{
    vector<pair<string, string>>::iterator iter;
	for(iter = headers.begin(); iter != headers.end(); iter++)
    {
		if((*iter).first == name)
        {
			return (*iter).first + ":" + (*iter).second;
		}
	}
	return "There is no header name " + name;
}

int HTTPRequest::setHTTPHeaderVector(vector<pair<string, string>>* header_vector)
{
    vector<pair<string, string>>::iterator iter;
	for(iter = header_vector->begin(); iter != header_vector->end(); iter++)
    {
		setHTTPHeader((*iter).first, (*iter).second);
	}
	return 0;
}

vector<pair<string, string>>* HTTPRequest::getHTTPHeaderVector()
{
	return &headers;
}

int HTTPRequest::setRequestBody(string& arg_body)
{
	body = arg_body;
	return 0;
}

string* HTTPRequest::getRequestBody()
{
	return &body;
}

//set, get func end

int HTTPRequest::parseRequestPacket()
{
    size_t cursor_begin = 0, cursor_end = 0;
	size_t header_cursor_begin, header_cursor_end;
	string http_method, http_protocol, header;
	string name, content;
	
	//http method
	http_method = updateCursor(cursor_begin, cursor_end, " ", request_packet, 1);
	if(http_method == "GET")
    {
		method = GET;
	}
    else if(http_method == "PUT")
    {
		method = PUT;
	}
    else if(http_method == "POST")
    {
		method = POST;
	}
    else
    {
		method = NOT_IMPLEMENTED;
		return 0;
	}

	//url
	url = updateCursor(cursor_begin, cursor_end, " ", request_packet, 1);

	//protocol
	http_protocol = updateCursor(cursor_begin, cursor_end, CRLF, request_packet, 1);
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

	cursor_begin++; //CRLF

	//header parse start here 
	while(1)
    {
		header = updateCursor(cursor_begin, cursor_end, CRLF, request_packet, 1);
		//separate header line by line

		header_cursor_begin = 0;
		header_cursor_end = 0;

		//name: content
		name = updateCursor(header_cursor_begin, header_cursor_end, ":", header, 2);
		content = updateCursor(header_cursor_begin, header_cursor_end, CRLF, header, 0);

		setHTTPHeader(name, content);
		cursor_begin++; //CRLF
		if(request_packet.substr(cursor_begin, 2) == CRLF) //one more CRLF
		{
			break;
		}
	}

	cursor_begin+=2;
	body = request_packet.substr(cursor_begin);
	
	return 0;
}

int HTTPRequest::makeRequest()
{
	string http_method, http_protocol;

	switch(method){
		case GET:
			http_method = "GET";
			break;
		case PUT:
			http_method = "PUT";
			break;
		case POST:
			http_method = "POST";
			break;
		default:
			return -1;
			break;
	}

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

	request_packet += http_method + " " + url + " " + http_protocol + CRLF;
	vector<pair<string, string>>::iterator iter;
	for(iter = headers.begin(); iter != headers.end(); iter++){
		request_packet += (*iter).first + ": " + (*iter).second + CRLF;
	}
	request_packet += CRLF;
	request_packet += body;
	return 0;
}

size_t HTTPRequest::getRequestSize()
{
	return request_packet.length();
}

string* HTTPRequest::getRequestData()
{
	return &request_packet;
}

string HTTPRequest::updateCursor(size_t& cursor_begin, size_t& cursor_end, string target, string obj, size_t next)
{
	string result;
	cursor_end = obj.find_first_of(target, cursor_begin);
	result = obj.substr(cursor_begin, cursor_end - cursor_begin);
	cursor_begin = cursor_end + next;
	return result;
}