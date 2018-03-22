#include<json/json.h>
#include"view_talk_one.h"
#include<iostream>
#include<arpa/inet.h>
#include<mysql/mysql.h>
#include<string>
#include<string.h>
#include<stdio.h>
using namespace std;

void view_talk_one::process(Json::Value val,int cli_fd)
{
	_cli_fd = cli_fd;
	MYSQL *mpcon = mysql_init((MYSQL *)0);
	MYSQL_RES *mp_res;
	MYSQL_ROW mp_row;
	
	//访问usr表    
	char name[20] = {0};
	string s1 = val["NAME"].asString();
	strcpy(name,s1.c_str());
	char cmd[100] = "select * from usr where name='";	
	strcat(cmd,name);
	strcat(cmd,"';");

	if(mysql_real_query(mpcon,cmd,strlen(cmd)))//查询失败
	{
		char respond[20] = "user name not exist";
		send(cli_fd,respond,sizeof(respond),0);	
		return;
	}
	//访问在线表
	char cmd1[100] = "select * from online where name='";
	strcat(cmd1,name);
	strcat(cmd1,"';");
	if(mysql_real_query(mpcon,cmd1,strlen(cmd1)))
	{
		//如果不在online表就将消息添加到离线表
		char cmd2[100] = "insert into offline values('";
		strcat(cmd2,name);
		strcat(cmd2,"','");
		string s2 = val["REASON"].asString();
		strcat(cmd2,s2.c_str());
		strcat(cmd2,"';");
		if(mysql_real_query(mpcon,cmd2,strlen(cmd2)))
		{
			cout<<"reason insert to offline fail"<<endl;
		}

	}
	else
	{
		//如果在online表
		
	//接收返回的指令
	mp_res = mysql_store_result(mpcon);
	//返回1行
	mp_row = mysql_fetch_row(mp_res);
	//accept?
	
	
	int fd;
	sscanf(mp_row[0],"%d",&fd);
	
	send(fd,mp_row[1],strlen(mp_row[1]),0);
	
	
	}
}
/*void view_login::responce()
{
	send(string     _cli_fd);
}*/
