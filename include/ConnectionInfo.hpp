#if !defined(_CONNECTIONINFO_H)
#define _CONNECTIONINFO_H

#include <boost/asio.hpp>
#include <ConnectionRemoteEndpoint.hpp>

class ConnectionInfo {

public:

	ConnectionRemoteEndpoint* connectionendpoint;

	bool operator==(const ConnectionInfo& connectioninfo) {
		return connectionendpoint->operator==(connectioninfo.connectionendpoint);
	}
	
};

inline bool operator<(const ConnectionInfo& ci1, const ConnectionInfo& ci2) {
	return ci1.connectionendpoint->operator<(ci2.connectionendpoint);
}

#endif
