
#if !defined(_DYNAMICARRAY_H)
#define _DYNAMICARRAY_H

template <typename T>
class DynamicArray {

public:

	DynamicArray::DynamicArray(size_t arraysize) {

		this->buffer_ = new T[arraysize];
		this->buffer_size_ = arraysize;
	}

	/* Copy constructor not possible because class manages memory which can only be deleted once. */
	DynamicArray(const DynamicArray& other) = delete;

	
	DynamicArray(DynamicArray&& other) {
		// MSVC with Visual Studio 2013 doesn't support default move constructors!?

		this->buffer_ = other.buffer_;
		this->buffer_size_ = other.buffer_size_;

		/* Other pointer must be a nullptr, this is checked again in the destructor
		to avoid calls to delete with a nullptr */
		other.buffer_ = nullptr;
		// We don't care about other->buffer_size_.

	}

	T* data() {
		return this->buffer_;
	}

	size_t size() {
		return this->buffer_size_;
	}

	DynamicArray::~DynamicArray() {

		/* Check if we do not have a null pointer.
		This could happen when object contents were moved to other object.*/
		if (this->buffer_) {
			delete this->buffer_;
		}

	}

protected:

private:

	T* buffer_;
	size_t buffer_size_;

};



#endif
