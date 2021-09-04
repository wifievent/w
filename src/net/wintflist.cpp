#include "wintflist.h"
#include <pcap.h>

WIntfList::WIntfList() {
	//
	// Initialize allDevs using pcap API.
	//
	pcap_if_t* allDevs;
	char errBuf[PCAP_ERRBUF_SIZE];
	int i = pcap_findalldevs(&allDevs, errBuf);
	if (i != 0) { // if error occured
		GTRACE("error in pcap_findalldevs_ex (%s)", errBuf);
		return;
	}

	//
	// Add all interfaces
	//
	pcap_if_t* dev = allDevs;
	i = 1;
	while (dev != nullptr) {
		WIntf intf;

		intf.index_ = i;
		intf.name_ = dev->name;
		intf.desc_ = dev->description != nullptr ? dev->description : intf.name_;
		if (intf.desc_ == "") intf.desc_ = intf.name_;

#ifdef WOS_LINUX
		intf.mac_ = getMac(dev->name);
		for(pcap_addr_t* pa = dev->addresses; pa != nullptr; pa = pa->next) {
			sockaddr* addr = pa->addr;
			sockaddr_in* addr_in = reinterpret_cast<sockaddr_in*>(addr);
			if(addr != nullptr && addr->sa_family == AF_INET)
				intf.ip_ = ntohl(addr_in->sin_addr.s_addr);

			addr = pa->netmask;
			addr_in = reinterpret_cast<sockaddr_in*>(addr);
			if(addr != nullptr && addr->sa_family == AF_INET) {
				intf.mask_ = ntohl(addr_in->sin_addr.s_addr);
			}
		}
#endif // WOS_LINUX
#ifdef WOS_WIN
		// TODO set (mac_, ip_, mask_) member variables
#endif // WOS_WIN
#ifdef WOS_MAC
		// TODO set (mac_, ip_, mask_) member variables
#endif // WOS_MAC
		intf.ip_and_mask_ = intf.ip_ & intf.mask_;
		// gateway_ is initialized later
		push_back(intf);
		dev = dev->next;
		i++;
	}
	pcap_freealldevs(allDevs);
}

#ifdef WOS_LINUX
WIntf* WIntfList::findByName(std::string name) {
	for (WIntf& intf: *this) {
		if (intf.name() == name)
			return &intf;
	}
	return nullptr;
}
#endif // WOS_LINUX

#ifdef WOS_WIN
WIntf* WIntfList::findByName(std::string name) {
	for (WIntf& intf: *this) {
		if (intf.name().find(name.data()) != -1)
			return &intf;
	}
	return nullptr;
}
#endif // WOS_WINv

#ifdef WOS_LINUX
#include <net/if.h> // for ifreq
#include <sys/ioctl.h> // for SIOCGIFHWADDR
#include <unistd.h> // for close
WMac WIntfList::getMac(char* intfName) {
	int s = socket(PF_INET, SOCK_DGRAM, 0);
	if (s == -1) {
		GTRACE("socket return -1 %s", strerror(errno));
		return WMac::nullMac();
	}

	struct ifreq buffer;
	memset(&buffer, 0x00, sizeof(buffer));
	strncpy(buffer.ifr_name, intfName, IFNAMSIZ - 1);

	int i = ioctl(s, SIOCGIFHWADDR, &buffer);
	::close(s);
	if (i == -1)
		return WMac::nullMac();

	const u_char* p = const_cast<const u_char*>(reinterpret_cast<u_char*>(buffer.ifr_ifru.ifru_hwaddr.sa_data));
	WMac res(p);
	return res;
}
#endif // WOS_LINUX

#ifdef WOS_WIN
// TODO implenent getMac function
#endif // WOS_WIN

#ifdef WOS_MAC
// TODO implenent getMac function
#endif // WOS_MAC
