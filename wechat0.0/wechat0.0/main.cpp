#include <iostream>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include "tcpserver.h"
using namespace std;

int main(int argc,char **argv)
{
	if(argc < 4)
	{
		cerr<<"arg not enough;errno:"<<errno<<endl;
		return 0;
	}

	if (strlen(argv[1]) > 15)
	{

		cerr<<"ip is erro;errno:"<<errno<<endl;
        return 0;
	}

	//解析参数   ip   port   pth_num
	char ip[16];
	memcpy(ip, argv[1], strlen(argv[1]) + 1);
	int port = atoi(argv[2]);
	int pth_num = atoi(argv[3]);

	cout << "ip = " << ip << endl;
	cout << "port = " << port << endl;
	cout << "pth_num = " << pth_num << endl;


	//构造服务器对象
	cout << "构造服务器对象" << endl;
	tcpserver ser(ip,port,pth_num);

	//运行服务器
	cout << "运行服务器" << endl;
	ser.run();

	return 0;
}
