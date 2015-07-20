
#if !defined(_DYNAMICARRAY_H)
#define _DYNAMICARRAY_H

/** This class is only used internally.
*/
template <typename T>
class DynamicArray {

public:


	DynamicArray() {

		this->buffer_ = nullptr;
	}
	

	DynamicArray(size_t arraysize) {

		this->buffer_ = new T[arraysize];
		this->buffer_size_ = arraysize;
	}

	/* Class is copyable but object copied from loses ownership of the underlying buffer.
	The only reason this constructor exists is because std::bind doesn't work that well with moving objects. */
	DynamicArray(DynamicArray& other) {

		this->buffer_ = other.buffer_;
		this->buffer_size_ = other.buffer_size_;
		this->has_ownership_ = other.has_ownership_;

		other.has_ownership_ = false;
	}

	DynamicArray(DynamicArray&& other) {
		// MSVC with Visual Studio 2013 doesn't support default move constructors!?

		this->buffer_ = other.buffer_;
		this->buffer_size_ = other.buffer_size_;
		this->has_ownership_ = other.has_ownership_;

		/* Other pointer must be a nullptr, this is checked again in the destructor
		to avoid calls to delete with a nullptr */
		other.buffer_ = nullptr;
		other.has_ownership_ = false;
		// We don't care about other->buffer_size_.

	}

	~DynamicArray() {

		/* Check if we do not have a null pointer.
		This could happen when object contents were moved to other object.*/
		if (this->has_ownership_ && this->buffer_) {
			delete this->buffer_;
		}

	}

	T* data() {
		return this->buffer_;
	}

	T** dataref() {
		return &this->buffer_;
	}

	size_t size() {
		return this->buffer_size_;
	}

	size_t& sizeref() {
		return this->buffer_size_;
	}

	bool& should_delete_ref() {
		return this->has_ownership_;
	}


private:

	T* buffer_;
	size_t buffer_size_;

	bool has_ownership_ = true;

};

#endif
