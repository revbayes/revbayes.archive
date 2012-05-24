/**
 * @file
 * This file contains the declaration of Alignment, which is
 * class that holds a character matrix in RevBayes.
 *
 * @brief Declaration of Alignment
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef CharacterData_H
#define CharacterData_H

#include "Character.h"
#include "Natural.h"
#include "RbString.h"
#include "RbVector.h"
#include "TaxonData.h"

#include <set>
#include <string>
#include <vector>

class ArgumentRule;
class DAGNode;


class CharacterData : public MemberObject {

    public:
                                            CharacterData(const std::string& characterType);                            //!< Constructor requires character type
                                            CharacterData(const CharacterData& x);                                      //!< Copy constructor to deal with sequenceTypeRule
        virtual                            ~CharacterData(void);                                                        //!< Destructor to deal with sequenceTypeRule

        // Overloaded operators
        CharacterData&                      operator=(const CharacterData& x);                                          //!< Assignment operator
        const TaxonData&                    operator[](size_t i) const;                                                 //!< Subscript operator (const)

        // Basic utility functions
        CharacterData*                      clone(void) const;                                                          //!< Clone object
        static const std::string&           getClassName(void);                                                         //!< Get class name
        static const TypeSpec&              getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                                    //!< Get language type of the object
        void                                printValue(std::ostream& o) const;                                          //!< Print value for user

        // Member variable functions
        const MemberRules&                  getMemberRules(void) const;                                                 //!< Get member rules
        void                                setSimpleMemberValue(const std::string& name, const RbPtr<const RbLanguageObject> &var);   //!< Catch setting of the topology

        // Member method inits
        const MethodTable&                  getMethods(void) const;                                                     //!< Get methods
        RbPtr<RbLanguageObject>             executeSimpleMethod(const std::string& name, const std::vector<const RbObject*>& args);            //!< Override to map member methods to internal functions
  
        // Container functions
        void                                clear(void);
        void                                setElement(const size_t index, RbLanguageObject* elem);                     //!< Set element with type conversion

        // Matrix functions
        const RbObject&                     getElement(size_t row, size_t col) const;                                   //!< Get element or subcontainer
        RbObject&                           getElement(size_t row, size_t col);                                         //!< Get element or subcontainer (non-const to return non-const element)
        void                                setElement(size_t row, size_t col, RbLanguageObject* var);                  //!< set element
        size_t                              size(void) const;
        void                                resize(size_t nRows, size_t nCols);                                         //!< Resize to new length vector
        void                                transpose(void);                                                            //!< Transpose the matrix

        // CharacterData functions
        void                                addTaxonData(TaxonData* obs);                                               //!< Add taxon data
        void                                addTaxonData(TaxonData* obs, bool forceAdd);                                //!< Add taxon data
        void                                excludeCharacter(size_t i);                                                 //!< Exclude character
        void                                excludeTaxon(size_t i);                                                     //!< Exclude taxon
        void                                excludeTaxon(std::string& s);                                               //!< Exclude taxon
        const Character&                    getCharacter(size_t tn, size_t cn) const;                                   //!< Return a reference to a character element in the character matrix
        const std::string                   getDataType(void) const;                                                    //!< Returns the data type for the matrix
        const std::string&                  getFileName(void) const;                                                    //!< Returns the name of the file the data came from
        bool                                getIsHomologyEstablished(void) const { return isHomologyEstablished; }      //!< Returns whether the homology of the characters has been established
        size_t                              getNumberOfCharacters(void) const;                                          //!< Number of characters
        size_t                              getNumberOfCharacters(size_t idx) const;                                    //!< Number of characters for a specific taxon
        size_t                              getNumberOfStates(void) const;                                              //!< Get the number of states for the characters in this matrix
        size_t                              getNumberOfTaxa(void) const;                                                //!< Number of taxa
        TaxonData&                          getTaxonData(size_t tn);                                                    //!< Return a reference to a sequence in the character matrix
        const TaxonData&                    getTaxonData(size_t tn) const;                                              //!< Return a reference to a sequence in the character matrix
        const std::string                   getTaxonNameWithIndex(size_t idx) const;                                    //!< Returns the idx-th taxon name
        bool                                isCharacterExcluded(size_t i) const;                                        //!< Is the character excluded
        bool                                isTaxonExcluded(size_t i) const;                                            //!< Is the taxon excluded
        bool                                isTaxonExcluded(std::string& s) const;                                      //!< Is the taxon excluded
        RbVector<char, Character>*          makeSiteColumn(size_t cn) const;                                            //!< Return a reference to a sequence in the character matrix
        void                                restoreCharacter(size_t i);                                                 //!< Restore character
        void                                restoreTaxon(size_t i);                                                     //!< Restore taxon
        void                                restoreTaxon(std::string& s);                                               //!< Restore taxon
        void                                setFileName(const std::string fn) { fileName = fn; }                        //!< Set the file name
        void                                setIsHomologyEstablished(bool tf) { isHomologyEstablished = tf; }           //!< Set whether the homology of the characters has been established
        void                                showData(void);                                                             //!< Show the data in the character matrix
    
    private:
        // Utility functions
        size_t                              indexOfTaxonWithName(std::string& s) const;                                 //!< Get the index of the taxon
        bool                                isCharacterConstant(size_t idx) const;                                      //!< Is the idx-th character a constant pattern?
        bool                                isCharacterMissingOrAmbiguous(size_t idx) const;                            //!< Does the character have missing or ambiguous data?
        size_t                              numConstantPatterns(void) const;                                            //!< The number of constant patterns
        size_t                              numMissAmbig(void) const;                                                   //!< The number of patterns with missing or ambiguous characters

        // Member variables
        std::set<size_t>                    deletedTaxa;                                                                //!< Set of deleted taxa
        std::set<size_t>                    deletedCharacters;                                                          //!< Set of deleted characters
        std::string                         fileName;                                                                   //!< The path/filename from where this matrix originated
        std::vector<std::string>            sequenceNames;                                                              //!< names of the sequences
        size_t                              sequenceLength;                                                             //!< The length of each sequence
        RbString                            characterType;                                                              //!< Rule describing sequence type
        TypeSpec                            typeSpec;                                                                   //!< The type of this character matrix including element type
        bool                                isHomologyEstablished;                                                      //!< Whether the homology of the characters has been established
        
        std::map<std::string, RbPtr<TaxonData> > taxonMap;
};

#endif

