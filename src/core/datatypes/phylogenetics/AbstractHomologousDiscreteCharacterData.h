#ifndef AbstractHomologousDiscreteCharacterData_H
#define AbstractHomologousDiscreteCharacterData_H

#include "AbstractDiscreteTaxonData.h"
#include "AbstractTaxonData.h"
#include "CharacterState.h"
#include "DiscreteCharacterState.h"
#include "HomologousCharacterData.h"
#include "MatrixReal.h"

#include <string>
#include <vector>

namespace RevBayesCore {
    
    /**
     * Abstract class for all DISCRETE character data objects.
     *
     * The DISCRETE character data class is derived from the interface character data
     * and simply specifies that all derived classes must contain discrete characters.
     * This simplifies return values because it is known that all are derived from discrete characters
     * and there are several function that only work on discrete characters and not on
     * continuous characters.
     *
     * Note that this class is a pure interface and thus contains only pure virtual functions!
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-02-16, version 1.0
     */
    class AbstractHomologousDiscreteCharacterData : public HomologousCharacterData {
        
    public:    
        virtual                                    ~AbstractHomologousDiscreteCharacterData(void) {}
        
        // Overloaded operators
        virtual const AbstractTaxonData&                        operator[](size_t i) const = 0;                                             //!< Subscript operator (const)
        
        // methods of the Cloneable interface
        virtual AbstractHomologousDiscreteCharacterData*        clone(void) const = 0;

        // methods of the Serializable interface
        virtual void                                            initFromString( const std::string &s ) = 0;                                 //!< Serialize (resurrect) the object from a string value
        virtual void                                            writeToFile(const std::string &dir, const std::string &fn) const;
        
        // CharacterData functions
        virtual AbstractHomologousDiscreteCharacterData&        concatenate(const AbstractCharacterData &d) = 0;                            //!< Concatenate data matrices
        virtual AbstractHomologousDiscreteCharacterData&        concatenate(const HomologousCharacterData &d) = 0;                          //!< Concatenate two sequences
        virtual AbstractHomologousDiscreteCharacterData&        concatenate(const AbstractHomologousDiscreteCharacterData &d) = 0;                    //!< Concatenate data matrices
        virtual MatrixReal                                      computeStateFrequencies(void) const = 0;                                    //!< Compute the state frequencies for this character data object
        virtual void                                            excludeCharacter(size_t i) = 0;                                             //!< Exclude character
        virtual const DiscreteCharacterState&                   getCharacter(size_t tn, size_t cn) const = 0;                               //!< Return a reference to a character element in the character matrix
        virtual std::string                                     getDataType(void) const = 0;                                                //!< Return the data type of this character data matrix
        virtual std::vector<double>                             getEmpiricalBaseFrequencies(void) const = 0;                                //!< Compute the empirical base frequencies
        virtual size_t                                          getNumberOfCharacters(void) const = 0;                                      //!< Number of characters
        virtual size_t                                          getNumberOfSegregatingSites(void) const = 0;                                //!< Compute the number of segregating sites
        virtual size_t                                          getNumberOfStates(void) const = 0;                                          //!< Get the number of states for the characters in this matrix
        virtual size_t                                          getNumberOfInvariantSites(void) const = 0;                                  //!< Number of invariant sites
        virtual double                                          getPaiwiseSequenceDifference(void) const = 0;                               //!< Get the average pairwise sequence distance.
        virtual AbstractDiscreteTaxonData&                      getTaxonData(size_t tn) = 0;                                                //!< Return a reference to a sequence in the character matrix
        virtual const AbstractDiscreteTaxonData&                getTaxonData(size_t tn) const = 0;                                          //!< Return a reference to a sequence in the character matrix
        virtual AbstractDiscreteTaxonData&                      getTaxonData(const std::string &tn) = 0;                                    //!< Return a reference to a sequence in the character matrix
        virtual const AbstractDiscreteTaxonData&                getTaxonData(const std::string &tn) const = 0;                              //!< Return a reference to a sequence in the character matrix
        virtual bool                                            isCharacterExcluded(size_t i) const = 0;                                    //!< Is the character excluded
        virtual bool                                            isCharacterResolved(size_t txIdx, size_t chIdx) const = 0;                  //!< Returns whether the character is fully resolved (e.g., "A" or "1.32") or not (e.g., "AC" or "?")
        virtual bool                                            isCharacterResolved(const std::string &tn, size_t chIdx) const = 0;         //!< Returns whether the character is fully resolved (e.g., "A" or "1.32") or not (e.g., "AC" or "?")
        virtual void                                            removeExludedCharacters(void) = 0;                                          //!< Remove all the excluded characters
        virtual void                                            restoreCharacter(size_t i) = 0;                                             //!< Restore character
        
    protected:
                                                                AbstractHomologousDiscreteCharacterData() {}                                          //!< Constructor requires character type
        
    };
    
    // Global functions using the class
    std::ostream&                                   operator<<(std::ostream& o, const AbstractHomologousDiscreteCharacterData& x);        //!< Overloaded output operator
    
}

#endif
