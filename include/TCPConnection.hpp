
#if !defined(_TCPCONNECTION_H)
#define _TCPCONNECTION_H

#include <Connection.hpp>
#include <DynamicArray.hpp>

#include <tuple>
#include <vector>

class ConnectionInfo;

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
	void handle_write(std::shared_ptr<Connection> connection, DynamicArray<char>, const boost::system::error_code& error, size_t bytes_transferred);

protected:

	TCPConnection(boost::asio::io_service& io_service, std::shared_ptr<Server> server_ptr);
	TCPConnection(TCPConnection&);

	void close_socket();

	ConnectionInfo constructConnectionInfo() override;
	
	bool endpoint_less_than(Connection*) const override;
	bool endpoint_equals(Connection*) const override;

private:

	const static int max_buf_length = 1024;
	char data_[max_buf_length];

	/* Used for locking the connection, make sure the connection cannot be closed
	 * by another thread while handlers are active */
	std::recursive_mutex connection_mtx_;

};


#endif // _TCPCONNECTION_H
