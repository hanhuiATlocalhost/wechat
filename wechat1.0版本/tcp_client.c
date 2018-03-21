#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>

#define BUFLEN 128
class 客户端类
{
public:
	客户端类构造(char *ip地址，short 端口号)
	{
		int sockfd = socket(AF_INET, SOCK_STREAM, 0);
		struct sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(short 端口号);
		addr.sin_addr.s_addr = inet_addr(char *ip地址);

		int res = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
		assert(res != -1);
	}
	~客户端析构（）
	{
		delete buf；;
	close(_sockfd);
	}

	void run()
	{
		type = int 选择类型（）；;//先选是登陆还是注册
		val["type"] = type;
		map[type]->process;
	}
	
private:
	int _sockfd;
	Json::Value val;
	char *buf = new char[128];
};

int main()
{
	客户端类 client（argv[1], argv[2]）;//实例化一个客户端类

	/*
	一开始只有登陆和注册界面
	也就是 选完登陆或者注册
	然后加上name 和password

	*/

	exit(EXIT_SUCCESS);
}
