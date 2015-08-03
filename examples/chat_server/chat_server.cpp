/* simpleNW chat server example.
 *
 */

#include <simpleNW.hpp>
#include <boost/asio.hpp>

#include <map>
#include <vector>

enum class messageType : unsigned char {
	HELLO_MSG,
	CHAT_MSG,
	LEAVE_MSG,
	ERROR_MSG
};

std::map<ConnectionInfo, std::vector<char>> client_messages;
bool last_info_set = false;
ConnectionInfo last_info;

class MessageHandler : public Handler {
	void OnReceive(std::shared_ptr<Connection> connection, char data[], size_t bytes_received) override {

		if (last_info_set) {
			std::cout << "ConnectionInfo equals: " << (last_info == connection->getConnectionInfo()) << std::endl;
		}

		last_info = connection->getConnectionInfo();
		last_info_set = true;


		for (int i1 = 0; i1 < bytes_received; i1++) {
			if(data[i1] == '\n') {
				// End of command receive, handle command.
				std::vector<char> command = client_messages[connection->getConnectionInfo()];
				for (int i1 = 0; i1 < command.size(); i1++) {
					std::cout << command[i1];
				}

				// Clear the message vector.
				client_messages[connection->getConnectionInfo()].clear();

			}
			else {
				// Push back character to the current command.
				client_messages[connection->getConnectionInfo()].push_back(data[i1]);
			}
		}

	}
};

int main(int argc, char* argv[]) {
	
	ServiceOptions options;
	options.server_port_ = 3000;
	options.threads_ = 4;
	options.transport_protocol_ = IPV6::TCP();

	NetworkService nwservice(options);

	MessageHandler hand;

	nwservice.addHandler(&hand);
	
	nwservice.start();

	std::cout << "Server started. Press enter to stop." << std::endl;
	std::cin.get();

	nwservice.stop();
	
	return 0;
}