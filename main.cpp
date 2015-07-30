
#include <simpleNW.hpp>
#include <iostream>
#include <memory>

#include <boost/asio.hpp>

class Echoing_Handler : public Handler {

	void OnConnectionOpen(std::shared_ptr<Connection> connection) override {
		//std::cout << "Connection openened: " << connection.get() << std::endl;
	}
	void OnConnectionClose(std::shared_ptr<Connection> connection) override {
		//std::cout << "Connection closed: " << connection.get() << std::endl;
	}
	void OnReceive(std::shared_ptr<Connection> connection, char data[], size_t bytes_received) override {
		//connection->send_nonblocking(data, bytes_received);
		std::cout << "Received message: ";
		for (int i1 = 0; i1 < bytes_received; i1++) {
			std::cout << data[i1];
		}
		std::cout << std::endl;

	}
	void OnSend(std::shared_ptr<Connection> connection, char data[], size_t data_size, size_t bytes_sent) override {
		
	}

};

int main(int argc, char* argv[]) {

	{
		ServiceOptions options;
		options.server_port_ = 5000;
		options.threads_ = 8;
		options.transport_protocol_ = IPV6::UDP();

		NetworkService ok(options);

		Echoing_Handler hand;

		ok.addHandler(&hand);

		ok.start();
		
		while (true) {
			boost::asio::io_service io_service;

			boost::asio::ip::udp::socket s(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0));

			boost::asio::ip::udp::resolver resolver(io_service);
			boost::asio::ip::udp::resolver::query query("127.0.0.1", "5000");
			boost::asio::ip::udp::endpoint endpoint = *resolver.resolve(query);
			s.send_to(boost::asio::buffer("Hallo", 5), endpoint);
			std::cout << "Sent message" << std::endl;
			Sleep(2000);
			//std::this_thread::yield();
		}

		std::cin.get();


	}

	std::cin.get();
}
