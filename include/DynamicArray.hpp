
#if !defined(_DYNAMICARRAY_H)
#define _DYNAMICARRAY_H

#include <cstddef>
#include <iostream>

/** This class is only used internally.
*/
template <typename T>
class DynamicArray {

public:
	
	DynamicArray(char* data, size_t data_size) {
		this->buffer_ = data;
		this->buffer_size_ = data_size;
		this->cleaned_up = new bool;
		*this->cleaned_up = false;
		this->has_ownership_ = false;
	}

	DynamicArray(size_t arraysize) {
		this->buffer_ = new T[arraysize];
		this->buffer_size_ = arraysize;
		this->cleaned_up = new bool;
		*this->cleaned_up = false;
	}

	/* Class is copyable but object copied from loses ownership of the underlying buffer.
	The only reason this constructor exists is because std::bind doesn't work that well with moving objects. */
	DynamicArray(const DynamicArray& other) {
		this->buffer_ = other.buffer_;
		this->buffer_size_ = other.buffer_size_;
		this->has_ownership_ = other.has_ownership_;
		this->cleaned_up = other.cleaned_up;
	}
	DynamicArray(DynamicArray&& other) {
		this->buffer_ = other.buffer_;
		this->buffer_size_ = other.buffer_size_;
		this->has_ownership_ = other.has_ownership_;
		other.has_ownership_ = false;
		this->cleaned_up = other.cleaned_up;
	}

	~DynamicArray() {
		/* Check if we do not have a null pointer.
		This could happen when objects were moved to other object. */
		if(this->has_ownership_ && this->buffer_ && !(*this->cleaned_up)) {
			delete this->buffer_;
			*this->cleaned_up = true;
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

	/* The move constructor in combination with std::bind doesn't work that well
	 * on some compilers(MSVC, GCC), therefore the copy constructor is using during
	 * the std::bind. Ownership of memory is not transferred, therefore the need
	 * for a flag to determine whether the memory region is already free'd. */
	bool* cleaned_up;

};

#endif
