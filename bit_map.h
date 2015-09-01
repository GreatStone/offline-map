#ifndef GREATSTONE_BIT_MAP_H
#define GREATSTONE_BIT_MAP_H

#include <stdint.h>

namespace utils {

typedef uint64_t bits;

int popcnt(bits a);

class BitMap {
public:
    BitMap();
    BitMap(const BitMap & src);

    BitMap & operator = (const BitMap & src);
    BitMap operator | (const BitMap & a) const;
    BitMap operator & (const BitMap & a) const;
    BitMap operator ~ () const;
    BitMap & operator |= (const BitMap & a);
    BitMap & operator &= (const BitMap & a);
    
    void set(int id);
    void reset(int id);
    void flip(int id);
    void set();
    void reset();
    void flip();
    bool test(int id) const;

    int lower_count(int range) const;
private:
    bits data[4];
};

}
#endif
