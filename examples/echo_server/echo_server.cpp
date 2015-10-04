#include <simpleNW.hpp>

#include <iostream>

/* Define our handler class, which gets called whenever data is received by the server.
*/
class EchoingHander : public Handler {

	/* Define what happens when data is received. */
	void OnReceive(ConnectionInfo connectioninfo, char data[], size_t bytes_received) override {

		// Send back the received data.
		connectioninfo.getConnection()->send_nonblocking(data, bytes_received);
	}
};


int main(int argc, char* argv[]) {
	
	{

		ServiceOptions options;
		options.server_port_ = 3000; // Use port 3000 for the server.
		options.threads_ = 4; // Run the data processing on 4 threads.
		options.transport_protocol_ = IPV6::TCP(); // Use IPv6 with the TCP tranport protocol.

		// Define the Service containing the server.
		NetworkService nwservice(options);

		// Define an instance of the handler specified above.
		EchoingHander hand;

		// Let the Network Service use the handler instance and start receiving.
		nwservice.addHandler(&hand);
		nwservice.start();

		std::cout << "Press any key to stop";
		std::cin.get();

		nwservice.stop();

	}

	std::cin.get();
}
