#include "../headers/trie.h"

#include <stdio.h>
#include <chrono>
#include <iostream>
#include <fstream>

using namespace std;

static mutex flag;

Trie::Trie(){
	this->root.next = (node_t *)malloc(sizeof(node_t)*ARR_SIZE);
	this->root.eow  = false;
}

/**
 * @brief Insert a word into the Trie
 * @param[in] word Word to insert into the trie
 * @detail Time Complexity O(m), where m is string length, Space Complexity O(1)
 */
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

/**
 * @brief Converts from ASCII notation to array position
 * @param[in] chr Char to convert
 * @return The array position of the character
 * @detail Time Complexity O(1), Space Complexity O(1)
 */
unsigned char charToArrPos(char chr){
	if ( (chr >= 65 && chr <= 90) || (chr >= 97 && chr <= 122) ) //Letter
		return ( toupper(chr) - ALPHABET_BEGINNING );
	else if ( chr >= 48 && chr <= 57 ) //Number
		return (ALPHABET_SIZE + (chr - NUMBER_BEGGINING) );
	else //Space
		return (ALPHABET_SIZE + 10);
}

/**
 * @brief Converts from array position to ASCII notation
 * @param[in] chr Char to convert
 * @return The ASCII representation of the character
* @detail Time Complexity O(1), Space Complexity O(1)
 */
unsigned char arrPosToChar(char chr){
	if (chr >= 0 && chr < ALPHABET_SIZE) //Letter
		return chr + ALPHABET_BEGINNING;
	else if (chr >= ALPHABET_SIZE && chr < (ALPHABET_SIZE + NUMBER_SIZE) ) //Number
		return (chr + NUMBER_BEGGINING) - ALPHABET_SIZE;
	else //Space
		return 32;
}



int main(){
	Trie trie;
	string test1 = "ARVORE" , test2 = "ABELHA", test3 = "ABEDO", test4 = "ABORIGENE", test5 = "AMAR", test6 = "ABELHO",
				 test7 = "AMARAS", test8 = "AMADOR", test9 = "ABORINA", test10 = "AR", word_to_test = "ALTS";
	trie.insertWord(test1); trie.insertWord(test2); trie.insertWord(test3);
	trie.insertWord(test4); trie.insertWord(test5); trie.insertWord(test6);
	trie.insertWord(test7); trie.insertWord(test8); trie.insertWord(test9);
	trie.insertWord(test10);
	cout << "WORD = " << trie.approximateWordSearch(word_to_test) << endl;

	return 0;
}


void Trie::printArr(const node_t *arr) const{
	for ( int i = 0 ; i < ARR_SIZE && arr != nullptr ; i++)
		cout << arrPosToChar(i) << " - [ " << arr[i].eow << "|" << arr[i].next << "] ,";
	cout << "\n";
}


/**
 * @brief Searches for the given string
 * @param[in] word Word to search for
 * @return True if string was found, false otherwise
 * @detail Time complexity is O(m), where m is the string length, Space complexity O(1)
 */
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

/**
 * @brief Counts the number of elements of this level of the trie
 * @param[in] arr Base level to search
 * @return How many elements it has
 */
unsigned char numberOfElements(const node_t *arr){
	unsigned char cont = 0;
	for (unsigned char i = 0 ; i < ARR_SIZE ; i++)
		if (arr != nullptr && arr[i].next != nullptr)
			cont++;

	return cont;
}

/**
 * @brief Computates the edit distance between the pattern and the text
 * @param[in] pattern The pattern to search for
 * @param[in] text The text to search in
 * @return The edit distance between the two parameters
 * @detail From solution of the TP11
 */
unsigned int editDistance(string &pattern, string &text){
	std::transform(pattern.begin(), pattern.end(),pattern.begin(), ::toupper);
	std::transform(text.begin(), text.end(),text.begin(), ::toupper);
	int n=text.length();
	vector<int> d(n+1);
	int old,neww;
	for (int j=0; j<=n; j++)
		d[j]=j;
	int m=pattern.length();
	for (int i=1; i<=m; i++) {
		old = d[0];
		d[0]=i;
		for (int j=1; j<=n; j++) {
			if (pattern[i-1]==text[j-1]) neww = old;
			else {
				neww = min(old,d[j]);
				neww = min(neww,d[j-1]);
				neww = neww +1;
			}
			old = d[j];
			d[j] = neww;
		}
	}
	return d[n];
}

