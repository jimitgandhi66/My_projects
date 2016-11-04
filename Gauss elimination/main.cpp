#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
using namespace std;




void display_matrix(vector<vector<double> > *matrixA, int N)
{
	for(int i = 0; i<N;i++)
	{
      for (int j = 0; j<N; j++)
	  {	
		cout<<(*matrixA)[i][j]<<"  ";
	  }
	  cout<<endl;
    }
}


void swap(int i, int k , vector< vector< double> > *matrixA, int N,vector<double> *b)
	{
		for(int j=0;j<N; j++)
		{
			double temp = (*matrixA)[i][j];
			(*matrixA)[i][j] = (*matrixA)[k][j];
			(*matrixA)[k][j] = temp;
		}
		double temp_b = (*b)[i];
		(*b)[i] = (*b)[k];
		(*b)[k] = temp_b;
		
	}
	

void partial_pivoting(vector<vector<double> > *matrixA, int N, vector<double> *b)
{
	//Finds maximum element and row index corresponding to the maximum element
	for (int i = 0; i <N; i++)
	{
		double max_element = (*matrixA)[i][i];	
		int max_row = i;
		for(int k =i; k<N;k++)
		{
			if((*matrixA)[k][i] > max_element)
			{
				max_element = (*matrixA)[k][i];
				max_row = k;				
			}
		}
      swap(i,max_row,matrixA,N,b);
	  
    }
	
}


void perform_foward_elimination(vector<vector<double> > *matrixA, int N, vector<double> *b)
{
	for(int i = 0;i<N-1; i++)
	{
		for (int j= i+1;j<N; j++)
		{
			double coeff = -((*matrixA)[j][i])/ ((*matrixA)[i][i]);
			for (int k =0;k< N ; k++)
			{
				
					(*matrixA)[j][k]+= coeff*(*matrixA)[i][k];
				
			}
			(*b)[j] += coeff* (*b)[i];
		}
		
	}
}

void back_substitution(vector<vector<double> > *matrixA, int N, vector<double> *b,vector<double> *result)
{

	for (int k = N-1 ; k>=0; k--)
	{
	  (*result)[k] = (*b)[k];
	  for (int j = k+1; j<=N-1; j++ )
	  {
		    (*result)[k] -= (*matrixA)[k][j] * (*result)[j];
	  }
	  
	  (*result)[k] = (*result)[k]/(*matrixA)[k][k];
	}
}

int main()
{
  string filename;
  //cout<<"Please enter a name  "<<endl;
 // cin>>filename;
  filename = filename+".txt";
  //cout<<"Opening file  "<<filename<<endl;
 	ifstream myfile;
	string s;
	int count=0;
    myfile.open("test.txt");
	while(!myfile.eof()) 
        {
	        getline(myfile,s); 
            count++; 
        }
myfile.close();

int N =3;
vector<double> b;


vector<vector<double> > A;
A.resize(N);

for (int i =0 ; i<N ; i++)
{
	A[i].resize(N);
}

A[0][0] = 1;
A[0][1] = 2;
A[0][2] = 3;
A[1][0] = 4;
A[1][1] = 5;
A[1][2] = 6;
A[2][0] = 1;
A[2][1] = 0;
A[2][2] = 1;
b.push_back(1.0);
b.push_back(1.0);
b.push_back(1.0);
vector<double> X;
X.resize(3);
partial_pivoting(&A,N,&b);
perform_foward_elimination(&A, N,&b);
display_matrix(&A,N);
back_substitution(&A,N,&b,&X);

for(int i=0;i<N;i++)
{
	cout<<X[i]<<"  ";
}
 }



	
	







void parser(std::string &str,std::string &delimiter, vector<int> *result)
{ 
  string res;
  size_t pos = 0;
while((pos= str.find(delimiter)) != string::npos)	
{
	
	res = str.substr(0,pos);
	cout<<atoi(res.c_str())<<endl;
	result->push_back(atoi(res.c_str()));
	str.erase(0, pos + delimiter.length());
}
res = str.substr(0,str.length());
//cout<<atoi(res.c_str())<<endl;
result->push_back(atoi(res.c_str()));
}