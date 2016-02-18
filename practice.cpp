#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main()
{
  string S = "123.1";
  
  double result;

  istringstream convert( S );

  if( !( convert >> result ) )
    result =  0.0;

  result += 1;

  cout << result << endl;
  
  return 0;
}
