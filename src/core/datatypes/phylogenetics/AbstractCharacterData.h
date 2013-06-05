/**
 * @file
 * This file contains the declaration of Alignment, which is
 * class that holds a character matrix in RevBayes.
 *
 * @brief Declaration of Alignment
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-07-10 14:09:12 +0200 (Tue, 10 Jul 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: CharacterData.h 1660 2012-07-10 12:09:12Z hoehna $
 */

#ifndef AbstractCharacterData_H
#define AbstractCharacterData_H

#include "CharacterState.h"
#include "Cloneable.h"
#include "MatrixReal.h"
#include "AbstractTaxonData.h"

#include <map>
#include <set>
#include <string>
#include <vector>

namespace RevBayesCore {

    class AbstractCharacterData : public Cloneable {
    
    public:    
        virtual                            ~AbstractCharacterData(void) {}
        
        // Overloaded operators
        AbstractCharacterData&              operator=(const AbstractCharacterData& x);                                  //!< Assignment operator
        virtual const AbstractTaxonData&    operator[](size_t i) const = 0;                                             //!< Subscript operator (const)
    
        // methods of the Cloneable interface
        AbstractCharacterData*              clone(void) const = 0;
        
        // Container functions
        virtual void                        clear(void) = 0;
    
        // CharacterData functions
        virtual void                        addTaxonData(const AbstractTaxonData &obs) = 0;                             //!< Add taxon data
        virtual void                        addTaxonData(const AbstractTaxonData &obs, bool forceAdd) = 0;              //!< Add taxon data
        virtual void                        excludeCharacter(size_t i) = 0;                                             //!< Exclude character
        virtual void                        excludeTaxon(size_t i) = 0;                                                 //!< Exclude taxon
        virtual void                        excludeTaxon(std::string& s) = 0;                                           //!< Exclude taxon
//        virtual MatrixReal                  computeStateFrequencies(void) = 0;
        virtual const CharacterState&       getCharacter(size_t tn, size_t cn) const = 0;                               //!< Return a reference to a character element in the character matrix
        virtual std::string                 getDatatype(void) const = 0;                                                //!< Return the data type of this character data matrix
        virtual const std::string&          getFileName(void) const = 0;                                                //!< Returns the name of the file the data came from
        virtual bool                        getIsHomologyEstablished(void) const = 0;                                   //!< Returns whether the homology of the characters has been established
        virtual size_t                      getNumberOfCharacters(void) const = 0;                                      //!< Number of characters
        virtual size_t                      getNumberOfCharacters(size_t idx) const = 0;                                //!< Number of characters for a specific taxon
        virtual size_t                      getNumberOfStates(void) const = 0;                                          //!< Get the number of states for the characters in this matrix
        virtual size_t                      getNumberOfTaxa(void) const = 0;                                            //!< Number of taxa
        virtual AbstractTaxonData&          getTaxonData(size_t tn) = 0;                                                //!< Return a reference to a sequence in the character matrix
        virtual const AbstractTaxonData&    getTaxonData(size_t tn) const = 0;                                          //!< Return a reference to a sequence in the character matrix
        virtual AbstractTaxonData&          getTaxonData(const std::string &tn) = 0;                                    //!< Return a reference to a sequence in the character matrix
        virtual const AbstractTaxonData&    getTaxonData(const std::string &tn) const = 0;                              //!< Return a reference to a sequence in the character matrix
        virtual const std::vector<std::string>& getTaxonNames(void) const = 0;                                          //!< Get the names of the taxa
        virtual const std::string&          getTaxonNameWithIndex(size_t idx) const = 0;                                //!< Returns the idx-th taxon name
        virtual bool                        isCharacterExcluded(size_t i) const = 0;                                    //!< Is the character excluded
        virtual bool                        isTaxonExcluded(size_t i) const = 0;                                        //!< Is the taxon excluded
        virtual bool                        isTaxonExcluded(std::string& s) const = 0;                                  //!< Is the taxon excluded
        virtual void                        restoreCharacter(size_t i) = 0;                                             //!< Restore character
        virtual void                        restoreTaxon(size_t i) = 0;                                                 //!< Restore taxon
        virtual void                        restoreTaxon(std::string& s) = 0;                                           //!< Restore taxon
        virtual void                        setFileName(const std::string &fn) = 0;                                     //!< Set the file name
        virtual void                        setIsHomologyEstablished(bool tf) = 0;                                      //!< Set whether the homology of the characters has been established
    
    protected:
        AbstractCharacterData() {}                                                                                      //!< Constructor requires character type

    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const AbstractCharacterData& x);                                //!< Overloaded output operator

}

#endif
