#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
using namespace std;
class p_id
{
	public:
		p_id()
		{
			one_done = false;
			two_done = false;
			fifo = false;
			state = 0;
		}
		void set_fifo_false()
		{
			fifo = false;
		}
		void set_pid(int i)
		{
			pid = i;
		}
		void set_arr(int i)
		{
			arrival_time = i;
		}
		void set_cpu_1(int i)
		{
			cpu_1 = i;
		}
		void set_cpu_2(int i)
		{
			cpu_2 = i;
		}
		void set_io_time(int i)
		{
			io_time = i;
		}
		void true_1_done()
		{
			one_done = true;
		}
		void true_2_done()
		{
			two_done = true;
		}
		int get_arr()
		{
			return arrival_time;
		}
		int get_cpu1()
		{
			return cpu_1;
		}
		int get_cpu2()
		{
			return cpu_2;
		}
		int get_io_time()
		{
			return io_time;
		}
		int get_pid()
		{
			return pid;
		}
		bool get_1_done()
		{
			return one_done;
		}
		bool get_2_done()
		{
			return two_done;
		}
		void set_final_time(int i)
		{
			final_time = i;
		}
		int get_final_time()
		{
			return final_time;
		}
		bool get_fifo()
		{
			return fifo;
		}
		void true_fifo()
		{
			fifo = true;
		}
		void state_plus()
		{
			state++;
		}
		int get_state()
		{
			return state;
		}
		void initial_state()
		{
			state = 0;
		}
	private:
		int arrival_time;
		int cpu_1;
		int io_time;
		int cpu_2; 
		int pid;
		bool one_done;
		bool two_done ;
		int final_time;
		bool fifo;
		int state;
};
void sort_by_arrival_time(p_id *p,int num)
{
	for(int i = num - 1;i > 0;i--)//sort by arrival time
	{
		for(int j = 0;j < i;j++)
		{
			if(p[j].get_arr() > p[j+1].get_arr())
			{
				p_id temp;
				temp = p[j];
				p[j] = p[j+1];
				p[j+1] = temp;
			}
			else if(p[j].get_arr() == p[j+1].get_arr())
			{
				if(p[j].get_pid() > p[j+1].get_pid())
				{
					p_id temp;
					temp = p[j];
					p[j] = p[j+1];
					p[j+1] = temp;
				}
			}
		}
	}
}
void sort_by_final_time(p_id *p,int num)
{
	for(int i = num - 1;i > 0;i--)//sort by arrival time
	{
		for(int j = 0;j < i;j++)
		{
			if(p[j].get_final_time() > p[j+1].get_final_time())
			{
				p_id temp;
				temp = p[j];
				p[j] = p[j+1];
				p[j+1] = temp;
			}
		}
	}
}
int s_smallest(p_id *p,int num,int time)
{
	int small=10000000;
	int re;
	int id =10000000;
	for(int i=0;i<num;i++)
	{
		if(p[i].get_1_done() == true && p[i].get_2_done() == true)continue;
		else if(p[i].get_1_done() == false && p[i].get_cpu1() < small && p[i].get_arr() <= time )
		{
			small = p[i].get_cpu1();
			if(id < p[i].get_pid() && p[i].get_cpu1() == small)
			{
				id = p[i].get_pid();
				re = re;
			}
			else
			{
				re = i;	
			}
		}
		else if(p[i].get_1_done() == true && p[i].get_cpu2() < small && p[i].get_arr() <= time )
		{
			small = p[i].get_cpu2();
			if(id < p[i].get_pid() && p[i].get_cpu1() == small)
			{
				id = p[i].get_pid();
				re = re;
			}
			else
			{
				re = i;	
			}
		}
	}
	return re;
}
int select_smallest(p_id *p,int num,int time)
{
	int small=100000000;
	int re;
	int id = 100000000;
	for(int i=0;i<num;i++)
	{
		if(p[i].get_1_done() == true && p[i].get_2_done() == true)continue;
		else if(p[i].get_1_done() == false && p[i].get_cpu1() <= small && p[i].get_arr() <= time)
		{
			small = p[i].get_cpu1();
			if(id < p[i].get_pid() && p[i].get_cpu1() == small)
			{
				id = p[i].get_pid();
				re = re;
			}
			else
			{
				re = i;	
			}
			
		}
		else if(p[i].get_1_done() == true && p[i].get_cpu2() <= small && p[i].get_arr() <= time)
		{
			small = p[i].get_cpu2();
			if(id < p[i].get_pid() && p[i].get_cpu1() == small)
			{
				id = p[i].get_pid();
				re = re;
			}
			else
			{
				re = i;	
			}
			
		}
		
	}
	return small;
}
void s_j_f(p_id *p,int num)//shortest-job-first
{
	int time = 0;
 	int k;
 	sort_by_arrival_time(p,num);
	time = time + p[0].get_cpu1();
 	p[0].true_1_done();
 	p[0].set_arr(p[0].get_io_time() + time);
 	for(int i=1;i < 2*num;i++)
 	{
 		k = s_smallest(p,num,time);	
 		time += select_smallest(p,num,time);
		//cout<<p[k].get_pid()<<" "<<p[k].get_arr()<<" "<<time<<endl;	
		 if(p[k].get_1_done()==false)
			{
				p[k].true_1_done();
			 	p[k].set_arr(time + p[k].get_io_time());
			}		
		 else if(p[k].get_1_done()==true && p[k].get_2_done()==false)
			{
			 	p[k].true_2_done();
			 	p[k].set_final_time(time);
			}
	}
 		
 	sort_by_final_time(p,num);
}
bool check_1_done_2_done(p_id *p,int num)
{
	bool check = true;
	for(int i = 0;i<num;i++)
	{
		if(p[i].get_1_done() == false || p[i].get_2_done() == false)
		{
			check = false;
		}
	}
	return check;
}
void s_r_t_f(p_id *p,int num)//Shortest-Remaining-Time-First
{
	sort_by_arrival_time(p,num);
	int time = 0;
	time = time + 1;
	p[0].set_cpu_1(p[0].get_cpu1() - 1);
	int k;
	while(!check_1_done_2_done(p,num))
	{
		k = s_smallest(p,num,time);	
 		time += 1;
 		if(p[k].get_1_done()==false)
 		{
 			p[k].set_cpu_1(p[k].get_cpu1() - 1);
		 }
		 else if(p[k].get_1_done()==true && p[k].get_2_done()==false)
		 {
		 	p[k].set_cpu_2(p[k].get_cpu2() - 1);
		 }
		 
		 if(p[k].get_1_done()==false && p[k].get_cpu1() == 0)
			{
				p[k].true_1_done();
			 	p[k].set_arr(time + p[k].get_io_time());
			}		
		 else if(p[k].get_1_done()==true && p[k].get_2_done()==false && p[k].get_cpu2() == 0)
			{
			 	p[k].true_2_done();
			 	p[k].set_final_time(time);
			}
	}
	sort_by_final_time(p,num);
}
void m_f_q(p_id *p,int num)//Multilevel Feedback Queue
{
	sort_by_arrival_time(p,num);
	queue <int> fifo;
	queue <int> rr;
	int time = 0;
	while(!check_1_done_2_done(p,num))
	{
		for(int i=0;i<num;i++)
		{
			if(p[i].get_arr() == time && p[i].get_fifo()==false)
			{
				rr.push(i);
			}
		}
		for(int i=0;i<num;i++)
		{
			if(p[i].get_arr() == time && p[i].get_fifo()==true)
			{
				fifo.push(i);
			}
		}
		if(!rr.empty())//do rr
		{

			int temp = rr.front();
			
			if(p[temp].get_1_done()==false)//cpu 1
			{
				p[temp].set_cpu_1(p[temp].get_cpu1()-1);
				p[temp].state_plus();
				
				if(p[temp].get_cpu1() == 0)
				{
					p[temp].set_arr(p[temp].get_io_time() + time + 1);
					p[temp].true_1_done();
					p[temp].initial_state();
					rr.pop();
				}
				else if(p[temp].get_state()==4 && p[temp].get_cpu1() != 0)
				{
					p[temp].true_fifo();
					p[temp].initial_state();
					rr.pop();
					fifo.push(temp);
				}
				
			}
			else if(p[temp].get_1_done()==true)//cpu2
			{
				p[temp].set_cpu_2(p[temp].get_cpu2()-1);
				p[temp].state_plus();
				if(p[temp].get_cpu2() == 0)
				{
					p[temp].set_final_time(time + 1);
					p[temp].true_2_done();
					rr.pop();
				}
				else if(p[temp].get_state()==4 && p[temp].get_cpu2() != 0)
				{
					rr.pop();
					fifo.push(temp);
				}
			}	
		}
		else if(!fifo.empty())
		{			
			int fi=fifo.front();
			if(p[fi].get_1_done()==false)//cpu 1
			{	
				p[fi].set_cpu_1(p[fi].get_cpu1()-1);
				if(p[fi].get_cpu1() == 0)
				{
					p[fi].set_arr(p[fi].get_io_time() + time + 1);
					p[fi].true_1_done();
					
					fifo.pop();
				
				}
			}
			else if (p[fi].get_1_done()==true)//cpu 2
			{
				p[fi].set_cpu_2(p[fi].get_cpu2()-1);
				if(p[fi].get_cpu2() == 0)
				{
					p[fi].set_final_time(time + 1);
					p[fi].true_2_done();
					fifo.pop();
				}
			}
			
		}
		
		time++;
	}	
	sort_by_final_time(p,num);
}
int main(int argc,char** argv)
{
	fstream file;
	file.open("input.txt");
	if (!file){
		cout << "Fail to open file: " << "input.txt" << endl;
	}
	int problem;
	file>>problem;
	int num;
	file>>num;
	p_id p[100];
		int temp;
	for(int i = 0;i < num;i++)
	{
		file>>temp;
		p[i].set_pid(temp);
		file>>temp;
		p[i].set_arr(temp);
		file>>temp;
		p[i].set_cpu_1(temp);
		file>>temp;
		p[i].set_io_time(temp);
		file>>temp;
		p[i].set_cpu_2(temp);
	}
	file.close();
	if(problem == 1)
	{
		s_j_f(p,num);
	}
	else if(problem == 2)
	{
		s_r_t_f(p,num);
	}
	else if(problem == 3)
	{
		m_f_q(p,num);
	}
	else
	{
		cout<<"error!!"<<endl;
	}
	fstream final_file;
	final_file.open("0316206.txt", ios::out);
	for(int i=0;i<num;i++)
	{
		final_file<<p[i].get_pid()<<":"<<p[i].get_final_time()<<endl;
	}
	final_file.close();
}
