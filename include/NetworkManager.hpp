
#if !defined(_NETWORKMANAGER_H)
#define _NETWORKMANAGER_H

#include "NetworkService.hpp"
#include "ServiceOptions.hpp"

class NetworkManager {
public:
    NetworkService createNetworkService(ServiceOptions service_options);

};

#endif  //_NETWORKMANAGER_H
