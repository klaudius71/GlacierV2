#ifndef _REF_H
#define _REF_H

template<class T>
class WeakPtr
{
public:
	WeakPtr() = default;
	WeakPtr(const WeakPtr&) = default;
	WeakPtr(const std::shared_ptr<T>& shrd)
		: ptr(shrd)
	{}
	WeakPtr& operator=(const WeakPtr&) = default;
	WeakPtr& operator=(const std::shared_ptr<T>& shrd)
	{
		ptr = shrd;
		return *this;
	}
	~WeakPtr() = default;

	std::weak_ptr<T>& getWeakPtr()
	{
		return ptr;
	}

	bool isExpired() const
	{
		return ptr.expired();
	}

	T* operator*()
	{
		return ptr.lock().get();
	}
	T* operator->()
	{
		assert(!ptr.expired());
		return ptr.lock().get();
	}

private:
	std::weak_ptr<T> ptr;
};

#endif _REF_H