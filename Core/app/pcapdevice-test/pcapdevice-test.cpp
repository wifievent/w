#include <iostream>
#include <thread>

#include "net/capture/wpcapdevice.h"
#include "net/process/wpacketdbg.h"
#include "base/wjson.h"

using namespace std;

bool active = true;
void captureAndDebug(WPcapDevice* pcapDevice) {
	WPacket packet;
	WPacketDbg packetDbg;
	while (active) {
		WPacket::Result res = pcapDevice->read(&packet);
		if (res == WPacket::Fail || res == WPacket::Eof) break;
		if (res == WPacket::None) continue;
		packetDbg.debug(&packet);
	}
}

int main() {
	WPcapDevice pcapDevice;
	Json::Value jv;
	if (WJson::loadFromFile("pcapdevice-test.json", jv)) {
		pcapDevice << jv["pcapDevice"];
	}
	pcapDevice >> jv["pcapDevice"];
	WJson::saveToFile("pcapdevice-test.json", jv);

	if (!pcapDevice.open()) {
		std::cout << pcapDevice.err_;
		return -1;
	}

	std::thread t(&captureAndDebug, &pcapDevice);

	std::cout << "Enter any key to close" << std::endl;
	std::string s;
	std::cin >> s;

	active = false;
	t.join();
	pcapDevice.close();

}
