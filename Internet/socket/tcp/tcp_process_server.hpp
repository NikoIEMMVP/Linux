#pragma once
        
#include <iostream>
#include <functional>
#include "tcp_socket.hpp"


#define CHECK_RET(exp) if(!(exp)){ \
  return false; \
}

typedef std::function<void(const std::string&, std::string*)> Handler;

class TcpProcessServer{
public:
  TcpProcessServer(){}

  ~TcpProcessServer(){
    Listen_sock.Close();
  }

  bool Start(const std::string& ip, uint16_t port, Handler handler){
    int ret = Listen_sock.Socket();
    if(ret < 0){
      perror("socket");
    }

    ret = Listen_sock.Bind(ip, port);
    if(ret < 0){
      perror("bind");
    }

    ret = Listen_sock.Listen();
    if(ret < 0){
      perror("listen");
    }

    while(true){
      TcpSocket client_sock;
      std::string peer_ip;
      uint16_t peer_port;
      bool ret = Listen_sock.Accept(&client_sock, &peer_ip, &peer_port);
      if(!ret){
        perror("accept");
        continue;
      }

      printf("[%s : %d] 有新客户连接！\n", peer_ip.c_str(), peer_port);
      Connect_Process(client_sock, peer_ip, peer_port, handler);
    }
  }

private:
  TcpSocket Listen_sock;

  void Connect_Process(TcpSocket& client_sock, const std::string& ip, const uint16_t port, Handler handler){
    pid_t ret = fork();
    if(ret > 0){
      client_sock.Close();
      return;
    }

    while(true){
      std::string req;
      int r = client_sock.Recv(&req);
      if(r < 0){
        continue;
      }
      if(r == 0){
        client_sock.Close();
        printf("[%s : %d]对端关闭了连接\n", ip.c_str(), port);
        break;
      }

      printf("[%s : %d]对端发送了：",ip.c_str(), port );
      std::cout << req << std::endl;
      std::string resp;
      handler(req, &resp);
      client_sock.Send(resp);
    }

    client_sock.Close();
    exit(0);
  }
};

