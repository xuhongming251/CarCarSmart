//=========================================================================
/**
 *  \file    BThreadPool.h
 *  \brief   创建文档
 *
 *
 *  \author xuminglong
 *  \date   2015年2月4日
 */
//=========================================================================
#ifndef BThreadPool_h__
#define BThreadPool_h__

#include <boost/threadpool.hpp>

class ThreadPool
{
public:
    static ThreadPool* instance()
    {
        static ThreadPool tp;
        return &tp;
    }

    void init(int thread_max_count)
    {
        tp_.size_controller().resize(thread_max_count);
    }

    void do_schedule(boost::threadpool::thread_pool<>::task_type t)
    {
        tp_.schedule(t);
    }

protected:
    ThreadPool() {}
    ~ThreadPool() {}

private:
    boost::threadpool::pool tp_;

};

#define ThreadPoolInstance ThreadPool::instance()



#endif // BThreadPool_h__
