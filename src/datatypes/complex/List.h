/**
 * @file
 * This file contains the declaration of List, a
 * complex type used to hold lists of variables.
 *
 * @brief Declaration of List
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef List_H
#define List_H

#include "MemberObject.h"
#include "VectorString.h"

#include <ostream>
#include <string>
#include <vector>

class VectorRealPos;


/**
 * @brief List class
 *
 * This class is used to hold lists of variables. Unlike a container,
 * a list can hold containers, as well as other lists. The list object
 * corresponds in many ways to the list object in R.
 */
class List : public MemberObject {

    public:
                                List(void);                                               //!< Construct an empty list

        // Overloaded operators
        DAGNode*                operator[](size_t i) const;                                 //!< Index op to variables

        // Basic utility functions
        List*                 clone(void) const;                                          //!< Clone object
        const VectorString&     getClass(void) const;                                       //!< Get class
        void                    printValue(std::ostream& o) const;                          //!< Print value for user
        std::string             richInfo(void) const;                                       //!< Complete info about object

        // Subscript access functions
        bool                    hasSubscript(void) { return true; }                         //!< We support subscripting
        DAGNode*                getSubelement(VectorInteger& index) const;                  //!< Return subscript[](index) element
        size_t                  getSubelementsSize(void) { return variables.size(); }       //!< Number of subscript elements

        // List function
        void                    addVariable(DAGNode* var, const std::string& name = "");    //!< Add variable to list

    private:
        std::vector<DAGNode*>   variables;                                                  //!< Store an extra copy of variables in subscript order
        VectorString            names;                                                      //!< Names of variables
};

#endif
