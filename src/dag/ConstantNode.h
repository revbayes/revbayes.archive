/**
 * @file
 * This file contains the declaration of ConstantNode, which is derived
 * from DAGNode. ConstantNode is used for DAG nodes holding constant
 * values.
 *
 * @brief Declaration of ConstantNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-11-19 17:29:33 +0100 (Tor, 19 Nov 2009) $
 * @author The RevBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id: ConstantNode.h 69 2009-11-19 16:29:33Z ronquist $
 */

#ifndef ConstantNode_H
#define ConstantNode_H

#include "DAGNode.h"

class RbObject;
class VectorString;

class ConstantNode : public DAGNode {

    public:
                                ConstantNode(RbObject* val);                                   //!< Constructor from value
                                ConstantNode(const std::string& valType);                      //!< Constructor from value type
                                ConstantNode(const ConstantNode& x);                           //!< Copy constructor
							   ~ConstantNode(void);                                            //!< Destructor

        // Assignment operator
        ConstantNode&           operator=(const ConstantNode& x);                              //!< Assignment operator

        // Basic utility functions
        ConstantNode*           clone(void) const;                                                  //!< Clone this object
        const VectorString&     getClass(void) const;                                               //!< Get class vector
        void                    printStruct(std::ostream& o) const;                                 //!< Print struct for user
        void                    printValue(std::ostream& o) const;                                  //!< Print struct for user
        std::string             toString(void) const;                                               //!< Complete info on object

        // ConstantNode functions
        const DAGNode*          getStoredValue(void) { return value; }                              //!< Get stored value
        const DAGNode*          getValElement(const VectorInteger& index) const;                    //!< Get element of value    
        const DAGNode*          getValue(void) { return value; }                                    //!< Get value
        const DAGNode*          getValue(void) const { return value; }                              //!< Get value (const)
        void                    setElement(const VectorInteger& index, RbObject* val);              //!< Set element of value
        void                    setValue(RbObject* val);                                            //!< Set value

        // DAG functions
        ConstantNode*           cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const;                     //!< Clone entire graph
        bool                    isMutableTo(const DAGNode* newNode) const;                                  //!< Is node mutable to newNode?
        bool                    isMutableTo(const VectorInteger& index, const RbObject* newValue) const;    //!< Is node mutable to contain newValue?
        void                    mutateTo(DAGNode* newNode);                                                 //!< Mutate to new node
        ConstantNode*           mutateTo(const VectorInteger& index, RbObject* newValue);                   //!< Mutate to contain newValue
        bool                    isTouched (void) const { return false; }                                    //!< Touched by a move?
        void                    touchAffected(void);                                                        //!< Tell affected nodes value is reset

    protected:
        DAGNode*                value;                                                              //!< The constant value
};

#endif

