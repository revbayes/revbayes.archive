/**
 * @file
 * This file contains the declaration of DistanceMatrix, which is
 * class that holds a distance matrix in RevBayes.
 *
 * @brief Declaration of DistanceMatrix
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef DistanceMatrix_H
#define DistanceMatrix_H

#include "MatrixReal.h"
#include "ValueRule.h"
#include "VectorReal.h"
#include "VectorString.h"

#include <set>
#include <string>
#include <vector>

class ArgumentRule;
class DAGNode;


const std::string DistanceMatrix_name = "Distance Matrix";

class DistanceMatrix : public MatrixReal {

    public:
                                            DistanceMatrix(const size_t nTaxa);                                         //!< Constructor requires character type
                                            DistanceMatrix(const DistanceMatrix& x);                                    //!< Copy constructor to deal with sequenceTypeRule
        virtual                            ~DistanceMatrix(void);                                                       //!< Destructor to deal with sequenceTypeRule

        // Overloaded operators
        DistanceMatrix&                     operator=(const DistanceMatrix& x);                                         //!< Assignment operator

        // Basic utility functions
        DistanceMatrix*                     clone(void) const;                                                          //!< Clone object
        const VectorString&                 getClass(void) const;                                                       //!< Get class vector   
        const TypeSpec&                     getTypeSpec(void) const;                                                    //!< Get language type of the object
        void                                printValue(std::ostream& o) const;                                          //!< Print value for user
        std::string                         richInfo(void) const;                                                       //!< Complete info

        // Member variable rules
        const MemberRules*                  getMemberRules(void) const;                                                 //!< Get member rules

        // Member method inits
        const MethodTable*                  getMethods(void) const;                                                     //!< Get methods
    
        // Container functions
        void                                clear(void);
        void                                setElement(const size_t index, RbLanguageObject* elem);                     //!< Set element with type conversion

        // Matrix functions
        const RbObject&                     getElement(size_t row, size_t col) const;                                   //!< Get element or subcontainer
        RbObject&                           getElement(size_t row, size_t col);                                         //!< Get element or subcontainer (non-const to return non-const element)
        void                                setElement(size_t row, size_t col, RbLanguageObject* var);                  //!< set element
        void                                resize(size_t nRows, size_t nCols);                                         //!< Resize to new length vector
        void                                transpose(void);                                                            //!< Transpose the matrix

        // CharacterData functions
        void                                addTaxonWithName(std::string s) { sequenceNames.push_back(s); }             //!< Add a taxon name to the matrix
        void                                excludeTaxon(size_t i);                                                     //!< Exclude taxon
        void                                excludeTaxon(std::string& s);                                               //!< Exclude taxon
        size_t                              getNumberOfTaxa(void) const;                                                //!< Number of taxa
        const std::string&                  getTaxonNameWithIndex(size_t idx) const;                                    //!< Returns the idx-th taxon name
        bool                                isTaxonExcluded(size_t i) const;                                            //!< Is the taxon excluded
        bool                                isTaxonExcluded(std::string& s) const;                                      //!< Is the taxon excluded
        void                                restoreTaxon(size_t i);                                                     //!< Restore taxon
        void                                restoreTaxon(std::string& s);                                               //!< Restore taxon
        void                                showData(void);                                                             //!< Show the data in the character matrix

    protected:
        RbLanguageObject*                   executeOperationSimple(const std::string& name, Environment* args);         //!< Execute method

    private:
        // Utility functions
        size_t                              indexOfTaxonWithName(std::string& s) const;                                 //!< Get the index of the taxon

        // Member variables
        std::set<size_t>                    deletedTaxa;                                                                //!< Set of deleted taxa
        std::vector<std::string>            sequenceNames;                                                              //!< names of the sequences
        TypeSpec                            typeSpec;                                                                   //!< The type of this character matrix including element type
};

#endif

