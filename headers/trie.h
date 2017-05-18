#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>
#include <queue>
#include <cctype>
#include <algorithm>

#define ARR_SIZE 37
#define ALPHABET_SIZE 26
#define NUMBER_SIZE 10

#define ALPHABET_BEGINNING 65
#define NUMBER_BEGGINING 48


struct node_t{
	bool eow;
	node_t *next = nullptr;
};

class Trie{


public:
	node_t root;
	Trie();

	void insertWord(const std::string &word);

	bool exactWordSearch(std::string &word) const;

	std::string approximateWordSearch(std::string &word) const;

	node_t *findWordInSubtrie(const std::string &word, node_t *arr, unsigned int &max_depth) const;

	void printArr(const node_t *arr) const;

private:

	inline bool arrContainsChar(char &chr,const node_t *arr) { return arr[this->charToArrPos(chr)].next != NULL;}

	unsigned int editDistance(const std::string &pattern,const  std::string &text) const;
	unsigned int findInitK(const std::string &word) const;
	unsigned char numberOfElements(const node_t *arr) const;

	int charExistsInArr(const char &chr, const node_t *arr) const;

	unsigned char charToArrPos(char chr) const;
	unsigned char arrPosToChar(char chr) const;

};



#endif
