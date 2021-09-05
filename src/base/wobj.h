#pragma once

#include "wbase.h"

struct JsonValue; // TODO replace with jsoncpp

struct WObj {
	WObj() {}
	virtual ~WObj() {}

	std::string err_;

	virtual void load(JsonValue& json);
	virtual void save(JsonValue& json);
};
