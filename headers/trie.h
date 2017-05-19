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

unsigned char charToArrPos(char chr);

class Trie{
public:
	node_t root;

	/**
	 * @brief Default constructor
	 */
	Trie();

	/**
	 * @brief Insert a word into the Trie
	 * @param[in] word Word to insert into the trie
	 * @detail Time Complexity O(m), where m is string length, Space Complexity O(1)
	 */
	void insertWord(const std::string &word);

	/**
	 * @brief Searches for the given string
	 * @param[in] word Word to search for
	 * @return True if string was found, false otherwise
	 * @detail Time complexity is O(m), where m is the string length, Space complexity O(1)
	 */
	bool exactWordSearch(std::string &word) const;

	/**
	 * @brief Approximate string matching wrapper
	 * @param[in] word Word to search for matches
	 * @return List containing the results
	 */
	std::string approximateWordSearch(std::string &word) const;
private:

	/**
	 * @brief Computates the edit distance between the pattern and the text
	 * @param[in] pattern The pattern to search for
	 * @param[in] text The text to search in
	 * @return The edit distance between the two parameters
	 * @detail From solution of the TP11
	 */
	unsigned int Trie::editDistance(string &pattern, string &text) const;

	/**
	 * @brief Finds the closest End Of Word from the node given
	 * @param[in] arr Starting node
	 * @param[out] depth Maximum depth to search
	 * @return The node of the closest EOW
	 * @detail Based on BFS algorithm, Time Complexity O(m), where m is the number of characters in the subtrie, Space Complexity O(m)
	 */
	node_t *Trie::closestEOW(node_t *arr, unsigned int &depth) const;


	int charExistsInArr(const char &chr, const node_t *arr) const;

	/**
	 * @brief Searches for the first occurrence of the next existant character in the subtrie
	 * @param[in] word The remainder of the word to search, usually a substring of a string
	 * @param[in] arr The node to start looking
	 * @param[out] max_depth Limits the depth of the search, also used to check how many characters were advanced
	 * @return The node which contains the first found character
	 * @detail Based on Breadth First Search
	 */
	node_t *findWordInSubtrie(const std::string &word, node_t *arr, unsigned int *max_depth) const;


	/**
	 * @brief Finds the initial estimated edit distances
	 * @param[in] word Word to search for edit distance
	 * @return Estimated edit distance
	 */
	unsigned int findInitK(const std::string &word) const;

	/**
	 * @brief Heart of approximate string matching
	 * @param[in] word Word to find match
	 * @param[in] pref Will contain the current preffix of the thread
	 * @param[out] min_dist The current minimum string matching distance
	 * @param[out] results Will hold the results of the matching
	 * @detail Launches multiple threads, I have no clue what is the Time Complexity
	 */
	void static suffixDFS(const std::string &word, const std::string pref , node_t chr, unsigned int *min_dist, std::list<std::string> *results);

	/**
	 * @brief Finds closest End Of Word from starting node
	 * @param[in] arr Starting node
	 * @param[out] depth How deep the EOW was
	 * @return The node which contains the closest EOW
	 * @detail Based on BFS, Time Complexity O(m), where m is the number of characters in the subtrie, Space Complexity O(m)
	 */
	node_t *closestEOW(node_t *arr, unsigned int &depth) const;

/**********************************************************************************************************
**************************************** UTILITIES ********************************************************
**********************************************************************************************************

	/**
	 * @brief Checks the character exists in the next level of the trie
	 * @param[in] chr Character to check the existance
	 * @param[in] arr Base node
	 * @return Position in the array where the character occurs, or -1 if it does not occur
	 */
	int Trie::charExistsInArr(const char &chr, const node_t *arr) const;

	/**
	 * @brief Converts from array position to ASCII notation
	 * @param[in] chr Char to convert
	 * @return The ASCII representation of the character
	 * @detail Time Complexity O(1), Space Complexity O(1)
	 */
	unsigned char arrPosToChar(char chr) const;

	/**
	 * @brief Converts from ASCII notation to array position
	 * @param[in] chr Char to convert
	 * @return The array position of the character
	 * @detail Time Complexity O(1), Space Complexity O(1)
	 */
	unsigned char charToArrPos(char chr) const;

	/**
	 * @brief Used to print a given array (purely for debugging purposes)
	 * @param[in] arr Array to be printed
	 */
	void printArr(const node_t *arr) const;

	/**
	 * @brief Counts the number of elements of this level of the trie
	 * @param[in] arr Base level to search
	 * @return How many elements it has
	 */
	unsigned char Trie::numberOfElements(const node_t arr) const;

	/**
	 * @brief Finds the position of the first element of the array
	 * @param[in] arr Array to search
	 * @return Position of the first element
	 * @detail Used only in suffixDFS in array which only have 1 element. Time Complexity O(n) , Space Complexity O(1)
	 */
	int Trie::findFirstElementPos(const node_t *arr) const;
};



#endif
