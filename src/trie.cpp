#include "../headers/trie.h"

#include <stdio.h>
#include <chrono>
#include <iostream>

using namespace std;

void printArr(node_t *tmp){
	for ( int i = 0 ; i < ALPHABET_SIZE ; i++)
		cout << (char)(i+ALPHABET_BEGINNING) << " - [ " << tmp[i].eow << "|" << tmp[i].next << "]\n";
}


Trie::Trie(){
	this->root.next = (node_t *)malloc(sizeof(node_t));
	this->root.eow  = false;
}

void Trie::insertWord(string &word){
	size_t i = 0;
	node_t *temp = this->root.next;
	for (i = 0 ; i < word.length()-1 ; i++){
		int pos = charToArrPos(word[i]);
		if ( temp[ pos ].next == NULL)
			temp[pos].next = (node_t *)malloc(sizeof(node_t)*ALPHABET_SIZE);

		temp = temp[pos].next;
	}

	if ( i == word.length()-1 ) // Might as well check
		temp[charToArrPos(word[i])].eow = true;
}

bool Trie::exactWordSearch(string &word){
	node_t *temp = this->root.next;
	unsigned int i = 0, pos = charToArrPos(word[i]);
	for (i = 0; i < word.length()-1; i++ , pos = charToArrPos(word[i])){
		if( temp[ pos ].next == NULL )
			break;

		temp = temp[pos].next;
	}
	return ( (i == word.length() -1) && temp[charToArrPos(word[i])].eow );
}


int editDistance(string pattern, string text)
{
	int n=text.length();
	vector<int> d(n+1);
	int old, //current min distance?
		  neww;
	for (int j=0; j<=n; j++)
		d[j]=j;
	int m=pattern.length();
	for (int i=1; i<=m; i++) { //text iterator
		old = d[0];
		d[0]=i;
		for (int j=1; j<=n; j++) { //pattern iterator
			if (pattern[j-1]==text[i-1])
				neww = old;
			else { //minimun between old min distance and previous spot
				neww = min(old,d[i]);
				neww = min(neww,d[i-1]);
				neww = neww +1;
			}
			old = d[i];
			d[i] = neww;
		}
	}
	return d[n];
}



int main(){
	Trie test;
	string test1 = "PINTOU", test2 = "PINTADOR", test3 = "CRLH", test4 = "PINTADORA";

	test.insertWord(test1);
	test.insertWord(test2);
	test.insertWord(test3);
	if( test.exactWordSearch(test1) && test.exactWordSearch(test2) && test.exactWordSearch(test3) && !test.exactWordSearch(test4) )
		printf("PINTOU CRLH! SIZE = %li\n",sizeof(node_t));
	else
		printf("NAO PINTOU :( \n");
}
