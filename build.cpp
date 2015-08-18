#include <stdio.h>
#include <string.h>

#include <queue>
#include <set>

#include "bit_num.h"
#include "trie.h"

namespace dfa_compress{

const int MAX_BUFFER_SIZE = 2048;
    
int build_tree(::utils::Trie& trie, FILE* in) {
    char buffer[MAX_BUFFER_SIZE];
    while (fgets(buffer,MAX_BUFFER_SIZE,in)) {
	int len = strlen(buffer);
	if (buffer[len - 1] != '\n') {
	    char temp = fgetc(in);
	    if (temp != EOF) {
		printf("Data is too long.");
		return -1;
	    }
	}
	trie.insert(std::string(buffer));
    }
    return 0;
}

std::queue<utils::BitNum>* dfa_compress (::utils::Trie& orig) {
    int size = orig.get_size();
    utils::BitNum all(size);
    utils::BitNum terminal(size);
    all.set();
    for (int i = 0; i < size; ++i) {
	if (orig.is_end(i)) {
	    terminal.set(i);
	}
    }
    
    std::queue< ::utils::BitNum >* result = new std::queue< ::utils::BitNum >;
    std::set< ::utils::BitNum > waiting;

    result->push(all);
    waiting.insert(all);

    result->push(terminal);
    waiting.insert(terminal);

    ::utils::RevertTrie* revert_trie = orig.get_revert_trie();
    
    while (!waiting.empty()) {
	::utils::BitNum cur = *(waiting.begin());
	waiting.erase(waiting.begin());
	for (char c = 0; c < 128; ++c) {
	    
	    ::utils::BitNum revert(size);
	    ::utils::SetIterator iterator(cur);
	    int temp;
	    while ((temp = iterator.next()) != -1) {
		::utils::TrieNode* e = revert_trie->get_node_by_id(temp);
		std::vector< ::utils::TrieNode* >& cur_childs = e->childs;
		for (int i = 0; i < cur_childs.size(); ++i) {
		    if (cur_childs[i]->c == c) {
			revert.set(i);
		    }
		}
	    }

	    std::queue< ::utils::BitNum > set_buffer;
	    
	    while (!result->empty()) {
		::utils::BitNum e = result->front();
		result->pop();

		utils::BitNum inter = revert & e;
		utils::BitNum except = e - revert;
		if (inter.empty() || except.empty()) {
		    set_buffer.push(e);
		    continue;
		}

		set_buffer.push(inter);
		set_buffer.push(except);

		std::set< ::utils::BitNum >::iterator wait_it;
		wait_it = waiting.find(e);
		if (wait_it == waiting.end()) {
		    if (inter.get_count() < except.get_count()) {
			waiting.insert(inter);
		    } else {
			waiting.insert(except);
		    }
		} else {
		    waiting.erase(wait_it);
		    waiting.insert(inter);
		    waiting.insert(except);
		}
	    }
	    while (!set_buffer.empty()) {
		result->push(set_buffer.front());
		set_buffer.pop();
	    }
	}
    }
    return result;
}

}

int main(int argc, char** argv) {
    utils::Trie trie;
    FILE* in = fopen(argv[1], "r");
    if (!in) {
	printf("No such a file.\n");
	return -1;
    }
    if (dfa_compress::build_tree(trie, in)) {
	printf("Fail to build trie.");
	return -1;
    }
    printf("Trie has %d nodes\n", trie.get_size());
    std::queue< ::utils::BitNum >* result = dfa_compress::dfa_compress(trie);
    printf("%d nodes in DFA.\n", result->size());
    fclose(in);
    return 0;
}
