#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <functional>
#include <queue>
#include <vector>
#include <unordered_map>
#include <cstdio>
#include <string>
#include <ctime>

using namespace std;

int N;

pair<int,int> node_to_config(int node)
{
	if(node>N*N)
	{
		cout<<"Out of bounds"<<endl;
	}
	int y = node % N;
	int x = node/N;
	pair<int,int> config;
	config.first = x;
	config.second = y;
	return config;
}
int config_to_node(pair<int,int> location)
{
	int x = location.first;
	int y = location.second;
	if(x<0 || y<0 || x>=N || y>=N)
	{
		cout<<"Out of Bounds"<<endl;
	}
	int node = x*N+y;
	return node;
}

class State_2d
{
public:
	pair <int,int> state;
	int cost;
	State_2d(pair<int,int> s)
	{
		state = s;
		cost = 100000;
	}
	State_2d()
	{
		state.first = -1; state.second=-1;
		cost = 10000;
	}
    //~State(void);
};

class State_3d
{
public:
	pair<int,int> state;
	pair <int,int> parent;
	int cost;
	int fcost;
	State_3d(pair<int,int> s)
	{
		state=s;
		parent.first = -1; parent.second=-1;
		cost = -1;
		fcost =-1;
	}
	State_3d()
	{
		state.first = -1 ; state.second =-1;
		parent.first = -1; parent.second=-1;
		fcost = -1;
		cost=-1;
	}
	vector< pair<int,int> > find_successor_3d();
    //~State(void);
};

vector< pair<int,int> > find_successor_2d(State_2d node)
{
	
	pair<int,int> curr_state;
	vector<pair<int,int> > successors;

	int x_moves[4]={0,0,1,-1};
	int y_moves[4] = {1,-1,0,0};
	//int curr_node = State.first;
	//pair<int,int> curr = node_to_config(curr_node);
	int curr_x = node.state.first;
	int curr_y = node.state.second;

	for(int i=0;i<4;i++)
	{
		int temp_x = curr_x+ x_moves[i];
		int temp_y = curr_y + y_moves[i];
		if(temp_x <0 || temp_x>= N ||temp_y <0 ||temp_y>=N)
		{
			//cout<<"out of bound move"<<endl;
		}
		else
		{
			pair<int,int> next_temp;
			next_temp.first = temp_x;
			next_temp.second = temp_y;
			//int node_id= config_to_node(next_temp);
			successors.push_back(next_temp);
		}
	}
	return successors;
	
}

vector< pair<int,int> > State_3d::find_successor_3d()
{
	int Moves[5]={0,N,-N,1,-1};
	int curr_pos = state.first;
	int curr_time = state.second;
	vector<pair<int,int> > successors;
	successors.reserve(10);
	int count =0;
	for(int i=0;i<5;i++)
	{
		int temp_pos = curr_pos+ Moves[i];
		int temp_time = curr_time + 1;
		if(temp_pos>N*N ||temp_pos<0)
		{
	     
		}
		else
		{
			pair<int,int> next_temp;
			next_temp.first = temp_pos;
			next_temp.second = temp_time;
			successors.push_back(next_temp);
		}
	}
	return successors;
	
}

void char_to_int(int* output, char* input)
{
	int index = 0;
	int output_index = 0;
	int count =0;

	if(!input[index])
	{
		cout<<"Error: Empty string"<<endl;
		return;
	}
	while(input[index-1]!=NULL)
	{
		if(input[index]==',' || input[index]==NULL)
		{
		  for(int i=index-count; i<index;i++)
		  {
			  output[output_index] += (input[i]-'0')* pow(10.0,index-i-1);			   
		  }
		  count =0;
		  output_index++;	
		}
		else
		{
			count++;
		}
		index++;
	}
}

struct pairhash {
public:
  template <typename T, typename U>
  std::size_t operator()(const std::pair<T, U> &x) const
  {
    return std::hash<T>()(x.first*N+x.second);
  }
};

auto cmp = [](State_2d left, State_2d right) { return left.cost > right.cost;}; 
auto cmp3 = [](State_3d left, State_3d right) { return left.fcost > right.fcost;};

void planner_dikstra(int* Map, int* heuristic,vector<State_2d*> Goals)
{
	cout<<endl<<"Running Dikstra for Hueristics...."<<endl;
	State_2d start;
	std::unordered_map<pair<int,int>, int, pairhash> closed_list;
	
	std::priority_queue<State_2d, std::vector<State_2d>, decltype(cmp)> open_list(cmp);
	start.state.first = 0;
	start.state.second = 0;
	start.cost = 0;
	for(vector<State_2d*>::const_iterator itr= Goals.begin();itr!=Goals.end();itr++)
	{
		State_2d g = **itr;
		g.cost = 0;
		open_list.push(g);
	}
	
     while(!open_list.empty())
	 {
	    State_2d Node;
	    Node = open_list.top();
		open_list.pop();
		auto check = closed_list.find(Node.state);
	if(check==closed_list.end())
	{
	   
	    closed_list[Node.state]= Node.cost;
		int index=N*Node.state.first + Node.state.second;
	    heuristic[index]= Node.cost;
		 vector<pair<int,int> > successors;
		 successors = find_successor_2d(Node);
		  
		   for(vector<pair<int,int> >::const_iterator iter= successors.begin(); iter!= successors.end();iter++)
		   {
			     auto search = closed_list.find(*iter);
		         if(search==closed_list.end())
				 {
			    	State_2d next;
				    next.state = *iter;
				    next.cost = Node.cost + Map[N*next.state.first+next.state.second];
				    open_list.push(next);					
			     }
			
	        }
	 }
	 }
	closed_list.clear();
}

