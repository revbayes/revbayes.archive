#include "RbBitSetGeneral.h"
#include "RbException.h"

using namespace RevBayesCore;



RbBitSetGeneral::RbBitSetGeneral(void) :
num_set_bits( 0 )
{
    
}


RbBitSetGeneral::RbBitSetGeneral(size_t n, bool def) :
    value(n, def),
    num_set_bits( 0 )
{
    
}


bool RbBitSetGeneral::operator[](size_t i) const
{
    // get the internal value
    return value[i];
}


/** Equals comparison */
bool RbBitSetGeneral::operator==(const RbBitSetGeneral& x) const
{
    
    return x.value == value;
}

/** Not-Equals comparison */
bool RbBitSetGeneral::operator!=(const RbBitSetGeneral& x) const
{
    
    return operator==(x) == false;
}


/** Smaller than comparison */
bool RbBitSetGeneral::operator<(const RbBitSetGeneral& x) const
{
    
    return x.value < value;
}

/** Bitwise and */
RbBitSetGeneral RbBitSetGeneral::operator&(const RbBitSetGeneral& x) const
{
    if(x.value.size() != value.size())
    {
        throw(RbException("Cannot and RbBitSetGenerals of unequal size"));
    }
    RbBitSetGeneral r(value.size());
    for(size_t i = 0; i < value.size(); i++)
    {
        if( value[i] && x.value[i] )
        {
            r.set(i);
        }
    }
    return r;
}

/** Bitwise or */
RbBitSetGeneral RbBitSetGeneral::operator|(const RbBitSetGeneral& x) const
{
    if(x.value.size() != value.size())
    {
        throw(RbException("Cannot or RbBitSetGenerals of unequal sizes"));
    }
    RbBitSetGeneral r(value.size());
    for(size_t i = 0; i < value.size(); i++)
    {
        if( value[i] || x.value[i] )
        {
            r.set(i);
        }
    }
    return r;
}

/** Bitwise xor */
RbBitSetGeneral RbBitSetGeneral::operator^(const RbBitSetGeneral& x) const
{
    if(x.value.size() != value.size())
    {
        throw(RbException("Cannot xor RbBitSetGenerals of unequal size"));
    }
    RbBitSetGeneral r(value.size());
    for(size_t i = 0; i < value.size(); i++)
    {
        if( value[i] != x.value[i] )
        {
            r.set(i);
        }
    }
    return r;
}

/** Unary not */
RbBitSetGeneral& RbBitSetGeneral::operator~()
{
    for(size_t i = 0; i < value.size(); i++)
    {
        value[i] = !value[i];
    }
    
    num_set_bits = value.size() - num_set_bits;
    
    return *this;
}

/** Bitwise and assignment */
RbBitSetGeneral& RbBitSetGeneral::operator&=(const RbBitSetGeneral& x)
{
    if(x.value.size() != value.size())
    {
        throw(RbException("Cannot and RbBitSetGenerals of unequal size"));
    }
    
    *this = *this & x;
    
    return *this;
}

/** Bitwise or assignment */
RbBitSetGeneral& RbBitSetGeneral::operator|=(const RbBitSetGeneral& x)
{
    if(x.value.size() != value.size())
    {
        throw(RbException("Cannot or RbBitSetGenerals of unequal size"));
    }
    
    *this = *this | x;
    
    return *this;
}


void RbBitSetGeneral::clear(void)
{
    // reset the bitset
    value = std::vector<bool>(value.size(),false);
    num_set_bits = 0;
}

bool RbBitSetGeneral::empty(void) const
{
    return value.empty();
}

void RbBitSetGeneral::flip(size_t i)
{
    value[i] = ( value[i] == false );
    num_set_bits += ( value[i] ? 1 : -1 );
}


size_t RbBitSetGeneral::getNumberSetBits( void ) const
{
    // get the internal value
    return num_set_bits;
}


bool RbBitSetGeneral::isSet(size_t i) const
{
    // get the internal value
    return value[i];
}

void RbBitSetGeneral::resize(size_t size)
{
    value.resize(size);
}

void RbBitSetGeneral::set(size_t i)
{
    // set the internal value
    value[i] = true;
    ++num_set_bits;
}


size_t RbBitSetGeneral::size(void) const
{
    // get the size from the actual bitset
    return value.size();
}


void RbBitSetGeneral::unset(size_t i)
{
    // set the internal value
    value[i] = false;
    --num_set_bits;
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const RevBayesCore::RbBitSetGeneral& x)
{
    
    o << "[";
    for (size_t i=0; i<x.size(); ++i)
    {
        o << ( x.isSet(i) ? "1" : "0");
    }
    o << "]";
    
    
    return o;
}
