#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#if 0
// 共享内存空间

int g_count = 0;

void* ThreadEntry(void* arg){
  (void) arg;
  while(1){
    printf("In ThreadEntry\n");
    ++g_count;
    sleep(1);
  }
  return NULL;
}

int main()
{
  pthread_t tid;
  pthread_create(&tid, NULL, ThreadEntry, NULL);
  pthread_detach(tid);
  while(1){
    printf("In MainThread %d\n", g_count);
    sleep(1);
  }
  return 0;
}
#endif




#if 0
// 利用多核资源


void* ThreadEntry(void* arg){
  (void) arg;
  while(1);
  return NULL;
}

int main()
{
  pthread_t tid1,tid2,tid3,tid4;
  pthread_create(&tid1, NULL, ThreadEntry, NULL);
  pthread_create(&tid2, NULL, ThreadEntry, NULL);
  pthread_create(&tid3, NULL, ThreadEntry, NULL); 
  pthread_create(&tid4, NULL, ThreadEntry, NULL);
  while(1);
  return 0;
}


#endif



#if 1

#define SIZE 100000000

// us  微秒
// ms  毫秒

int64_t GetUs(){
 struct timeval tv;
 gettimeofday(&tv, NULL);
 return tv.tv_sec * 1000000 + tv.tv_usec;
}

void Calc(int* arr, int beg, int end){
  int i = 0;
  for( i = beg; i < end; i++){
    arr[i] = arr[i] * arr[i];
  }
}


#if 0
// 单一线程
int main()
{
  int* arr = (int*)malloc(sizeof(int) * SIZE);
  int64_t beg = GetUs();
  Calc(arr, 0, SIZE);
  int64_t end = GetUs();
  printf("time --> %ld\n", end - beg);
  return 0;
}
#endif


#if 1
// 多个线程

#define THREAD_NUM 4

typedef struct{
  int beg;
  int end;
  int* arr;
}Arg;

void* ThreadEntry(void* arg){
  Arg* p = (Arg*)arg;
  Calc(p->arr, p->beg, p->end);
  return NULL;
}

int main()
{

  int i = 0;
  int* arr = (int*)malloc(sizeof(int) * SIZE);
  Arg args[THREAD_NUM];
  int base = 0;
  for(i = 0; i < THREAD_NUM; i++){
    args[i].beg = base;
    args[i].end = base + SIZE / THREAD_NUM;
    args[i].arr = arr;
    base += SIZE / THREAD_NUM;
  
  pthread_t tid[THREAD_NUM];
  int64_t beg = GetUs();
  for(i = 0; i < THREAD_NUM; i++){
    pthread_create(&tid[i], NULL, ThreadEntry, &args[i]);
  }
  for(i = 0; i < THREAD_NUM; i++){
    pthread_join(tid[i], NULL);
  }
  int64_t end = GetUs();
  printf("time --> %ld\n", end - beg);
  return 0;
}




#endif

#endif





















