/***************************************************
/* Program Name: Processor.cpp 
/* Description: This program simulates the Processor given in assignment-1 for OS
/* 
/* Creator: Trusit Shah(tss140030)
/* 
/* Created On: 02-18-2015 
/* Last Modified On: 02-26-2015
/*
****************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<sys/types.h>
#include <unistd.h>
#include<string.h>
#include<ctype.h>
#include<time.h>

using namespace std;

// pipe Definations 
int pipe_memory_to_cpu[2]; // pipe for the communication from memory to cpu
int pipe_cpu_to_memory[2]; // pipe for the communication from cpu to memory


/*********************************************************
/* Class Name: Memory
/* Description: This class is responsible for the memory operation read and write
/* 
/*********************************************************/

class Memory
{
	// memory array.
	int memory[2000];

	public:

	/****************************************************
	/*
	/* Function Name: Read
	/* Prototype: int Read(int index);
	/* Parameters: index- the memory location which you want to read
	/* Return: The value at the memory location.
	/*
	/*********************************************************/

	int Read(int index)
	{	
		return memory[index];
	}

	/****************************************************
	/*
	/* Function Name: Write
	/* Prototype: void Write(int index,int data);
	/* Parameters: index- the memory location which you want to write
	/*             data- data you want to write.
	/*
	/*********************************************************/

	void Write(int index,int data)
	{
		memory[index] = data;
	}

	/*********************************************
	/* Function Name : init
	/* Prototype     : int init(char * filename);
	/* Parameter     : filename 
	/* return        : 0 for success
	/*                 -1 for file not found
	/*
	/*********************************************/

	int Init(char * filename)
	{
		char line[30];
		FILE *f = fopen(filename,"r+");
		int address=0;
		char ch;

		if(f == NULL)
			return -1;

		fscanf(f,"%s",line);             //reading the first line.
		if(line[0] == '.')
		{
			address = atoi(line+1);
		}
		else if(line[0]!=0)
		{
			memory[address] = atoi(line);
			address++;
		}

		while(1)
		{
			fscanf(f,"%c",&ch);

			//waiting for the line to end then read the next instruction or next memory location pointer.

			if(ch == '\n')                             
			{
				fscanf(f,"%s",line);

				if(line[0] == 0)
					break;

				if(line[0] == '.')
				{
					address = atoi(line+1);
				}
				else if(line[0]>='0' && line[0]<='9')
				{
					memory[address] = atoi(line);
					address++;
				}

				line[0] = 0;
			}
		}
		return 1;
	}
};

class CPU
{
	// variables for the registers in CPU.
	int PC,SP,IR,X,Y,AC;
	// integer for the mode.
	int mode;
	// timer count at which timer interrupt should fire.
	int timer_count;
	// current count of instruction executed
	int inst_executed;
	// actives when we reach in timer intterupt to stop instruction counting inside it.
	int timer_mode;

	public:
	
	/***********************************************
	/* Function: Constructor of CPU class.
	/* Parameters: count- the timer_count value at which timer interrupt should fire.
	/* 
	/**********************************************/
	CPU(int count)
	{
		// Init all the registers and modes.
		PC = IR = X = Y = AC = 0;
		SP = 1000;		
		this->timer_count = count;
		inst_executed = 0;
		mode = 0;
		timer_mode = 0;
	}

	/*****************************************
	/*
	/* Function Name: Run()
	/* Descrition: This is the main function which will execute the all the instructions.
	/* 
	/***************************************/
	void run()
	{
		while(1)
		{
			Decode();
		}
	}
	
	/****************************************
	/*
	/* Function Name: Random()
	/* Description: This is random number generate
	/*
	/****************************************/ 

	int random()
	{
		int x;
		srand(time(0)); //setting up the seed.
		x = rand();
		return (x%100); //returns the value according to seed. the %100 will return the value from 0-99. 
	}
	
	/****************************************
	/*
	/* Function Name: Decode()
	/* Description:This will decode the instruction which means read all the memory related to the instruction.
	/*
	/***************************************/

