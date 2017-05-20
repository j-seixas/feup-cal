#include "../headers/trie.h"

#include <stdio.h>
#include <chrono>
#include <iostream>
#include <fstream>

using namespace std;

static mutex flag;

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

/**
 * @brief Counts the number of elements of this level of the trie
 * @param[in] arr Base level to search
 * @return How many elements it has
 */
unsigned char numberOfElements(const node_t arr){
	unsigned char cont = 0;
	for (unsigned char i = 0 ; i < ARR_SIZE ; i++)
		if (arr.eow || arr.next[i].next != nullptr || arr.next[i].eow)
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

/**
 * @brief Finds the position of the first element of the array
 * @param[in] arr Array to search
 * @return Position of the first element
 * @detail Used only in suffixDFS in array which only have 1 element. Time Complexity O(n) , Space Complexity O(1)
 */
int findFirstElementPos(const node_t *arr){
	for (unsigned char pos = 0 ; pos < ARR_SIZE && arr != nullptr ; pos++)
		if (arr[pos].next != nullptr || arr[pos].eow)
			return pos;

	return -1;
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

/*
int main(){
	Trie trie;
	string insert1 = "ARVORE" , insert2 = "ABELHA", insert3 = "ABEDO", insert4 = "ABORIGENE", insert5 = "AMAR", insert6 = "ABELHO",
				 insert7 = "AMARAS", insert8 = "AMADOR", insert9 = "ABORINA", insert10 = "AR",
				 test1 = "ARINR",  test2 = "ARI", test3 = "ARVOREDO", test4 = "ABOLA", test5 = "ABRLI",
				 test6 = "ZBALE", test7 = "ABRA", test8 = "ABELHUDO", test9 = "CABEDAL";
	trie.insertWord(insert1); trie.insertWord(insert2); trie.insertWord(insert3);
	trie.insertWord(insert4); trie.insertWord(insert5); trie.insertWord(insert6);
	trie.insertWord(insert7); trie.insertWord(insert8); trie.insertWord(insert9);
	trie.insertWord(insert10);

	trie.approximateWordSearch(test1);
	cout << "	Expected : [ARVORE, AMAR, AR]\n";
	trie.approximateWordSearch(test2);
	cout << "	Expected : [AR]\n";
	trie.approximateWordSearch(test3);
	cout << "	Expected : [ARVORE]\n";
	trie.approximateWordSearch(test4);
	cout << "	Expected : [ABELHA]\n";
	trie.approximateWordSearch(test5);
	cout << "	Expected : [ABELHO , ABELHA , ABEDO , AR]\n";
	trie.approximateWordSearch(test6);
	cout << "	Expected : [ABELHA, ABELHO, AMAR, ABEDO, AR]\n";
	trie.approximateWordSearch(test7);
	cout << "	Expected : [AR]\n";
	trie.approximateWordSearch(test8);
	cout << "	Expected : [ABELHO]\n";
	trie.approximateWordSearch(test9);
	cout << "	Expected : [ABEDO]\n";


	// ifstream in("names.txt");
	// ofstream out("ExactSearchResults.csv");
	// string line;
	//
	// while( getline(in	, line) )
	// 	trie.insertWord(line);
	//
	// in.close();
	// in.open("names.txt");
	//
	// while ( getline(in,line) ){
	// 	std::chrono::high_resolution_clock::time_point current = std::chrono::high_resolution_clock::now();
	// 	trie.exactWordSearch(line);
	// 	std::chrono::high_resolution_clock::time_point final = std::chrono::high_resolution_clock::now();
	// 	out << line.length() << ";" << std::chrono::duration_cast<std::chrono::duration<double>>(final - current).count() <<endl;
	//
	// }
	// in.close();
	// out.close();
	// return 0;
}
*/

void Trie::printArr(const node_t *arr) const{
	for ( int i = 0 ; i < ARR_SIZE && arr != nullptr ; i++)
		cout << arrPosToChar(i) << " - [ " << arr[i].eow << "|" << arr[i].next << "]\n";
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


list<string> *Trie::approximateWordSearch(string &word) const {
	unsigned int *min_dist = (unsigned int *)malloc(sizeof(unsigned int));
	*min_dist = this->findInitK(word);
	cout << word << " initial K = " << (*min_dist) << endl;
	list<string> *results = new list<string>;
	thread(suffixDFS,word,"",this->root,min_dist,results).join();

	cout << "[";
	for (auto it = results->begin() ; it != results->end() ; it++)
		cout << (*it) << " , ";

	cout << "]\n";

	return results;
}

void Trie::suffixDFS(const string &word, const string pref , node_t chr, unsigned int *min_dist, list<string> *results){
	//cout << this_thread::get_id() << " - NEW THREAD - " << pref << endl;
	string preffix = pref, tmp_word;
	unsigned int dist = 0 , n_elems = 0;
	int pos = 0;
	list<thread> all_threads;

	while( ((n_elems = numberOfElements(chr)) == 1 &&  ((pos = findFirstElementPos(chr.next)) != -1) ) ){
		if ( !chr.eow ){
			preffix += (char)arrPosToChar(pos);
			//cout << this_thread::get_id() << " - Added letter : " << (char)arrPosToChar(pos) << ", word = " << preffix << endl;
			chr = chr.next[pos];
		}
		else{
			//cout << this_thread::get_id() << "Breaking, word = " << preffix << endl;
			break;
		}
	}

	if (!chr.eow)
		tmp_word = word.substr(0,preffix.length());
	else{
		//cout << "	!!EOW!! , n_elem = " << n_elems << endl;;
		tmp_word = word;
	}

	dist = editDistance(preffix , tmp_word);
	flag.lock();
	if ( dist <= (*min_dist) || chr.eow ){
		if ( chr.eow && dist <= (*min_dist)){
			if (dist < (*min_dist) ){
				//cout << "		!!!ERASING LIST CONTENTS!!! \n";
				results->clear();
				(*min_dist) = dist;
			}
			//cout << this_thread::get_id() << "Updating with " << preffix << ", DIST = " << dist <<  endl;
			results->push_front( preffix );
		}
		flag.unlock();
		for (unsigned int i = 0 ; i < ARR_SIZE ; i++)
			if (chr.next != nullptr  && (chr.next[i].next != nullptr || chr.next[i].eow ) )
				all_threads.push_front(thread(&Trie::suffixDFS,word, preffix+(char)arrPosToChar(i), chr.next[i] , min_dist, results));
	}
	else
		flag.unlock();

	//cout << this_thread::get_id() << " - Waiting for threads\n";
	for (auto it = all_threads.begin() ; it != all_threads.end() ; it++)
		it->join();
}


unsigned int Trie::findInitK(const std::string &word) const {
	node_t *temp = this->root.next, *ttmp;
	unsigned int edit_dist = 0, i = 0;
	unsigned char pos = 0;

	for(i = 0 ; i < word.length() ; i++){
		pos = charToArrPos(word[i]);
		//cout << "Searching for " << word[i] << endl;

		if (temp != nullptr && temp[pos].next == nullptr ){ //does not have current character
			unsigned int str_len = (word.length() - i);
			string tmp = word.substr(i, str_len);
			if ( (ttmp = this->findWordInSubtrie(tmp, temp, &str_len)) != nullptr ) { //suffix exists in subtrie
				temp = ttmp;
				i+=str_len;
				//cout << "Found chr on subtrie " << word[i] << " ,str_len = " << str_len << " , edit_dist = " << edit_dist << endl;
				edit_dist+=str_len;
				continue;
			}
			else{ //suffix does not exist
				//cout << "GONNA BREAK\n";
				break;
			}
		}
		else if ( temp == nullptr ) //reached end of trie
			break;

		temp = temp[charToArrPos(word[i])].next;
	}

	unsigned int min = 0;
	//cout << "Temp=  " << temp << " , temp.next = " << temp[pos].next << ", chr = " << arrPosToChar(pos) << endl;
	this->closestEOW(temp,min);
	if ( (i+min) < word.length()) //End of trie reached, but there is still words to search
		min += (word.length() - i - 1);

	//cout << "	MIN = " << min << " | EDIT DIST = " << edit_dist << endl;


	return edit_dist+min;
}


node_t *Trie::findWordInSubtrie(const string &word, node_t *arr, unsigned int *max_depth) const{
	unsigned int max_init = (*max_depth), cont = 1, pre_cont = 0;
	unsigned int pos = 0;
	queue<node_t *> q;
	q.push(arr);

	while( !q.empty() && (*max_depth) > 0){
		node_t * tmp = q.front();		q.pop();
		cont--;
		pos = charToArrPos( word[max_init-(*max_depth)] );
		//cout << "	Subtrie search for : " << word[max_init-(*max_depth)] << endl;
		if ( tmp[pos].next != nullptr ){ //character found
			//cout << "!CHAR FOUND! , max init = " << max_init << " , max_depth = " << (*max_depth) << endl;
			(*max_depth) = max_init - (*max_depth); //edit distance
			return tmp[pos].next;
		}

		for (unsigned int i = 0 ; i < ARR_SIZE ; i++){ //insert next nodes
			if(tmp[i].next != nullptr){
				//cout << "Inserting to queue char " << arrPosToChar(i) << endl;
				q.push(tmp[i].next);
				pre_cont++;
			}
		}
		//cout << "CONT = " << cont << " | PRE = " << pre_cont << " | depth = " << (*max_depth) << endl;
		if (cont <= 0){ //Next Level
			cont=pre_cont;
			pre_cont=0;
			(*max_depth)--;
		}
	}
	//cout << "Word not found in subtrie, returning null\n";
	return nullptr;
}


node_t *Trie::closestEOW(node_t *arr, unsigned int &depth) const{
	unsigned int cont = 1, pre_cont = 0;
	depth = 1;
	queue<node_t *> q;
	q.push(arr);
	while( !q.empty() && arr != nullptr){
		node_t * tmp = q.front();		q.pop();
		//cout << "	READ FROM QUEUE\n";
		//printArr(tmp);
		cont--;

		//cout << "	DEPTH " << depth << endl
		for (unsigned int i = 0 ; i < ARR_SIZE ; i++){ //insert next nodes
			if( tmp[i].eow /*tmp[i].next != nullptr && tmp[i].next->eow */ ){
				//cout << "CHAR = " << arrPosToChar(i) << endl;
				//cout << "EOW\n";
				//depth++;
				return tmp[i].next;
			}
			else if (tmp[i].next != nullptr){
				//cout << "	PUSHING TO STACK :" << endl;
				//cout << "ORIGIN, char = " << arrPosToChar(i) << endl;
				//printArr(tmp[i].next);
				q.push(tmp[i].next);
				pre_cont++;
			}
		}

		if (cont <= 0){ //Next Level
			cont=pre_cont;
			if (pre_cont != 0)
				depth++;
			pre_cont=0;

		}

	}

	return nullptr;
}
