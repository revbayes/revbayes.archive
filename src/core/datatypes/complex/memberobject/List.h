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

#include "ArgumentFrame.h"
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
                                    List(bool ref=false);                                       //!< Construct an empty list of references(ref==true) or values (ref==false)
    
        // Overloaded operators
        DAGNode*                    operator[](size_t i);                                       //!< Index op (non-const) to elements
        const DAGNode*              operator[](size_t i) const;                                 //!< Index op (const) to elements

        // Basic utility functions
        List*                       clone(void) const;                                          //!< Clone object
        List*                       cloneWithoutConnections(void) const;                        //!< Make a clone with only constant member variables
        const VectorString&         getClass(void) const;                                       //!< Get class
        void                        printValue(std::ostream& o) const;                          //!< Print value for user
        std::string                 richInfo(void) const;                                       //!< Complete info about object

        // Member rules and methods
        const MemberRules&          getMemberRules(void) const;                                 //!< Get member rules
        const MethodTable&          getMethods(void) const;                                     //!< Get member methods

        // Index access to elements
        DAGNode*                    getElement(size_t index);                                   //!< Return element
        size_t                      getElementIndex(std::string& elemName) const;               //!< Convert string to numerical index
        size_t                      getElementsSize(void) const { return elements.size(); }     //!< Number of elements
        void                        setElement(size_t index, DAGNode* var, bool convert=true);  //!< Set element
        bool                        supportsIndex(void) const { return true; }                  //!< We support index operator
    
        // List functions
        void                        addElement(DAGNode* var, const std::string& name = "");     //!< Add element to list
        const ArgumentFrame&        getElements(void) { return elements; }                      //!< Elements

    protected:
        ArgumentFrame               elements;                                                   //!< Elements
        bool                        referenceList;                                              //!< Is this a reference list?
};

#endif
