#pragma once
#include <pthread.h>
#include <stdio.h>
#include "blocking_queue.hpp"


class Task{
public:
  virtual void Run(){
    printf("base Run\n");
  }

  virtual ~Task(){

  }

private:


};

// 线程池对象启动的时候会创建一组线程
// 每个线程都需要完成一定的任务(执行一定的代码逻辑，这个逻辑是由
// 调用者来决定的)
// 任务是一段代码，可以用函数来表示

// 提前把线程创建好，避免频繁创建销毁线程的开销
// 线程不必一次创建太多，复用一个线程来完成多个任务
//

class ThreadPool{
public:
  // n 表示创建线程的数量
  ThreadPool(int n): queue_(100), work_count(n){

    for(int i = 0; i < work_count; ++i){
      pthread_t tid;
      pthread_create(&tid, NULL, ThreadEntry, this);
      workers_.push_back(tid);
    }
  } 

  ~ThreadPool(){
    // 先让线程退出，然后再回收
    for(size_t i = 0; i < workers_.size(); ++i){
      pthread_cancel(workers_[i]);
    }

    for(size_t i = 0; i < workers_.size(); ++i){
      pthread_join(workers_[i], NULL);
    }
  }
  

  // 使用线程池的时候，就需要由调用者加入一些任务，
  // 让线程池去执行
  void AddTask(Task* task){
    queue_.Push(task);
  }


private:
  BlockingQueue<Task*> queue_;
  int work_count;
  std::vector<pthread_t> workers_;

  static void* ThreadEntry(void* arg){
    ThreadPool* pool = (ThreadPool*) arg;
    while(true){
      // 循环中尝试从阻塞队列中获取任务
      // 获取到一个任务就执行一个任务
      Task* task = NULL;
      pool->queue_.Pop(&task);
      // task 表面上看是一个 Task* 类型， 实际上指向的是 MyTask*
      // 执行的是 子类， 用户自定制的逻辑
      task->Run();
      delete task;
    }
  }

};
