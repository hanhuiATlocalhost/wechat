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

	int rdlen;
	char buf[BUFLEN + 1] = { 0 };
	if ((rdlen = recv(fd, buf, BUFLEN, 0)) > 0)
	{
		Json::Value val;
		Json::Reader reader;
		res = reader.parse(buf, val);
		cout << "type:" << val["type"].asString() << endl;

		if (val["type"].asString() == "1")//登录
		{
			//先找mysql里面wechat有没有这个用户 密码对不对
			MYSQL *mpcon = mysql_init((MYSQL *)0);
			MYSQL_RES *mp_res;
			MYSQL_ROW mp_row;
			res = mysql_real_connect(mpcon, "127.0.0.1", "root", "123456", NULL, 3306, NULL, 0);
			assert(res == 0);
			mysql_select_db(mpcon, "stu");
			char cmd1[100] = "select *from user where name='";
			strcat(strcat(cmd1, val["name"]), "';");
			if (0 == mysql_real_query(mpcon, cmd, strlen(cmd)))
			{
				//接收返回的指令
				mp_res = mysql_store_result(mpcon);

				mp_row = mysql_fetch_row(mp_res);
				if (mp_row == 0)
				{
					//没有该用户
				}
				else
				{
					//比较密码
					res = strcmp(val["pw"].asString(), mp_row[1]);
					if (res != 0)
					{
						//密码不正确
					}
					else
					{
						//如果密码正确就访问online
						char cmd1[100] = "select * from online where name ='";
						strcat(cmd1, name);
						strcat(cmd1, "';");
						//密码正确 访问online表
						if (mysql_real_query(mpcon, cmd1, strlen(cmd1)) == 0)
						{
							mp_res = mysql_store_result(mpcon);
							mp_row = mysql_fetch_row(mp_res);


						}
					}
				}
			}

			
			mysql_real_query(mpcon, cmd, strlen(cmd));

		}
	}
	else
	{
		//取消注册的fd对应的事件
		map<int, struct event *> *pmap = p->get_clfd_event_map();
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
