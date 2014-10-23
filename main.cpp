
#include "NetworkManager.hpp";

int main(int argc, char* argv[]) {

	NetworkManager manager;
	ServiceOptions options;
	options.server_port_ = 5000;
	options.starts_immediately_ = false;
	options.threads_ = 1;
	options.transport_protocol_ = IPV4::TCP();

	auto ok = manager.createNetworkService(options);

	ok.start();

	while (true) {

	}

}
