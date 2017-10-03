#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;
int main(int argc,char** argv)
{
	
	int n;
	cin>>n;
	long long int out = 1;
	if(n==0)
	{
		out = 1;	
	}
	else 
	{
		for(int i = 1;i <= n;i++)
		{
			out = out * i;
		}
	}
	//return out;
	write(STDOUT_FILENO,&out, sizeof(long long int));
	//return EXIT_SUCCESS;
}

