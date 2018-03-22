//client
#include<iostream>
#include<string.h>
#include<string>
#include<json/json.h>
#include<errno.h>
#include<arpa/inet.h>
#include<signal.h>
using namespace std;
#define NAME_SIZE 20
#define PW_SIZE 20
#define BUFF_SIZE 256


const int fd = socket(AF_INET,SOCK_STREAM,0);




void view_exit()
{

	}
void cli_exit()
{
	//told tcp
	char res[10] = "end";
	send(fd,res,strlen(res),0);
	close(fd);
}
void view_talk_one()
{
	cout<<"talkone"<<endl;
}
void menu()
{
	char name[NAME_SIZE] = {0};
	char pw[PW_SIZE] = {0};
	while(1)
	{
		char choose[10] = {0};
		cout<<"welcome to chat"<<endl;
		cout<<"make your choose in number"<<endl;
		cout<<"1.register"<<endl;
		cout<<"2.login"<<endl;
		cout<<"3.exit"<<endl;
		cin>>choose;

		if(strcmp(choose,"1") == 0)//register
		{
			cout<<"name:";
			cin>>name;
			cout<<"passworld:";
			cin>>pw;
			//create json package
			Json::Value val;
			val["NAME"] = name;
			val["PW"] = pw;
			val["TYPE"] = 0;

			send(fd,val.toStyledString().c_str(),strlen(val.toStyledString().c_str()),0);

			char buff[100] = {0};

			if(0 < recv(fd,buff,99,0))
			{
				Json::Value val2;
				Json::Reader read;
				if(read.parse(buff,val2) == -1)
				{
					cout<<"json read fail"<<endl;
				}
				cout<<val2["BUFF"].asString()<<endl<<endl;
			}
		}
		if(strcmp(choose,"2") == 0)//login
		{
			cout<<"name:";
			cin>>name;
			cout<<"passworld:";
			cin>>pw;
			//create json package
			Json::Value val;
			val["NAME"] = name;
			val["PW"] = pw;
			val["TYPE"] = 1;

			send(fd,val.toStyledString().c_str(),strlen(val.toStyledString().c_str()),0);

			char buff[100] = {0};

			if(0 < recv(fd,buff,99,0))
			{
				Json::Value val2;
				Json::Reader read;
				if(read.parse(buff,val2) == -1)
				{
					cout<<"json read fail"<<endl;
				}
				if(val2["RESPOND"].asInt() != 0)
				{
					if(val2["RESPOND"].asInt() == 1)
					{
						cout<<val2["BUFF"].asString()<<endl<<endl;
					}
					if(val2["RESPOND"].asInt() == 3)
					{
						cout<<"login sucess"<<endl;
						cout<<"that's your offline respond"<<endl;
						cout<<val2["BUFF"].asString()<<endl<<endl;
					}
					//login sucess
					while(1)
					{
						char ch[10] = {0};
						cout<<"1.talk_one"<<endl;
						cout<<"2.offline"<<endl;
						cin>>ch;

						if(strcmp(ch,"1") == 0)//talk_one
						{
							view_talk_one();
						}
						if(strcmp(ch,"2") == 0)//exit
						{
							view_exit();
						}
					}
				}
				else if(val2["RESPOND"].asInt() == 0)
				{
					cout<<val2["BUFF"].asString()<<endl<<endl;
					continue;
				}

			}
		}
		if(strcmp(choose,"3") == 0)
		{
			cli_exit();
			break;
		}
	}
}
void fun(int sig)
{
	view_exit();
	signal(SIGINT,SIG_DFL);
}
int main()
{
	//int fd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == fd)
	{
		cerr<<"clifd creat fail;errnoz:"<<errno<<endl;
		return 0;
	}

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(5000);
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(-1 == connect(fd,(struct sockaddr*)&saddr,sizeof(saddr)))
	{
		cerr<<"clifd connect fail;errno:"<<errno<<endl;
		return 0;
	}


	signal(SIGINT,fun);
	menu();
	return 0;
}
