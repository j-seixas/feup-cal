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

int editDistance(string pattern, string text)
{
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

#endif
