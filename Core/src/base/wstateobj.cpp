#include "wstateobj.h"

bool WStateObj::open() {
	if (state_ != Closed) {
		SET_ERR(wformat("state is not Closed(%d)", int(state_)));
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
		SET_ERR(wformat("state is not Opened(%d)", int(state_)));
		return false;
	}

	state_ = Closing;
	doClose();
	state_ = Closed;
	return true;
}

bool WStateObj::doOpen() {
	SET_ERR("virtual function call");
	return false;
}

bool WStateObj::doClose() {
	SET_ERR("virtual function call");
	return false;
}
