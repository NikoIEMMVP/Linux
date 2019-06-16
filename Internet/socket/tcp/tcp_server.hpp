// 通用的 tcp 服务器
//



// #pragma once

#include <functional>
#include "tcp_socket.hpp"

#define CHECK_RET(exp) if(!(exp)) { \
  return false;  \ 
}

typedef std::function<void (const std::string&, std::string*)> Handler;

class TcpServer{
public:
  TcpServer(){

  }


  ~TcpServer(){

  }

  bool Start(const std::string& ip, uint16_t port, Handler handler){
    // 创建 socket
    bool ret = listen_sock.Socket();
    if(!ret){
      perror("socket");
      return false;
    }
    // 绑定端口号
    ret = listen_sock.Bind(ip,port);
    // 进行监听
    CHECK_RET(listen_sock.Listen());
    printf("Server Start!\n");
    // 进入主循环
    
    while(true){
      // 通过 accept 获取到一个连接
      TcpSocket client_sock;
      std::string ip;
      uint16_t port;
      // 核心问题在于，第一次 Accept 之后就进入到了一个循环
      // 在这个操作过程中，循环一直没有结束，Accept 没有被重新调用
      // 后续连接过来的客户端都在内核的连接队列中排队呢
      // 应该想办法让我们的程序能够迅速多次的调用 Accept
      bool ret = listen_sock.Accept(&client_sock, &ip, &port);
      if(!ret){
        continue;
      }
      printf("[%s:%d] 有客户端连接\n",ip.c_str(), port);
      // 和客户端进行具体的沟通,一次连接，进行多次交互
      while(true){
        // 读取请求
        std::string req;
        int r = client_sock.Recv(&req);
        if(r < 0){
          continue;
        }
        if(r == 0){
          // 对端关闭
          client_sock.Close();
          printf("[%s:%d]对端关闭了连接:\n", ip.c_str(), port);
          break;
        }
        printf("[%s:%d] 客户端发送了：%s \n",ip.c_str(), port, req.c_str());
        // 根据请求计算响应
        std::string resp;
        handler(req, &resp);

        // 把响应写回客户端
        client_sock.Send(resp);
      }
    }

  }
private:
  TcpSocket listen_sock;
};
