/**
 * @file
 * This file contains the declaration of CharacterMatrix, which is
 * class that holds a character matrix in RevBayes.
 *
 * @brief Declaration of CharacterMatrix
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef CharacterMatrix_H
#define CharacterMatrix_H

#include "Matrix.h"
#include "Character.h"
#include "ValueRule.h"
#include "VectorCharacters.h"
#include "VectorString.h"

#include <set>
#include <string>
#include <vector>

class ArgumentRule;
class DAGNode;


const std::string CharacterMatrix_name = "Character Matrix";

class CharacterMatrix : public Matrix {

    public:
                                            CharacterMatrix(const std::string& characterType);                          //!< Constructor requires character type
                                            CharacterMatrix(const CharacterMatrix& x);                                  //!< Copy constructor to deal with sequenceTypeRule
        virtual                            ~CharacterMatrix(void);                                                      //!< Destructor to deal with sequenceTypeRule

        // Overloaded operators
        CharacterMatrix&                    operator=(const CharacterMatrix& x);                                        //!< Assignment operator
        const VectorCharacters&             operator[](size_t i) const;                                                 //!< Subscript operator (const)

        // Basic utility functions
        CharacterMatrix*                    clone(void) const;                                                          //!< Clone object
        const VectorString&                 getClass(void) const;                                                       //!< Get class vector   
        void                                printValue(std::ostream& o) const;                                          //!< Print value for user
        std::string                         richInfo(void) const;                                                       //!< Complete info

        // Member variable rules
        const MemberRules&                  getMemberRules(void) const;                                                 //!< Get member rules

        // Member method inits
        const MethodTable&                  getMethods(void) const;                                                     //!< Get methods
        RbLanguageObject*                   executeOperation(const std::string& name, Environment& args);               //!< Execute method
    
    
        // Container and matrix functions
        void                                clear(void);                                                                //!< Clear
        VectorCharacters*                   getElement(size_t index);                                                   //!< Get element or subcontainer
        Character*                          getElement(size_t row, size_t col);                                         //!< Get element or subcontainer
        void                                setElement(size_t index, RbLanguageObject* var);                            //!< Allow to set element
        void                                setElement(size_t row, size_t col, RbLanguageObject* var);                  //!< set element
        void                                resize(size_t nRows);                                                       //!< Resize to new length vector
        void                                resize(size_t nRows, size_t nCols);                                         //!< Resize to new length vector
        size_t                              size(void) const;                                                           //!< Get total number of elements
        void                                transpose(void);                                                            //!< Transpose the matrix

        // CharacterMatrix functions
        void                                addSequence(const std::string tName, VectorCharacters* obs);                //!< Add taxon name
        void                                excludeCharacter(size_t i);                                                 //!< Exclude character
        void                                excludeTaxon(size_t i);                                                     //!< Exclude taxon
        void                                excludeTaxon(std::string& s);                                               //!< Exclude taxon
        const Character&                    getCharacter(size_t tn, size_t cn) const;                                   //!< Return a reference to a character element in the character matrix
        const std::string&                  getDataType(void) const { return characterType; }     //!< Returns the data type for the matrix
        std::string                         getFileName(void) const { return fileName; }                                //!< Returns the name of the file the data came from
        size_t                              getNumCharacters(void) const;                                               //!< Number of characters
        size_t                              getNumStates(void) const;                                                   //!< Get the number of states for the characters in this matrix
        size_t                              getNumTaxa(void) const { return members.size(); }                           //!< Number of taxa
        const VectorCharacters&             getSequence(size_t tn) const;                                               //!< Return a reference to a sequence in the character matrix
        std::string                         getTaxonWithIndex(size_t idx) const;                                        //!< Returns the idx-th taxon name
        bool                                isCharacterExcluded(size_t i) const;                                        //!< Is the character excluded
        bool                                isTaxonExcluded(size_t i) const;                                            //!< Is the taxon excluded
        bool                                isTaxonExcluded(std::string& s) const;                                      //!< Is the taxon excluded
        VectorCharacters*                   makeSiteColumn(size_t cn) const;                                            //!< Return a reference to a sequence in the character matrix
        void                                restoreCharacter(size_t i);                                                 //!< Restore character
        void                                restoreTaxon(size_t i);                                                     //!< Restore taxon
        void                                restoreTaxon(std::string& s);                                               //!< Restore taxon
        void                                setFileName(const std::string fn) { fileName = fn; }                        //!< Set the file name

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
        VectorString                        sequenceNames;                                                              //!< names of the sequences
        std::string                         characterType;                                                              //!< Rule describing sequence type
};

#endif

