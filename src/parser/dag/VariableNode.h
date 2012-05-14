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

class Plate;
class StochasticNode;

#include <set>

class VariableNode : public DAGNode {

public:
    friend class DeterministicNode;                     //!< Make getAffected etc available for DeterministicNode in some compilers
    friend class StochasticNode;                        //!< Make getAffected etc available for StochasticNode in some compilers
    friend class DAGNode;                               //!< Make getAffected etc available for DAGNode in some compilers

    virtual                                        ~VariableNode(void);                                                                     //!< Virtual destructor

    // Utility functions you have to override
    virtual VariableNode*                           clone(void) const = 0;                                                                  //!< Clone this node
    static const std::string&                       getClassName(void);                                                                     //!< Get class name
    static const TypeSpec&                          getClassTypeSpec(void);                                                                 //!< Get class type spec
    virtual const RbPtr<const RbLanguageObject>&    getValue(void) const = 0;                                                               //!< Get value (const)
    virtual const RbPtr<RbLanguageObject>&          getValue(void) = 0;                                                                     //!< Get value (non-const)
    virtual void                                    printStruct(std::ostream& o) const = 0;                                                 //!< Print struct for user
    virtual void                                    printValue(std::ostream& o) const = 0;                                                  //!< Print value for user

    // DAG function you should not override
    void                                            addParentNode( DAGNode* p);                                                             //!< Add parent node
    StochasticNode*                                 getFactorRoot(void) const;
    bool                                            isTouched(void) const { return touched; }                                               //!< Is node touched by move or parser?
    void                                            markChildrenForRecalculation(void);                                                     //!< Mark all my children for recalculation
    void                                            removeParentNode(DAGNode* p) { parents.erase(p); }                                      //!< Remove a child node
    void                                            setFactorRoot(StochasticNode* r);
    

    // DAG functions you have to override
//    virtual double                              calculateSummedLnProbability(size_t nodeIndex) = 0;                                   //!< Calculate summed log conditional probability over all possible states
    virtual double                                  calculateEliminatedLnProbability(bool enforceProbabilityCalculation) = 0;               //!< Calculate summed log conditional probability over all possible states
//    virtual std::vector<StochasticNode*>        constructSumProductSequence(void) = 0;                                                //!< Construct the sum-product sequence
    virtual void                                    constructSumProductSequence(std::set<VariableNode*>& nodes, std::vector<StochasticNode*>& sequence) = 0;//!< Construct the set of all nodes which are eliminated
    virtual DAGNode*                                cloneDAG(std::map<const DAGNode*, RbPtr<DAGNode> >& newNodes) const = 0;                //!< Clone entire graph
    virtual InferenceDagNode*                       createLeanDag(std::map<const DAGNode*, InferenceDagNode*>& newNodes) const = 0;         //!< Create a lean DAG from this "fat" DAG
    virtual void                                    expand(void) = 0;                                                                       //!< Expand the current value n times. This is equivalent to dropping this node on a plate of size n.
    virtual void                                    markForRecalculation(void) = 0;
    virtual bool                                    isEliminated(void) const = 0;
    virtual bool                                    isNotInstantiated(void) const = 0;
    virtual void                                    likelihoodsNeedUpdates() = 0;                                                           //!< Tell this node that the likelihoods need to be updated
    virtual bool                                    needsRecalculation(void) const = 0;                                                     //!< Does this node need to recalculate its probability or likelihood?
    virtual void                                    swapParentNode( DAGNode* oldP, DAGNode* newP) = 0;                                      //!< Swap a parent node

protected:
    VariableNode(const RbPtr<const Plate> &p);                                                                                          //!< Constructor of empty node with plate on whch this node sits
    VariableNode(const VariableNode &v);                                                                                                //!< Copy Constructor
        
    virtual void                                    getAffected(std::set<RbPtr<StochasticNode> >& affected) = 0;                            //!< Mark and get affected nodes

//    virtual void                                        keepMe(void) = 0;                                                             //!< Keep value of myself
//    virtual void                                        restoreMe(void) = 0;                                                          //!< Restore value of this nodes
//    virtual void                                        touchMe(void) = 0;                                                            //!< Touch myself (flag for recalculation)

    StochasticNode*                                 factorRoot;

    // Member variables
    bool                                            touched;                                                                        //!< Is touched by move?
};

#endif

