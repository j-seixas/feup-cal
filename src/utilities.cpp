#include <string>
#include <iostream>
using namespace std;
static string previous = "";

string nextName(string name){
	int sz = previous.size();
	if (sz == 0)
		previous = "A";
	else{
		if (previous[ sz-1 ] == 'Z' ){
			int i = sz-1;
			for (; i >= 0 ; i--){  //searches string for first char different from Z
				if ('Z' != previous[i]){
					previous[i]++;
					break;
				}
			}
			if (-1 == i)
				previous+="A";
			
			for(i=i+1; i<sz ; i++)
					previous[i] = 'A';
		}
		else
			previous[ sz - 1]++;
	}
	cout << "Generated |" << previous << "|\n";
	return previous;
}

string nextStreetName(){
	return nextName(previous);
}



