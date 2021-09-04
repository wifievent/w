#include "wstateobj.h"

WStateObj::WStateObj() {}

bool WStateObj::open() {
	if (state_ != Closed) {
		GTRACE("state is not Closed(%d)", int(state_));
		return false;
	}
	err_.clear();

	state_ = Opening;
	bool res = doOpen();
	if (!res) {
		state_ = Closing;
		doClose();
		state_ = Closed;
		return false;
	}

	state_ = Opened;
	return true;
}

bool WStateObj::close() {
	if (state_ == Closed)
		return true;

	if (!active()) {
		GTRACE("state is not Opened(%d)", int(state_));
		return false;
	}

	state_ = Closing;
	doClose();
	state_ = Closed;
	return true;
}

bool WStateObj::doOpen() {
	err_ = "virtual function call";
	GTRACE("%s", err_.data());
	return false;
}

bool WStateObj::doClose() {
	err_ = "virtual function call";
	GTRACE("%s", err_.data());
	return false;
}
