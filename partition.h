#ifndef GREATSTONE_PARTITION_H
#define GREATSTONE_PARTITION_H

#include <list>

namespace utils {
//all list use two guard
//but _eles has no guard
struct PartNode;
struct EleNode;
struct WaitNode;
struct InterNode;

struct PartNode {
    struct PartNode* pre;
    struct PartNode* nxt;
    int begin;
    int end;
    struct InterNode* inter;
    struct WaitNode* wait;
    int size;
};

struct EleNode {
    int pre;
    int nxt;
    struct PartNode* own;
};

struct WaitNode {
    struct WaitNode* pre;
    struct WaitNode* nxt;
    struct PartNode* orig;
};

struct InterNode {
    struct PartNode* orig;
    std::list<int> data;
    int size;
};

class WaitSet {
public:
    WaitSet();
    ~WaitSet();
    struct WaitNode* push_part(struct PartNode* part);
    void remove_node(struct WaitNode* aim);
    inline struct WaitNode* begin() {
        return _begin;
    }
    inline struct WaitNode* end() {
        return _end;
    }
private:
    struct WaitNode* _begin;
    struct WaitNode* _end;
};

class Partition {
public:
    Partition(int size);
    ~Partition();
    inline struct PartNode* begin() {
        return _begin;
    }
    inline struct PartNode* end() {
        return _end;
    }
    inline int get_nxt(int id) {
        return _eles[id].nxt;
    }
    inline int get_pre(int id) {
        return _eles[id].pre;
    }
    inline struct PartNode* get_own(int id) {
        return _eles[id].own;
    }
    void refine(const std::list<int> & pivot, WaitSet & wait);
private:
    struct EleNode* _eles;
    PartNode* _begin;
    PartNode* _end;
    int _size;
    void remove_part(struct PartNode * aim);
    void remove_ele(int id);
    struct PartNode * push_part(struct InterNode *aim);
};

}

#endif
