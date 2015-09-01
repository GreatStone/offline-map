#ifndef GREATSTONE_DFA_H
#define GREATSTONE_DFA_H

#include <vector>

#include "bit_map.h"

namespace utils {

struct Transform {
    int target;
    int skip_cnt;
    unsigned char c;
};

inline bool operator < (const Transform & a, const Transform & b) {
    return a.c < b.c;
}

class State{
public:
    inline int id() const{
        return _id;
    }
    inline bool is_end() const{
        return _end;
    }
    bool target(unsigned char c, Transform * save) const;
    void add_transform(unsigned char c, int skip_cnt, int target);
    inline bool has_transform(unsigned char c) {
        return _map.test(c);
    }
private:
    BitMap _map;
    int _id;
    std::vector<Transform> _trans;
    bool _end;
};

class DFA {
public:
    DFA();
    const State & begin();
    inline const State & state(int id) const{
        return _states[id];
    }
    inline bool is_end(int id) const{
        return _states[id].is_end();
    }
    void add_state(int id, bool end);
    void add_transform(int orig, unsigned char c, int skip_cnt, int target);
private:
    std::vector<State> _states;
    int _begin;
};

}

#endif
