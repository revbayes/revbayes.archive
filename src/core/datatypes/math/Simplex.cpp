#include "Simplex.h"


using namespace RevBayesCore;


Simplex::Simplex( void ) : RbVector<double>()
{
    
}


Simplex::Simplex( size_t n ) : RbVector<double>( n )
{
    normalize();
}


Simplex::Simplex( size_t n, double v) : RbVector<double>(n,v)
{
    normalize();
}


Simplex::Simplex( const std::vector<double> &s ) : RbVector<double>(s)
{
    normalize();
}


Simplex::Simplex( const Simplex &m ) : RbVector<double>( m )
{
    
}


Simplex::~Simplex( void )
{
}


Simplex* Simplex::clone( void ) const
{
    return new Simplex(*this);
}



void Simplex::normalize( void )
{
    double sum = 0.0;
    for (size_t i=0; i<size(); ++i)
    {
        double v = this->operator[](i);
        if ( v < 0.0)
        {
            throw RbException("Cannot construct a simplex from negative numbers!");
        }
        sum += v;
    }
    for (size_t i=0; i<size(); ++i)
    {
         this->operator[](i) /= sum;
    }
}
