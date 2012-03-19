/**
 * @file
 * This file contains the declaration of VariableNode, which is the base
 * class for variable nodes in a model DAG.
 *
 * @brief Declaration of VariableNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 *
 * $Id$
 */

#ifndef VariableNode_H
#define VariableNode_H

#include "DAGNode.h"

class StochasticNode;

#include <set>

class VariableNode : public DAGNode {

public:
    friend class DeterministicNode;                     //!< Make getAffected etc available for DeterministicNode in some compilers
    friend class StochasticNode;                        //!< Make getAffected etc available for StochasticNode in some compilers
    friend class DAGNode;                               //!< Make getAffected etc available for DAGNode in some compilers

    virtual                                    ~VariableNode(void);                                                             //!< Virtual destructor

    // Utility functions you have to override
    virtual VariableNode*                       clone(void) const = 0;                                                          //!< Clone this node
    static const std::string&                   getClassName(void);                                                             //!< Get class name
    static const TypeSpec&                      getClassTypeSpec(void);                                                         //!< Get class type spec
    virtual const RbLanguageObject&             getStoredValue(void) const = 0;                                                 //!< Get stored value
    virtual const RbLanguageObject&             getValue(void) const = 0;                                                       //!< Get value (const)
    virtual RbLanguageObject&                   getValue(void) = 0;                                                             //!< Get value (non-const)
//    virtual const RbLanguageObject*             getValuePtr(void) const = 0;                                                    //!< Get value pointer
    virtual void                                printStruct(std::ostream& o) const = 0;                                         //!< Print struct for user
    virtual void                                printValue(std::ostream& o) const = 0;                                          //!< Print value for user

    // DAG function you should not override
    void                                        addParentNode( DAGNode* p);                                                     //!< Add parent node
    StochasticNode*                             getFactorRoot(void) const;
    bool                                        isTouched(void) const { return touched; }                                       //!< Is node touched by move or parser?
    void                                        removeParentNode(DAGNode* p) { parents.erase(p); }                              //!< Remove a child node
    void                                        setFactorRoot(StochasticNode* r);
    
    // DAG functions you may want to override
    virtual bool                                isConst(void) const { return false; }                                           //!< Is DAG node const value?

    // DAG functions you have to override
//    virtual double                              calculateSummedLnProbability(size_t nodeIndex) = 0;                             //!< Calculate summed log conditional probability over all possible states
    virtual double                              calculateEliminatedLnProbability(void) = 0;                                     //!< Calculate summed log conditional probability over all possible states
//    virtual std::vector<StochasticNode*>        constructSumProductSequence(void) = 0;                                          //!< Construct the sum-product sequence
    virtual void                                constructSumProductSequence(std::set<VariableNode*>& nodes, std::vector<StochasticNode*>& sequence) = 0;//!< Construct the set of all nodes which are eliminated
    virtual DAGNode*                            cloneDAG(std::map<const DAGNode*, RbDagNodePtr>& newNodes) const = 0;           //!< Clone entire graph
    virtual bool                                isEliminated(void) const = 0;
    virtual bool                                isNotInstantiated(void) const = 0;
    virtual void                                likelihoodsNeedUpdates() = 0;                                                   //!< Tell this node that the likelihoods need to be updated
    virtual void                                swapParentNode( DAGNode* oldP, DAGNode* newP) = 0;                              //!< Swap a parent node

protected:
    VariableNode( void );                                                                                                       //!< Constructor of empty node
    VariableNode(const VariableNode &v);                                                                                        //!< Copy Constructor
        
    virtual void                                getAffected(std::set<StochasticNode*>& affected) = 0;                                  //!< Mark and get affected nodes

//    virtual void                                        keepMe(void) = 0;                                                           //!< Keep value of myself
//    virtual void                                        restoreMe(void) = 0;                                                        //!< Restore value of this nodes
//    virtual void                                        touchMe(void) = 0;                                                          //!< Touch myself (flag for recalculation)

    StochasticNode*                             factorRoot;

    // Member variables
    bool                                        touched;                                                                        //!< Is touched by move?
};

#endif

