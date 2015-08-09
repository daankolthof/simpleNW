#include <BufferingHandler.hpp>

#include <Connection.hpp>
#include <ConnectionInfo.hpp>

#include <iostream>


BufferingHandler::BufferingHandler(std::function<void(std::shared_ptr<Connection>, std::vector<char>)> callback) {
	this->tocall_ = callback;
}

char BufferingHandler::getDelimiter() {
	return this->delimiter_.load();
}

void BufferingHandler::setDelimiter(char delimiter) {
	this->delimiter_.store(delimiter);
}

void BufferingHandler::OnReceive(std::shared_ptr<Connection> connection, char data[], size_t bytes_received) {
	
	std::pair<std::unique_ptr<std::vector<char>>, std::unique_ptr<std::mutex>> vec_mtx_pair;
	
	std::vector<char>* vec;
	std::mutex* vec_mtx;

	/* Lock the buffers_ mutex so the pair can be taken out safely. */
	{
		std::lock_guard<std::mutex> buffers_lock(buffers_mtx);

		std::map<ConnectionInfo, std::pair<std::unique_ptr<std::vector<char>>, std::unique_ptr<std::mutex>>>::iterator it = buffers_.find(connection->getConnectionInfo());

		// Connection has not been seen before.
		if (it == buffers_.end()) {
			// Construct new pair.
			std::unique_ptr<std::mutex> mtx_ptr(new std::mutex);
			std::unique_ptr<std::vector<char>> vec_ptr(new std::vector<char>);
			std::pair<std::unique_ptr<std::vector<char>>, std::unique_ptr<std::mutex>> vec_mtx_pair(std::move(vec_ptr), std::move(mtx_ptr));
			
			buffers_[connection->getConnectionInfo()] = std::move(vec_mtx_pair);

			// Put it in the iterator.
			it = buffers_.find(connection->getConnectionInfo());
		}

		vec = it->second.first.get();
		vec_mtx = it->second.second.get();
		
		// Lock is released.
	}

	/* Lock the individual vector. */
	{
		std::lock_guard<std::mutex> vec_lock(*vec_mtx);

		for (int i1 = 0; i1 < bytes_received; i1++) {

			// Push character in buffer.
			vec->push_back(data[i1]);

			// If it's the delimiter, call the handler function.
			if (data[i1] == this->getDelimiter()) {
				this->tocall_(connection, *vec);

				// Clear the message vector and start a new one.
				vec->clear();
			}
		}

		// Lock is released.
	}
	

}
