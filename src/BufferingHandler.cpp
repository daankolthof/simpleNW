#include <BufferingHandler.hpp>

#include <Connection.hpp>
#include <ConnectionInfo.hpp>

#include <iostream>


BufferingHandler::BufferingHandler(std::function<void(ConnectionInfo, std::vector<char>)> callback) : tocall_(callback) {}

char BufferingHandler::getDelimiter() {
	return this->delimiter_;
}

/*
void BufferingHandler::setDelimiter(char delimiter) {
	this->delimiter_.store(delimiter);
}
*/

void BufferingHandler::OnReceive(ConnectionInfo connectioninfo, char data[], size_t bytes_received) {
		
	std::vector<char>* vec;
	std::mutex* vec_mtx;

	/* Lock the buffers_ mutex so the pair can be taken out safely. */
	{
		std::lock_guard<std::mutex> buffers_lock(buffers_mtx);

		std::map<ConnectionInfo, std::pair<std::unique_ptr<std::vector<char>>, std::unique_ptr<std::mutex>>>::iterator it = buffers_.find(connectioninfo);

		// Connection has not been seen before.
		if (it == buffers_.end()) {
			// Construct new pair.
			std::unique_ptr<std::mutex> mtx_ptr(new std::mutex);
			std::unique_ptr<std::vector<char>> vec_ptr(new std::vector<char>);
			std::pair<std::unique_ptr<std::vector<char>>, std::unique_ptr<std::mutex>> vec_mtx_pair(std::move(vec_ptr), std::move(mtx_ptr));
			
			buffers_[connectioninfo] = std::move(vec_mtx_pair);

			// Put it in the iterator.
			it = buffers_.find(connectioninfo);
		}

		vec = it->second.first.get();
		vec_mtx = it->second.second.get();
		
		// Lock is released.
	}

	/* Lock the individual vector. */
	{
		std::lock_guard<std::mutex> vec_lock(*vec_mtx);

		for (unsigned int i1 = 0; i1 < bytes_received; i1++) {

			// Push character in buffer.
			vec->push_back(data[i1]);

			// If it's the delimiter, call the handler function.
			if (data[i1] == this->getDelimiter()) {
				this->tocall_(connectioninfo, *vec);

				// Clear the message vector and start a new one.
				vec->clear();
			}
		}

		// Lock is released.
	}
	

}