	int Decode()
	{
		int inst = ReadMemory(PC);
		IR = inst; //puts the inst value in IR.

		//printf("instruction executing %d value of PC is %d value of SP is %d value of AC is %d\n",inst,PC,SP,AC);
		//getchar();		
		
		if(inst == 1)
		{
			PC++;
			Execute(inst,ReadMemory(PC));
		}
		else if(inst == 2)
		{
			PC++;
			Execute(inst,ReadMemory(ReadMemory(PC)));
		}
		else if(inst == 3)
		{
			PC++;
			Execute(inst,ReadMemory(ReadMemory(ReadMemory(PC))));
		}
		else if(inst == 4)
		{
			PC++;
			Execute(inst,ReadMemory(ReadMemory(PC)+X));
			
		}
		else if(inst == 5)
		{
			PC++;
			Execute(inst,ReadMemory(ReadMemory(PC)+Y));
		}
		else if(inst == 6)
		{
			Execute(inst,ReadMemory(SP+X));
		}
		else if(inst == 7)
		{
			PC++;
			WriteMemory(ReadMemory(PC),AC);
			Execute(inst);
		}
		else if(inst == 8)
		{
			Execute(inst);
		}
		else if(inst == 9)
		{
			PC++;
			Execute(inst,ReadMemory(PC));
		}
		else if(inst >=10 && inst<=19)
		{
			Execute(inst);
		}
		else if(inst >= 20 && inst<=22)
		{
			PC++;
			Execute(inst,ReadMemory(PC));
		}
		else if(inst == 23)
		{
			PC++;
			Execute(inst,ReadMemory(PC));
		}
		else if(inst == 24)
		{
			Execute(inst);
		}
		
		else if(inst == 25 || inst == 26)
		{
			Execute(inst);
		}
		else if(inst == 27)
		{
			Execute(inst);
		}
		else if(inst == 28)
		{
			Execute(inst);
		}
		else if(inst == 29)
		{
			Execute(inst);
		}
		else if(inst == 30)
		{	
			Execute(inst);
		}
		else if(inst == 50)
		{
			char temp_string[11];
			sprintf(temp_string,"3\n");
		
			write(pipe_cpu_to_memory[1], temp_string, (strlen(temp_string)+1));
			
			exit(0);
		}

	}

	/***********************************************************
	/* 
	/* Function Name: int ReadMemory(int address);
	/* Parameter: int address - address at which the value is being read.
	/* Description: This function will read data from particular memory address from the memory process.
	/*
	/************************************************************/
	
		// data format for the pipe communication 
		// for reading memory sends address followed by 1.
		// for writing memory sends address followed by 2.
		// to send the end of execution sends 3.
		
		// e.g. it sends "1123" to read the memory address 123.
		
	int ReadMemory(int address)
	{
		char readbuffer[11]; // buffer to store the data from pipe

		//checks for the user and kernel mode and gives error if in user mode you are accessing the address of kernel space.
		if(address>=1000 && mode == 0)        
		{
			printf("Error in memory read. User mode can't read %d memory\n",address);
			char temp_string[11];
			
			//sends the string to the memory that error is occured to terminate the program.
			sprintf(temp_string,"3\n");
			write(pipe_cpu_to_memory[1], temp_string, (strlen(temp_string)+1));

			exit(0);
		}

		char temp_string[11];
				
		sprintf(temp_string,"1%04d\n",address);
		
		//sends the data to the pipe to read the memory.
		write(pipe_cpu_to_memory[1], temp_string, (strlen(temp_string)+1));
		//read the pipe data sent from memory
		read(pipe_memory_to_cpu[0], readbuffer, sizeof(readbuffer));
		//convert the received data into integer.
		return (atoi(readbuffer));
	}
	
	/*************************************************
	/*
	/* Function Name: WriteMemory();
	/* Description: Writes memory to the address given. Sends data to the process of memory.
	/*
	/************************************************/
	
	void WriteMemory(int address,int data)
	{
		// checks the memory access violation.
		if(address>=1000 && mode == 0)
		{
			char temp_string[11];
			sprintf(temp_string,"3\n");
		
			write(pipe_cpu_to_memory[1], temp_string, (strlen(temp_string)+1));

			printf("Error in memory write. User mode can't write %d memory\n",address);
			exit(0);
		}

		char temp_string[13];
		//sends the address and data to the memory process.
		sprintf(temp_string,"2%04d-%05d\n",address,data);
		write(pipe_cpu_to_memory[1], temp_string, (strlen(temp_string)+1));

	}
	
	/*********************************************
	/*
	/* Function Name: push(int data)
	/* Parameters: int data - data to be push into stack.
	/* Description: pushes data into stack. First it will decrement the SP and then put the data into it.
	/*
	/*********************************************/
	void push(int data)
	{
		SP--;
		WriteMemory(SP,data);
	}
	
	/******************************************
	/*
	/* Function Name: pop()
	/* return value: returns the pop value.
	/* Description: It will pop the value from stack
	/*
	/*******************************************/
	int pop()
	{
		return ReadMemory(SP++);
	}

	/****************************************
	/*
	/* Function Name: interrupt_call()
	/* Description: This function will do the process which is done at interrupt calling.
	/*
	/****************************************/
	
	void interrupt_call()
	{
		mode = 1;
		stack_switch();
		push(PC+1);
		PC = 1500;
	}
	
	/*************************************
	/*
	/* Function Name: timer_call()
	/* Returns: Return 1 - if timer interrupt executes properly.
	/*          Return 0 - if timer interrupt is on hold if another interrupt is also in progress. 
	/* Description: This function perform the process done while timer interrupt.
	/*
	/***********************************/
	
	int timer_call()
	{
		if(mode == 1)
		{
			//inst_executed--;
			return 0;
		}		
		mode = 1;
		stack_switch();
		push(PC);
		PC = 1000;
		inst_executed = 0;
		timer_mode = 1;
		return 1;
	}
	
