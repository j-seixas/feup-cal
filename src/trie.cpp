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
	string insert1 = "ARVORE" , insert2 = "ABELHA", insert3 = "ABEDO", insert4 = "ABORIGENE", insert5 = "AMAR", insert6 = "ABELHO",
				 insert7 = "AMARAS", insert8 = "AMADOR", insert9 = "ABORINA", insert10 = "AR",
				 test1 = "ARINR",  test2 = "ARI", test3 = "ARVOREDO", test4 = "ABOLA", test5 = "ABRLI",
				 test6 = "ZBALE", test7 = "ABRA";
	trie.insertWord(insert1); trie.insertWord(insert2); trie.insertWord(insert3);
	trie.insertWord(insert4); trie.insertWord(insert5); trie.insertWord(insert6);
	trie.insertWord(insert7); trie.insertWord(insert8); trie.insertWord(insert9);
	trie.insertWord(insert10);

	cout << "WORD = " << trie.approximateWordSearch(test1) << " , expected ARVORE" << endl;
	//cout << "WORD = " << trie.approximateWordSearch(test2) << " , expected 4" << endl;
	//cout << "WORD = " << trie.approximateWordSearch(test3) << " , expected 2" << endl;
	//cout << "WORD = " << trie.approximateWordSearch(test4) << " , expected 4" << endl;
	//cout << "WORD = " << trie.approximateWordSearch(test5) << " , expected 3" << endl;
	//cout << "WORD = " << trie.approximateWordSearch(test6) << " , expected 4" << endl;
	//cout << "WORD = " << trie.approximateWordSearch(test7) << " , expected 3" << endl;


	return 0;
}


void Trie::printArr(const node_t *arr) const{
	for ( int i = 0 ; i < ARR_SIZE && arr != nullptr ; i++)
		cout << arrPosToChar(i) << " - [ " << arr[i].eow << "|" << arr[i].next << "]\n";
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
	for (unsigned char i = 0 ; i < ARR_SIZE && arr != nullptr ; i++)
		if (arr[i].next != nullptr || arr[i].eow)
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
	string closest_word = info->word;
	free(info);
	return closest_word;
}

int findFirstElementPos(const node_t *arr){
	for (unsigned char pos = 0 ; pos < ARR_SIZE && arr != nullptr ; pos++)
		if (arr[pos].next != nullptr || arr[pos].eow)
			return pos;

	return -1;
}

void Trie::suffixDFS(const string &word, const string pref , node_t *arr, approx_search *info){
	cout << this_thread::get_id() << " - NEW THREAD - " << pref << endl;

	string preffix = pref;
	unsigned int dist = 0 , n_elems = 0;
	int pos = 0;
	list<thread> all_threads;
	while( (n_elems = numberOfElements(arr)) == 1 &&  ((pos = findFirstElementPos(arr)) != -1) /* && arr[pos].eow)*/  ){
		if (pos != -1){
			preffix += (char)arrPosToChar(pos);
			cout << this_thread::get_id() << " - Added letter : " << (char)arrPosToChar(pos) << ", word = " << preffix << endl;
			if (!arr[pos].eow)
				arr = arr[pos].next;
			else{
				cout << this_thread::get_id() << "Breaking, word = " << preffix << endl;
				break;
			}
		}
	}

	string tmp_word = word.substr(0,preffix.length());
	dist = editDistance(preffix , tmp_word);

	flag.lock();
	if ( dist <= info->min_dist && n_elems > 0){
		if ( arr[pos].eow ){
			cout << this_thread::get_id() << "Updating with " << preffix << endl;
			info->min_dist = dist;
			info->word = preffix;
		}

		for (unsigned int i = 0 ; i < ARR_SIZE ; i++){
			if (arr[i].next != nullptr){
				cout << this_thread::get_id() << " - FORK : m = " << info->min_dist << " curr : " << info->word << ", pref = " << preffix << ", dist = " << dist << ", chr = " << arrPosToChar(i) << endl;
				all_threads.push_front(thread(&Trie::suffixDFS,word, preffix+(char)arrPosToChar(i), arr[i].next , info));
			}
		}

			flag.unlock();
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
				//cout << "Found chr on subtrie " << word[i] << " ,str_len = " << str_len << " , edit_dist = " << edit_dist << endl;
				temp = ttmp;
				edit_dist+=str_len;
				i+=str_len;
				continue;
			}
			else{ //suffix does not exist
				break;
			}
		}
		else if ( temp == nullptr ) //reached end of trie
			break;

		temp = temp[charToArrPos(word[i])].next;
	}

	unsigned int min = 0;
	//cout << "Temp=  " << temp << " , temp.next = " << temp[pos].next << endl;
	if (!temp[pos].eow){
		//printArr(temp);
		this->closestEOW(temp,min);
	}
	else{
		//cout << "reached end of trie\n";
		min = (word.length() - i - 1);
	}
	//cout << "	MIN = " << min << " | EDIT DIST = " << edit_dist << endl;


	return edit_dist+min;
}

/**
 * @brief Searches for the first occurrence of the next existant character in the subtrie
 * @param[in] word The remainder of the word to search, usually a substring of a string
 * @param[in] arr The node to start looking
 * @param[out] max_depth Limits the depth of the search, also used to check how many characters were advanced
 * @return The node which contains the first found character
 * @detail Based on Breadth First Search
 */
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
