#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <map>
using namespace std;


/**
 * @brief  This function removes unwanted characters such as "  ", " + ". It removes the delimiters and returns a vector of strings
 * @param str - Input string to be processed
 * @param delimiter - Unwanted Character
 * @param result - A vector of strings that were separated by the unwanted character
 */
void parser(std::string &str,std::string &delimiter, vector<string> *result)
{ 
  string res;
  size_t pos = 0;
while((pos= str.find(delimiter)) != string::npos)	
{
	
	res = str.substr(0,pos);
	
	
	result->push_back(res.c_str());
	
	str.erase(0, pos + delimiter.length());
}
res = str.substr(0,str.length());

result->push_back(res.c_str());
}



/**
 * @brief  This parser separates an entire equation into LHS and RHS. Similar to parser function above
 * @param str - Equation in the string form.
 * @param delimiter - Here the delimiter is "=".
 * @param LHS - LHS of the equation.
 * @param RHS - RHS of the equation.
 */
void parser_separate_LHS_RHS(std::string &str,std::string &delimiter, vector<string> *LHS, vector<string> *RHS)
{ 
  string res;
  size_t pos = 0;
  
	while((pos= str.find(delimiter)) != string::npos)	
	{
	
		res = str.substr(0,pos);
	
		LHS->push_back(res.c_str());
	
		str.erase(0, pos + delimiter.length());
	}
	res = str.substr(0,str.length());
	//cout<<atoi(res.c_str())<<endl;
	RHS->push_back(res.c_str());
}


/**
 * @brief A tool to display matrix after the operations
 * @param matrixA - Any 2-dimensional square matrix 
 * @param N- Dimension of the matrix
 */
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

/**
 * @brief This function is used by Gaussian Elimination method. It swaps entire rows based on desired preference.
 * @param i - one of the row number to be swapped.
 * @param k - Other row to be swapped.
 * @param matrixA - Square matrix whose rows are to be swapped. This is LHS of the equation Ax=b.
 * @param N - Dimension of matrixA.
 * @param b - This is the RHS of the equation Ax=b.
 */
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
	
/**
 * @brief This is partial pivoting method of Gauss elimination. It uses swap function above to perform row swaps based on preference of maximum element in a column.
 * @param matrixA -Square matrix whose rows are to be swapped. This is LHS of the equation Ax=b.
 * @param N - Dimension of matrixA.
 * @param b -This is the RHS of the equation Ax=b.
 */
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

/**
 * @brief  This function performs foward elimination. This is an effort to reduce matrixA in to row-echolon form
 * @Souce http://mathworld.wolfram.com/GaussianElimination.html
 * @param matrixA- Square matrix whose rows are to be swapped. This is LHS of the equation Ax=b.
 * @param N - Dimension of matrixA.
 * @param b -This is the RHS of the equation Ax=b.
 */
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


/**
 * @brief Back substitution fo the row echelon form of matrix A
 * @source http://mathworld.wolfram.com/GaussianElimination.html
 * @param matrixA - Square matrix whose rows are to be swapped. This is LHS of the equation Ax=b.
 * @param N - Dimension of matrixA.
 * @param b -This is the RHS of the equation Ax=b.
 * @param result - Final solution of the equation
 */
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
  cout<<"Please enter the name of the file without extension .txt:  "<<endl;
  cin>>filename;
  filename = filename+".txt";
  //cout<<"Opening file  "<<filename<<endl;
 	ifstream myfile;
	string s;
 
	 myfile.open(filename.c_str());
	// myfile.open("test.txt");
	string delimiter = "=";
		vector<string> LHS;
		vector<string> RHS;
		vector<vector<string> > lhs_all;
		vector<vector<string> > rhs_all;
	map<string,int> variables;
	int count = 0;
	int N =0;
	while(!myfile.eof()) 
        {
	        getline(myfile,s); 
			s.erase(remove(s.begin(), s.end(), ' '), s.end());   //Removes unwanted spaces
			parser_separate_LHS_RHS(s,delimiter,&LHS,&RHS);   //Removes "=" and separates LHS from RHS
			string delimiter2 = "+";
			
			vector<string> temp2;
			parser(LHS[0], delimiter2,&temp2); //Removes "+" in LHS
			lhs_all.push_back(temp2);  //Store equation in a vector of lhs
			
			/** Below we iterate through all the values of LHS and check whether they are in the map of variables.
			 * If not then add them to the map. else do nothing
		      */
			for(vector<string>::const_iterator it = temp2.begin();it!= temp2.end();it++)
			{			
				char *c = const_cast<char*>(it->c_str());
				if(isalpha(c[0]))
				{
					if(variables.find(*it)==variables.end())
					{
					variables[*it] = count;
					 count++;
					}
				}
			}
            
			//Do same for every RHS
			vector<string> temp;
			parser(RHS[0], delimiter2,&temp);//Removes "+" in LHS
			
			/** Below we iterate through all the values of RHS and check whether they are in the map of variables.
			 * If not then add them to the map. else do nothing
		      */
			for(vector<string>::const_iterator it = temp.begin();it!= temp.end();it++)
			{			
				char *c = const_cast<char*>(it->c_str());
				if(isalpha(c[0]))
				{
					if(variables.find(*it)==variables.end())
					{
					variables[*it] = count;
					 count++;
					}
				}
			}
			rhs_all.push_back(temp);	//Store RHS in the list of other rhs
		    LHS.clear();
			RHS.clear();
			N++;
        }
		
		vector<double> b_values; //This is the vector b of Ax=b where we store all values of the RHS.
		vector<vector<double> > A_matrix; 
		//Initialize matrix A of Ax=b with all zeros and size N*N where N is the number of equations.
		A_matrix.resize(N);

