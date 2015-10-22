
#include <TCPConnectionBase.hpp>

#include <DynamicArray.hpp>
#include <ConnectionInfo.hpp>
#include <ConnectionRemoteEndpoint.hpp>
#include <TCPConnectionRemoteEndpoint.hpp>

#include <iostream>

TCPConnectionBase::TCPConnectionBase(boost::asio::io_service& io_service, std::shared_ptr<Server> server_ptr)
: Connection(server_ptr)
{
}

TCPConnectionBase::~TCPConnectionBase() {
	/* The destruction will be called when all shared pointers are destroyed.
	 * Which  means this object will not be used again.
	 */
}



void TCPConnectionBase::close() {
	{
		std::unique_lock<std::recursive_mutex> lock(this->connection_mtx_);

		this->close_socket();
	}
}

void TCPConnectionBase::close_socket() {
	this->OnConnectionClose();
	this->this_shared_ptr_.reset();
}

ConnectionInfo TCPConnectionBase::constructConnectionInfo() {

	ConnectionInfo result(this_shared_ptr_);
	return result;
}
