
// 获取已终止程序的退出码

#include <stdio.h>
#include <pthread.h>



void* ThreadEntry1(void* arg){
  (void) arg;
  printf("thread 1 returning\n");
  return (void*)1;
}

void* ThreadEntry2(void* arg){
  (void) arg;
  printf("thread 2 exiting\n");
  pthread_exit((void*)2);
}


int main()
{
  int err;
  pthread_t tid1, tid2;
  void* tret;

  err = pthread_create(&tid1, NULL, ThreadEntry1, NULL);
  if(err != 0){
    printf("can't create thread1\n");
  }

  err = pthread_create(&tid2, NULL, ThreadEntry2, NULL);
  if(err != 0){
    printf("can't create thread2\n");
  }

  err = pthread_join(tid1, &tret);
  if(err != 0){
    printf("can't join in thread 1\n");
  }
  printf("thread 1 exit code %ld\n", (long)tret);
  
  err = pthread_join(tid2, &tret);
  if(err != 0){
    printf("can't join in thread 2\n");
  }
  printf("thread 2 exit code %ld\n",(long)tret);

  return 0;
}
