#ifndef AbstractDiscreteCharacterData_H
#define AbstractDiscreteCharacterData_H

#include "AbstractCharacterData.h"
#include "CharacterState.h"
#include "DiscreteCharacterState.h"
#include "AbstractDiscreteTaxonData.h"
#include "AbstractTaxonData.h"
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
    class AbstractDiscreteCharacterData : public AbstractCharacterData {
        
    public:    
        virtual                                    ~AbstractDiscreteCharacterData(void) {}
        
        // Overloaded operators
        virtual const AbstractTaxonData&            operator[](size_t i) const = 0;                                             //!< Subscript operator (const)
        
        // methods of the Cloneable interface
        virtual AbstractDiscreteCharacterData*      clone(void) const = 0;
        
        // Container functions
        virtual void                                clear(void) = 0;
        
        // CharacterData functions
        virtual AbstractDiscreteCharacterData&      add(const AbstractCharacterData &d) = 0;                                    //!< Addition operator used for example in '+=' statements
        virtual AbstractDiscreteCharacterData&      add(const AbstractDiscreteCharacterData &d) = 0;                            //!< Addition operator used for example in '+=' statements
        virtual void                                addTaxonData(const AbstractTaxonData &obs) = 0;                             //!< Add taxon data
        virtual void                                addTaxonData(const AbstractDiscreteTaxonData &obs) = 0;                     //!< Add taxon data of discrete characters
        virtual MatrixReal                          computeStateFrequencies(void) const = 0;                                    //!< Compute the state frequencies for this character data object
        virtual void                                excludeCharacter(size_t i) = 0;                                             //!< Exclude character
        virtual void                                excludeTaxon(size_t i) = 0;                                                 //!< Exclude taxon
        virtual void                                excludeTaxon(std::string& s) = 0;                                           //!< Exclude taxon
        virtual const DiscreteCharacterState&       getCharacter(size_t tn, size_t cn) const = 0;                               //!< Return a reference to a character element in the character matrix
        virtual std::string                         getDatatype(void) const = 0;                                                //!< Return the data type of this character data matrix
        virtual const std::string&                  getFileName(void) const = 0;                                                //!< Returns the name of the file the data came from
        virtual const std::string&                  getFilePath(void) const = 0;                                                //!< Returns the name of the file path the data came from
        virtual size_t                              getIndexOfTaxon(const std::string &n) const = 0;                            //!< Get the index of the taxon with name 'n'.
        virtual size_t                              getNumberOfCharacters(void) const = 0;                                      //!< Number of characters
        virtual size_t                              getNumberOfCharacters(size_t idx) const = 0;                                //!< Number of characters for a specific taxon
        virtual size_t                              getNumberOfStates(void) const = 0;                                          //!< Get the number of states for the characters in this matrix
        virtual size_t                              getNumberOfIncludedTaxa(void) const = 0;                                    //!< Number of included taxa
        virtual size_t                              getNumberOfTaxa(void) const = 0;                                            //!< Number of taxa
        virtual AbstractDiscreteTaxonData&          getTaxonData(size_t tn) = 0;                                                //!< Return a reference to a sequence in the character matrix
        virtual const AbstractDiscreteTaxonData&    getTaxonData(size_t tn) const = 0;                                          //!< Return a reference to a sequence in the character matrix
        virtual AbstractDiscreteTaxonData&          getTaxonData(const std::string &tn) = 0;                                    //!< Return a reference to a sequence in the character matrix
        virtual const AbstractDiscreteTaxonData&    getTaxonData(const std::string &tn) const = 0;                              //!< Return a reference to a sequence in the character matrix
        virtual const std::vector<std::string>&     getTaxonNames(void) const = 0;                                              //!< Get the names of the taxa
        virtual const std::string&                  getTaxonNameWithIndex(size_t idx) const = 0;                                //!< Returns the idx-th taxon name
        virtual bool                                isCharacterExcluded(size_t i) const = 0;                                    //!< Is the character excluded
        virtual bool                                isHomologyEstablished(void) const = 0;                                      //!< Returns whether the homology of the characters has been established
        virtual bool                                isTaxonExcluded(size_t i) const = 0;                                        //!< Is the taxon excluded
        virtual bool                                isTaxonExcluded(std::string& s) const = 0;                                  //!< Is the taxon excluded
        virtual void                                restoreCharacter(size_t i) = 0;                                             //!< Restore character
        virtual void                                restoreTaxon(size_t i) = 0;                                                 //!< Restore taxon
        virtual void                                restoreTaxon(std::string& s) = 0;                                           //!< Restore taxon
        virtual void                                setFileName(const std::string &fn) = 0;                                     //!< Set the file name
        virtual void                                setFilePath(const std::string &fn) = 0;                                     //!< Set the file path
        virtual void                                setHomologyEstablished(bool tf) = 0;                                        //!< Set whether the homology of the characters has been established
        virtual void                                show(std::ostream &out) = 0;                                                //!< Show the entire content

    protected:
                                                    AbstractDiscreteCharacterData() {}                                          //!< Constructor requires character type
        
    };
    
    // Global functions using the class
    std::ostream&                                   operator<<(std::ostream& o, const AbstractDiscreteCharacterData& x);        //!< Overloaded output operator
    
}

#endif
