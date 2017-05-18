#include "../headers/trie.h"

#include <stdio.h>
#include <chrono>
#include <iostream>
#include <fstream>

using namespace std;

void Trie::printArr(const node_t *arr) const{
	for ( int i = 0 ; i < ARR_SIZE ; i++)
		cout << this->arrPosToChar(i) << " - [ " << arr[i].eow << "|" << arr[i].next << "]\n";
}


Trie::Trie(){
	this->root.next = (node_t *)malloc(sizeof(node_t)*ARR_SIZE);
	this->root.eow  = false;
}

void Trie::insertWord(const string &word){
	size_t i = 0;
	node_t *temp = this->root.next;
	for (i = 0 ; i < word.length()-1 ; i++){
		unsigned char pos = charToArrPos(word[i]);
		if (temp[pos].next == nullptr )
			temp[pos].next = (node_t*)malloc(sizeof(node_t)*ARR_SIZE);

		temp = temp[pos].next;
	}
	if ( i == word.length()-1 ) // Might as well check
		temp[charToArrPos(word[i])].eow = true;
}

unsigned char Trie::charToArrPos(char chr) const{
	if ( (chr >= 65 && chr <= 90) || (chr >= 97 && chr <= 122) ) //Letter
		return ( toupper(chr) - ALPHABET_BEGINNING );
	else if ( chr >= 48 && chr <= 57 ) //Number
		return (ALPHABET_SIZE + (chr - NUMBER_BEGGINING) );
	else //Space
		return (ALPHABET_SIZE + 10);
}

unsigned char Trie::arrPosToChar(char chr) const{
	if (chr >= 0 && chr < ALPHABET_SIZE) //Letter
		return chr + ALPHABET_BEGINNING;
	else if (chr >= ALPHABET_SIZE && chr < (ALPHABET_SIZE + NUMBER_SIZE) ) //Number
		return (chr + NUMBER_BEGGINING) - ALPHABET_SIZE;
	else //Space
		return 32;
}

int main(){
	Trie trie;
	string test1 = "ARVORE" , test2 = "ABELHA", test3 = "ABEDO", test4 = "ABORIGENE", test5 = "AMAR", test6 = "ABELHO";
	trie.insertWord(test1); trie.insertWord(test2); trie.insertWord(test3);
	trie.insertWord(test4); trie.insertWord(test5); trie.insertWord(test6);
	unsigned int max = 4;
	node_t * node = trie.findWordInSubtrie("PKLO", trie.root.next, max);
	if(node != nullptr){
		trie.printArr(node);
		cout << "NOT NULL | MAX = " << max << endl;
	}
	else
		cout << "NULL\n";
	cout << "COMPLETED\n";

	return 0;
}




bool Trie::exactWordSearch(string &word) const{
	node_t *temp = this->root.next;
	unsigned int i = 0;
	unsigned char pos = charToArrPos(word[i]);
	for (i = 0; i < word.length()-1; i++ , pos = charToArrPos(word[i])){
		if( temp[ pos ].next == nullptr )
			break;

		temp = temp[pos].next;
	}
	return ( (i == word.length() -1) && temp[charToArrPos(word[i])].eow );
}


unsigned char Trie::numberOfElements(const node_t *arr) const{
	unsigned char cont = 0;
	for (unsigned char i = 0 ; i < ARR_SIZE ; i++)
		if (arr[i].next != nullptr)
			cont++;

	return cont;
}

unsigned int Trie::editDistance(const string &pattern, const string &text) const{
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

string Trie::approximateWordSearch(string &word) const {
	return word;
}
/*
unsigned int Trie::findInitK(const std::string &word) const {
	node_t *temp = this->root.next;
	unsigned int edit_dist = 0;
	for( unsigned int i = 0 ; i < word.length() ; i++){
		if ( temp[this->charToArrPos(word[i])] == nullptr ){ //does not have current character
			edit_dist++;
			node_t *tmp;
			if ( (tmp = this->findWordInSubtrie(word[i], temp)) == nullptr){ //No other subtrie has the next character

			}
		}
	}
}
*/
/**
 * @brief Searches for the first occurrence of the next existant character in the subtrie
 * @param[in] word The remainder of the word to search, usually a substring of a string
 * @param[in] arr The node to start looking
 * @param[out] max_depth Limits the depth of the search, also used to check how many characters were advanced
 * @return The node which contains the first found character
 * @detail Based on Breadth First Search
 */
node_t *Trie::findWordInSubtrie(const string &word, node_t *arr, unsigned int &max_depth) const{
	unsigned int max_init = max_depth, cont = 1, pre_cont = 0;
	int pos = 0;
	queue<node_t *> q;
	q.push(arr);
	while( !q.empty() && max_depth > 0){
		node_t * tmp = q.front();		q.pop();
		this->printArr(tmp);
		cont--;
		if ( (pos = this->charExistsInArr(word[max_init-max_depth], tmp)) != -1){ //character found
			max_depth = max_init - max_depth; //edit distance
			return tmp[pos].next;
		}

		for (unsigned int i = 0 ; i < ARR_SIZE ; i++){ //insert next nodes
			if(tmp[i].next != nullptr){
				q.push(tmp[i].next);
				pre_cont++;
			}
		}

		if (cont <= 0){ //Next Level
			cont=pre_cont;
			pre_cont=0;
			max_depth--;
		}
	}

	return nullptr;
}

int Trie::charExistsInArr(const char &chr, const node_t *arr) const {
	cout << "Searching chr = " << chr << endl;
	int i = 0;
	for (i = 0 ; i < ARR_SIZE ; i++){
		if (arr[i].next != nullptr && arr[i].next[this->charToArrPos(chr)].next != nullptr && arr[i].eow == false)
			return i;

	}

	return -1;
}
