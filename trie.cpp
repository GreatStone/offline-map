#include "trie.h"

#include <vector>
#include <string>

namespace utils {

Trie::Trie() {
    _root = add_new_node(static_cast<char>(-1));
    _root->fth = -1;
}

Trie::~Trie() {
    for (size_t i = 0; i < _node_by_id.size(); ++i) {
	delete _node_by_id[i];
    }
}
    
TrieNode* Trie::add_new_node(char c) {
    TrieNode* ret = new TrieNode;
    ret->c = c;
    ret->id = _node_by_id.size();
    ret->lang_sz = 0;
    _node_by_id.push_back(ret);
    _is_end.push_back(false);
    return ret;
}
    
void Trie::insert(const std::string & src) {
    const char* input = src.c_str();
    int len = src.length();
    TrieNode* cur = _root;
    for (int i = 0; i < len; ++i) {
	++(cur->lang_sz);
	TrieNode* find = NULL;
	int sz = cur->childs.size();
	for (int j = 0; j < sz; ++j) {
	    if (cur->childs[j]->c == input[i]) {
		find = cur->childs[j];
		break;
	    }
	}
	if (find == NULL) {
	    TrieNode* temp = add_new_node(input[i]);
	    cur->childs.push_back(temp);
            temp->fth = cur->id;
            cur = temp;
	} else {
	    cur = find;
	}
    }
    ++(cur->lang_sz);
    _is_end[cur->id] = true;
}

}
