#pragma once

#include <vector>
#include <functional>

// Generic event with data
template<typename T>
class Event
{
private:
	std::vector<std::function<void(const T&)>> listeners;
public:
	// subscribe
	size_t Subscribe(std::function<void(const T&)> listener)
	{
		listeners.push_back(listener);
		return listeners.size() - 1;
	}

	// unsubscribe
	void Unsubscribe(size_t& handle)
	{
		if (handle < listeners.size()) {
			listeners[handle] = nullptr;
			handle = static_cast<size_t>(-1); // mark handle as invalid
		}
	}

	// trigger
	void Invoke(const T& data)
	{
		for (auto& listener : listeners) {
			if (listener) {
				listener(data);
			}
		}
	}
	// clear all listeners
	void Clear()
	{
		listeners.clear();
	}
};

// unpayloaded event
class SimpleEvent
{
private:
	std::vector<std::function<void()>> listeners;
public:

	size_t Subscribe(std::function<void()> listener)
	{
		listeners.push_back(listener);
		return listeners.size() - 1;
	}

	void Unsubscribe(size_t& handle)
	{
		if (handle < listeners.size()) {
			listeners[handle] = nullptr;
			handle = static_cast<size_t>(-1); // mark handle as invalid
		}
	}

	void Invoke()
	{
		for (auto& listener : listeners) {
			if (listener) {
				listener();
			}
		}
	}

	void Clear()
	{
		listeners.clear();
	}

};