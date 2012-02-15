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
#include "RbDagNodePtr.h"

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
    virtual const RbLanguageObject&             getStoredValue(void) const = 0;                                                 //!< Get stored value
    virtual const RbLanguageObject&             getValue(void) const = 0;                                                       //!< Get value (const)
    virtual RbLanguageObject&                   getValue(void) = 0;                                                             //!< Get value (non-const)
//    virtual const RbLanguageObject*             getValuePtr(void) const = 0;                                                    //!< Get value pointer
    virtual void                                printStruct(std::ostream& o) const = 0;                                         //!< Print struct for user
    virtual void                                printValue(std::ostream& o) const = 0;                                          //!< Print value for user

    // DAG function you should not override
    void                                        addParentNode(const RbDagNodePtr& p);                                         //!< Add parent node
    bool                                        isTouched(void) const { return touched; }                                       //!< Is node touched by move or parser?
    void                                        removeParentNode(const RbDagNodePtr& p) { parents.erase(p); }                 //!< Remove a child node

    // DAG functions you may want to override
    virtual bool                                isConst(void) const { return false; }                                           //!< Is DAG node const value?

    // DAG functions you have to override
    virtual RbDagNodePtr                      cloneDAG(std::map<const DAGNode*, RbDagNodePtr >& newNodes) const = 0;        //!< Clone entire graph
    virtual void                                swapParentNode(const RbDagNodePtr& oldP, const RbDagNodePtr& newP) = 0;     //!< Swap a parent node

protected:
    VariableNode(const std::string& valType);                                                                       //!< Constructor of empty node
    VariableNode(const VariableNode &v);                                                                            //!< Copy Constructor

//    virtual void                                getAffected(std::set<RbDagNodePtr<StochasticNode> >& affected) = 0;                    //!< Mark and get affected nodes

//    virtual void                                        keepMe(void) = 0;                                                           //!< Keep value of myself
//    virtual void                                        restoreMe(void) = 0;                                                        //!< Restore value of this nodes
//    virtual void                                        touchMe(void) = 0;                                                          //!< Touch myself (flag for recalculation)

    
    // Member variables
    bool                                        touched;                                                                        //!< Is touched by move?
};

#endif

