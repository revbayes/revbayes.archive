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
        void                                    addCharacter(const CharacterState &newChar );                       //!< Push back a new character
        void                                    addCharacter(const DiscreteCharacterState &newChar );               //!< Push back a new character
        void                                    addCharacter(const charType &newChar );                             //!< Push back a new character
        void                                    addCharacter(const CharacterState &newChar, bool tf);               //!< Push back a new character
        void                                    addCharacter(const DiscreteCharacterState &newChar, bool tf);       //!< Push back a new character
        void                                    addCharacter(const charType &newChar, bool tf);                     //!< Push back a new character
        DiscreteTaxonData&                      concatenate(const AbstractTaxonData &d);                            //!< Concatenate sequences
        DiscreteTaxonData&                      concatenate(const AbstractDiscreteTaxonData &d);                    //!< Concatenate sequences
        DiscreteTaxonData&                      concatenate(const DiscreteTaxonData &d);                            //!< Concatenate sequences
        const charType&                         getCharacter(size_t index) const;                                   //!< Get the character at position index
        charType&                               getCharacter(size_t index);                                         //!< Get the character at position index (non-const to return non-const character)
        size_t                                  getNumberOfCharacters(void) const;                                  //!< How many characters
        const std::string&                      getTaxonName(void) const;                                           //!< Return the name of the character vector
        bool                                    isCharacterResolved(size_t idx) const;                          //!< Returns whether the character is fully resolved (e.g., "A" or "1.32") or not (e.g., "AC" or "?")
        void                                    removeCharacters(const std::set<size_t> &i);                        //!< Remove all the characters with a given index
        void                                    setTaxonName(const std::string &tn);                                //!< Set the taxon name
        size_t                                  size(void) const;
        
    private:
        std::string                             taxonName;                                                          //!< Name of the taxon for this vector of characters               
        std::vector<charType>                   sequence;
        std::vector<bool>                       isResolved;
    
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
    sequence() {
    
}


/**
 * Subscript const operator for convenience access.
 *
 * \param[in]    i    The position of the character.
 *
 * \return            A non-const reference to the character
 */
template<class charType>
charType& RevBayesCore::DiscreteTaxonData<charType>::operator[](size_t i) {
    
    if (i >= sequence.size())
    {
        throw RbException("Index out of bounds");
    }
    
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
    {
        throw RbException("Index out of bounds");
    }
    
    return sequence[i];
}


/**
 * Add another character data object to this character data object.
 *
 * \param[in]    obsd    The CharacterData object that should be added.
 */
template<class charType>
RevBayesCore::DiscreteTaxonData<charType>& RevBayesCore::DiscreteTaxonData<charType>::concatenate(const AbstractTaxonData &obsd)
{
    
    const DiscreteTaxonData<charType>* rhs = dynamic_cast<const DiscreteTaxonData<charType>* >( &obsd );
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
template<class charType>
RevBayesCore::DiscreteTaxonData<charType>& RevBayesCore::DiscreteTaxonData<charType>::concatenate(const AbstractDiscreteTaxonData &obsd)
{
    
    const DiscreteTaxonData<charType>* rhs = dynamic_cast<const DiscreteTaxonData<charType>* >( &obsd );
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
template<class charType>
RevBayesCore::DiscreteTaxonData<charType>& RevBayesCore::DiscreteTaxonData<charType>::concatenate(const DiscreteTaxonData<charType> &obsd)
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
    isResolved.push_back(true);
}


/**
 * Push back a new character.
 * 
 * \param[in]    newChar    The new character.
 */
template<class charType>
void RevBayesCore::DiscreteTaxonData<charType>::addCharacter( const CharacterState &newChar, bool tf )
{
    
#   ifdef ASSERTIONS_ALL
    if ( dynamic_cast<const charType&>( newChar ) == NULL ) 
    {
        throw RbException("Inserting wrong character type into TaxonData!!!");
    }
#   endif
    
    addCharacter( static_cast<const charType &>(newChar), tf );
}


/**
 * Push back a new character.
 * 
 * \param[in]    newChar    The new character.
 */
template<class charType>
void RevBayesCore::DiscreteTaxonData<charType>::addCharacter( const DiscreteCharacterState &newChar, bool tf )
{
    
#   ifdef ASSERTIONS_ALL
    if ( dynamic_cast<const charType&>( newChar ) == NULL ) 
    {
        throw RbException("Inserting wrong character type into TaxonData!!!");
    }
#   endif
    
    addCharacter( static_cast<const charType &>(newChar), tf );
    
}


/**
 * Push back a new character.
 * 
 * \param[in]    newChar    The new character.
 */
template<class charType>
void RevBayesCore::DiscreteTaxonData<charType>::addCharacter( const charType &newChar, bool tf )
{
    
    sequence.push_back( newChar );
    isResolved.push_back(tf);
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
    {
        throw RbException("Index out of bounds");
    }
    
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
    {
        throw RbException("Index out of bounds");
    }
    
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
 * Determines whether the character at idx is "resolved," meaning there is a fully-resolved character state at that position.
 *
 * \return            True (resolved) or false (unresolved).
 */
template<class charType>
bool RevBayesCore::DiscreteTaxonData<charType>::isCharacterResolved(size_t idx) const {

    if (idx >= isResolved.size())
        {
        throw RbException("Index out of bounds");
        }
    return isResolved[idx];
}


/**
 * Remove characters.
 *
 */
template<class charType>
void RevBayesCore::DiscreteTaxonData<charType>::removeCharacters(const std::set<size_t> &idx)
{
    
//    size_t alreadyRemoved = 0;
//    for (std::set<size_t>::const_iterator it = idx.begin(); it != idx.end(); ++it)
//    {
//        size_t index = *it - alreadyRemoved;
//        sequence.erase(sequence.begin() + index);
//        ++alreadyRemoved;
//    }
    
    std::vector<charType> included;
    for (size_t i = 0; i < sequence.size(); ++i)
    {
        if ( idx.find(i) == idx.end() )
        {
            included.push_back( sequence[i] );
        }
    }
    
    sequence = included;
        
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
