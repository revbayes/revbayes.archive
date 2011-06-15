/**
 * @file
 * This file contains the declaration of ConstantNode, which is derived
 * from DAGNode. ConstantNode is used for DAG nodes holding constant
 * values in a model DAG.
 *
 * @brief Declaration of ConstantNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-11-19 17:29:33 +0100 (Tor, 19 Nov 2009) $
 * @author The RevBayes Development Core Team
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
                                ConstantNode(RbObject* val);                                                //!< Constructor from value
                                ConstantNode(const std::string& typeSpec);                                  //!< Constructor from language object type

        // Assignment operator
        ConstantNode&           operator=(const ConstantNode& x);                                           //!< Assignment operator

        // Basic utility functions
        ConstantNode*           clone(void) const;                                                          //!< Clone this object
        const VectorString&     getDAGClass(void) const;                                                    //!< Get DAG node class vector
        void                    printStruct(std::ostream& o) const;                                         //!< Print struct for user
        void                    printValue(std::ostream& o) const;                                          //!< Print struct for user
        std::string             richInfo(void) const;                                                       //!< Complete info on object

        // ConstantNode functions
        const RbObject*         getStoredValue(void) { return value; }                                      //!< Get stored value
        const RbObject*         getValue(void) const { return value; }                                      //!< Get value (const)

        // DAG functions
        ConstantNode*           cloneDAG(std::map<const DAGNode*, DAGNode*>& newNodes) const;               //!< Clone entire graph
        bool                    isTouched (void) const { return false; }                                    //!< Touched by a move?
};

#endif