	/**************************************
	/* 
	/* Function Name: stack_switch();
	/* Description: Changes the stack from user to kernel.
	/*
	/************************************/
	void stack_switch()
	{
		int old_sp = SP;
		SP = 2000;
		push(old_sp);
	}

	/***********************************
	/*
	/* Function Name: interrupt_ret();
	/* Description: The the process which is done while IRET is called.
	/*
	/***********************************/
	void interrupt_ret()
	{
		PC = pop();
		SP = pop();
		mode = 0;
		timer_mode = 0;
	}

	/********************************
	/*
	/* Function Name: Execute(int inst,int val=0);
	/* Parameters: inst- instruction to be executed.
	/*             val- value related to the instruction.
	/* Description: Executes the instruction.
	/*
	/********************************/
	
	void Execute(int inst,int val=0)
	{
		int pc_change = 0;

		switch(inst)
		{
			case 1: AC = val; break;
			case 2: AC = val; break;
			case 3: AC = val; break;
			case 4: AC = val; break;
			case 5: AC = val; break;
			case 6: AC = val; break;
			case 7: PC++;pc_change = 1;break;
			case 8: AC = random(); break;
			case 9: if(val == 1) printf("%d",AC);
					else printf("%c",AC);
					//getchar();
					break;
			case 10: AC+=X;break;
			case 11: AC+=Y;break;
			case 12: AC-=X;break;
			case 13: AC-=Y;break;
			case 14: X = AC;break;
			case 15: AC = X;break;
			case 16: Y = AC;break;
			case 17: AC = Y;break;
			case 18: SP = AC;break;
			case 19: AC = SP;break;
			case 20: PC = val;pc_change = 1;break;
			case 21: if(AC==0){PC = val;pc_change = 1;}break;
			case 22: if(AC!=0){PC = val;pc_change = 1;}break;
			case 23: push(PC+1);PC = val;pc_change = 1;break;//call
			case 24: PC = pop();pc_change = 1;break;//ret
			case 25: X++;break;
			case 26: X--;break;
			case 27: push(AC);break;//push
			case 28: AC = pop();break;//pop
			case 29: interrupt_call();pc_change = 1;break;//interrupt
			case 30: interrupt_ret();pc_change = 1;break;//iret
			case 50: break;//end
		}
		
		// if pc is already is changed it should not be changed again. 
		if(pc_change == 0)
			PC++;
		
		// To stop counting the instruction during timer mode to avoid nested timer interrupts.
		if(timer_mode == 0)
			inst_executed++;
		
		// Raise the timer event.
		// reason for inst_executed>=timer_count
		// if when the timer interrupt occurred during interrupt it will be in waiting because processor don't
		// support interrupt nesting. At that time inst_executed will become more than timer_count.
		// And at the interrupt_ret the value of inst_executed become zero.
		
		if(inst_executed >= timer_count)
		{
			if(timer_call())
				pc_change = 1;
		}


	}
}; 

int main(int argc,char* argv[])
{
	//memory class
	Memory m;
	char readbuffer[13];

	int pid;	
	//init the memory at the beginning.
	int a = m.Init(argv[1]);
	
	if(a == -1)
	{
		printf("error can not able to find file");
		exit(0);
	}
	//pipes define.
	pipe(pipe_memory_to_cpu);
	pipe(pipe_cpu_to_memory);
	
	//fork new process of CPU
	pid = fork();

	if(pid == -1)
	{
		printf("Can't fork process. Try again later");
		exit(0);
	}
	//CPU process runs the CPU code.
	else if(pid == 0)
	{
		
		CPU cpu(atoi(argv[2]));
		cpu.run();	
	}

	else
	{
		// Memory process for memory code.
		while(1)
		{
			//reads the pipe from the CPU.
		    read(pipe_cpu_to_memory[0], readbuffer, sizeof(readbuffer));
			// read process.
			if(readbuffer[0] == '1')
			{
				int memory_address = (readbuffer[1]-48)*1000 + (readbuffer[2]-48)*100 + (readbuffer[3]-48)*10 + (readbuffer[4]-48);
				int temp = m.Read(memory_address);
				char temp_string[10];
				sprintf(temp_string,"%d\n",temp);
				write(pipe_memory_to_cpu[1], temp_string, (strlen(temp_string)+1));
			}
			//write process
			else if(readbuffer[0] == '2')
			{
				int memory_address = (readbuffer[1]-48)*1000 + (readbuffer[2]-48)*100 + (readbuffer[3]-48)*10 + (readbuffer[4]-48);
				int data = (readbuffer[6]-48)*10000 + (readbuffer[7]-48)*1000 + (readbuffer[8]-48)*100 + (readbuffer[9]-48)*10+readbuffer[10]-48;
				m.Write(memory_address,data);
			}
			// exit signal received.
			else if(readbuffer[0] == '3')
			{
				exit(0);
			}
		}
	}
}
