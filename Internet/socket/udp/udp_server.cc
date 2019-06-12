#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>




int main()
{
  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock < 0){
    perror("socket");
    return 1;
  }

  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr("0.0.0.0");
  addr.sin_port = htons(9090);

  int ret = bind(sock, (sockaddr*)&addr, sizeof(addr));
  if(ret < 0){
    perror("bind");
    return 1;
  }
  printf("server is strat!\n");

  while(true){
    sockaddr_in peer;
    // 输出型参数
    socklen_t len = sizeof(peer);
    char buf[1024] = {0};
    ssize_t n = recvfrom(sock, buf, sizeof(buf) - 1, 0, (sockaddr*)&peer, &len);
    if(n < 0){
      perror("recvfrom");
      continue;
    }
    buf[n] = '\0';
    printf("[%s : %d] buf : %s\n",inet_ntoa(peer.sin_addr), ntohs(peer.sin_port), buf);
    n = sendto(sock, buf, strlen(buf), 0, (sockaddr*)&peer, len);
    if(n < 0){
      perror("sendto");
      continue;
    } 
  }
  close(sock);
  return 0;
}
