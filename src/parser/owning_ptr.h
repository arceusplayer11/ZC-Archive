#include "../zc_malloc.h"
#include "../undefine_astdint.h"

#include <boost/type_traits.hpp>
#include <boost/atomic.hpp>
#include <cassert>
#include <cstddef>


////////////////////////////////////////////////////////////////
// owning_ptr - Owns its pointer, calls clone() on copy

template <class Type>
class owning_ptr
{
	// Safe bool idiom.
	typedef void (owning_ptr::* safe_bool_type) () const;
	void safe_bool_func() const {}

public:
	explicit owning_ptr(Type* p = NULL) : data_(p) {}
	owning_ptr(owning_ptr const& other) : data_(other.clone()) {}
	template <class OtherType>
	owning_ptr(owning_ptr<OtherType> const& other)
		: data_(other.clone()) {}
	~owning_ptr() { delete data_; }
	owning_ptr& operator=(owning_ptr const& rhs)
	{
		delete data_;
		data_ = rhs.clone();
		return *this;
	}
	owning_ptr& operator=(Type* pointer)
	{
		delete data_;
		data_ = pointer;
		return *this;
	}

	// Return a clone of the contents, or NULL if none.
	Type* clone() const
	{
		if (data_) return data_->clone();
		return NULL;
	}

	operator safe_bool_type() const
	{
		return data_ ? &owning_ptr::safe_bool_func : NULL;
	}

	Type* get() { return data_; }
	Type const* get() const { return data_; }
	Type& operator*() { return *data_; }
	Type const& operator*() const { return *data_; }
	Type* operator->() { return data_; }
	Type const* operator->() const { return data_; }
	Type* release()
	{
		Type* temp = data_;
		data_ = NULL;
		return temp;
	}
	void reset(Type* p = NULL)
	{
		delete data_;
		data_ = p;
	}

private:
	Type* data_;
};

template <class Type, class Type2>
bool operator==(owning_ptr<Type> const& lhs, owning_ptr<Type2> const& rhs)
{
	return lhs.get() == rhs.get();
}

template <class Type, class Type2>
bool operator!=(owning_ptr<Type> const& lhs, owning_ptr<Type2> const& rhs)
{
	return lhs.get() != rhs.get();
}

template <class Type, class Type2>
bool operator==(owning_ptr<Type> const& lhs, Type2* rhs)
{
	return lhs.get() == rhs;
}

template <class Type, class Type2>
bool operator!=(owning_ptr<Type> const& lhs, Type2* rhs)
{
	return lhs.get() != rhs;
}

template <class Type, class Type2>
bool operator==(Type* lhs, owning_ptr<Type2> const& rhs)
{
	return lhs == rhs.get();
}

template <class Type, class Type2>
bool operator!=(Type* lhs, owning_ptr<Type2> const& rhs)
{
	return lhs != rhs.get();
}

namespace zq {

// Based of off std::shared_ptr spec but won't have move semantics, custom deleters, weak_ptr stuff
// NB: Doesn't support arrays
template<typename T>
class shared_ptr {
public:
	//using element_type = std::remove_extent_t<T>;
	typedef T element_type;
	typedef T* pointer;
	typedef uint8_t* count_ptr;

	/*constexpr*/ shared_ptr() /*noexcept*/ : ptr_(pointer()), count_(count_ptr()) {}
	//constexpr shared_ptr(std::nullptr_t) noexcept;
	template<typename Y>
	explicit shared_ptr(Y* ptr = boost::add_pointer<Y>::type()) /*noexcept*/ : ptr_(ptr)
	{
		if (count_ptr() == count_) {
			count_ = new uint8_t(); // Needs to last long
		}
		++(*count_);
	}

	shared_ptr(const shared_ptr& r) /*noexcept*/ : ptr_(r.ptr_), count_(r.count_)
	{
		++(*count_);
	}

	template<typename Y>
	shared_ptr(const shared_ptr<Y>& r) /*noexcept*/ : ptr_(r.ptr_), count_(++r.count_)
	{
		++(*count_);
	}

	~shared_ptr()
	{
		if (count_ && (--(*count_) == 0)) {
			delete_s(ptr_);
			delete_s(count_);
		}
	}

	shared_ptr& operator=(const shared_ptr& r) /*noexcept*/
	{
		shared_ptr(r).swap(*this);
		return *this;
	}

	template<typename Y>
	shared_ptr& operator=(const shared_ptr<Y>& r) /*noexcept*/
	{
		shared_ptr<T>(r).swap(*this);
		return *this;
	}


	void swap(shared_ptr& r) /*noexcept*/
	{
		T* temp = r.ptr_;
		r.ptr_ = this->ptr_;
		this->ptr_ = temp;
		temp = pointer();

		uint8_t* temp2 = r.count_;
		r.count_ = this->count_;
		this->count_ = temp2;
		temp2 = pointer();
	}

	void reset() /*noexcept*/ { shared_ptr().swap(*this); }

	template<typename Y>
	void reset(Y* ptr) { shared_ptr<T>(ptr).swap(*this); }

	element_type* get() const /*noexcept*/ { return ptr_; }

	T& operator*() const /*noexcept*/ { return *get(); }
	T* operator->() const /*noexcept*/ { return get(); }

	// Returns number of copies
	long use_count() const /*noexcept*/
	{
		if (count_ptr() != count_) {
			return *count_;
		}
		return 0L;
	}

	// This doesn't work in C++03!!!!
	//explicit operator bool() const /*noexcept*/ { return (pointer() != get()); }
	bool unique() const /*noexcept*/ { return (use_count() == 1); }

	// not putting owner_before
private:
	T* ptr_;
	uint8_t* count_;
};

} // namespace zq
