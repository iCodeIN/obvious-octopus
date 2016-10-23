#include <iostream>
#include <map>

#include "distribution.hpp"
#include "ismoothdistribution.hpp"

using namespace numeric;
using namespace std;

int main()
{
   map<string,double> m;
   m["apple"] = 2.0;
   m["banana"] = 4.0;
   m["citrus"] = 8.0;
   m["date"] = 16.0;

   auto m2 = Distribution<string,double>::relative(m);  

   cout << "relative distribution : " << endl;
   for(auto &pair : m2)
   {
      cout << pair.first << "\t" << pair.second << endl;
   } 
   cout << endl;   

   cout << "relative distribution (bottom 50%) : " << endl;
   auto m3 = Distribution<string,double>::left(m2, 0.5);
   for(auto &pair : m3)
   {
      cout << pair.first << "\t" << pair.second << endl;
   } 
   cout << endl;
   
   cout << "relative distribution (top 50%) : " << endl;
   auto m4 = Distribution<string,double>::right(m2, 0.5);
   for(auto &pair : m4)
   {
      cout << pair.first << "\t" << pair.second << endl;
   } 
   cout << endl;

   auto smoothD = ISmoothDistribution<string,double>(m2);
   for(auto &pair : m2)
   {
      cout << pair.first << "\t" << smoothD[pair.first] << endl;
   }
}
