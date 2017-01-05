// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_CONDITION_H
#define MUDUO_BASE_CONDITION_H

#include <muduo/base/Mutex.h>

#include <boost/noncopyable.hpp>
#include <pthread.h>

namespace muduo
{

	class Condition : boost::noncopyable
	{
	public:
		explicit Condition(MutexLock& mutex)
			: mutex_(mutex)
		{
			MCHECK(pthread_cond_init(&pcond_, NULL)); // 首先初始化一个cond
		}

		~Condition()
		{
			MCHECK(pthread_cond_destroy(&pcond_));
		}

		void wait()
		{
			MutexLock::UnassignGuard ug(mutex_);
			MCHECK(pthread_cond_wait(&pcond_, mutex_.getPthreadMutex()));
		}

		  // returns true if time out, false otherwise.
		bool waitForSeconds(double seconds);

		void notify()
		{
			MCHECK(pthread_cond_signal(&pcond_)); // 唤醒一个线程
		}

		void notifyAll()
		{
			MCHECK(pthread_cond_broadcast(&pcond_)); // 唤醒多个线程
		}

	private:
		MutexLock& mutex_;
		pthread_cond_t pcond_;
	};

}
#endif  // MUDUO_BASE_CONDITION_H
