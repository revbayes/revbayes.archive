#include "RbBitSet.h"
#include "RbException.h"

using namespace RevBayesCore;



RbBitSet::RbBitSet(void) :
    value(2,false),
    num_set_bits( 0 )
{
//    throw RbException("Whaaaat???");
    
}


RbBitSet::RbBitSet(size_t n) : value(n,false),
    num_set_bits( 0 )
{
    
}


bool RbBitSet::operator[](size_t i) const
{
    // get the internal value
    return value[i];
}


/** Equals comparison */
bool RbBitSet::operator==(const RbBitSet& x) const
{
    
    return x.value == value;
}

/** Not-Equals comparison */
bool RbBitSet::operator!=(const RbBitSet& x) const
{
    
    return operator==(x) == false;
}


/** Smaller than comparison */
bool RbBitSet::operator<(const RbBitSet& x) const
{
    
    return x.value < value;
}


void RbBitSet::clear(void)
{
    // reset the bitset
    value = std::vector<bool>(value.size(),false);
    num_set_bits = 0;
}


void RbBitSet::flip(size_t i)
{
    value[i] = ( value[i] == false );
    num_set_bits += ( value[i] ? 1 : -1 );
}


size_t RbBitSet::getNumberSetBits( void ) const
{
    // get the internal value
    return num_set_bits;
}


bool RbBitSet::isSet(size_t i) const
{
    // get the internal value
    return value[i];
}


void RbBitSet::set(size_t i)
{
    // set the internal value
    value[i] = true;
    ++num_set_bits;
}


size_t RbBitSet::size(void) const
{
    // get the size from the actual bitset
    return value.size();
}


void RbBitSet::unset(size_t i)
{
    // set the internal value
    value[i] = false;
    --num_set_bits;
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const RevBayesCore::RbBitSet& x)
{
    
    o << "[";
    for (size_t i=0; i<x.size(); ++i)
    {
        o << ( x.isSet(i) ? "1" : "0");
    }
    o << "]";
    
    
    return o;
}
