// ----------------------------------------------------------------------------
//
// G Library
//
// http://gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#pragma once

#include "wbase.h"

struct WBuf final {
	byte* data_;
	size_t size_;

	WBuf() {}
	WBuf(byte* data, size_t size) : data_(data), size_(size) {}

	void clear() {
		data_ = nullptr;
		size_ = 0;
	}

	bool valid() {
		return data_ != nullptr;
	}
};