for (int i =0 ; i<N ; i++)
{
	A_matrix[i].resize(N);
	for(int j=0; j<N;j++)
	{
		A_matrix[i][j]=0;
	}
}
				
		//Iterate through all the lhs and rhs equations and form the matrix A and vector b of Ax=b
		int iter = 0;
		for(vector<vector<string> >::const_iterator itl = rhs_all.begin();itl!= rhs_all.end();itl++)
			{
				vector<string> temp2;
				temp2 = *itl;
				
				double val = 0;
			for(vector<string>::const_iterator it = temp2.begin();it!= temp2.end();it++)
			{
				char *c = const_cast<char*>(it->c_str());
				if(isalpha(c[0]))
				{
					/*Check the position of the particular variable in the map we have and initialize it to -1 since 
					 * this variable is on the RHS and we desire all the variables to be on the LHS in Ax=b 
					 * */
				  int pos = variables[*it];
				  A_matrix[iter][pos] = -1;
				  
				}
				else
				{
					//If it is an integer then keep it in the list of b_values which is b vector of Ax = b.
					val += atoi(it->c_str());
				}
				
			}
			b_values.push_back(val);
			iter++;
			}
		


iter =0;
for(vector<vector<string> >::const_iterator itl = lhs_all.begin();itl!= lhs_all.end();itl++)
			{
				vector<string> temp2;
				temp2 = *itl;
				
			for(vector<string>::const_iterator it = temp2.begin();it!= temp2.end();it++)
			{
				//cout<<*it<<endl;
				char *c = const_cast<char*>(it->c_str());
				/*Check the position of the particular variable in the map we have and initialize it to 1 since 
					 * this variable is on the LHS and we desire all the variables to be on the LHS in Ax=b 
					 * */
				if(isalpha(c[0]))
				{
					int pos = variables[*it];
					A_matrix[iter][pos]= 1;
				}
			}
			iter++;
			}	
	
myfile.close();



/*THis is the final solution vector*/
vector<double> X;
X.resize(N);


/*Perform Guassian Elimination*/
partial_pivoting(&A_matrix,N,&b_values);
perform_foward_elimination(&A_matrix, N,&b_values);


/*Uncomment the following line to see the matrix after forward elimination*/
//display_matrix(&A_matrix,N);


back_substitution(&A_matrix,N,&b_values,&X);
map<double, string> final_result;// Final result with variable name

// Store each variable name with its corresponding value is a new map
for (map<string,int>::const_iterator itr = variables.begin();itr!=variables.end();itr++)
{
	string variable_name = itr->first;
	int index = itr->second;
	final_result[X[index]]= variable_name;
}

//Display variable and its final output.
for (map<double,string>::const_iterator itr = final_result.begin();itr!=final_result.end();itr++)
{
 cout<<itr->second<<"  = "<<itr->first<<endl;
}
 }



	
	






