
#include "simpleNW.hpp"
#include <memory>


class Echoing_Handler : public Handler {

	void OnConnectionOpen(std::shared_ptr<Connection> connection) override {
		//std::cout << "Connection openened: " << connection.get() << std::endl;
	}
	void OnConnectionClose(std::shared_ptr<Connection> connection) override {
		//std::cout << "Connection closed: " << connection.get() << std::endl;
	}
	void OnReceive(std::shared_ptr<Connection> connection, char data[], size_t bytes_received) override {
		connection->send_nonblocking(data, bytes_received);

	}
	void OnSend(std::shared_ptr<Connection> connection, char data[], size_t data_size, size_t bytes_sent) override {
		
	}

};

int main(int argc, char* argv[]) {

	{
		ServiceOptions options;
		options.server_port_ = 5000;
		options.threads_ = 8;
		options.transport_protocol_ = IPV4::TCP();

		NetworkService ok(options);

		Echoing_Handler hand;

		ok.addHandler(&hand);

		ok.start();
		
		while (true) {
			std::this_thread::yield();
		}

		std::cin.get();


	}

	std::cin.get();
}
