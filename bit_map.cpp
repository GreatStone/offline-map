#include "bit_map.h"

namespace utils {

int popcnt(bits n) {
    n = (n & 0x5555555555555555) + ((n >> 1) & 0x5555555555555555);
    n = (n & 0x3333333333333333) + ((n >> 2) & 0x3333333333333333);
    n = (n & 0x0f0f0f0f0f0f0f0f) + ((n >> 4) & 0x0f0f0f0f0f0f0f0f);
    n = (n & 0x00ff00ff00ff00ff) + ((n >> 8) & 0x00ff00ff00ff00ff);
    n = (n & 0x0000ffff0000ffff) + ((n >> 16) & 0x0000ffff0000ffff);
    n = (n & 0x00000000ffffffff) + ((n >> 32) & 0x00000000ffffffff);
    return static_cast<int>(n);
}

BitMap::BitMap() {
    for (int i = 0; i < 4; ++i) {
        data[i] = 0;
    }
}

BitMap::BitMap(const BitMap & src) {
    for (int i = 0; i < 4; ++i) {
        data[i] = src.data[i];
    }
}

BitMap & BitMap::operator = (const BitMap & src) {
    if (this == & src) {
        return *this;
    }
    for (int i = 0; i < 4; ++i) {
        data[i] = src.data[i];
    }
    return *this;
}

BitMap BitMap::operator | (const BitMap & a) const {
    BitMap ret;
    for (int i = 0; i < 4; ++i) {
        ret.data[i] = data[i] | a.data[i];
    }
    return ret;
}

BitMap BitMap::operator & (const BitMap & a) const {
    BitMap ret;
    for (int i = 0; i < 4; ++i) {
        ret.data[i] = data[i] & a.data[i];
    }
    return ret;
}

BitMap BitMap::operator ~ () const {
    BitMap ret;
    for (int i = 0; i < 4; ++i) {
        ret.data[i] = ~data[i];
    }
    return ret;
}

BitMap & BitMap::operator |= (const BitMap & a) {
    for (int i = 0; i < 4; ++i) {
        data[i] |= a.data[i];
    }
    return *this;
}

BitMap & BitMap::operator &= (const BitMap & a) {
    for (int i = 0; i < 4; ++i) {
        data[i] &= a.data[i];
    }
    return *this;
}

void BitMap::set(int id) {
    data[id >> 6] |= 1ull << (id & 0x3f);
}

void BitMap::set() {
    for (int i = 0; i < 4; ++i) {
        data[i] = 0xffffffffffffffff;
    }
}

void BitMap::reset(int id) {
    data[id >> 6] &= 1ull << (~(id & 0x3f));
}

void BitMap::reset() {
    for (int i = 0; i < 4; ++i) {
        data[i] = 0;
    }
}

void BitMap::flip() {
    for (int i = 0; i < 4; ++i) {
        data[i] = ~data[i];
    }
}

bool BitMap::test(int id) const{
    return data[id >> 6] & (1ull << ~(id & 0x3f));
}

int BitMap::lower_count(int range) const{
    int ret = 0;
    int grpsz = range >> 6;
    int mod = range & 0x3f;
    for (int i = 0; i < grpsz; ++i) {
        ret += popcnt(data[i]);
    }
    if (mod) {
        ret += popcnt(data[grpsz] & ((1ull << mod) - 1));
    }
    return ret;
}

}
