#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <cstring>
#include <event2/event.h>
#include <event2/listener.h>
#include <iostream>
#define SPORT 5001
using namespace std;

void listen_cb(struct evconnlistener *e, evutil_socket_t s, struct sockaddr *a, int socklen, void *arg){
    cout << "listen_cb" << endl;
}

int main(){
    //忽略管道信号，避免发送数据给已关闭的socket引起宕掉
    if(signal(SIGPIPE, SIG_IGN) == SIG_ERR)
       return 1;
    std::cout << "test libevent server!\n";
    event_base *base = event_base_new();
    if(base){
        cout << "event_base_new success" << endl;


    }

    //监听端口
    //socket, bind, listen
    sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(SPORT);
    
    evconnlistener * ev = evconnlistener_new_bind(
            base,                                    //libevent的上下文
            listen_cb,                               //接收到连接的回调函数
            base,                                    //回调函数获取的参数arg
            LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, //地址重用，evconnlistener关闭同时关闭socket
            10,                                      //连接队列大小，对应listen函数
            (sockaddr*)&sin,                          //绑定的地址端口
            sizeof(sin)
            );

    //事件分发处理
    if(base)
        event_base_dispatch(base);
    if(ev)
        evconnlistener_free(ev);
    if(base)
        event_base_free(base);



    return 0;
}
