#include "cute.h"
#include "ide_listener.h"
#include "cute_runner.h"
#include <fstream>


using namespace std;


float numApproximateStringMatching(string filename, string toSearch);


void KMPTable(const string &word , vector<int> &letters){
	int pos = 2 , cnd = 0;
	letters[0] = -1 , letters[1] = 0;
	for (unsigned int i = 2 ; i < word.length() ; i++){
		while( cnd > 0 && word[cnd] != word[pos] )
			cnd = letters[cnd-1];
		if (word[cnd] == word[i])
			cnd++;
		letters[i] = cnd;
	}
}

int KMPSearch(string filename , string toSearch){
	vector<int> words(toSearch.length());
	KMPTable(filename,words);
	int beg = 0 , pos_w = 0;

	while ( (beg + pos_w) < filename.length()) {
		if (toSearch[pos_w] == filename[beg+pos_w]){
			if( pos_w == (toSearch.length() - 1))
				return beg;
			pos_w++;
		}
		else{
			if ( words[pos_w] > -1){
				beg=beg+1-words[beg];
				pos_w = words[beg];
			}
			else{
				beg++;
				pos_w=0;
			}
		}
	}
	return filename.length();
}

int numStringMatching(string filename, string toSearch){
	ifstream in;
	string line;
	int cont = 0;
	in.open(filename);
	while ( getline(in,line) )
		cont += (KMPSearch(filename,toSearch) != filename.length());
	return cont;
}

void test_stringMatching() {
	string toSearch="estrutura de dados";

	string filename1="text1.txt";
	int num1=numStringMatching(filename1,toSearch);

	ASSERT_EQUAL(3,num1);

	string filename2="text2.txt";
	int num2=numStringMatching(filename2,toSearch);

	ASSERT_EQUAL(2,num2);

}


void test_approximateStringMatching() {

	/*string toSearch="estrutur";

	string filename1="text1.txt";
	float dist1=numApproximateStringMatching(filename1,toSearch);

	ASSERT_EQUAL_DELTA(7.76,dist1,0.01);

	string filename2="text2.txt";
	float dist2=numApproximateStringMatching(filename2,toSearch);

	ASSERT_EQUAL_DELTA(7.49,dist2,0.01);*/

}




void runSuite(){
	cute::suite s;
	s.push_back(CUTE(test_stringMatching));
	s.push_back(CUTE(test_approximateStringMatching));

	cute::ide_listener<> lis;
	cute::makeRunner(lis)(s, "CAL 2012/2013 - Aula Pratica 11");
}

int main(){
    runSuite();
    return 0;
}



