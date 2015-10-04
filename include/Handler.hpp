
#if !defined(_HANDLER_H)
#define _HANDLER_H

#include <ConnectionInfo.hpp>

#include <memory>

class Connection;
class ConnectionInfo;

/** Handler is a class that can be registered to a NetworkService and will have its
 * functions called when things happen on the underlying connection.
 * This class should be inherited and its member functions should be defined in child classes.
 */
class Handler {
public:

	/** Will be called whenever a connection opens.
	 * Will be called whenever a connection is opened in the NetworkService the handler is registered to.
	 */
	virtual void OnConnectionOpen(ConnectionInfo) {}

	/** Will be called whenever a connection closes.
	 * Will be called whenever a connection is closed in the NetworkService the handler is registered to.
	 */
	virtual void OnConnectionClose(ConnectionInfo) {}

	/** Will be called whenever data has been received on the connection.
	 * Will be called whenever data has been received on a connection in the NetworkService the handler is registered to.
	 */
	virtual void OnReceive(ConnectionInfo, char[], size_t) {}

	/** Will be called whenever data has been succesfully send through the connection.
	 * Will be called whenver data has been succesfully send through the connection in the NetworkService the handler is registered to.
	 */
	virtual void OnSend(ConnectionInfo, char[], size_t, size_t) {}
};

#endif  //_HANDLER_H
