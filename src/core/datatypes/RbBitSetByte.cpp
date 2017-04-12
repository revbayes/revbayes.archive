#include "RbBitSetByte.h"
#include "RbException.h"

#include <cmath>

using namespace RevBayesCore;



RbBitSetByte::RbBitSetByte(void) :
    value( 0 ),
    num_bits( 0 ),
    num_set_bits( 0 )
{
    
}


RbBitSetByte::RbBitSetByte(size_t n, bool set) :
    value( 0 ),
    num_bits( n ),
    num_set_bits( 0 )
{
  
    if ( set == true )
    {
        value = 0xFF;
    }
    else
    {
        value = 0x00;
    }
    
}


RbBitSetByte::~RbBitSetByte(void)
{

}


bool RbBitSetByte::operator[](size_t i) const
{
    // get the internal value
    return ( (value >> i) & 1 ) != 0;
}


/** Equals comparison */
bool RbBitSetByte::operator==(const RbBitSetByte& x) const
{
    
    return x.value == value;
}

/** Not-Equals comparison */
bool RbBitSetByte::operator!=(const RbBitSetByte& x) const
{
    
    return operator==(x) == false;
}


/** Smaller than comparison */
bool RbBitSetByte::operator<(const RbBitSetByte& x) const
{
    
    return x.value < value;
}

/** Bitwise and */
RbBitSetByte RbBitSetByte::operator&(const RbBitSetByte& x) const
{
    if ( x.num_bits != num_bits )
    {
        throw RbException("Cannot and RbBitSetBytes of unequal size");
    }
    RbBitSetByte bs(num_bits);
    bs.value = value & x.value;
    
    return bs;
}

/** Bitwise or */
RbBitSetByte RbBitSetByte::operator|(const RbBitSetByte& x) const
{
    if ( x.num_bits != num_bits )
    {
        throw RbException("Cannot or RbBitSetBytes of unequal sizes");
    }
    RbBitSetByte bs(num_bits);
    
    bs.value = x.value | value;

    return bs;
}

/** Bitwise xor */
RbBitSetByte RbBitSetByte::operator^(const RbBitSetByte& x) const
{
    if ( x.num_bits != num_bits )
    {
        throw RbException("Cannot xor RbBitSetBytes of unequal size");
    }
    RbBitSetByte bs(num_bits);
    
    bs.value = value ^ x.value;

    return bs;
}

/** Unary not */
RbBitSetByte& RbBitSetByte::operator~()
{
  
    value = ~value;
    num_set_bits = num_bits - num_set_bits;
    
    return *this;
}

/** Bitwise and assignment */
RbBitSetByte& RbBitSetByte::operator&=(const RbBitSetByte& x)
{
    if ( x.num_bits != num_bits )
    {
        throw RbException("Cannot and RbBitSetBytes of unequal size");
    }
    
    *this = *this & x;
    
    return *this;
}

/** Bitwise or assignment */
RbBitSetByte& RbBitSetByte::operator|=(const RbBitSetByte& x)
{
    if (x.num_bits != num_bits)
    {
        throw RbException("Cannot or RbBitSetBytes of unequal size");
    }
    
    *this = *this | x;
    
    return *this;
}


//void RbBitSetByte::clear(void)
//{
//    // reset the bitset
//    delete [] value;
//    value = NULL;
//
//    num_bits = 0;
//    num_set_bits = 0;
//}


void RbBitSetByte::clearBits(void)
{
    
    num_set_bits = 0;
    value = 0x00;
    
}

bool RbBitSetByte::empty(void) const
{
    return num_bits == 0;
}

void RbBitSetByte::flip(size_t i)
{
    value ^= 1 << i;
    num_set_bits += ( isSet(i) ? 1 : -1 );
}


size_t RbBitSetByte::getFirstSetBit( void ) const
{
    size_t index = 0;
    char tmp = value;
    while ( index < num_bits && (tmp & 1) > 0 )
    {
        ++index;
        tmp >>= 1;
        
    }
    
    return index;
}


size_t RbBitSetByte::getNumberSetBits( void ) const
{
    // get the internal value
    return num_set_bits;
}


bool RbBitSetByte::isSet(size_t i) const
{
    // get the internal value
    return ( (value >> i) & 1 ) != 0;
}

void RbBitSetByte::resize(size_t size)
{
    
    num_bits = size;
    value = 0x00;
    
}

void RbBitSetByte::set(size_t i)
{
    // set the internal value
    value |= 1 << i;
    
    ++num_set_bits;
}


size_t RbBitSetByte::size(void) const
{
    // get the size from the actual bitset
    return num_bits;
}


void RbBitSetByte::unset(size_t i)
{
    // set the internal value
    value &= ~(1 << i);
    
    --num_set_bits;
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const RevBayesCore::RbBitSetByte& x)
{
    
    o << "[";
    for (size_t i=0; i<x.size(); ++i)
    {
        o << ( x.isSet(i) ? "1" : "0");
    }
    o << "]";
    
    
    return o;
}
