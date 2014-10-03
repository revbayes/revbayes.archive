#include "Taxon.h"

using namespace RevBayesCore;



/**
 * Default constructor
 */
Taxon::Taxon(void) :
    date(  ),
    name( "" ),
    speciesName( "" )
{
    
}


/**
 * Constructor simply initiating the object and its members.
 *
 * \param[in]    n     The name of the taxon.
 */
Taxon::Taxon(const std::string &n, const std::string &sn) :
    date(  ),
    name( n ),
    speciesName( sn )
{
    if (sn == "" ) {
        speciesName = n; //we put as default species name the tip name
    }
}


/**
 * Constructor from another Taxon.
 *
 * \param[in]    t     another Taxon object.
 */
Taxon::Taxon(const Taxon &t) :
date( t.date ),
name( t.name ),
speciesName( t.speciesName )
{
}


/**
 * Equals operator.
 * We check the species name and the individuals name.
 */
bool Taxon::operator==(const RevBayesCore::Taxon &t) const
{
    
    if ( speciesName != t.speciesName )
    {
        return false;
    }
    
    if ( name != t.name)
    {
        return false;
    }
    
    //Shouldn't we also compare dates?
    
    return true;
}


/**
 * Not equals operator. We simply invert the result of the equals operation.
 */
bool Taxon::operator!=(const RevBayesCore::Taxon &t) const
{
    
    return !operator==(t);
}


/**
 * Less-than operator.
 * We check first the species name and then the indidivuals name.
 */
bool Taxon::operator<(const RevBayesCore::Taxon &t) const
{
    
    if ( speciesName < t.speciesName)
    {
        return true;
    }
    else if ( speciesName > t.speciesName )
    {
        return false;
    }
    
    if ( name < t.name )
    {
        return true;
    }
    else if ( name > t.name )
    {
        return false;
    }
    
    // by default return true.
    return true;
}


Taxon& Taxon::operator=(const Taxon& t) {
    
    if (this != &t) {
        name = t.name;
        date = t.date;
        speciesName = t.speciesName;
    }
    
    return *this;

    
}


Taxon* Taxon::clone(void) const {
    return new Taxon(*this);
}



/**
 * Get the date info for this taxon.
 *
 * \return    The date.
 */
const TimeAndDate& Taxon::getDate( void ) const
{
    return date;
}


/**
 * Get the name info for this taxon.
 *
 * \return    The name.
 */
const std::string& Taxon::getName( void ) const
{
    return name;
}


/**
 * Get the species name for this taxon.
 *
 * \return    The species name.
 */
const std::string& Taxon::getSpeciesName( void ) const
{
    return speciesName;
}


/**
 * Set the date info for this taxon.
 *
 * \param[in]    d     The date.
 */
void Taxon::setDate( const TimeAndDate &d )
{
    date = d;
}


/**
 * Set the name info for this taxon.
 *
 * \param[in]    n     The name.
 */
void Taxon::setName( const std::string &n )
{
    name = n;
}


/**
 * Set the species name for this taxon.
 *
 * \param[in]    n     The species name.
 */
void Taxon::setSpeciesName( const std::string &sn )
{
    speciesName = sn;
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const Taxon& x) {
    o << x.getName() << ":" << x.getSpeciesName() << ":" << x.getDate();    
    return o;
}
