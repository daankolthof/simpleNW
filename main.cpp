
#include "simpleNW.hpp"
#include <memory>


class Echoing_Handler : public Handler {

	void OnConnectionOpen(std::shared_ptr<Connection> connection) {

	}
	void OnConnectionClose(std::shared_ptr<Connection> connection) {

	}
	void OnReceive(std::shared_ptr<Connection> connection, const char data[], size_t bytes_received) {
		if (!connection->is_open())
			return;
	}
	void OnSend(std::shared_ptr<Connection> connection, const char data[], size_t data_size, size_t bytes_sent) = 0;

};

int main(int argc, char* argv[]) {

	{
		ServiceOptions options;
		options.server_port_ = 5000;
		options.threads_ = 1;
		options.transport_protocol_ = IPV4::TCP();

		NetworkService ok(options);

		ok.start();
		
		while (true) {

		}

		std::cin.get();


	}


	std::cin.get();
}
