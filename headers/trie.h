#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <cctype>

#define ALPHABET_SIZE 26
#define ALPHABET_BEGINNING 65


struct node_t{
	bool eow;
	node_t *next;
};

class Trie{
	node_t root;

public:
	Trie();

	void insertWord(std::string &word);

	bool exactWordSearch(std::string &word);


private:
	inline int charToArrPos(char &chr){ return (toupper(chr) - ALPHABET_BEGINNING); }
};

#endif
