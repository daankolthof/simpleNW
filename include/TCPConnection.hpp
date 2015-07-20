
#if !defined(_TCPCONNECTION_H)
#define _TCPCONNECTION_H

#include <Connection.hpp>
#include <DynamicArray.hpp>

#include <tuple>
#include <vector>

class TCPConnection : public Connection {

	friend class TCPServer;

public:

	~TCPConnection();

	boost::asio::ip::tcp::socket socket_;
	
	bool is_open();

	void close();

	void send_nonblocking(char data[], size_t bytes_to_send);
	void send_nonblocking_buffer(char data[], size_t bytes_to_send);

	void start_read();
	void handle_read(std::shared_ptr<Connection> connection, const boost::system::error_code& error, size_t bytes_transferred);

	void start_write();
	void handle_write(std::shared_ptr<Connection> connection, DynamicArray<char> arr, const boost::system::error_code& error, size_t bytes_transferred);


protected:

	TCPConnection(boost::asio::io_service& io_service, std::shared_ptr<Server> server_ptr);
	
	void close_socket();
	

private:

};





#endif // _TCPCONNECTION_H
