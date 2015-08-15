#ifndef GREATSTONE_BIT_NUM_H
#define GREATSTONE_BIT_NUM_H

#include <stdint.h>
#include <string.h>

namespace utils {

typedef uint32_t unit;

const unit _ones[] = {1, 1<<1, 1<<2, 1<<3, 1<<4, 1<<5, 1<<6, 1<<7, 1<<8, 1<<9, 1<<10, 1<<11, 1<<12, 1<<13, 1<<14, 1<<15, 1<<16, 1<<17, 1<<18, 1<<19, 1<<20, 1<<21, 1<<22, 1<<23, 1<<24, 1<<25, 1<<26, 1<<27, 1<<28, 1<<29, 1<<30, 1<<31};

    const unit _zeros[] = {0xFFFFFFFE, 0xFFFFFFFD, 0xFFFFFFFB, 0xFFFFFFF7, 0xFFFFFFEF, 0xFFFFFFDF, 0xFFFFFFBF, 0xFFFFFF7F, 0xFFFFFEFF, 0xFFFFFDFF, 0xFFFFFBFF, 0xFFFFF7FF, 0xFFFFEFFF, 0xFFFFDFFF, 0xFFFFBFFF, 0xFFFF7FFF, 0xFFFEFFFF, 0xFFFDFFFF, 0xFFFBFFFF, 0xFFF7FFFF, 0xFFEFFFFF, 0xFFDFFFFF, 0xFFBFFFFF, 0xFF7FFFFF, 0xFEFFFFFF, 0xFDFFFFFF, 0xFBFFFFFF, 0xF7FFFFFF, 0xEFFFFFFF, 0xDFFFFFFF, 0xBFFFFFFF, 0x7FFFFFFF};

class BitNum {
public:
    BitNum(int bit_sz) : _bit_sz(bit_sz) {
	if (bit_sz < 1) {
	    bit_sz = 1;
	} 
	_data = new unit[get_grp_sz()];
	memset(_data, 0, sizeof(unit) * get_grp_sz());
    }
    ~BitNum() {
	delete[] _data;
    }
    BitNum(const BitNum & src);

    void reset(int index);
    void reset();
    void set(int index);
    void set();
    void flip(int index);
    void flip();
    bool test(int index) const;
    bool empty() const;

    BitNum operator & (const BitNum & x) const;
    BitNum operator | (const BitNum & x) const;
    BitNum operator - (const BitNum & x) const;
    BitNum operator ~ () const;
    
    bool operator < (const BitNum & x) const;

    int get_bit_sz() const{
	return _bit_sz;
    }
private:
    int _bit_sz;
    unit* _data;
    static const unit _all_set = 0xFFFFFFFF;
    static const unit _all_none = 0x0;
    
    inline int get_grp_sz () const{
	return (_bit_sz + 32) /32;
    }
};
}

#endif
