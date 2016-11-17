#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


void calculate_new_policy(vector<float> *V, vector<vector<float> > *T, float gamma, vector<float> *policy,vector<float> *rewards)
{
   vector<float> curr_policy = *policy;
   for(int i =0; i<3 ; i++)
    {
     float exp_val = 0;
     for(int j=0;j<3 ; j++)
      {
       exp_val += (*T)[curr_policy[i]][j] * (*V)[j];
      }
      (*V)[i] = (*rewards)[i] + gamma* exp_val;
     }
   vector<float> v;
   v.resize(3);
   for(int i=0;i<3;i++)
{  v[i] = 0;}


for(int x = 0 ; x<3 ; x++)
 {
   for(int y = 0 ; y<3;y++)
   {
    float exp_val = 0;
         for(int z = 0; z<3;z++)
            { 
               exp_val+= (*T)[y][z] * (*V)[z];
             }
     v[y]= (*rewards)[x] + gamma * exp_val;
   }
   (*policy)[x] = max_element(v.begin(),v.end())-v.begin();
  }
   } 
int main()
{
  float gamma = 0.5; //Discount factor
  vector<vector< float> > p_t;
  p_t.resize(3);
  for(int i =0;i <3 ; i++)
  {
     p_t[i].resize(3); 

  }
  p_t[0][0] = 0.05;
  p_t[0][1] = 0.05;
  p_t[0][2] = 0.9;
  p_t[1][0] = 0.25;
  p_t[1][1] = 0.5;
  p_t[1][2] = 0.25;
  p_t[2][0] = 0.01;
  p_t[2][1] = 0.02;
  p_t[2][2] = 0.97;
 
vector<float> rewards;
rewards.resize(3);
rewards[0] = 1;
rewards[1] = -1;
rewards[2] = 0;

vector<float> V_state;
V_state.resize(3);

vector<float> policy;
policy.resize(3);
policy[0] = 2;
policy[1] = 2;
policy[2] = 2;
calculate_new_policy(&V_state, &p_t,gamma, &policy, &rewards);
cout<<policy[0]<<"  "<<policy[2]<<"   "<<policy[0]<<"  :Final   policy"<<endl;
}
