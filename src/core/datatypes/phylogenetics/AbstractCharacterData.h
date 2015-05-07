#ifndef AbstractCharacterData_H
#define AbstractCharacterData_H

#include "CharacterState.h"
#include "Cloneable.h"
#include "AbstractTaxonData.h"

#include <string>
#include <vector>

namespace RevBayesCore {

    /**
     * Abstract class for all character data objects.
     *
     * This abstract class provides the base class for all character data objects.
     * A character data object contains a vector of TaxonData objects and defines additional
     * convenience functions to access the data stored in these TaxonData objects.
     * Note that this class is a pure interface and thus contains only pure virtual functions!
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2013-04-15, version 1.0
     */
    class AbstractCharacterData : public Cloneable {
    
    public:    
        virtual                                ~AbstractCharacterData(void) {}
        
        // Overloaded operators
        virtual const AbstractTaxonData&        operator[](size_t i) const = 0;                                             //!< Subscript operator (const)
    
        virtual bool                            operator==(const AbstractCharacterData &rm) const { return this == &rm; }
        virtual bool                            operator!=(const AbstractCharacterData &rm) const { return !operator==(rm); }
        virtual bool                            operator<(const AbstractCharacterData &rm) const { return this < &rm; }
        virtual bool                            operator<=(const AbstractCharacterData &rm) const { return operator<(rm) || operator==(rm); }

        // methods of the Cloneable interface
        virtual AbstractCharacterData*          clone(void) const = 0;
        
        // Container functions
        virtual void                            clear(void) = 0;
    
        // CharacterData functions
        virtual void                            addTaxonData(const AbstractTaxonData &obs) = 0;                                 //!< Add taxon data
        virtual AbstractCharacterData&          concatenate(const AbstractCharacterData &d) = 0;                                //!< Concatenate two sequences
        virtual void                            excludeAllCharacters(void) = 0;                                                 //!< Exclude all characters
        virtual void                            excludeCharacter(size_t i) = 0;                                                 //!< Exclude character
        virtual void                            excludeTaxon(size_t i) = 0;                                                     //!< Exclude taxon
        virtual void                            excludeTaxon(const std::string& s) = 0;                                         //!< Exclude taxon
        virtual std::string                     getDatatype(void) const = 0;                                                    //!< Return the data type of this character data matrix
        virtual const std::string&              getFileName(void) const = 0;                                                    //!< Returns the name of the file the data came from
        virtual const std::string&              getFilePath(void) const = 0;                                                    //!< Returns the name of the file path
        virtual size_t                          getIndexOfTaxon(const std::string &n) const = 0;                                //!< Get the index of the taxon with name 'n'.
        virtual size_t                          getNumberOfCharacters(void) const = 0;                                          //!< Number of characters
        virtual size_t                          getNumberOfIncludedCharacters(void) const = 0;                                  //!< Number of characters
        virtual size_t                          getNumberOfTaxa(void) const = 0;                                                //!< Number of taxa
        virtual size_t                          getNumberOfIncludedTaxa(void) const = 0;                                        //!< Number of included taxa
        virtual AbstractTaxonData&              getTaxonData(size_t tn) = 0;                                                    //!< Return a reference to a sequence in the character matrix
        virtual const AbstractTaxonData&        getTaxonData(size_t tn) const = 0;                                              //!< Return a reference to a sequence in the character matrix
        virtual AbstractTaxonData&              getTaxonData(const std::string &tn) = 0;                                        //!< Return a reference to a sequence in the character matrix
        virtual const AbstractTaxonData&        getTaxonData(const std::string &tn) const = 0;                                  //!< Return a reference to a sequence in the character matrix
        virtual const std::vector<std::string>& getTaxonNames(void) const = 0;                                                  //!< Get the names of the taxa
        virtual const std::string&              getTaxonNameWithIndex(size_t idx) const = 0;                                    //!< Returns the idx-th taxon name
        virtual void                            includeCharacter(size_t i) = 0;                                                 //!< Include character
        virtual void                            includeTaxon(const std::string& s) = 0;                                         //!< Include taxon
        virtual bool                            isCharacterExcluded(size_t i) const = 0;                                        //!< Is the character excluded
        virtual bool                            isCharacterResolved(size_t txIdx, size_t chIdx) const = 0;                      //!< Returns whether the character is fully resolved (e.g., "A" or "1.32") or not (e.g., "AC" or "?")
        virtual bool                            isCharacterResolved(const std::string &tn, size_t chIdx) const = 0;             //!< Returns whether the character is fully resolved (e.g., "A" or "1.32") or not (e.g., "AC" or "?")
        virtual bool                            isHomologyEstablished(void) const = 0;                                          //!< Returns whether the homology of the characters has been established
        virtual bool                            isSequenceMissing(const std::string &n) const = 0;                              //!< Returns whether the contains only missing data or has some actual observations
        virtual bool                            isTaxonExcluded(size_t i) const = 0;                                            //!< Is the taxon excluded
        virtual bool                            isTaxonExcluded(const std::string& s) const = 0;                                //!< Is the taxon excluded
        virtual void                            removeExludedCharacters(void) = 0;                                              //!< Remove all the excluded characters
        virtual void                            restoreCharacter(size_t i) = 0;                                                 //!< Restore character
        virtual void                            restoreTaxon(size_t i) = 0;                                                     //!< Restore taxon
        virtual void                            restoreTaxon(const std::string& s) = 0;                                         //!< Restore taxon
        virtual void                            setFileName(const std::string &fn) = 0;                                         //!< Set the file name
        virtual void                            setFilePath(const std::string &fn) = 0;                                         //!< Set the file path
        virtual void                            setHomologyEstablished(bool tf) = 0;                                            //!< Set whether the homology of the characters has been established
        virtual void                            setTaxonName(const std::string& currentName, const std::string& newName) = 0;   //!< Change the name of a taxon
        virtual void                            show(std::ostream &out) = 0;                                                    //!< Show the entire content
        
    protected:
                                                AbstractCharacterData() {}                                                                                      //!< Constructor requires character type

    };
    
    // Global functions using the class
    std::ostream&                               operator<<(std::ostream& o, const AbstractCharacterData& x);                    //!< Overloaded output operator

}

#endif
