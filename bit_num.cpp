#include "bit_num.h"

#include <stdio.h>
#include <string.h>

namespace utils {

BitNum::BitNum(const BitNum & src) {
    _bit_sz = src._bit_sz;
    int sz = src.get_grp_sz();
    _data = new unit[sz];
    memcpy(_data, src._data, sizeof(unit) * sz);
}

void BitNum::reset(int index) {
    _data[index / 32] &= _zeros[index % 32];
}

void BitNum::reset() {
    for (int i = 0; i < get_grp_sz(); ++i) {
	_data[i] = _all_none;
    }
}

void BitNum::set(int index) {
    _data[index / 32] |= _ones[index % 32];
}

void BitNum::set() {
    for (int i = 0; i < get_grp_sz(); ++i) {
	_data[i] = _all_set;
    }
}

void BitNum::flip(int index) {
    _data[index / 32] ^= _ones[index % 32];
}

void BitNum::flip() {
    for (int i = 0; i < get_grp_sz(); ++i) {
	_data[i] = ~_data[i];
    }
}

inline bool BitNum::test(int index) const {
    return _data[index / 32] & _ones[index % 32];
}

bool BitNum::empty() const {
    int sz = get_grp_sz();
    for (int i = 0; i < sz; ++i) {
	if (_data[i]) {
	    return false;
	}
    }
    return true;
}

BitNum BitNum::operator & (const BitNum & x) const {
    BitNum ret(_bit_sz);
    int sz = get_grp_sz();
    for (int i = 0; i < sz; ++i) {
	ret._data[i] = _data[i] & x._data[i];
    }
    return ret;
}

BitNum BitNum::operator | (const BitNum & x) const {
    BitNum ret(_bit_sz);
    int sz = get_grp_sz();
    for (int i = 0; i < sz; ++i) {
	ret._data[i] = _data[i] | x._data[i];
    }
    return ret;
}

BitNum BitNum::operator - (const BitNum & x) const {
    BitNum ret(_bit_sz);
    int sz = get_grp_sz();
    for (int i = 0; i < sz; ++i) {
	ret._data[i] = (_data[i] ^ x._data[i]) & _data[i];
    }
    return ret;
}

BitNum BitNum::operator ~ () const {
    BitNum ret(*this);
    int sz = get_grp_sz();
    for (int i = 0; i < sz; ++i) {
	ret._data[i] = ~ret._data[i];
    }
    return ret;
}

bool BitNum::operator < (const BitNum & x) const {
    int sz = get_grp_sz();
    for (int i = sz - 1; i >= 0; i++) {
	if (_data[i] < x._data[i]) {
	    return true;
	} else if (_data[i] > x._data[i]) {
	    return false;
	}
    }
    return false;
}

}

