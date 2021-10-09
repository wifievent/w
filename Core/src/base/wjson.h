#pragma once

#include "json/json.h"
#include "gtrace.h"

struct WJson {
	static bool loadFromFile(std::string fileName, Json::Value &json);
	static bool saveToFile(std::string fileName, Json::Value &json);
};

struct WObj;
void operator >> (Json::Value& json, WObj& obj);
void operator << (Json::Value& json, WObj& obj);

void operator >> (Json::Value& json, int& i);
void operator << (Json::Value& json, int& i);

void operator >> (Json::Value& json, std::string& s);
void operator << (Json::Value& json, std::string& s);
