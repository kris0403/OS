#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
using namespace std;
int main(int argc,char** argv)
{
	cout<<"Main proccess ID :"<<getpid()<<endl;
	cout<<"--------------------------------------------"<<endl;
	int pipe_fd[3][2];
	long long int sum = 0;
	
	
	for(int i = 0;i < 3;i++)
	{	
		if(pipe(pipe_fd[i]) == -1)
		{
		cout<<"Error: Unable to create pipe."<<endl;
		}
		//pipe(pipe_fd[i]);
		pid_t p_id;
		p_id = fork();
		
		if (p_id < 0)
		{
			cout<<"error in fork!"<<endl;
		}	
		else if(p_id == 0)//child process
		{		
			
			cout<<"I'm the child "<<getpid()<<"."<<endl;
			cout<<"I can receive input! Please enter a number."<<endl;
			cout<<endl;
			close(pipe_fd[i][0]);
			dup2(pipe_fd[i][1], STDOUT_FILENO);
			close(pipe_fd[i][1]);
			execl("calculate.out","./calculate.out",(char*)0);					
		} 
		else//Parent Process
		{
			long long int gotans;
			close(pipe_fd[i][1]);
			//int len;
			wait(0);
			read(pipe_fd[i][0], &gotans, sizeof(long long int));
			cout<<endl;
			cout<<"The output number is "<<gotans<<"."<<endl;
			cout<<"--------------------------------------------"<<endl;	
			sum += gotans; 	
			//cout<<gotans;
			//cout<<sum<<endl;
		}	
	}
		
	cout<<"I'm the parent "<<getpid()<<"."<<endl;
	cout<<"Sum of results from three child processes : "<<sum<<endl;
	cout<<"--------------------------------------------"<<endl;
}
