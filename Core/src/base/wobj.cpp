#include "wobj.h"

void WObj::load(Json::Value& json) {
	(void)json;
}

void WObj::save(Json::Value& json) {
	(void)json;
}

void operator << (WObj& obj, Json::Value& json) {
	obj.load(json);
}

void operator >> (WObj& obj, Json::Value& json) {
	obj.save(json);
}
