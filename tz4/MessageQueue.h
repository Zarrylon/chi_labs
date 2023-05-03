#pragma once

#include <iostream>
#include <chrono>
#include <queue>
#include <tuple>

template<typename T>
class MessageQueue
{
public:
	using message_type = std::tuple<int, std::chrono::system_clock::time_point, T>;

	MessageQueue(size_t s) : size(s) {}

	bool isFull()
	{
		return queue.size() == size;
	}

	bool isExpired(const message_type& message)
	{
		return std::get<1>(message) <= std::chrono::system_clock::now();
	}

	// debug
	/*bool isExpired(const message_type& message)
	{
		if (std::get<1>(message) <= std::chrono::system_clock::now())
		{
			std::cout << "Bool Expired: " << std::get<2>(message) << std::endl;
			return true;
		}
		else
		{
			std::cout << "Bool Not Expired: " << std::get<2>(message) << std::endl;
			return false;
		}
	}*/

	void pop()
	{
		if (!queue.empty()) queue.pop();
		else throw std::runtime_error("Can't pop empty queue.");
	}

	void popExpired()
	{
		//std::cout << "Pop" << std::endl;
		while (!queue.empty() && isExpired(queue.top()))
		{
			queue.pop();
		}
	}

	void push(const message_type& message)
	{
		//std::cout << "Push" << std::endl;
		if (!isExpired(message))
		{
			if (isFull()) popExpired();
			if (isFull()) throw std::runtime_error("Queue is full.");

			queue.push(message);
		}
	}

	T getMessage()
	{
		//std::cout << "Get" << std::endl;
		popExpired();
		if (!queue.empty())
		{
			T message = std::get<2>(queue.top());
			queue.pop();

			return message;
		}
		else return {};
	}

	/*void print()
	{
		auto q = queue;

		while (!q.empty())
		{
			std::cout << std::get<0>(q.top()) << ' ' << ' ' << std::get<2>(q.top()) << std::endl;

			q.pop();
		}
	}*/

	~MessageQueue() = default;

private:
	std::priority_queue<message_type, std::vector<message_type>, std::greater<>> queue;
	size_t size;

	template<typename T> friend class QueueAnalyzer;
};