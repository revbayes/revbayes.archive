/**
 * @file
 * This file contains the declaration of VariableNode, which is the base
 * class for variable nodes in a model DAG.
 *
 * @brief Declaration of VariableNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-11 16:58:43 -0800 (Fre, 11 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 *
 * $Id: DAGNode.h 187 2009-12-12 00:58:43Z jhuelsenbeck $
 */

#ifndef VariableNode_H
#define VariableNode_H

#include "DAGNode.h"
#include "RbPtr.h"

class Monitor;
class Move;
class MoveSchedule;
class StochasticNode;
class VectorString;

#include <set>

const std::string VariableNode_name = "Variable node";

class VariableNode : public DAGNode {

public:
    virtual                                    ~VariableNode(void);                                                             //!< Virtual destructor

    // Utility functions you have to override
    virtual VariableNode*                       clone(void) const = 0;                                                          //!< Clone this node
    virtual const VectorString&                 getClass(void) const;                                                           //!< Get DAG node class 
    virtual RbPtr<const RbObject>               getStoredValue(void) const = 0;                                                 //!< Get stored value
    virtual RbPtr<const RbObject>               getValue(void) const = 0;                                                       //!< Get value
    virtual RbPtr<RbObject>                     getValue(void) = 0;                                                             //!< Get value (non-const to return non-const value)
    virtual void                                printStruct(std::ostream& o) const = 0;                                         //!< Print struct for user
    virtual void                                printValue(std::ostream& o) const = 0;                                          //!< Print value for user
    virtual std::string                         richInfo(void) const = 0;                                                       //!< Complete info about object

    // DAG function you should not override
    void                                        addParentNode(RbPtr<DAGNode> p);                                                //!< Add parent node
    bool                                        isTouched(void) const { return touched; }                                       //!< Is node touched by move or parser?
    void                                        removeParentNode(RbPtr<DAGNode> p) { parents.erase(p); }                        //!< Remove a child node

    // DAG functions you may want to override
    virtual bool                                isConst(void) const { return false; }                                           //!< Is DAG node const value?

    // DAG functions you have to override
    virtual RbPtr<DAGNode>                      cloneDAG(std::map<const DAGNode*, RbPtr<DAGNode> >& newNodes) const = 0;        //!< Clone entire graph
    virtual void                                getAffected(std::set<RbPtr<StochasticNode> >& affected) = 0;                    //!< Mark and get affected nodes
    virtual void                                keep(void) = 0;                                                                 //!< Keep current state
    virtual void                                keepAffected(void) = 0;                                                         //!< Keep value of affected nodes
    virtual void                                restoreAffected(void) = 0;                                                      //!< Restore value of affected nodes
    virtual void                                swapParentNode(RbPtr<DAGNode> oldP, RbPtr<DAGNode> newP) = 0;                   //!< Swap a parent node
    virtual void                                touchAffected(void) = 0;                                                        //!< Tell affected nodes value is reset

protected:
    VariableNode(const std::string& valType);                                                                       //!< Constructor of empty node
    VariableNode(const VariableNode &v);                                                                            //!< Copy Constructor

    // Member variables
    bool                                        touched;                                                                        //!< Is touched by move?
    RbPtr<RbObject>                             storedValue;                                                                    //!< Stored value
};

#endif

