#include <event.h>
#include <cerrno>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <iostream>
#include "childthread.h"
using namespace std;

//socketpair1上有数据时 调用的回调函数
void socketpair1_cb(int fd, short event, void *argc);

void clfd_cb(int fd, short event, void *argc);

Childthread::childthread(int fd)
{
	cout << "childthread()" << endl;
	_socketpair1 = fd;

	_base = event_init();
}

void Childthread::run()
{
	cout << "run()" << endl;

	struct event* socketpair1_event = event_new(_base, _socketpair1, EV_READ | EV_PERSIST, socketpair1_cb, this);
	event_add(socketpair1_event, NULL);
	event_base_dispatch(_base);
}

void socketpair1_cb(int fd, short event, void *argc)
{
	childthread *p = (childthread *)argc;
	struct event_base * base = p->get_base();
	map<int, struct event *> *pmap = p->get_clfd_event_map();

	int clfd;
	read(fd, &clfd, sizeof(int));

	struct event *clfd_event = event_new(base, clfd, EV_READ | EV_PERSIST, clfd_cb, argc);
	event_add(clfd_event, NULL);
	(*pmap)[clfd] = clfd_event;

	int size = pmap->size();
	write(fd, &size, sizeof(int));
}

#define BUFLEN 128
void clfd_cb(int fd, short event, void *argc)
{
	cout << "clfd_cb" << endl;

	childthread *p = (childthread *)argc;
	map<int, struct event *> *pmap = p->get_clfd_event_map();


	int rdlen;
	char buf[BUFLEN + 1] = { 0 };
	if ((rdlen = recv(fd, buf, BUFLEN, 0)) > 0)
	{
		printf("%s", buf);
		memset(buf, 0, sizeof(buf));
	}
	else
	{
		//取消注册的fd对应的事件
		struct event * sock_event = (*pmap)[fd];
		cout << sock_event << endl;
		event_del(sock_event);
		map<int, struct event *>::iterator it = pmap->find(fd);
		if (it != pmap->end())
		{
			pmap->erase(it);
		}
		else
		{
			cerr << "_clfd_event_map表没有clfd" << endl;
		}

		//关闭套接字
		close(fd);
	}
}
