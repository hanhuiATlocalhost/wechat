#ifndef TCPSERVER_H_
#define TCPSERVER_H_
#include <map>
using namespace std;

typedef class tcpserver
{
	public:
		//用传进来的ip地址 socketfd绑定的段口号 线程数 初始化这个服务器对象
		tcpserver(char *ip,unsigned short port,int pth_num);
		void run();
		struct event_base* get_base()
		{
			return _base;
		}

		map<int,int> *get_pth_num_map()
		{
			return &_pth_num_map;
		}
	private:
		void create_pth();


		/*
		 * 下面是成员变量
		 */
		struct event_base* _base;//libevent
		int _listen_fd;//listenfd
		int _pth_num;//线程个数
		map<int,int> _pth_num_map;//线程监听数量的map表
}Tcpserver,*Ptcpserver;

#endif /* TCPSERVER_H_ */
