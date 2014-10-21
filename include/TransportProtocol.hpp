
#if !defined(_TRANSPORTPROTOCOL_H)
#define _TRANSPORTPROTOCOL_H

#include <cstdint>

class TransportProtocol {
public:

    enum transpprtcl {
		default,
		ipv4_default,
		ipv4_tcp,
		ipv4_udp,
		ipv6_default,
		ipv6_tcp,
		ipv6_udp,
    };

	transpprtcl protocol_ = default;

	TransportProtocol(transpprtcl protocol);	
    
};

namespace IPV4 {

	TransportProtocol DEFAULT();
	TransportProtocol TCP();
	TransportProtocol UDP();

}

namespace IPV6 {

	TransportProtocol DEFAULT();
	TransportProtocol TCP();
	TransportProtocol UDP();

}

#endif  //_TRANSPORTPROTOCOL_H
