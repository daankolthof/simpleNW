
#include "simpleNW.hpp"
#include <memory>


class Echoing_Handler : public Handler {

	void OnConnectionOpen(std::shared_ptr<Connection> connection) {
		std::cout << "Connection openened: " << connection.get() << std::endl;
	}
	void OnConnectionClose(std::shared_ptr<Connection> connection) {
		std::cout << "Connection closed: " << connection.get() << std::endl;
	}
	void OnReceive(std::shared_ptr<Connection> connection, const char data[], size_t bytes_received) {
		std::cout << "Received message from: " << connection.get() << std::endl;
		std::cout << "Message:" << std::endl;

		for (int i1 = 0; i1 < bytes_received; i1++) {
			std::cout << data[i1];
		}

		std::cout << std::endl;

		connection->send_nonblocking(data, bytes_received);

	}
	void OnSend(std::shared_ptr<Connection> connection, const char data[], size_t data_size, size_t bytes_sent) {
		
	}

};

int main(int argc, char* argv[]) {

	{
		ServiceOptions options;
		options.server_port_ = 5000;
		options.threads_ = 1;
		options.transport_protocol_ = IPV4::TCP();

		NetworkService ok(options);

		Echoing_Handler hand;

		ok.addHandler(&hand);

		ok.start();
		
		while (true) {

		}

		std::cin.get();


	}


	std::cin.get();
}
