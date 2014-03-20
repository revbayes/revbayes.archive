#include "ContinuousTaxonData.h"
#include "ContinuousCharacterState.h"
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
ContinuousCharacterState& ContinuousTaxonData::operator[](size_t i) 
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
const ContinuousCharacterState& ContinuousTaxonData::operator[](size_t i) const 
{
    
    if (i >= sequence.size())
        throw RbException("Index out of bounds");
    
    return sequence[i];
}



/**
 * Push back a new character.
 * 
 * \param[in]    newChar    The new character.
 */
void ContinuousTaxonData::addCharacter( const CharacterState &newChar ) 
{
    
#ifdef ASSERTIONS_ALL
    if ( dynamic_cast<const charType&>( newChar ) == NULL ) 
    {
        throw RbException("Inserting wrong character type into TaxonData!!!");
    }
#endif
    
    addCharacter( static_cast<const ContinuousCharacterState &>(newChar) );
    
}


/**
 * Push back a new character.
 * 
 * \param[in]    newChar    The new character.
 */
void ContinuousTaxonData::addCharacter( const ContinuousCharacterState &newChar ) 
{
    
    sequence.push_back( newChar );
    
}


/**
 * Push back a new character.
 * 
 * \param[in]    index    The position character.
 */
ContinuousCharacterState& ContinuousTaxonData::getCharacter(size_t index) 
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
const ContinuousCharacterState& ContinuousTaxonData::getCharacter(size_t index) const 
{
    
    if (index >= sequence.size())
        throw RbException("Index out of bounds");
    
    return sequence[index];
}


/**
 * Getter  for convenience access.
 *
 * \param[in]    i    The position of the character.
 *
 * \return            A non-const reference to the character
 */
ContinuousCharacterState& ContinuousTaxonData::getElement(size_t i) 
{
    
    if (i >= sequence.size())
        throw RbException("Index out of bounds");
    
    return sequence[i];
}


/**
 * Getter for convenience access.
 *
 * \param[in]    i    The position of the character.
 *
 * \return            A const reference to the character
 */
const ContinuousCharacterState& ContinuousTaxonData::getElement(size_t i) const 
{
    
    if (i >= sequence.size())
        throw RbException("Index out of bounds");
    
    return sequence[i];
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


/**
 * Set the name of the taxon.
 *
 * \param[in]    tn    The new name of the taxon.
 */
void ContinuousTaxonData::setTaxonName(std::string tn) 
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



template<class charType>
std::ostream& operator<<(std::ostream& o, const ContinuousTaxonData& x) {
    
    o << x.getTaxonName() << ":" << std::endl;
    for (size_t i = 0; i < x.getNumberOfCharacters(); ++i) 
    {
        o << x[i];
    }
    o << std::endl;
    
    return o;
}

