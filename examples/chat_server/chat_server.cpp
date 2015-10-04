/* simpleNW chat server example.
 *
 */

#include <simpleNW.hpp>
#include <boost/asio.hpp>

#include <map>
#include <vector>
#include <unordered_set>

std::unordered_set<ConnectionInfo> connections;

class ConnectHandler : public Handler {
	void OnConnectionOpen(ConnectionInfo connectioninfo) override {
		std::cout << "Someone joined chat" << std::endl;
		connections.insert(connectioninfo);

		for (ConnectionInfo con : connections) {
			if (con == connectioninfo) continue;
			con.getConnection()->send_nonblocking("Someone joined chat\n", 21);
		}
	}

	void OnConnectionClose(ConnectionInfo connectioninfo) override {
		std::cout << "Someone left chat" << std::endl;
		connections.erase(connectioninfo);

		for (ConnectionInfo con : connections) {
			if (con == connectioninfo) continue;
			con.getConnection()->send_nonblocking("Someone left chat\n", 19);
		}
	}
};

int main(int argc, char* argv[]) {
	
	ServiceOptions options;
	options.server_port_ = 5000;
	options.threads_ = 4;
	options.transport_protocol_ = IPV6::TCP();

	NetworkService nwservice(options);

	BufferingHandler bufhand(
		[](ConnectionInfo connectioninfo, std::vector<char> buffer) {
			for (int i1 = 0; i1 < buffer.size(); i1++) {
				std::cout << buffer[i1];
			}

			for (ConnectionInfo con : connections) {
				if (con == connectioninfo) continue;
				con.getConnection()->send_nonblocking(buffer.data(), buffer.size());
			}
		}
	);

	ConnectHandler conhand;

	nwservice.addHandler(&bufhand);
	nwservice.addHandler(&conhand);
	
	nwservice.start();

	std::cout << "Server started. Press enter to stop." << std::endl;
	std::cin.get();

	nwservice.stop();
	
	return 0;
}
