#ifndef ContinuousCharacterData_H
#define ContinuousCharacterData_H

#include "ContinuousTaxonData.h"
#include "HomologousCharacterData.h"
#include "MemberObject.h"

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
    class ContinuousCharacterData : public HomologousCharacterData, public MemberObject<double> {

    public:
                                                        ContinuousCharacterData(void);                                              //!< Default constructor
        
        // Overloaded operators
        const ContinuousTaxonData&                      operator[](size_t i) const;                                                 //!< Subscript operator (const)
        ContinuousTaxonData&                            operator[](size_t i);                                                       //!< Subscript operator (const)
        
        // implemented methods of the Cloneable interface
        ContinuousCharacterData*                        clone(void) const;
        
        // implemented methods of the Serializable interface
        void                                            initFromFile( const std::string &dir, const std::string &fn );              //!< Read and resurrect this object from a file in its default format.
        void                                            initFromString( const std::string &s );                                     //!< Serialize (resurrect) the object from a string value
        void                                            writeToFile(const std::string &dir, const std::string &fn) const;           //!< Write this object into a file in its default format.

        // CharacterData functions
<<<<<<< HEAD:src/core/datatypes/phylogenetics/characterdata/ContinuousCharacterData.h
        ContinuousCharacterData&                        concatenate(const ContinuousCharacterData &d, std::string type = "");                              //!< Concatenate data matrices
        ContinuousCharacterData&                        concatenate(const HomologousCharacterData &d, std::string type = "");                              //!< Concatenate data matrices
        ContinuousCharacterData&                        concatenate(const AbstractCharacterData &d, std::string type = "");                                //!< Concatenate data matrices
=======
        void                                            concatenate(const ContinuousCharacterData &d, std::string type = "");                              //!< Concatenate data matrices
        void                                            concatenate(const HomologousCharacterData &d, std::string type = "");                              //!< Concatenate data matrices
        void                                            concatenate(const AbstractCharacterData &d, std::string type = "");                                //!< Concatenate data matrices
>>>>>>> development:src/core/datatypes/phylogenetics/characterdata/ContinuousCharacterData.h
        void                                            excludeAllCharacters(void);                                                 //!< Exclude all characters
        void                                            excludeCharacter(size_t i);                                                 //!< Exclude character
        void                                            executeMethod(const std::string &n, const std::vector<const DagNode*> &args, double &rv) const;     //!< Map the member methods to internal function calls
        const double&                                   getCharacter(size_t tn, size_t cn) const;                                   //!< Return a reference to a character element in the character matrix
        std::string                                     getDataType(void) const;
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
        void                                            removeExcludedCharacters(void);                                              //!< Remove all the excluded characters
        void                                            restoreCharacter(size_t i);                                                 //!< Restore character

        
    protected:
        std::set<size_t>                                deletedCharacters;                                                          //!< Set of deleted characters

    };
    
    
}

#endif

