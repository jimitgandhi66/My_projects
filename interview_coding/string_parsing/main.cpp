#include<iostream>
#include<string>
#include<stdlib.h> 
#include<vector>
#include<unordered_map>
using namespace std;
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
	


class A
{
public:
  A()  { cout << "A's constructor called" << endl; }
};
 
class B
{
public:
  B()  { cout << "B's constructor called" << endl; }
};
 
class C: public A, public B  // Note the order
{
public:
  C()  { cout << "C's constructor called" << endl; }
};
int main()
{
   string s = "1,2,3,4,5,6,7,8,9";
   string delimiter = ",";
   string result;
   vector<int> arr;
   parser(s,delimiter,&arr);
   for (vector<int>::const_iterator itr = arr.begin();itr!=arr.end();itr++)
   {
	   cout<<(*itr)*2<<"Kya bolta h"<<endl;
   }
   C c;
}
	

