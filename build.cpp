#include <stdio.h>
#include <string.h>

#include <queue>
#include <set>
#include <list>

#include "trie.h"
#include "partition.h"

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
        else {
            buffer[len-1] = 0;
        }
	trie.insert(std::string(buffer));
    }
    return 0;
}

utils::Partition* dfa_compress (utils::Trie& orig) {
    utils::Partition* result = new utils::Partition(orig.get_size());
    utils::WaitSet waiting;
    result->begin()->nxt->wait = waiting.push_part(result->begin()->nxt);

    std::list<int> terminal;
    for (int i = 0; i < orig.get_size(); ++i) {
	if (orig.is_end(i)) {
	    terminal.push_back(i);
	}
    }
    
    result->refine(terminal, waiting);

    std::list<int> all_revert[256];
    while (waiting.begin()->nxt != waiting.end()) {
        struct utils::WaitNode* waited = waiting.end()->pre;
        waiting.remove_node(waited);
        waited->orig->wait = NULL;
        struct utils::PartNode* cur = waited->orig;
	delete waited;

        for (int i = 0; i < 256; ++i) {
            all_revert[i].clear();
        }
        for (int it = cur->begin; ; it = result->get_nxt(it)) {
            int fth = orig.get_node_by_id(it)->fth;
            if (fth != -1) {
                utils::TrieNode* revert_node = orig.get_node_by_id(fth);
                if (revert_node->childs.size()) {
                    all_revert[static_cast<unsigned char>(orig.get_node_by_id(it)->c)].push_back(fth);
                }
            }
            if (it == cur->end) {
                break;
            }
        }

        for (int i = 0; i < 256; ++i) {
            if (all_revert[i].empty()) {
                continue;
            }
            result->refine(all_revert[i], waiting);
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

    utils::Partition* result = dfa_compress::dfa_compress(trie);
    
    int count = 0;
    for (struct utils::PartNode* cur = result->begin()->nxt; cur != result->end(); cur = cur->nxt) {
        /*int cnt = 0;
	for (int it = cur->data.begin; ; it = result->get_nxt(it)) {
            cnt++;
            printf("%d ", it);
            if (it == cur->data.end)
                break;
        }
        printf(" %%  %d\n", cnt);*/
        count ++;
    }
    printf("%d nodes in DFA.\n", count);
    delete result;
    fclose(in);
    return 0;
}