string Trie::approximateWordSearch(string &word) const {
	approx_search *info = (approx_search *)malloc(sizeof(approx_search));
	info->min_dist = this->findInitK(word);
	thread(suffixDFS,word,"",this->root.next,info).join();
	cout << "	!!SUFFIX RETURNED!!\n";
	string closest_word = info->word;
	free(info);
	return closest_word;
}


void Trie::suffixDFS(const string &word, const string pref , node_t *arr, approx_search *info){
	cout << this_thread::get_id() << " - NEW THREAD\n";

	//for ( int i = 0 ; i < ARR_SIZE && arr != nullptr ; i++)
	//	cout << arrPosToChar(i) << " - [ " << arr[i].eow << "|" << arr[i].next << "] ,";
	//cout << "\n";

	string preffix = pref;
	unsigned int dist = 0 , pos = 0, n_elems = 0, cont = 0;
	list<thread> all_threads;
	while( (n_elems = numberOfElements(arr)) == 1 && !arr->eow){
		cout << this_thread::get_id() << " - LEVEL " << cont << endl;
		for (pos = 0 ; pos < ARR_SIZE ; pos++)
			if ( arr[pos].next != nullptr){
				cout << this_thread::get_id() << " - Added letter : " << (char)arrPosToChar(pos) << endl;
				preffix += (char)arrPosToChar(pos);
				arr = arr[pos].next;
				break;
			}
	}

	string text = word.substr(0,preffix.length());
	flag.lock();
	cout << this_thread::get_id() << " - Found fork, min = " << info->min_dist << " curr : " << preffix << ", pos = " << pos <<  endl;
	if ( (dist = editDistance(preffix , text)) < info->min_dist && n_elems > 0){
		cout << this_thread::get_id() << " - dist = " << dist << endl;
		if (text.length() == word.length()){
			info->min_dist = dist;
			info->word = preffix;
		}
		flag.unlock();
		for (unsigned int i = 0 ; i < ARR_SIZE ; i++)
			if (arr[i].next != nullptr){
				cout << "ptr = " << arr[i].next << endl;
				all_threads.push_front(thread(&Trie::suffixDFS,word, preffix, arr[i].next , info));
			}
	}
	else
		flag.unlock();

	cout << this_thread::get_id() << " - Waiting for threads\n";
	for (auto it = all_threads.begin() ; it != all_threads.end() ; it++)
		it->join();
}

/**
 * @brief Finds the initial estimated edit distances
 * @param[in] word Word to search for edit distance
 * @return Estimated edit distance
 */
unsigned int Trie::findInitK(const std::string &word) const {
	node_t *temp = this->root.next, *prev = temp;
	unsigned int edit_dist = 0;
	for( unsigned int i = 0 ; i < word.length() ; i++){
		unsigned char pos = charToArrPos(word[i]);

		if ( temp[pos].next == nullptr ){ //does not have current character
			unsigned int str_len = (word.length() - i);
			string tmp = word.substr(i, str_len);
			if ( (temp = this->findWordInSubtrie(tmp, prev, str_len)) != nullptr ) { //suffix exists in subtrie
				edit_dist+=str_len;
				i+=str_len;
			}
			else{ //suffix does not exist
				edit_dist += (word.length() - i);
				break;
			}
		}
		else{ //current character exists
			prev = temp;
			temp = temp[pos].next;
		}
	}

	return edit_dist;
}

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

/**
 * @brief Checks the character exists in the next level of the trie
 * @param[in] chr Character to check the existance
 * @param[in] arr Base node
 * @return Position in the array where the character occurs, or -1 if it does not occur
 */
int Trie::charExistsInArr(const char &chr, const node_t *arr) const {
	int i = 0;
	for (i = 0 ; i < ARR_SIZE ; i++){
		if (arr[i].next != nullptr && arr[i].next[charToArrPos(chr)].next != nullptr && arr[i].eow == false)
			return i;

	}

	return -1;
}
