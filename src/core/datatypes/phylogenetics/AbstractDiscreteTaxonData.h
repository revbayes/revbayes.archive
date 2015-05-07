#ifndef AbstractDiscreteTaxonData_H
#define AbstractDiscreteTaxonData_H

#include "AbstractTaxonData.h"
#include "DiscreteCharacterState.h"

#include <string>

namespace RevBayesCore {
    
    /**
     * Abstract class for all taxon objects.
     *
     * This abstract class provides the base class for all taxon data objects.
     * A taxon data object contains a vector of character objects and defines additional
     * convenience functions to access the data.
     * Note that this class is a pure interface and thus contains only pure virtual functions!
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2013-04-15, version 1.0
     */
    class AbstractDiscreteTaxonData : public AbstractTaxonData {
        
    public:
        virtual                                    ~AbstractDiscreteTaxonData(void) {}
        
        virtual DiscreteCharacterState&             operator[](size_t i) = 0;                                           //!< Index op allowing change
        virtual const DiscreteCharacterState&       operator[](size_t i) const = 0;                                     //!< Const index op
        
        // AbstractTaxonData functions
        virtual void                                addCharacter(const CharacterState &newChar ) = 0;                   //!< Push back a new character
        virtual void                                addCharacter(const DiscreteCharacterState &newChar ) = 0;           //!< Push back a new character
        virtual void                                addCharacter(const CharacterState &newChar, bool tf) = 0;           //!< Push back a new character
        virtual void                                addCharacter(const DiscreteCharacterState &newChar, bool tf) = 0;   //!< Push back a new character
        virtual AbstractDiscreteTaxonData&          concatenate(const AbstractTaxonData &d) = 0;                        //!< Concatenate sequences
        virtual AbstractDiscreteTaxonData&          concatenate(const AbstractDiscreteTaxonData &d) = 0;                //!< Concatenate sequences
        virtual const DiscreteCharacterState&       getCharacter(size_t index) const = 0;                               //!< Get the character at position index
        virtual DiscreteCharacterState&             getCharacter(size_t index) = 0;                                     //!< Get the character at position index (non-const to return non-const character)
        virtual size_t                              getNumberOfCharacters(void) const = 0;                              //!< How many characters
        virtual double                              getPercentageMissing(void) const = 0;                               //!< Returns the percentage of missing data for this sequence
        virtual const std::string&                  getTaxonName(void) const = 0;                                       //!< Return the name of the character vector
        virtual bool                                isCharacterResolved(size_t idx) const = 0;                          //!< Returns whether the character is fully resolved (e.g., "A" or "1.32") or not (e.g., "AC" or "?")
        virtual void                                setTaxonName(const std::string &tn) = 0;                            //!< Set the taxon name
        
    protected:
        AbstractDiscreteTaxonData() {}                                                                                  //!< Default constructor
        
    };
    
}

#endif
