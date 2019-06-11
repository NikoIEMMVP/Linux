#pragma once

#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>


// 一般 BlockingQueue 都要求长度是有上限的
// 如果队列为空，去执行Pop就会阻塞
// 如果队列满了，去执行Push就会阻塞
// 信号量表示互斥比较简单，但是表示同步就比较复杂

// 信号量表示可用资源的个数
// 一个信号量表示当前队列中元素的个数
// 另一个信号量表示当前队列中空格的个数
// 插入元素就是在消耗一个空格资源，释放了一个元素资源
// 删除元素就是在消耗一个元素资源，释放了一个空格资源

// 如果用信号量表示互斥，P操作和V操作在同一个函数中
// 如果用信号量表示同步，P操作和V操作在不同的函数中

// ** 信号量这个计数器的 +1 -1 操作是原子的 **

template <typename T>
class BlockingQueue{
public:
  BlockingQueue(int max_size)
  :queue_(max_size),head_(0), tail_(0), size_(0), max_size_(max_size){
    sem_init(&lock_, 0, 1);
    sem_init(&elem_, 0, 0);
    sem_init(&blank_, 0, max_size);
  }

  ~BlockingQueue(){
    sem_destroy(&lock_);
    sem_destroy(&elem_);
    sem_destroy(&blank_);
  }


  void Push(const T& data){
    // 每次要插入元素，就得先申请一个空格资源
    // 如果没有空格资源(信号量为0)，说明队列满了
    // 满了就不能继续插入， 并且在 Push 中阻塞
    sem_wait(&blank_);
    
    sem_wait(&lock_);
    queue_[tail_] = data;
    ++tail_;
    ++size_;
    sem_post(&lock_);

    sem_post(&elem_);
  }

  // data 表示出队列的这个元素
  void Pop(T* data){
    // 每次出队列的时候，就先申请一个元素资源
    // 如果没有元素资源，队列为空
    // 就不能直接出队列，而需要在 Pop 中阻塞
    sem_wait(&elem_);


    sem_wait(&lock_);
    *data = queue_[head_];
    ++head_;
    --size_;
    sem_post(&lock_);

    sem_post(&blank_);
  }


private:
  std::vector<T> queue_;
  int head_;
  int tail_;
  int size_;
  int max_size_;
  // 可以用一个 二元信号量(非0即1)表示互斥锁
  sem_t lock_;
  // 可用元素的个数
  sem_t elem_;
  // 可用空格的个数
  sem_t blank_;
};
