
#if !defined(_TCPCONNECTIONBASE_H)
#define _TCPCONNECTIONBASE_H

#include <Connection.hpp>
#include <DynamicArray.hpp>

#include <tuple>
#include <vector>

class ConnectionInfo;

class TCPConnectionBase : public Connection {

	friend class TCPServer;

public:

	~TCPConnectionBase();

	void close();

	virtual void send_nonblocking(char data[], size_t bytes_to_send) = 0;
	virtual void send_nonblocking_buffer(char data[], size_t bytes_to_send) = 0;

	virtual void start_read() = 0;
	virtual void handle_read(std::shared_ptr<Connection> connection, const boost::system::error_code& error, size_t bytes_transferred) = 0;

	virtual void start_write() = 0;
	virtual void handle_write(std::shared_ptr<Connection> connection, DynamicArray<char>, const boost::system::error_code& error, size_t bytes_transferred) = 0;

protected:

	TCPConnectionBase(boost::asio::io_service& io_service, std::shared_ptr<Server> server_ptr);
	TCPConnectionBase(TCPConnectionBase&);

	void close_socket();

	ConnectionInfo constructConnectionInfo() override;

	const static int max_buf_length = 1024;
	char data_[max_buf_length];

	/* Used for locking the connection, make sure the connection cannot be closed
	* by another thread while handlers are active */
	std::recursive_mutex connection_mtx_;

private:

};


#endif // _TCPCONNECTIONBASE_H
