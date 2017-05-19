#ifndef TRIE_H
#define TRIE_H

#include <list>
#include <mutex>
#include <thread>
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

struct approx_search{
	unsigned int min_dist;
	std::string word;
};

unsigned char charToArrPos(char chr);

class Trie{


public:
	node_t root;
	Trie();

	void insertWord(const std::string &word);

	bool exactWordSearch(std::string &word) const;

	std::string approximateWordSearch(std::string &word) const;

	node_t *findWordInSubtrie(const std::string &word, node_t *arr, unsigned int *max_depth) const;

	void printArr(const node_t *arr) const;

	unsigned int findInitK(const std::string &word) const;

	void static suffixDFS(const std::string &word, const std::string pref , node_t *arr, approx_search *info);

	node_t *closestEOW(node_t *arr, unsigned int &depth) const;

private:

	inline bool arrContainsChar(char &chr,const node_t *arr) { return arr[charToArrPos(chr)].next != NULL;}

	int charExistsInArr(const char &chr, const node_t *arr) const;
};



#endif
