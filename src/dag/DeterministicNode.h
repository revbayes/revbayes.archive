/**
 * @file
 * This file contains the declaration of DeterministicNode, which is derived
 * from DAGNode. DeterministicNode is used for DAG nodes associated with
 * an expression (equation) that determines their value.
 *
 * @brief Declaration of DeterministicNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id$
 */

#ifndef DeterministicNode_H
#define DeterministicNode_H

#include "VariableNode.h"

#include <string>
#include <vector>

class RbFunction;

class DeterministicNode : public VariableNode {

    public:
                            DeterministicNode(RbFunction* func);            //!< Constructor from function
                            DeterministicNode(const DeterministicNode& d);  //!< Copy constructor
                            ~DeterministicNode();                           //!< Destructor

        // Assignment operator
        DeterministicNode&  operator=(const DeterministicNode& x);  //!< Assignment operator

        // Basic utility functions
        DeterministicNode*  clone() const;                          //!< Clone the deterministic node
        const StringVector& getClass() const;                       //!< Get class vector
        const RbObject*     getStoredValue();                       //!< Get stored value intelligently
        const RbObject*     getValElement(const IntVector& index) const;  //!< Get value element
        const RbObject*     getValue();                             //!< Get current value intelligently
        const RbObject*     getValue() const;                       //!< Get current value stupidly
        const StringVector& getValueClass() const;                  //!< Get value class
        void                printStruct(std::ostream& o) const;     //!< Print struct for user
        void                printValue(std::ostream& o) const;      //!< Print value for user
        void                setElement(const IntVector& index, RbObject* value);    //!< Set value element
        void                setValue(RbObject* value);              //!< Set value
        std::string         toString(void) const;                   //!< Complete info about object

        // Functions for updating part of a DAG
        void    	        getAffected(std::set<StochasticNode*>& affected);//!< Mark and get affected nodes
        void    	        keepAffected();                         //!< Keep value of affected nodes
        void                restoreAffected();                      //!< Restore value of affected nodes

   protected:
        // Utility function
        void                update(void);                           //!< Update value and storedValue

        // Member variables
        bool                changed;                                //!< True when value updated after touch
        RbFunction*         function;                               //!< Function calculating value
        const RbObject*     value;                                  //!< Ptr to current value
        RbObject*           storedValue;                            //!< Stored value
};

#endif


