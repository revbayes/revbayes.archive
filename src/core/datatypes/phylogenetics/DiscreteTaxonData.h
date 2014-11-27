#ifndef DiscreteTaxonData_H
#define DiscreteTaxonData_H

#include "AbstractDiscreteTaxonData.h"
#include "DiscreteCharacterState.h"
#include "RbOptions.h"


#include <string>
#include <vector>

namespace RevBayesCore {

    template<class charType>
    class DiscreteTaxonData : public AbstractDiscreteTaxonData {
    
    public:
        DiscreteTaxonData(const std::string &tname);                                                                //!< Set type spec of container from type of elements
    
        charType&                               operator[](size_t i);                                               //!< Index op allowing change
        const charType&                         operator[](size_t i) const;                                         //!< Const index op
                       
        // TaxonData functions
        DiscreteTaxonData&                      add(const AbstractTaxonData &d);                                    //!< Addition operator used for example in '+=' statements
        DiscreteTaxonData&                      add(const AbstractDiscreteTaxonData &d);                            //!< Addition operator used for example in '+=' statements
        DiscreteTaxonData&                      add(const DiscreteTaxonData &d);                                    //!< Addition operator used for example in '+=' statements
        void                                    addCharacter(const CharacterState &newChar );                       //!< Push back a new character
        void                                    addCharacter(const DiscreteCharacterState &newChar );               //!< Push back a new character
        void                                    addCharacter(const charType &newChar );                             //!< Push back a new character
        const charType&                         getCharacter(size_t index) const;                                   //!< Get the character at position index
        charType&                               getCharacter(size_t index);                                         //!< Get the character at position index (non-const to return non-const character)
        size_t                                  getNumberOfCharacters(void) const;                                  //!< How many characters
        const std::string&                      getTaxonName(void) const;                                           //!< Return the name of the character vector
        void                                    setTaxonName(const std::string &tn);                                //!< Set the taxon name
        size_t                                  size(void) const;
        
    private:
        std::string                             taxonName;                                                          //!< Name of the taxon for this vector of characters               
        std::vector<charType>                   sequence;
    
    };

    // Global functions using the class
    template<class charType>
    std::ostream&                       operator<<(std::ostream& o, const DiscreteTaxonData<charType>& x);          //!< Overloaded output operator

    
}


#include "CharacterState.h"
#include "RbException.h"


/**
 * Constructor with taxon name.
 * Does nothing except instanciating the object.
 */
template<class charType>
RevBayesCore::DiscreteTaxonData<charType>::DiscreteTaxonData(const std::string &tname) : 
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
template<class charType>
charType& RevBayesCore::DiscreteTaxonData<charType>::operator[](size_t i) 
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
template<class charType>
const charType& RevBayesCore::DiscreteTaxonData<charType>::operator[](size_t i) const 
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
template<class charType>
RevBayesCore::DiscreteTaxonData<charType>& RevBayesCore::DiscreteTaxonData<charType>::add(const AbstractTaxonData &obsd)
{
    
    const DiscreteTaxonData<charType>* rhs = dynamic_cast<const DiscreteTaxonData<charType>* >( &obsd );
    if ( rhs == NULL )
    {
        throw RbException("Adding wrong character data type into TaxonData!!!");
    }
    
    
    return add( *rhs );
}


/**
 * Add another character data object to this character data object.
 *
 * \param[in]    obsd    The CharacterData object that should be added.
 */
template<class charType>
RevBayesCore::DiscreteTaxonData<charType>& RevBayesCore::DiscreteTaxonData<charType>::add(const AbstractDiscreteTaxonData &obsd)
{
    
    const DiscreteTaxonData<charType>* rhs = dynamic_cast<const DiscreteTaxonData<charType>* >( &obsd );
    if ( rhs == NULL )
    {
        throw RbException("Adding wrong character data type into TaxonData!!!");
    }
    
    
    return add( *rhs );
}


/**
 * Add another character data object to this character data object.
 *
 * \param[in]    obsd    The CharacterData object that should be added.
 */
template<class charType>
RevBayesCore::DiscreteTaxonData<charType>& RevBayesCore::DiscreteTaxonData<charType>::add(const DiscreteTaxonData<charType> &obsd)
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
template<class charType>
void RevBayesCore::DiscreteTaxonData<charType>::addCharacter( const CharacterState &newChar ) 
{
    
#   ifdef ASSERTIONS_ALL
    if ( dynamic_cast<const charType&>( newChar ) == NULL ) 
    {
        throw RbException("Inserting wrong character type into TaxonData!!!");
    }
#   endif
    
    addCharacter( static_cast<const charType &>(newChar) );
    
}


/**
 * Push back a new character.
 * 
 * \param[in]    newChar    The new character.
 */
template<class charType>
void RevBayesCore::DiscreteTaxonData<charType>::addCharacter( const DiscreteCharacterState &newChar ) 
{
    
#   ifdef ASSERTIONS_ALL
    if ( dynamic_cast<const charType&>( newChar ) == NULL ) 
    {
        throw RbException("Inserting wrong character type into TaxonData!!!");
    }
#   endif
    
    addCharacter( static_cast<const charType &>(newChar) );
    
}


/**
 * Push back a new character.
 * 
 * \param[in]    newChar    The new character.
 */
template<class charType>
void RevBayesCore::DiscreteTaxonData<charType>::addCharacter( const charType &newChar ) 
{
    
    sequence.push_back( newChar );
    
}


/**
 * Push back a new character.
 * 
 * \param[in]    index    The position character.
 */
template<class charType>
charType& RevBayesCore::DiscreteTaxonData<charType>::getCharacter(size_t index) 
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
 * \return            A non-const reference to the character
 */
template<class charType>
const charType& RevBayesCore::DiscreteTaxonData<charType>::getCharacter(size_t index) const 
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
template<class charType>
size_t RevBayesCore::DiscreteTaxonData<charType>::getNumberOfCharacters(void) const 
{
    
    return sequence.size();
}


/**
 * Get the name of the taxon.
 *
 * \return            The taxon's name.
 */
template<class charType>
const std::string& RevBayesCore::DiscreteTaxonData<charType>::getTaxonName(void) const 
{
    
    return taxonName;
}


/**
 * Set the name of the taxon.
 *
 * \param[in]    tn    The new name of the taxon.
 */
template<class charType>
void RevBayesCore::DiscreteTaxonData<charType>::setTaxonName(const std::string &tn)
{
    
    taxonName = tn;
}


/**
 * Get the size of the taxon which is the same as the number of characters.
 *
 * \return            The number of characters.
 */
template<class charType>
size_t RevBayesCore::DiscreteTaxonData<charType>::size(void) const 
{
    
    return sequence.size();
}



template<class charType>
std::ostream& RevBayesCore::operator<<(std::ostream& o, const DiscreteTaxonData<charType>& x) {
    
    o << x.getTaxonName() << ":" << std::endl;
    for (size_t i = 0; i < x.getNumberOfCharacters(); ++i) 
    {
        o << x[i];
    }
    o << std::endl;
    
    return o;
}


#endif
