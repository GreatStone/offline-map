#include "bit_num.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <algorithm>

namespace utils {

BitNum::BitNum(const BitNum & src) {
    _bit_sz = src._bit_sz;
    int sz = src.get_grp_sz();
    _data = new unit[sz];
    _count = src._count;
    memcpy(_data, src._data, sizeof(unit) * sz);
}

void BitNum::reset(int index) {
    _data[index / 32] &= _zeros[index % 32];
    --_count;
}

void BitNum::reset() {
    for (int i = 0; i < get_grp_sz(); ++i) {
	_data[i] = _all_none;
    }
    _count = 0;
}

void BitNum::set(int index) {
    _data[index / 32] |= _ones[index % 32];
    ++_count;
}

void BitNum::set() {
    for (int i = 0; i < get_grp_sz(); ++i) {
	_data[i] = _all_set;
    }
    _count = _bit_sz;
}

void BitNum::flip(int index) {
    if (test(index)) {
	--_count;
    } else {
	++_count;
    }
    _data[index / 32] ^= _ones[index % 32];
}

void BitNum::flip() {
    for (int i = 0; i < get_grp_sz(); ++i) {
	_data[i] = ~_data[i];
    }
    _count = _bit_sz - _count;
}

inline bool BitNum::test(int index) const {
    return _data[index / 32] & _ones[index % 32];
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

int SetIterator::next() {
    while (!_set.get_grp(cur_grp)) {
	++cur_grp;
	if (cur_grp >= grp_sz) {
	    return -1;
	}
    }
    unit res = low_bit(static_cast<int32_t>(_set.get_grp(cur_grp)));
    return std::lower_bound(_ones, _ones + 32, res) - _ones;
}

unit SetIterator::low_bit(const int32_t& x) {
    return static_cast<unit>(x & (-x));
}

}

