服务器的编译：g++ childthread.cpp main.cpp newmain.cpp tcpserver.cpp -lpthread –levent
<br>运行的时候 ./a.out 127.0.0.1 6000 4

<br>客户端直接运行

<br>这是一个初期的版本
客户端发送消息 服务器接受消息并打印

<br>解决了 基本的多线程通信问题 后面只要进行拓展业务
