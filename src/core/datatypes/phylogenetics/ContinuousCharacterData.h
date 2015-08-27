#ifndef ContinuousCharacterData_H
#define ContinuousCharacterData_H

#include "ContinuousTaxonData.h"
#include "HomologousCharacterData.h"

#include <map>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

namespace RevBayesCore {
    
    /**
     * Class for all Continuous character data objects.
     *
     * The continuous character data class is class for all continuous character data objects.
     * Here the data is stored via a set of ContinuousTaxonData objects.
     * Several convenience functions are provided to access, store and delete the data.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-02-16, version 1.0
     */
    class ContinuousCharacterData : public HomologousCharacterData {
        
    public:
                                                        ContinuousCharacterData(void);                                              //!< Default constructor
        
        // Overloaded operators
        const ContinuousTaxonData&                      operator[](size_t i) const;                                                 //!< Subscript operator (const)
        
        // implemented methods of the Cloneable interface
        ContinuousCharacterData*                        clone(void) const;
        
        // CharacterData functions
        ContinuousCharacterData&                        concatenate(const ContinuousCharacterData &d);                              //!< Concatenate data matrices
        ContinuousCharacterData&                        concatenate(const HomologousCharacterData &d);                              //!< Concatenate data matrices
        ContinuousCharacterData&                        concatenate(const AbstractCharacterData &d);                                //!< Concatenate data matrices
        void                                            excludeAllCharacters(void);                                                 //!< Exclude all characters
        void                                            excludeCharacter(size_t i);                                                 //!< Exclude character
        const double&                                   getCharacter(size_t tn, size_t cn) const;                                   //!< Return a reference to a character element in the character matrix
        std::string                                     getDatatype(void) const;
        size_t                                          getNumberOfCharacters(void) const;                                          //!< Number of characters
        size_t                                          getNumberOfIncludedCharacters(void) const;                                  //!< Number of characters
        ContinuousTaxonData&                            getTaxonData(size_t tn);                                                    //!< Return a reference to a sequence in the character matrix
        const ContinuousTaxonData&                      getTaxonData(size_t tn) const;                                              //!< Return a reference to a sequence in the character matrix
        ContinuousTaxonData&                            getTaxonData(const std::string &tn);                                        //!< Return a reference to a sequence in the character matrix
        const ContinuousTaxonData&                      getTaxonData(const std::string &tn) const;                                  //!< Return a reference to a sequence in the character matrix
        void                                            includeCharacter(size_t i);                                                 //!< Include character
        bool                                            isCharacterExcluded(size_t i) const;                                        //!< Is the character excluded
        bool                                            isCharacterResolved(size_t txIdx, size_t chIdx) const;                      //!< Returns whether the character is fully resolved (e.g., "A" or "1.32") or not (e.g., "AC" or "?")
        bool                                            isCharacterResolved(const std::string &tn, size_t chIdx) const;             //!< Returns whether the character is fully resolved (e.g., "A" or "1.32") or not (e.g., "AC" or "?")
        void                                            removeExludedCharacters(void);                                              //!< Remove all the excluded characters
        void                                            restoreCharacter(size_t i);                                                 //!< Restore character

        
    protected:
        std::set<size_t>                                deletedCharacters;                                                          //!< Set of deleted characters

    };
    
    
}

#endif

