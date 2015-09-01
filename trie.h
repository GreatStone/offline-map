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
    int fth;
} TrieNode;

class Trie {
public:
    Trie();
    ~Trie();
    void insert(const std::string & src);
    inline TrieNode* get_root() {
        return _node_by_id[0];
    }
    inline TrieNode* get_node_by_id(int id) {
        return _node_by_id[id];
    }
    int get_size() {
	return _node_by_id.size();
    }
    inline bool is_end(int id) {
        return _is_end[id];
    }
private:
    TrieNode* _root;
    std::vector<TrieNode*> _node_by_id;
    std::vector<bool> _is_end;
    TrieNode* add_new_node(char c);
};

}

#endif
