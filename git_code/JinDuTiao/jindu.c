#include <stdio.h>
#include <unistd.h>
#define RED "\e[0;31m"
#define GREEN "\e[0;32m"
#define YELLOW "\e[0;33m"


int main()
{
  char buffer[ 1024] = {0};
  const char* model = "-\\|/";
  int i = 0;
  for(; i < 100; i++){
    buffer[i] = '*';  
    if(i < 20){
    printf(RED"[%d%%][%c][%s][%c]\r",i,model[i%4],buffer,model[i%4]);  
   }
    else if(i >= 20 && i < 50){
    printf(YELLOW"[%d%%][%c][%s][%c]\r" ,i,model[i%4],buffer,model[i%4]);
    }
    else{
    printf(GREEN"[%d%%][%c][%s][%c]\r" ,i,model[i%4],buffer,model[i%4]);
    }
    fflush(stdout);
    usleep(100 * 1000);
  }
  return 0;
}
