/**
 * @file
 * This file contains the declaration of ObjectSlot, which
 * holds a slot for a variable, including info about the
 * required type and dim of the variable, as well as a
 * pointer to the current variable itself (which can be NULL).
 *
 * @brief Declaration of ObjectSlot
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-17, version 1.0
 *
 * $Id$
 */

#ifndef ObjectSlot_H 
#define ObjectSlot_H

#include "RbInternal.h"

#include <string>

class DAGNode;

class ObjectSlot {

    public:
                            ObjectSlot(DAGNode* var);                               //!< Constructor from variable
                            ObjectSlot(const std::string& valType, int valDim=0);   //!< Constructor from type info
                            ObjectSlot(const ObjectSlot& x);                        //!< Copy constructor
                            ~ObjectSlot();                                          //!< Destructor removes value

        // Assignment operator
        ObjectSlot&         operator=(const ObjectSlot& x);                         //!< Assignment operator needed

        // Basic utility functions
        ObjectSlot*         clone(void) const { return new ObjectSlot(*this); }     //!< Clone this object
        void                printValue(std::ostream& o) const;                      //!< Print table for user
        std::string         toString(void) const;                                   //!< Complete info to string

        // Regular functions
        int                 getDim(void) const { return dim; }                      //!< Get dim
        std::string         getType(void) const { return type; }                    //!< Get type
        std::string         getTypeDescr(void) const;                               //!< Get type description
        DAGNode*            getVariable(void) { return variable; }                  //!< Get variable
        const DAGNode*      getVariable(void) const { return variable; }            //!< Get const variable
        void                setVariable(DAGNode* var);                              //!< Set variable

    private:
        std::string         type;                   //!< The value type 
        int                 dim;                    //!< The dimension of the slot
        DAGNode*            variable;               //!< The variable itself
};

#endif


