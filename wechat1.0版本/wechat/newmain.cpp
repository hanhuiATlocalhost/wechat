#include <iostream>
#include <cstdlib>
#include "childthread.h"
using namespace std;
/*
 * void *childthread(void *arg)相当于int main（）
 * 这是一个新的线程了
 */
void *newthread(void *arg)
{
	cout << "childthread" << endl;
	int socketpairfd = (long)arg;
	cout << "socketpair1 = " << socketpairfd << endl;

	//构造子线程对象
	childthread child_thd(socketpairfd);

	child_thd.run();

	return 0;
}
