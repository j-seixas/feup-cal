#include "../headers/trie.h"

#include <stdio.h>
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
	size_t i = 0;
	for (i = 0; i < word.length()-1; i++){
		int pos = charToArrPos(word[i]);
		if( temp[ pos ].next == NULL )
			break;

		temp = temp[pos].next;
	}
	
	return ( (i == word.length() -1) && temp[charToArrPos(word[i])].eow );
}


int main(){
	Trie test;
	string tmp = "PINTOU";
	test.insertWord(tmp);
	if( test.exactWordSearch(tmp) )
		printf("PINTOU CRLH!\n");
	else
		printf("NAO PINTOU :( \n");
}
