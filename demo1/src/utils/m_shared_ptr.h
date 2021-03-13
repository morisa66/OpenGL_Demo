#ifndef __M_SHARED_PTR_H_
#define	__M_SHARED_PTR_H_

#include <atomic>

#include "../_morisa.h"

MORISA_NAMESPACE_BEGIN

class m_counter
{
public:
	m_counter()noexcept :count(1) {}

	void increase()noexcept
	{
		count.fetch_add(1, std::memory_order_relaxed);
	}

	long decrease()noexcept { return --count; }

	long get() const noexcept { return count; }

private:
	std::atomic_long count;
};


template <typename T>
class m_shared_ptr
{
public:
	template <typename V>
	friend class m_shared_ptr;

	explicit m_shared_ptr(T* _ptr = nullptr) : ptr(_ptr)
	{
		if (ptr) {
			counter = new m_counter();
		}
	}

	~m_shared_ptr()
	{
		if (ptr && !counter->decrease()) {
			delete ptr;
			delete counter;
			ptr = nullptr;
			counter = nullptr;
		}
	}

	T* get() const noexcept { return ptr; }
	T& operator*() const noexcept { return *ptr; }
	T* operator->() const noexcept { return ptr; }
	operator bool() const noexcept { return ptr; }

	m_shared_ptr& operator=(m_shared_ptr other)
	{
		// compiler default copy constructer
		m_shared_ptr(other).swap(*this);
		return *this;
	}

	m_shared_ptr(const m_shared_ptr& other) noexcept
	{
		ptr = other.ptr;
		if (ptr) {
			other.counter->increase();
			counter = other.counter;
		}
	}

	template <typename V>
	m_shared_ptr(const m_shared_ptr<V>& other) noexcept
	{
		ptr = other.ptr;
		if (ptr) {
			other.counter->increase();
			counter = other.counter;
		}
	}

	template <typename V>
	m_shared_ptr(m_shared_ptr<V>&& other) noexcept
	{
		ptr = other.ptr;
		if (ptr) {
			counter = other.counter;
			other.ptr = nullptr;
		}
	}

	template <typename V>
	m_shared_ptr(const m_shared_ptr<V>& other, T* _ptr) noexcept
	{
		ptr = _ptr;
		if (ptr) {
			other.counter->increase();
			counter = other.counter;
		}
	}

	void swap(m_shared_ptr& other) noexcept
	{
		std::swap(ptr, other.ptr);
		std::swap(counter, other.counter);
	}

	void release() noexcept
	{
		if (ptr) {
			if (!counter->decrease()) {
				delete ptr;
				delete counter;
			}
			ptr = nullptr;
			counter = nullptr;

		}
	}

	long get_counter() const noexcept
	{
		if (ptr) return counter->get();
		return 0;
	}
private:
	T* ptr;
	m_counter* counter;
};

template <typename T, typename V>
m_shared_ptr<T> m_const_cast(const m_shared_ptr<V>& other) noexcept
{
	T* ptr = const_cast<T*>(other.get());
	return m_shared_ptr<T>(other, ptr);
}

template <typename T, typename V>
m_shared_ptr<T> m_static_cast(const m_shared_ptr<V>& other) noexcept
{
	T* ptr = static_cast<T*>(other.get());
	return m_shared_ptr<T>(other, ptr);
}

template <typename T, typename V>
m_shared_ptr<T> m_dynamic_cast(const m_shared_ptr<V>& other) noexcept
{
	T* ptr = dynamic_cast<T*>(other.get());
	return m_shared_ptr<T>(other, ptr);
}

template <typename T, typename V>
m_shared_ptr<T> m_reinterpret_cast(const m_shared_ptr<V>& other) noexcept
{
	T* ptr = reinterpret_cast<T*>(other.get());
	return m_shared_ptr<T>(other, ptr);
}


MORISA_NAMESPACE_END
#endif