#include "mdp-simulation.h"
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <iterator>
#include <time.h>
#include <random>
#include <fstream>
#include <map>
using namespace std;

vector<vector<float> > generateQTable(int numStates, int numActions)
{
	vector<vector<float> > qTable(numStates, vector<float>(numActions));
 

  
  vector<vector<int> > numVisits(numStates, vector<int>(numActions));
  ofstream myFile;
  myFile.open("rewards.txt");
  float gamma = 0.9;
  int iterEpisode = 0;
  float epsilon, alpha;
  int numEpisodes = 100000;
	while (iterEpisode < numEpisodes)
  {
    State nextState = State(rand()%MAX_GRID, rand()%MAX_GRID);
    //cout<<nextState.x<<nextState.y<<endl;
    int iterSteps = 0;
    int sumRewards = 0;
    int act;
    epsilon = 1.0*(numEpisodes - iterEpisode)/(1.0*numEpisodes);
    double randomEpsilon = 0;
    //cout<<epsilon<<"    "<<iterEpisode<<endl;
    while (iterSteps < 100)
    {
      State currentState = nextState;
      int currentID = MAX_GRID * currentState.x + currentState.y;
      randomEpsilon = (double)rand()/((double)RAND_MAX);
      
      if (randomEpsilon < epsilon)
        {act = rand()%numActions;
       //cout<<act<<endl;
//cout<<epsilon<<endl;}
       //   cout<<"here2"<<iterEpisode<<endl;}
    
       else
       { act = max_element(qTable[currentID].begin(), qTable[currentID].end()) - qTable[currentID].begin();
         //   cout<< "here 11"<<endl;}
   //cout<<act<<endl;
}
      alpha = 1.0/(1 + numVisits[currentID][act]);
      sumRewards = sumRewards + my_reward(currentState);
      //cout<<my_reward(currentState)<<endl;
      nextState = my_next_state(currentState, Action(act));
      int nextID = MAX_GRID * nextState.x + nextState.y;
      float sample = my_reward(currentState) + gamma * (*max_element(qTable[nextID].begin(), qTable[nextID].end()));
 //     cout<<sample<<endl;
      qTable[currentID][act] = (1-alpha) * qTable[currentID][act] + alpha * sample;
      numVisits[currentID][act]++; 
      iterSteps++;
    }
    myFile<<sumRewards<<"\n";
    iterEpisode++;
  }
  myFile.close();
 // print qtable
  for (int i = 0; i < numStates ; i++)
  {
    for (int j = 0; j < numActions ; j++)
{     
 cout<<qTable[i][j]<<"  ";
}
    cout<<endl;
  }
  return qTable;
}

vector<vector<Action> > generatePolicy(vector<vector<float> > qTable)
{
  vector<vector<Action> > optimalPolicy(MAX_GRID, vector<Action>(MAX_GRID));
  for (int i =0; i<qTable.size(); i++)
  {
    int x = i/MAX_GRID;
    int y = i%MAX_GRID;
    int act = max_element(qTable[i].begin(), qTable[i].end()) - qTable[i].begin();
    optimalPolicy[x][y] = Action(act);
  }
  for (int i = 0; i < MAX_GRID; i++)
  {
    for (int j = 0; j < MAX_GRID; j++)
    {
      if (optimalPolicy[i][j] == 0)
        cout<<"N"<<"  ";
      else if (optimalPolicy[i][j] == 1)
        cout<<"S"<<"  ";
      else if (optimalPolicy[i][j] == 2)
        cout<<"E"<<"  ";
      else if (optimalPolicy[i][j] == 3)
        cout<<"W"<<"  ";
    }
    cout<<endl;
  } 
  return optimalPolicy;
}

int main (void)
{
  //State nextState = my_next_state(State(0, 0), N);
  //cout << "From 0, 0 to " << nextState.x  << ", " << nextState.y << endl;
  //Reward reward = my_reward(State(1, 1));
  //cout << "Receiving reward " << reward << endl;
  srand(time(NULL));
  int numStates = MAX_GRID * MAX_GRID;
  int numActions = 4;
  vector<vector<float> > qTable(numStates, vector<float>(numActions));
  qTable = generateQTable(numStates, numActions);
  vector<vector<Action> > optimalPolicy;
  optimalPolicy = generatePolicy(qTable);
  cout<<"Here"<<endl;

  return 0;
}
