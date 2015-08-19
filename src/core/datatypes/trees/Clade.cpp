#include "Clade.h"

#include <algorithm>
#include <iostream>
#include <sstream>


using namespace RevBayesCore;


/**
 * Default constructor required by the revlanguage code.
 * We use an empty string and an age of 0.0 for
 * this default object.
 */
Clade::Clade( void ) :
    age( 0.0 ),
    taxonNames( std::vector<std::string>() )
{
    
}


/**
 * Default constructor that instantiates the object.
 * Additionally, we sort the vector of taxon names.
 *
 * \param[in]   n    The vector containing the taxon names.
 */
Clade::Clade(const std::vector<std::string> &n, double a) :
    age( a ),
    taxonNames( n ) 
{
    
    // for identifiability we always keep the taxon names sorted
    std::sort(taxonNames.begin(), taxonNames.end());    
}

//SK
/**
 * Constructor that takes a single string object containing a list of taxon names,
 * in the format produced by the 'toString' function.
 * Additionally, we sort the vector of taxon names.
 *
 * \param[in]   n    String containing the taxon names.
 */
Clade::Clade(std::string cladeString, double a)
{
	char delim = ',';
	std::string tN;
    
	if (cladeString[0] == '{')
	{
		cladeString.erase(std::remove(cladeString.begin(), cladeString.end(), '{'), cladeString.end());
		cladeString.erase(std::remove(cladeString.begin(), cladeString.end(), '}'), cladeString.end());
	}
    
	std::stringstream ss(cladeString);
	while (std::getline(ss, tN, delim))
	{
		taxonNames.push_back(tN);
	}
    
	age = a;
    
	// for identifiability we always keep the taxon names sorted
	std::sort(taxonNames.begin(), taxonNames.end());
}


/**
 * Overloaded equals operator.
 * Only if we have the extact same taxon names then these two clades are equal.
 */
bool Clade::operator==(const Clade &c) const 
{
    
    if ( c.size() != taxonNames.size() )
        return false;
    
    for (size_t i = 0; i < taxonNames.size(); ++i) 
    {
        if ( taxonNames[i] != c.getTaxonName(i) )
        {
            return false;
        }
    }
    
    return true;
}


/**
 * Not equals operator that uses the equals operator.
 */
bool Clade::operator!=(const Clade &c) const 
{
    return !operator==( c );
}


/**
 * Less than operator so that we can sort the clades.
 */
bool Clade::operator<(const Clade &c) const 
{
    return taxonNames.size() < c.size();
}


/**
 * Less than operator so that we can sort the clades.
 */
bool Clade::operator<=(const Clade &c) const
{
    return operator<( c ) || operator==( c );
}



/**
 * Get the const-iterator to the first taxon name.
 */
std::vector<std::string>::const_iterator Clade::begin(void) const 
{
    return taxonNames.begin();
}


/**
 * Get the iterator to the first taxon name.
 */
std::vector<std::string>::iterator Clade::begin(void) 
{
    return taxonNames.begin();
}


/**
 * Get the const-iterator after the last taxon name.
 */
std::vector<std::string>::const_iterator Clade::end(void) const 
{
    return taxonNames.end();
}


/**
 * Get the iterator after the last taxon name.
 */
std::vector<std::string>::iterator Clade::end(void) 
{
    return taxonNames.end();
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself 
 */
Clade* Clade::clone(void) const 
{
    return new Clade(*this);
}


/**
 * Get the clade age.
 *
 * \return    The stored age.
 */
double Clade::getAge( void ) const 
{
    
    return age;
}


/**
 * Get the taxon name at position i.
 *
 * \param[in]    i    The index for the taxon name we are interested in.
 *
 * \return       The name of the taxon.
 */
const std::string& Clade::getTaxonName(size_t i) const 
{
    return taxonNames[i];
}


/**
 * Get all taxon names.
 *
 * \return       The vector of taxon names.
 */
const std::vector<std::string>& Clade::getTaxonNames( void ) const 
{
    return taxonNames;
}


/**
 * Get the number of taxa contained in this clade.
 *
 * \return       Size of the taxon name vector.
 */
size_t Clade::size(void) const 
{
    return taxonNames.size();
}


/**
 * Write the value of this clade as a string.
 *
 * \return    A single string containing the entire clade.
 */
std::string Clade::toString( void ) const
{
    std::string s = "{";
    
    for (size_t i = 0; i < taxonNames.size(); ++i) 
    {
        if ( i > 0 )
        {
            s += ",";
        }
        s += taxonNames[i];
    }
    s += "}";
    
    return s;
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const Clade& x) {
   
    o << x.toString();
   
    return o;
}

