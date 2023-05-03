#pragma once

#include <map>
#include <fstream>
#include <iomanip>
#include "MessageQueue.h"

template<typename T>
class QueueAnalyzer
{
public:
	QueueAnalyzer(MessageQueue<T>& q) : queue_friend(q) {}

	void Analyzer(std::string filename)
	{
		std::ofstream file(filename, std::ofstream::app);

		// current time
		time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		file << "1) Time: " << std::put_time(localtime(&now), "%F %T") << std::endl;

		// queue size
		file << "2) Queue size: " << queue_friend.queue.size() << std::endl;

		if (queue_friend.queue.size() != 0)
		{
			// priority count
			auto q = queue_friend.queue;
			std::map<int, int> counter;

			PriorityPercentage(counter);

			file << "3) " << std::endl;
			for (auto it = counter.begin(); it != counter.end(); ++it) {
				file << "Priority " << it->first << ": " << it->second * 100 / queue_friend.queue.size() << "%" << std::endl;
			}

			// KB
			file << "4) Size in KB: " << CalculateSizeKB() / 1024 << std::endl;

			// time interval
			file << "5) Max interval: " << MaxInterval().count() << " seconds" << std::endl << std::endl;
		}
		else file << "3) -" << std::endl << "4) 0 kb" << std::endl << "5) 0 seconds" << std::endl << std::endl;
	}

	void PriorityPercentage(std::map<int, int>& mp)
	{
		auto q = queue_friend.queue;

		while (!q.empty())
		{
			mp[std::get<0>(q.top())]++;
			q.pop();
		}
	}

	double CalculateSizeKB()
	{
		return (double)queue_friend.queue.size() * sizeof(typename MessageQueue<T>::message_type);
	}

	std::chrono::seconds MaxInterval()
	{
		auto q = queue_friend.queue;
		auto late_expire = std::get<1>(q.top());
		auto early_expire = late_expire;
		q.pop();

		while (!q.empty())
		{
			if (std::get<1>(q.top()) < early_expire)
			{
				early_expire = std::get<1>(q.top());
			}

			if (std::get<1>(q.top()) > late_expire)
			{
				late_expire = std::get<1>(q.top());
			}

			q.pop();
		}

		auto max_interval = std::chrono::duration_cast<std::chrono::seconds>(late_expire - early_expire);

		return max_interval;
	}

	~QueueAnalyzer() = default;

private:
	MessageQueue<T>& queue_friend;
};