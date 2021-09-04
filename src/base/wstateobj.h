#pragma once

#include "wobj.h"

struct WStateObj : WObj {
	enum State {
		Closed, // 0
		Opening, // 1
		Opened, // 2
		Closing // 3
	};

	WStateObj();

	bool active() { return state_ == Opened; }
	bool open();
	bool close();

protected:
	virtual bool doOpen();
	virtual bool doClose();

protected:
	State state_{Closed};
};
