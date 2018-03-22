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

int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6000);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	int res = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
	assert(res != -1);

	char buf[BUFLEN + 1];
	while (1)
	{
		fgets(buf, BUFLEN, stdin);
		if (strncmp(buf, "end", 3) == 0)
		{
			break;
		}
		else
		{
			res = send(sockfd, buf, strlen(buf), 0);
			assert(res != -1);
		}
	}
	close(sockfd);

	exit(EXIT_SUCCESS);
}
