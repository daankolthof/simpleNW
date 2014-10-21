
#include "NetworkManager.hpp"

#include <iterator>

NetworkService NetworkManager::createNetworkService(ServiceOptions service_options) {
	return NetworkService(service_options);
}
