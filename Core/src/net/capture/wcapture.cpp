#include "wcapture.h"

WPacket::Result WCapture::read(WPacket* packet) {
	(void)packet;
	SET_ERR("virtual function");
	return WPacket::Fail;
}

WPacket::Result WCapture::write(WBuf buf) {
	(void)buf;
	SET_ERR("virtual function");
	return WPacket::Fail;
}

WPacket::Result WCapture::write(WPacket* packet) {
	(void)packet;
	SET_ERR("virtual function");
	return WPacket::Fail;
}

WPacket::Result WCapture::relay(WPacket* packet) {
	(void)packet;
	SET_ERR("virtual function");
	return WPacket::Fail;
}

WPacket::Result WCapture::drop(WPacket* packet) {
	(void)packet;
	SET_ERR("virtual function");
	return WPacket::Fail;
}
