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
class �ͻ�����
{
public:
	�ͻ����๹��(char *ip��ַ��short �˿ں�)
	{
		int sockfd = socket(AF_INET, SOCK_STREAM, 0);
		struct sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(short �˿ں�);
		addr.sin_addr.s_addr = inet_addr(char *ip��ַ);

		int res = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
		assert(res != -1);
	}
	~�ͻ�����������
	{
		delete buf��;
	close(_sockfd);
	}

	void run()
	{
		type = int ѡ�����ͣ�����;//��ѡ�ǵ�½����ע��
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
	�ͻ����� client��argv[1], argv[2]��;//ʵ����һ���ͻ�����

	/*
	һ��ʼֻ�е�½��ע�����
	Ҳ���� ѡ���½����ע��
	Ȼ�����name ��password

	*/

	exit(EXIT_SUCCESS);
}
