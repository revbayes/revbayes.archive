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

#include "MemberObject.h"
#include <set>
#include <string>
#include <vector>

class ArgumentRule;
class Character;
class DAGNode;
class VectorCharacters;
class VectorString;


class CharacterMatrix : public MemberObject {

    public:
                                                    CharacterMatrix(void);                                             //!< Default constructor
    
        VectorCharacters&                           operator[](size_t i);                                              //!< Subscript operator
        const VectorCharacters&                     operator[](size_t i) const;                                        //!< Subscript operator (const)

        // Basic utility functions
        CharacterMatrix*                            clone(void) const;                                                 //!< Clone object
        const VectorString&                         getClass(void) const;                                              //!< Get class vector   
        void                                        printValue(std::ostream& o) const;                                 //!< Print value for user
        std::string                                 richInfo(void) const;                                              //!< Complete info

        // Member variable rules
        const MemberRules&                          getMemberRules(void) const;                                        //!< Get member rules

        // Member method inits
        const MethodTable&                          getMethods(void) const;                                            //!< Get methods
        
        // CharacterMatrix functions
        void                                        addTaxonName(const std::string tName);                             //!< Add taxon name
        void                                        addTaxonObservations(VectorCharacters* obs);                       //!< Add obserervations
        bool                                        areCharactersDiscrete(void) const;                                 //!< Returns whether this is a matrix of discrete characters
        void                                        excludeCharacter(size_t i);                                        //!< Exclude character
        void                                        excludeTaxon(size_t i);                                            //!< Exclude taxon
        void                                        excludeTaxon(std::string& s);                                      //!< Exclude taxon
        Character*                                  getCharacter(size_t tn, size_t cn);                                //!< Return a pointer to a character element in the character matrix 
        std::string                                 getDataType(void) { return dataType; }                             //!< Returns the data type for the matrix
        std::string                                 getFileName(void) const { return fileName; }                       //!< Returns the name of the file the data came from
        size_t                                      getNumCharacters(void) const;                                      //!< Number of characters
        size_t                                      getNumStates(void) const;                                          //!< Get the number of states for the characters in this matrix
        size_t                                      getNumTaxa(void) const { return taxonObservations.size(); }        //!< Number of taxa
        std::string                                 getTaxonWithIndex(size_t idx) { return taxonNames[idx]; }          //!< Returns the idx-th taxon name
        bool                                        isCharacterExcluded(size_t i);                                     //!< Is the character excluded
        bool                                        isTaxonExcluded(size_t i);                                         //!< Is the taxon excluded
        bool                                        isTaxonExcluded(std::string& s);                                   //!< Is the taxon excluded
        void                                        restoreCharacter(size_t i);                                        //!< Restore character
        void                                        restoreTaxon(size_t i);                                            //!< Restore taxon
        void                                        restoreTaxon(std::string& s);                                      //!< Restore taxon
        void                                        setDataType(const std::string dt) { dataType = dt; }               //!< Set the data type
        void                                        setFileName(const std::string fn) { fileName = fn; }               //!< Set the file name
        DAGNode*                                    wrapIntoVariable(void); 

	protected:
        DAGNode*                                    executeOperation(const std::string& name, ArgumentFrame& args);    //!< Execute method
    
    private:
        size_t                                      indexOfTaxonWithName(std::string& s);                              //!< Get the index of the taxon
        bool                                        isCharacterConstant(size_t idx);                                   //!< Is the idx-th character a constant pattern?
        bool                                        isCharacterMissAmbig(size_t idx);                                  //!< Does the character have missing or ambiguous data?
        size_t                                      numConstantPatterns(void);                                         //!< The number of constant patterns
        size_t                                      numMissAmbig(void);                                                //!< The number of patterns with missing or ambiguous characters
        std::vector<VectorCharacters*>              taxonObservations;                                                 //!< Vector of character observations
        std::vector<std::string>                    taxonNames;                                                        //!< Vector of taxon names
        std::set<size_t>                            deletedTaxa;                                                       //!< Set of deleted taxa
        std::set<size_t>                            deletedCharacters;                                                 //!< Set of deleted characters
        std::string                                 dataType;                                                          //!< Type of data in matrix
        std::string                                 fileName;                                                          //!< The path/filename from where this matrix originated
};

#endif

