
#if !defined(_TCPCONNECTION_H)
#define _TCPCONNECTION_H

#include "Connection.hpp"

#include <tuple>
#include <vector>

class TCPConnection : public Connection {

public:

	TCPConnection(boost::asio::io_service& io_service, Server* server_ptr);
	~TCPConnection();

	boost::asio::ip::tcp::socket socket_;
	
	bool is_open();

	void close(std::shared_ptr<Connection> connection);

	void send_nonblocking(std::shared_ptr<Connection> connection, char data[], size_t bytes_to_send);

	void start_read(std::shared_ptr<Connection> connection);
	void handle_read(std::shared_ptr<Connection> connection, const boost::system::error_code& error, size_t bytes_transferred);

	void start_write(std::shared_ptr<Connection> connection);
	void handle_write(std::shared_ptr<Connection> connection, size_t buffervec_index, const boost::system::error_code& error, size_t bytes_transferred);

protected:

	void close_when_owning_mutex(std::shared_ptr<Connection> connection);
	

private:

};

#endif
