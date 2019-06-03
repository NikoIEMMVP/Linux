#include <stdio.h>
#include <stdlib.h>

void my_exit1(void)
{
  printf("first exit handler\n");
}

void my_exit2(void)
{
  printf("second exit handler\n");
}

int main()
{
  if(atexit(my_exit2) != 0){
    printf("Can't register my_exit2\n");
  }

  if(atexit(my_exit1) != 0){
    printf("Can't register my_ezit1\n");
  }
  
  if(atexit(my_exit1) != 0){
    printf("Can't register my_ezit1\n");
   }
  printf("main is done\n");
  return 0;
}
