#include<iostream>
#include<assert.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<map>
#include<event.h>
#include<unistd.h>
#include<memory.h>
#include<stdlib.h>
#include<vector>
#include "tcpserver.h"

//listenfd上有数据时 调用的回调函数
void sockfd_cb(int fd, short event, void *argc);
//socketpair0上有数据时 调用的回调函数
void socketpair0_cb(int fd, short event, void *argc);
//子线程的事情
void *newthread(void *arg);

Tcpserver::tcpserver(char *ip,unsigned short port,int pth_num)
{
	//创建服务器
	cout << "tcpserver()" << endl;

	//socket()
	int listenfd = socket(AF_INET, SOCK_STREAM, NULL);
	assert(listenfd != -1);

	cout << "socket返回的fd=" << listenfd << endl;

	//bind()
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	int res = bind(listenfd, (struct sockaddr *)&addr, sizeof(addr));
	assert(res != -1);

	//listen()
	listen(listenfd, 5);

	_listen_fd = listenfd;
	_pth_num = pth_num;

	//初始化libevent
	//event_init()
	struct event_base *base = event_init();
	_base = base;
}

void Tcpserver::run()
{
	cout << "run" << endl;

	//创建线程
	create_pth();

	//将监听套接子libevent
	struct event* socket_event = event_new(_base, _listen_fd, EV_READ | EV_PERSIST, sockfd_cb, this);
	event_add(socket_event, NULL);

	//循环监听
	cout << "循环监听base=" << _base << endl;
	event_base_dispatch(_base);
	cout << "退出循环base=" << _base << endl;
}

void sockfd_cb(int fd, short event, void *argc)
{
	/*
	 * sockfd上有数据时 表示客户端connect了
	 * 那么服务器就accept 并将accept返回的clfd通过
	 * _pth_num_map查表 找到num最小的 也就是监听量最少的子线程
	 * 通过sockpair0端发送clfd过去
	 */
	cout << "sockfd_cb" << "sockfd = " << fd << endl;

	tcpserver* p = (tcpserver*)argc;
	struct event_base* base = p->get_base();
	map<int, int> * pmap = p->get_pth_num_map();
	struct sockaddr_in cli_addr;
	int addrlen = sizeof(cli_addr);
	int clfd;
	if ((clfd = accept(fd, (struct sockaddr *)&cli_addr, (unsigned int *)&addrlen)) != -1)
	{
		/*
		 * 选择一个合适的线程 将clfd发给它
		 */
		map<int, int>::iterator it = pmap->begin();
		int socketpairfd = it->first;
		int num = it->second;
		for (; it != pmap->end(); ++it)
		{
			if (it->second < num)
			{
				socketpairfd = it->first;
				num = it->second;
			}
		}
		cout << "主线程发送fd给其他线程" << "clfd = " << clfd  << "socketpairfd = " << socketpairfd << endl;
		write(socketpairfd, &clfd, sizeof(clfd));
	}
}

//初始化_pth_num_map 创建线程 将socketpair0端加入到_base中
void Tcpserver::create_pth()
{
	cout << "void Tcpserver::create_pth()" << endl;

	vector<pthread_t> pid(_pth_num);
	for (int i = 0; i < _pth_num; ++i)
	{
		/*先调用socketpair获得双向管道，然后开启线程，新线程监听fd[1]
		 获得需要进行处理的主线程传过来的clfd 主线程则通过fd[0]传数据
		 */
		int fd[2];//fd[0]给主线程 fd[1]给子线程
		int res = socketpair(AF_UNIX, SOCK_STREAM, 0, fd);
		assert(res != -1);

		cout << "fd[0] = " << fd[0] << "fd[1] = " << fd[1] << endl;

		struct event* socket_event = event_new(_base, fd[0], EV_READ | EV_PERSIST, socketpair0_cb, this);
		event_add(socket_event, NULL);

		//初始化_pth_num_map
		_pth_num_map[fd[0]] = 0;

		/*
		创建一个新的线程，然后让新线程监听fd[1]
		 */
		res = pthread_create(&pid[i], NULL, newthread, (void *)fd[1]);
		assert(res == 0);
	}
}

void socketpair0_cb(int fd, short event, void *argc)
{
	/*
	 * socketpair0上有数据 说明子线程要把更新的
	 * 监听的文件描述符个数 更新_pth_num_map
	 */
	cout << "socketpair0_cb" << "socketpair0 = " << fd << endl;

	tcpserver* p = (tcpserver*)argc;
	map<int, int> * pmap = p->get_pth_num_map();
	int num;
	int res = read(fd, &num, sizeof(int));
	assert(res == 4);

	(*pmap)[fd] = num;
}
