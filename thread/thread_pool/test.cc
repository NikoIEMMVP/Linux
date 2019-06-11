#include "threadPool.hpp"


// 这个类由用户自定制
// 需要哪些数据和依赖都可以随意添加

class MyTask : public Task{
public:
  MyTask(int id): id_(id){

  }

  void Run(){
    // 执行用户自定制的逻辑
    printf("id = %d\n", id_);
  }

private:
  int id_;


};

int main()
{
  ThreadPool pool(10);
  for(int i = 0; i < 20; ++i){
    pool.AddTask(new MyTask(i));  

  }
  while(1){
    sleep(1);
  }
  return 0;
}
