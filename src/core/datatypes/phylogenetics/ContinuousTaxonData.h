#ifndef ContinuousTaxonData_H
#define ContinuousTaxonData_H

#include "AbstractTaxonData.h"
#include "ContinuousCharacterState.h"
#include "RbOptions.h"


#include <string>
#include <vector>

namespace RevBayesCore {
    
    class ContinuousTaxonData : public AbstractTaxonData {
        
    public:
        ContinuousTaxonData(void);                                                                                    //!< Set type spec of container from type of elements
        ContinuousTaxonData(const std::string &tname);                                                                //!< Set type spec of container from type of elements
        
        ContinuousCharacterState&               operator[](size_t i);                                               //!< Index op allowing change
        const ContinuousCharacterState&         operator[](size_t i) const;                                         //!< Const index op
        
        // TaxonData functions
        void                                    addCharacter(const CharacterState &newChar );                       //!< Push back a new character
        void                                    addCharacter(const ContinuousCharacterState &newChar );               //!< Push back a new character
        const ContinuousCharacterState&         getCharacter(size_t index) const;                                   //!< Get the character at position index
        ContinuousCharacterState&               getCharacter(size_t index);                                         //!< Get the character at position index (non-const to return non-const character)
        ContinuousCharacterState&               getElement(size_t i);                                               //!< Index op allowing change
        const ContinuousCharacterState&         getElement(size_t i) const;                                         //!< Const index op
        size_t                                  getNumberOfCharacters(void) const;                                  //!< How many characters
        const std::string&                      getTaxonName(void) const;                                           //!< Return the name of the character vector
        void                                    setTaxonName(std::string tn);                                       //!< Set the taxon name
        size_t                                  size(void) const;
        
    private:
        std::string                             taxonName;                                                          //!< Name of the taxon for this vector of characters               
        std::vector<ContinuousCharacterState>   sequence;
        
    };
    
    // Global functions using the class
    template<class charType>
    std::ostream&                       operator<<(std::ostream& o, const ContinuousTaxonData& x);          //!< Overloaded output operator
    
    
}


#endif
