#if !defined(_CONNECTIONREMOTEENDPOINT_H)
#define _CONNECTIONREMOTEENDPOINT_H

class ConnectionRemoteEndpoint {

public:
	virtual bool operator<(ConnectionRemoteEndpoint*) = 0;
	virtual bool operator==(ConnectionRemoteEndpoint*) = 0;
};

#endif