vector<int> planner(State_3d start,vector<pair<int,int> > goals,int* Map,int* weights,int* heuristics,int* cost,int* nodes_expanded)
{
	vector<int> plan;
	std::unordered_map<pair<int,int>,pair<int,int>,pairhash> closed_list;	
	std::priority_queue<State_3d, std::vector<State_3d>, decltype(cmp3)> open_list(cmp3);
    std::unordered_map<pair<int,int>,int,pairhash> opened_list_cost;	
	vector<pair<int,int> > *successors;
	successors->reserve(10);	  
	int time_limit = goals.size();
	start.cost = 0;
	start.parent.first  = -1;
	start.parent.second = -1;
	start.fcost = heuristics[start.state.first];
  	open_list.push(start);
	plan.push_back(start.state.first);
	int count=0;
    while(!open_list.empty())
	 {
		 successors->clear();
	     State_3d *Node = new State_3d();
	    *Node = open_list.top();
		 open_list.pop();
		 auto check = closed_list.find(Node->state);
	if(check==closed_list.end())
	{
	   closed_list[Node->state]= Node->parent;
	   vector<pair<int, int> >::const_iterator it;
	   it = find(goals.begin(),goals.end(),Node->state);
       if(it!=goals.end())
	   {
		   //Back Track to plan
		   cout<<"Plan Found"<<endl;
		   *cost = Node->cost;
		   *nodes_expanded = closed_list.size();
		   pair<int,int> current = Node->state;
		    plan.push_back(current.first);
		   while(current != start.parent)
		   {
			   plan.push_back(current.first);
			   current = closed_list.at(current);
		   }
		   break;
	   }
	   else
	   {
		   *successors = Node->find_successor_3d();
		   vector<pair<int,int> >::const_iterator iter= successors->begin();
		   for(iter; iter!= successors->end();iter++)
		   {
			    auto search_closed = closed_list.find(*iter);
				State_3d next;
				next.state = *iter;
				next.parent = Node->state;
				next.cost = Node->cost + Map[next.state.first];
				next.fcost = weights[0]*next.cost + weights[1]*heuristics[next.state.first];
				if(next.state.second<=time_limit)
				{
				if(search_closed ==closed_list.end() || opened_list_cost.at(next.state) >next.fcost )
					{
						open_list.push(next);
						opened_list_cost[next.state]= next.fcost;
					}
				}
			   }
		     successors->clear();
			}
	     }
	     }
       
	return plan;
}

void print_path(vector<int> path)
{
	vector<int>::const_iterator itr = path.end();
	itr--;
	cout<<" Path taken by robot to catch the target in [x , y] format"<<endl;
	for(itr; itr!=path.begin(); itr--)
	{
		pair<int,int> coord = node_to_config(*itr);
		cout <<"[ "<<coord.first<<" , "<<coord.second<<" ]"<<endl; 
	}

}

