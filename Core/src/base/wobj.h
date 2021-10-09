#pragma once

#include "wbase.h"
#include "wjson.h"

struct WObj {
	WObj() {}
	virtual ~WObj() {}

	std::string err_;

	virtual void load(Json::Value& json);
	virtual void save(Json::Value& json);
};

void operator << (WObj& obj, Json::Value& json);
void operator >> (WObj& obj, Json::Value& json);
