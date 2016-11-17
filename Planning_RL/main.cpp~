#include "mdp-simulation.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <vector>
#include <fstream>
using namespace std;
#define MAX (9)


int MAX_ACTIONS =4;
int Total_Episodes = 100000;
State getInitialState()
{
  State coord;
 // srand (time(NULL));
  coord.x= rand()%10;
  coord.y = rand()%10;
return coord;
}


void display_matrix(vector<vector<double> > *matrixA, int N, int M)
{
	for(int i = 0; i<N;i++)
	{
      for (int j = 0; j<M; j++)
	  {	
		cout<<(*matrixA)[i][j]<<"  ";
	  }
	  cout<<endl;
    }
}

void display_matrix(vector<vector<int> > *matrixA, int N, int M)
{
	for(int i = 0; i<N;i++)
	{
      for (int j = 0; j<M; j++)
	  {	
		cout<<(*matrixA)[i][j]<<"  ";
	  }
	  cout<<endl;
    }
}



vector<Action> getallAction()
{
  vector<Action> action_list;
  for(int i = 0;i<4 ; i++)
{
  action_list.push_back(static_cast<Action>(i));
}
return action_list;
}

int get_Max_Qvalue(vector<vector<double> >*Q_matrix, int curr_id)
{
  vector<double> Q = (*Q_matrix)[curr_id];
  vector<double>::const_iterator temp = max_element(Q.begin(),Q.end());
  int max_Q = temp - Q.begin();

return max_Q;
}

void display_policy(vector<vector<double> > *matrixA , int N, int M)
{
  for(int i = 0;i<N;i++)
  {
    string temp;
    int j = get_Max_Qvalue(matrixA,i);
    if( i% MAX_GRID==0)
     { 
       cout<<endl;
      } 
     switch(j)
             {
              case 0: temp = "N";break;
              case 1: temp = "S";break;
              case 2: temp = "E";break;
              case 3: temp = "W";break;
              default: cout<<"error"<<endl;
             }
   cout<<temp<<"    ";  
}
}



int perform_episode(vector<vector<double> > *Q_matrix, vector<vector<int> > *visits, int Episode_num)
{
 
  double gamma = 0.9;
  State current_state = getInitialState();
 
  double totalReward = 0;
  
  double eps = 1.0*(Total_Episodes - Episode_num)/(1.0*Total_Episodes);
  
  for(int i =0; i <100; i++)
{
 // cout<<eps<<endl;
  double prob_random_action = (double)rand()/(double)RAND_MAX;
  Action action;
  int curr_id = current_state.x * MAX_GRID + current_state.y;
   
  if(prob_random_action < eps)
  {
  action = static_cast<Action>(rand()%MAX_ACTIONS);
  }
  else
  {

  action = static_cast<Action>(get_Max_Qvalue(Q_matrix, curr_id));

//cout<<action<<endl;
  }
   State next_state = 	my_next_state(current_state,action);	
   int next_id = next_state.x * MAX_GRID + next_state.y;
   float alpha = 1.0 / (1.0+(*visits)[curr_id][action]);
   int best_action = get_Max_Qvalue(Q_matrix, next_id);
   Reward reward = my_reward(current_state);
   totalReward += reward;
   double q = reward + gamma * ((*Q_matrix)[next_id][best_action]);
   (*Q_matrix)[curr_id][action] = ((1-alpha)* (*Q_matrix)[curr_id][action] ) + alpha * q;
   (*visits)[curr_id][action] +=1;
current_state = next_state;  
}
return totalReward;	
}


int main (void)
{
  vector< vector< double> > Q_matrix;
  vector< vector< int> > visits;
  int world_size = MAX_GRID;
 world_size = world_size*world_size;
  int max_actions = 4;
  Q_matrix.resize(world_size);
  visits.resize(world_size);
  for(int i=0;i<world_size; i++)
  {
    Q_matrix[i].resize(max_actions);
   visits[i].resize(max_actions);
    for(int j=0; j<max_actions;j++)
	{
           Q_matrix[i][j]=0;
           visits[i][j] = 0;
	}
  }

  


ofstream myFile;
myFile.open("plot.txt");

srand(time(NULL));
for(int i = 0; i < Total_Episodes; i++)
{

int reward = perform_episode( &Q_matrix,  &visits, i);

myFile<<reward<<"\n";
}
//display_matrix(&Q_matrix,world_size,max_actions);

//cout<<"here is visits"<<endl;
//display_matrix(&visits,world_size,max_actions);
display_policy(&Q_matrix,world_size,max_actions);
return 0;
}






/*vector<Action> getAction()
{
  vector<Action> actions;
srand(time(NULL));
  for(int i = 0; i<100;i++)
 {
    
   int temp = rand() % 4;
   Action a = static_cast<Action>(temp);
    actions.push_back(a);
 }
return actions;
}
*/


/*State getBestAction(State st, vector<Action> *actions, vector<vector<double> > *Q_matrix)
{
  double best_Q = 0;
State best_state = my_next_state(st,(*actions)[0]); 
 for(vector<Action>::const_iterator it= actions->begin(); it!= actions->end(); it++)
   { 
       State nextState = my_next_state(st, *it);
       if((*Q_matrix)[nextState.x][nextState.y] > best_Q)
          { 
           best_Q = (*Q_matrix)[nextState.x][nextState.y];
           best_state = nextState;
          }
   }
 return best_state;
}
*/
