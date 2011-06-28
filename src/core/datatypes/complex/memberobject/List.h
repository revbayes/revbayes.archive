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
 * $Id:$
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
                                    List(void);                                                 //!< Construct an empty list
                                    List(const List& x);                                        //!< Copy constructor
        virtual                    ~List(void);                                                 //!< Destructor
    
        // Overloaded operators
        List&                       operator=(const List& x);                                   //!< Assignment operator
        DAGNode*                    operator[](size_t i) const;                                 //!< Index op to variables

        // Basic utility functions
        List*                       clone(void) const;                                          //!< Clone object
        const VectorString&         getClass(void) const;                                       //!< Get class
        void                        printValue(std::ostream& o) const;                          //!< Print value for user
        std::string                 richInfo(void) const;                                       //!< Complete info about object

        // Index access to elements
        DAGNode*                    getElement(size_t i);                                       //!< Return element
        size_t                      getElementIndex(std::string& s) const;                      //!< Convert string to numerical index
        size_t                      getElementsSize(void) const { return elements.size(); }     //!< Number of elements
        void                        setElement(size_t index, DAGNode* var, bool convert=true);  //!< Set element
        bool                        supportsIndex(void) const { return true; }                  //!< We support index operator
    
        // List function
        void                        addElement(DAGNode* var, const std::string& name = "");     //!< Add element to list

    protected:
        std::vector<VariableSlot*>  elements;                                                   //!< Elements
        VectorString                names;                                                      //!< Names of elements
};

#endif
