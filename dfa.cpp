#include "dfa.h"

#include <vector>
#include <algorithm>

#include "bit_map.h"

namespace utils {

bool State::target(unsigned char c, Transform * save) const {
    if (!_map.test(c)) {
        return false;
    }
    *save = _trans[_map.lower_count(c) - 1];
    return true;
}

void State::add_transform(unsigned char c, int skip_cnt, int target) {
    Transform trans;
    trans.c = c;
    trans.skip_cnt = skip_cnt;
    trans.target = target;
    std::vector<Transform>::iterator it = lower_bound(_trans.begin(), _trans.end(), trans);
    _trans.insert(it, trans);
}

}
