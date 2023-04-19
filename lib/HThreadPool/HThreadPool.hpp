#pragma once

#include<vector>
#include<mutex>
#include<queue>
#include<thread>
#include<condition_variable>
#include<memory>
#include<type_traits>
#include<functional>
#include<future>
#include<stdexcept>

/*
*			ThreadPool in Centhoo Library: Thread
* 
* Idea comes basically from the most stared C++ Thread Pool on github.
*/

namespace ceh {
	namespace Concurrency
	{
		class HThreadPool;
	}
}

class ceh::Concurrency::HThreadPool
{
private:
	std::vector< std::thread> workers;
	std::queue < std::function<void()> > tasks;

	std::mutex tasks_queue_mutexLock;
	std::condition_variable workers_condition;
	bool stop;

public:
	HThreadPool(size_t slotsNum):stop(false)
	{
		for (size_t slot = 0; slot < slotsNum; ++slot)
		{
			workers.emplace_back(
				[this]
				{
					while (true)
					{
						std::function<void()> task;
						{
							std::unique_lock lock(tasks_queue_mutexLock);

							workers_condition.wait(lock,
								[this] {return stop || !tasks.empty(); });
							if (stop && tasks.empty())
								return;
							task = tasks.front();
							tasks.pop();
						}
						task();
					}
				}
				);
		}
	}
	~HThreadPool()
	{
		{
			std::unique_lock lock(tasks_queue_mutexLock);
			stop = true;
		}
		workers_condition.notify_all();
		for (auto& worker : workers)
			worker.join();
	}

	template<typename Function, typename... Args>
	auto addTask(Function&& function, Args&&... args)
		-> std::future<typename std::invoke_result<Function, Args...>::type>
	{
		using return_type = typename std::invoke_result<Function, Args...>::type;

		auto task = std::make_shared<std::packaged_task<return_type()>>
			(
				std::bind(std::forward<Function>(function), std::forward<Args>(args)...)
				);

		std::future<return_type> ret = task->get_future();
		{
			std::unique_lock lock(tasks_queue_mutexLock);
			if (stop)
			{
				std::runtime_error addTaskToStoppedPool("[HThreadPool] Adding task into the stopped "
					"thread pool is not allowed.");

				throw addTaskToStoppedPool;
			}
			tasks.emplace([task]() {(*task)(); });
		}
		workers_condition.notify_one();
		return ret;
	}
};
