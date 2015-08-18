#ifndef GREATSTONE_TRIE_H
#define GREATSTONE_TRIE_H

#include <vector>
#include <string>

namespace utils {

class BitNum;

typedef struct TrieNode {
    std::vector<TrieNode*> childs;
    char c;
    int id;
    int lang_sz;
} TrieNode;

class RevertTrie {
public:
    RevertTrie();
    ~RevertTrie();
    TrieNode* get_root();
    TrieNode* get_node_by_id(int id);
    TrieNode* add_new_node(int fth, char c);
private:
    std::vector<TrieNode*> _node_by_id;
};
 
class Trie {
public:
    Trie();
    ~Trie();
    void insert(const std::string & src);
    TrieNode* get_root();
    TrieNode* get_node_by_id(int id);
    int get_size() {
	return _node_by_id.size();
    }
    RevertTrie* get_revert_trie() {
	return &_revert_trie;
    }
    bool is_end(int id);
private:
    TrieNode* _root;
    std::vector<TrieNode*> _node_by_id;
    std::vector<bool> _is_end;
    RevertTrie _revert_trie;
    TrieNode* add_new_node(char c);
};

}

#endif
