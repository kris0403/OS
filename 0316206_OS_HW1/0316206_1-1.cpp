#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <vector>
using namespace std;
int main(int argc,char** argv)
{
	int pa_id;
	cout<<"Main proccess ID :"<<getpid()<<endl;
	cout<<endl;
	pid_t p_id;
	pid_t ch_id;
	
	for(int i = 1;i < 4;i++)
	{	
		int k;	
		p_id = fork();
		if(p_id < 0)
		{
			cout<<"error in fork!"<<endl;
		}	
		else if(p_id == 0)//get parent_id
		{		
			cout<<"fork "<<i<<". I'm the child "<<getpid()<<" ,my parent is "<<getppid()<<endl;
		}
		
		else//get child_id
		{
			wait(0);				 
			cout<<"fork "<<i<<". I'm the parent "<<getpid()<<" , my child is "<<p_id<<endl;		
		}
	}
}

