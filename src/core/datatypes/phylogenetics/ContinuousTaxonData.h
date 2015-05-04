#ifndef ContinuousTaxonData_H
#define ContinuousTaxonData_H

#include "AbstractTaxonData.h"
#include "RbOptions.h"


#include <string>
#include <vector>

namespace RevBayesCore {
    
    class ContinuousTaxonData : public AbstractTaxonData {
        
    public:
        ContinuousTaxonData(void);                                                                                  //!< Set type spec of container from type of elements
        ContinuousTaxonData(const std::string &tname);                                                              //!< Set type spec of container from type of elements
        
        double&                                 operator[](size_t i);                                               //!< Index op allowing change
        const double&                           operator[](size_t i) const;                                         //!< Const index op
        
        // TaxonData functions
        ContinuousTaxonData&                    concatenate(const AbstractTaxonData &d);                            //!< Concatenate sequences
        ContinuousTaxonData&                    concatenate(const ContinuousTaxonData &d);                          //!< Concatenate sequences
        void                                    addCharacter(const double &newChar );                               //!< Push back a new character
        const double&                           getCharacter(size_t index) const;                                   //!< Get the character at position index
        double&                                 getCharacter(size_t index);                                         //!< Get the character at position index (non-const to return non-const character)
        size_t                                  getNumberOfCharacters(void) const;                                  //!< How many characters
        const std::string&                      getTaxonName(void) const;                                           //!< Return the name of the character vector
        bool                                    isCharacterResolved(size_t idx) const;                          //!< Returns whether the character is fully resolved (e.g., "A" or "1.32") or not (e.g., "AC" or "?")
        void                                    removeCharacters(const std::set<size_t> &i);                        //!< Remove all the characters with a given index
        void                                    setTaxonName(const std::string &tn);                                //!< Set the taxon name
        size_t                                  size(void) const;
        
    private:
        std::string                             taxonName;                                                          //!< Name of the taxon for this vector of characters               
        std::vector<double>                     sequence;
        std::vector<bool>                       isResolved;
        
    };
    
    // Global functions using the class
    std::ostream&                               operator<<(std::ostream& o, const ContinuousTaxonData& x);          //!< Overloaded output operator
    
    
}


#endif
