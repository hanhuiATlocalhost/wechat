#ifndef CHILDTHREAD_H_
#define CHILDTHREAD_H_

#include <map>
using namespace std;

typedef class childthread
{
public:
	childthread(int fd);
	void run();

	struct event_base* get_base()
	{
		return _base;
	}

	map<int, struct event*> *get_clfd_event_map()
	{
		return &_clfd_event_map;
	}
private:
	struct event_base* _base;
	int _socketpair1;
	map<int, struct event*> _clfd_event_map;//accept返回的clfd与struct event* 的map表
}Childthread;

#endif /* CHILDTHREAD_H_ */
