#include "ContinuousTaxonData.h"
#include "RbException.h"


using namespace RevBayesCore;


/**
 * Default constructor.
 * Does nothing except instanciating the object.
 */
ContinuousTaxonData::ContinuousTaxonData(void) : 
taxonName(""), 
sequence() 
{
    
}


/**
 * Constructor with taxon name.
 * Does nothing except instanciating the object.
 */
ContinuousTaxonData::ContinuousTaxonData(const std::string &tname) : 
taxonName(tname), 
sequence() 
{
    
}


/**
 * Subscript const operator for convenience access.
 *
 * \param[in]    i    The position of the character.
 *
 * \return            A non-const reference to the character
 */
double& ContinuousTaxonData::operator[](size_t i)
{
    
    if (i >= sequence.size())
        throw RbException("Index out of bounds");
    
    return sequence[i];
}


/**
 * Subscript const operator for convenience access.
 *
 * \param[in]    i    The position of the character.
 *
 * \return            A const reference to the character
 */
const double& ContinuousTaxonData::operator[](size_t i) const
{
    
    if (i >= sequence.size())
        throw RbException("Index out of bounds");
    
    return sequence[i];
}


/**
 * Add another character data object to this character data object.
 *
 * \param[in]    obsd    The CharacterData object that should be added.
 */
ContinuousTaxonData& ContinuousTaxonData::concatenate(const AbstractTaxonData &obsd)
{
    
    const ContinuousTaxonData* rhs = dynamic_cast<const ContinuousTaxonData* >( &obsd );
    if ( rhs == NULL )
    {
        throw RbException("Adding wrong character data type into TaxonData!!!");
    }
    
    
    return concatenate( *rhs );
}


/**
 * Add another character data object to this character data object.
 *
 * \param[in]    obsd    The CharacterData object that should be added.
 */
ContinuousTaxonData& ContinuousTaxonData::concatenate(const ContinuousTaxonData &obsd)
{
    
    sequence.insert( sequence.end(), obsd.sequence.begin(), obsd.sequence.end() );
    
    // return a reference to this object
    return *this;
}


/**
 * Push back a new character.
 * 
 * \param[in]    newChar    The new character.
 */
void ContinuousTaxonData::addCharacter( const double &newChar )
{
    
    sequence.push_back( newChar );
    
}


/**
 * Push back a new character.
 * 
 * \param[in]    index    The position character.
 */
double& ContinuousTaxonData::getCharacter(size_t index)
{
    
    if (index >= sequence.size())
        throw RbException("Index out of bounds");
    
    return sequence[index];
}


/**
 * Get-operator for convenience access.
 *
 * \param[in]    i    The position of the character.
 *
 * \return            A const reference to the character
 */
const double& ContinuousTaxonData::getCharacter(size_t index) const
{
    
    if (index >= sequence.size())
        throw RbException("Index out of bounds");
    
    return sequence[index];
}


/**
 * Get the number of character stored in this object
 *
 * \return            The number of characters
 */
size_t ContinuousTaxonData::getNumberOfCharacters(void) const 
{
    
    return sequence.size();
}


/**
 * Get the name of the taxon.
 *
 * \return            The taxon's name.
 */
const std::string& ContinuousTaxonData::getTaxonName(void) const 
{
    
    return taxonName;
}


bool ContinuousTaxonData::isCharacterResolved(size_t idx) const {

    return true;
}


/**
 * Remove characters.
 *
 */
void ContinuousTaxonData::removeCharacters(const std::set<size_t> &idx)
{
    
    size_t alreadyRemoved = 0;
    for (std::set<size_t>::const_iterator it = idx.begin(); it != idx.end(); ++it)
    {
        size_t index = *it + alreadyRemoved;
        sequence.erase(sequence.begin() + index);
        ++alreadyRemoved;
    }
}


/**
 * Set the name of the taxon.
 *
 * \param[in]    tn    The new name of the taxon.
 */
void ContinuousTaxonData::setTaxonName(const std::string &tn)
{
    
    taxonName = tn;
}


/**
 * Get the size of the taxon which is the same as the number of characters.
 *
 * \return            The number of characters.
 */
size_t ContinuousTaxonData::size(void) const 
{
    
    return sequence.size();
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const RevBayesCore::ContinuousTaxonData& x) {
    
    o << x.getTaxonName() << ":" << std::endl;
    for (size_t i = 0; i < x.getNumberOfCharacters(); ++i) 
    {
        if ( i > 0 )
        {
            o << ", ";
        }
        o << x[i];
    }
    o << std::endl;
    
    return o;
}

