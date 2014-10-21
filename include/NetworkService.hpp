
#if !defined(_NETWORKSERVICE_H)
#define _NETWORKSERVICE_H

#include "Server.hpp"
#include "ServiceOptions.hpp"
#include "TransportProtocol.hpp"
#include "Handler.hpp"

#include <mutex>
#include <vector>

class NetworkService {
public:

	NetworkService(ServiceOptions options);

    void addHandler(Handler* handler);
    void start();
    void stop();

protected:

private:

	std::vector<Handler*> handlers_;
	Server underlying_server_;

	std::mutex handler_mtx_;

	bool started_server_ = false;

};

#endif  //_NETWORKSERVICE_H
