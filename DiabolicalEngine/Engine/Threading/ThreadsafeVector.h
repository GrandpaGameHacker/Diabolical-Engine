#pragma once
#include <vector>
#include <mutex>
#include "../Logging/Logging.h"

struct InvalidIndex : std::exception
{
	const char* what() const throw()
	{
		return "Invalid Index";
	}
};

template<typename T>
class ThreadsafeVector
{
private:
	std::vector<T> data;
	mutable std::mutex m;
public:
	ThreadsafeVector() {}
	ThreadsafeVector(const ThreadsafeVector& other)
	{
		std::lock_guard<std::mutex> lock(other.m);
		data = other.data;
	}
	ThreadsafeVector& operator=(const ThreadsafeVector&) = delete;

	void Insert(T new_value)
	{
		std::lock_guard<std::mutex> lock(m);
		data.push_back(new_value);
	}

	size_t Size()
	{
		std::lock_guard<std::mutex> lock(m);
		return data.size();
	}

	T* GetAt(size_t Index)
	{
		std::lock_guard<std::mutex> lock(m);
		if (data.size() <= Index) throw InvalidIndex();
		return &data[Index];
	}

	void Clear()
	{
		std::lock_guard<std::mutex> lock(m);
		data.clear();
	}
};

