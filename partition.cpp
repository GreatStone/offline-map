#include "partition.h"

#include <stdio.h>
#include <list>

namespace utils {

int show_list(std::list<int> const* x) {
    int cnt = 0;
    for (std::list<int>::const_iterator it = x->begin(); it != x->end(); ++it) {
        cnt++;
        printf("%d ", *it);
    }
    printf("\n");
    return cnt;
}

int show_part_node(PartNode* x, Partition * res) {
    printf("part %p wait %p  :  ", x, x->wait);
    int it;
    int cnt = 0;
    for (it = x->begin; ; it = res->get_nxt(it)) {
        cnt++;
        printf("%d ", it);
        if (it == x->end) {
            break;
        }
    }
    printf("\n");
    return cnt;
}

int show_part(Partition* res) {
    PartNode* cur;
    int cnt = 0;
    for (cur = res->begin()->nxt; cur != res->end(); cur = cur->nxt) {
        cnt++;
        show_part_node(cur, res);
    }
    return cnt;
}

int show_wait_node(WaitNode* node, Partition * res) {
    printf("wait %p  :  ", node);
    return show_part_node(node->orig, res);
}

int show_wait(WaitSet* wait, Partition * res) {
    int cnt = 0;
    for (WaitNode* cur = wait->begin()->nxt; cur != wait->end(); cur = cur->nxt) {
        cnt++;
        show_wait_node(cur, res);
    }
    return cnt;
}

struct WaitNode* new_wait_node() {
    struct WaitNode* ret = new struct WaitNode;
    ret->orig = NULL;
    return ret;
}

WaitSet::WaitSet() {
    _begin = new_wait_node();
    _end = new_wait_node();
    _begin->pre = NULL;
    _end->nxt = NULL;
    _begin->nxt = _end;
    _end->pre = _begin;
}

WaitSet::~WaitSet() {
    WaitNode* tmp;
    for (WaitNode* cur = _begin; cur != _end; ) {
	tmp = cur;
	cur = cur->nxt;
	delete tmp;
    }
    delete _end;
}

struct WaitNode* WaitSet::push_part(struct PartNode* part) {
    struct WaitNode* ret = new_wait_node();

    ret->orig = part;
    ret->pre = _end->pre;
    ret->nxt = _end;
    _end->pre->nxt = ret;
    _end->pre = ret;
    
    return ret;
}

void WaitSet::remove_node(struct WaitNode* aim) {
    aim->nxt->pre = aim->pre;
    aim->pre->nxt = aim->nxt;
}

struct PartNode* new_part_node() {
    struct PartNode* ret = new struct PartNode;
    ret->inter = NULL;
    ret->wait = NULL;
    return ret;
}

Partition::Partition(int size) : _size(size){
    _begin = new_part_node();
    _end = new_part_node();

    struct PartNode* first = new_part_node();
    
    _begin->pre = NULL;
    _begin->nxt = first;
    first->pre = _begin;
    first->nxt = _end;
    first->begin = 0;
    first->end = size-1;
    first->size = size;
    _end->pre = first;
    _end->nxt = NULL;
    

    struct EleNode* temp = new struct EleNode[size + 2];
    _eles = temp + 1;
    for (int i = 0; i < size; ++i) {
        _eles[i].pre = i - 1;
        _eles[i].nxt = i + 1;
        _eles[i].own = first;
    }
}

Partition::~Partition() {
    delete[] &(_eles[-1]);
    
    PartNode* tmp;
    for (PartNode* cur = _begin; cur != _end; ) {
	tmp = cur;
	cur = cur->nxt;
	delete tmp;
    }
    delete _end;
}

void Partition::remove_part(struct PartNode * aim) {
    aim->pre->nxt = aim->nxt;
    aim->nxt->pre = aim->pre;
}

void Partition::remove_ele(int id) {
    _eles[_eles[id].pre].nxt = _eles[id].nxt;
    _eles[_eles[id].nxt].pre = _eles[id].pre;
}

struct PartNode * Partition::push_part(struct InterNode * aim) {
    int cur,end;
    struct PartNode * ret = new_part_node();
    if (_begin->nxt == _end) {
        _begin->nxt = ret;
        _end->pre = ret;
        ret->pre = _begin;
        ret->nxt = _end;
        cur = -1;
        end = _size;
    } else {
        _end->pre->nxt = ret;
        ret->pre = _end->pre;
        ret->nxt = _end;
        _end->pre = ret;
        cur = ret->pre->end;
        end = get_nxt(cur);
    }
    ret->begin = *(aim->data.begin());
    std::list<int>::iterator it;
    int cnt = 0;
    for (it = aim->data.begin(); it != aim->data.end(); ++it) {
        ++cnt;
        _eles[*it].own = ret;
        _eles[cur].nxt = *it;
        _eles[*it].pre = cur;
        cur = *it;
    }
    ret->size = cnt;
    ret->end = cur;
    _eles[cur].nxt = end;
    return ret;
}

void Partition::refine(const std::list<int> & pivot, WaitSet & wait) {
    /*    printf("refine by ");
    show_list(&pivot);
    printf("******\n");
    printf("before refine ");
    show_part(this);
    printf("******\n");
    printf("init waiting ");
    show_wait(&wait, this);
    printf("*****\n");*/
    std::list<InterNode*> inters;
    std::list<int>::const_iterator it;
    for (it = pivot.begin(); it != pivot.end(); ++it) {
        struct PartNode* own = get_own(*it);
        if (own->inter == NULL) {
            own->inter = new struct InterNode;
            own->inter->orig = own;
            own->inter->size = 0;
            inters.push_back(own->inter);
        }
        own->inter->data.push_back(*it);
        if (*it == own->begin) {
            own->begin = get_nxt(*it);
        } else if (*it == own->end) {
            own->end = get_pre(*it);
        }
        own->size--;
        own->inter->size++;
        remove_ele(*it);
    }
    for (std::list<InterNode*>::iterator inter_it = inters.begin(); inter_it != inters.end(); ++inter_it) {
        if ((*inter_it)->orig->size == 0) {
            remove_part((*inter_it)->orig);
            PartNode* inter = push_part(*inter_it);
            if ((*inter_it)->orig->wait != NULL) {
                (*inter_it)->orig->wait->orig = inter;
                inter->wait = (*inter_it)->orig->wait;
            }
            delete (*inter_it)->orig;
        } else {
            PartNode* inter = push_part(*inter_it);
            if ((*inter_it)->orig->wait != NULL) {
                inter->wait = wait.push_part(inter);
            } else if ((*inter_it)->size < (*inter_it)->orig->size) {
                inter->wait = wait.push_part(inter);
            } else {
                (*inter_it)->orig->wait = wait.push_part((*inter_it)->orig);
            }
	    (*inter_it)->orig->inter = NULL;
        }
        delete *inter_it;
    }/*
    printf("after refine ");
    show_part(this);
    printf("******\n");
    printf("waiting ");
    show_wait(&wait, this);
    printf("------\n");*/
}

}