int main(int argc, char**argv)
{
	
 ifstream myInputFile;
 cout<<"Enter the name of the file without .txt extension "<<endl;
 string filename;
 cin>> filename;

 //Comment the lines below if filename is to be hardcoded entered manually
 filename= filename+".txt";
 myInputFile.open(filename);

  /* Uncomment the line below if you donot wish to enter the filename via terminal */
 
 // myInputFile.open("test.txt");


char** myChar = 0;
myChar =  new char*[10000];
int line =0;

 
 while(!myInputFile.eof())
 {
		 line++;
	  myChar[line] = new char[10000];
	 myInputFile>>myChar[line];
	 
 }
 int state=0;
 int world=0;
 int goals=0;
 int cost = 0;
 for(int j=1;j<=line;j++)
 {
	 if(*myChar[j]=='N')
	 {
		 world = j;
	 }
	 else if(*myChar[j]=='R')
	 {
		 state = j;
	 }
	 else if(*myChar[j]=='T')
	 {
		 goals= j;
	 }
	 else if(*myChar[j]=='B')
	 {
		 cost = j;
	 }
 }


 // World size

 char_to_int(&N,myChar[world+1]);

 //Robot Initial State
 int R_initial[2] = {0,0};
 char_to_int(R_initial,myChar[state+1]);
 pair<int,int> coord_R;
 coord_R.first= R_initial[0];
 coord_R.second = R_initial[1];
 int rid = config_to_node(coord_R);
 pair<int,int> R;
 R.first = rid;
 R.second = 0;

 //Goals or trajectory of the target
 vector<pair<int,int> > T;
int ind =0; 
 for(int k=goals+1;k<cost;k++)
 {
	 int P[2] ={0,0};
	 char_to_int(P,myChar[k]);
	 pair<int,int> temp;
	 pair<int,int> Temp_goal;
	 temp.first= P[0];
	 temp.second = P[1];
	 int id = config_to_node(temp);
	 Temp_goal.first = id;
	 Temp_goal.second = k-goals-1;
	 T.push_back(Temp_goal);
	
 }


 //Collect the cost for Map
 vector<int*> Cost_map;
 
 for(int k=cost+1;k<=line;k++)
 {
	 int *costs = new int[N];
	  for(int l=0; l<N;l++)
	 {
		 costs[l] = 0;
	 }
	 char_to_int(costs, myChar[k]);
	 Cost_map.push_back(costs);
 }

 int NodeId=0;
 
 for(vector<int*>::const_iterator it = Cost_map.begin(); it!= Cost_map.end(); it++)
 { 
 	  int* ct = *it;
	  for(int i=0;i<N;i++)
	  {
       pair<int,int> coord;
       coord = node_to_config(NodeId);
       int coord_x = coord.first;
       int coord_y = coord.second;
	   NodeId++;
	  }

 }
 int *Map = new int [N*N];
 int row_number=0;
  int **CostMap=0;
 

 for(vector<int*>::const_iterator it = Cost_map.begin(); it!= Cost_map.end(); it++)
 {
	 for(int i =0;i<N;i++)
	 {
		 int *ct = *it;
		 Map[row_number*N + i] = ct[i];
		// cout<<ct[i]<<"   "<<endl;
	 }
	 //cout<<endl;
	 row_number++;
 }


 
 

 State_3d goal_node(T[6]);
 vector<pair<int,int> > Goals_3d; 
 vector<State_2d*> Goals_2d; 
 for(vector<pair<int, int> >::const_iterator iter=T.begin();iter!=T.end();iter++)
 {
		pair<int,int> coord_2d;
		coord_2d = node_to_config(iter->first);
		State_2d *g2=new State_2d(coord_2d);
		Goals_3d.push_back(*iter);
		Goals_2d.push_back(g2);
 }
 State_3d start_node(R);
 
 clock_t start;
 double duration;



 int* heuristics = new int[N*N];

 cout<<"World Size = "<<N<<endl;
 cout<<"Initial State = [ "<<R_initial[0]<<" , "<<R_initial[1]<<" ]"<<endl;
  cout<<"Number of Goals=  "<<Goals_2d.size()<<endl; 
 
 //Run Dikstra for Heuristics
 start = std::clock();
 
 planner_dikstra(Map,heuristics,Goals_2d);

 duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
 cout<<"Time to run dikstra for Heuristics =  "<<duration<<"seconds"<<endl;




 //normal A-star without any weights
  start= clock();
  int *cost1 = new int;
  int *nodes_expanded1= new int;
 int weights[2] = {1,1};
 cout<<endl<<"Planner 1: Running A-Star...."<<endl;
 
 vector<int>path1 = planner(start_node, Goals_3d,Map, weights, heuristics,cost1,nodes_expanded1);
 
 duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
 cout<<"Complete path for planner 1 "<<endl;
 print_path(path1);
 cout<<endl<<"Time to run Normal A-star =  "<<duration<<" seconds"<<endl;
 cout<<"Number of steps to catch the target = "<< path1.size()<<endl;
 cout<<"Total Path cost = "<<*cost1<<endl;
 cout<<"Number of Nodes Expanded = "<<*nodes_expanded1<<endl<<endl;
 cout<<"---------------------------------------------------------------------------------------"<<endl;
 cout<<"Press any key to run the 2nd planner"<<endl; 
 system("PAUSE");
 cout<<"---------------------------------------------------------------------------------------"<<endl;


  //second planner is weighted A-star
  start= clock();
 weights[1] = 10;
 int *cost2 = new int;
 int *nodes_expanded2= new int;
 cout<<endl<<"Planner 2: Running Weighted A-Star...."<<endl;

  vector<int> path2= planner(start_node, Goals_3d,Map, weights, heuristics,cost2,nodes_expanded2);
 
 cout<<"Complete path for planner 2"<<endl;
 print_path(path2);
 duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
 cout<<endl<<"Time taken for Planner 2: Weighted A-star =  "<<duration<<" seconds"<<endl;
 cout<<"Number of steps to catch the target = "<< path2.size()<<endl;
 cout<<"Total Path cost = "<<*cost2<<endl;
 cout<<"Number of Nodes Expanded = "<<*nodes_expanded2<<endl<<endl;

 

  system("PAUSE");
}
