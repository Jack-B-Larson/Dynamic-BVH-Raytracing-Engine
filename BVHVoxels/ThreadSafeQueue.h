#pragma once
#include <mutex>
#include <queue>
template<typename _Ty>
class ThreadSafeQueue
{
public:
	void push(_Ty _Val)
	{
		//lock push
		std::unique_lock<std::mutex> lock(queueMutex);

		//execute
		dataQueue.push(_Val);

		//allow a waiter to enter
		conditionVariable.notify_one();
	}
	_Ty pop()
	{
		//lock pop
		std::unique_lock<std::mutex> lock(queueMutex);

		//wait for entrance
		conditionVariable.wait(lock, [this]()
							   {
								   return !dataQueue.empty();
							   });

		//execute
		_Ty item = dataQueue.front();
		dataQueue.pop();

		//return
		return item;
	}
	bool empty()
	{
		return dataQueue.empty();
	}
private:
	std::mutex queueMutex = std::mutex();
	std::condition_variable conditionVariable = std::condition_variable();
	std::queue<_Ty> dataQueue = std::queue<_Ty>();
};

