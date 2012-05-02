/**
 * @file
 * This file contains the declaration of VariableInferenceNode, which is the base
 * class for variable nodes in a model DAG.
 *
 * @brief Declaration of VariableInferenceNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-11 16:26:25 +0200 (Wed, 11 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-04-16, version 1.0
 *
 * $Id: VariableInferenceNode.h 1375 2012-04-11 14:26:25Z hoehna $
 */

#ifndef VariableInferenceNode_H
#define VariableInferenceNode_H

#include "InferenceDagNode.h"


#include <set>

class VariableInferenceNode : public InferenceDagNode {
    
public:
    
    virtual                                    ~VariableInferenceNode(void);                                                                //!< Virtual destructor
        
    // DAG function you should not override
    void                                        addParentNode( InferenceDagNode* p);                                                        //!< Add parent node
    StochasticInferenceNode*                    getFactorRoot(void) const;
    bool                                        isTouched(void) const { return touched; }                                                   //!< Is node touched by move or parser?
    void                                        markChildrenForRecalculation(void);                                                         //!< Mark all my children for recalculation
    void                                        removeParentNode(InferenceDagNode* p) { parents.erase(p); }                                 //!< Remove a child node
    void                                        setFactorRoot(StochasticInferenceNode* r);
    
    // DAG functions you have to override
    //    virtual double                              calculateSummedLnProbability(size_t nodeIndex) = 0;                             //!< Calculate summed log conditional probability over all possible states
    virtual double                              calculateEliminatedLnProbability(bool enforceProbabilityCalculation) = 0;                   //!< Calculate summed log conditional probability over all possible states
    virtual void                                constructSumProductSequence(std::set<VariableInferenceNode*>& nodes, std::vector<StochasticInferenceNode*>& sequence) = 0;//!< Construct the set of all nodes which are eliminated
    virtual void                                getAffected(std::set<StochasticInferenceNode*>& affected) = 0;                              //!< get affected nodes
    virtual void                                markForRecalculation(void) = 0;
    virtual bool                                isEliminated(void) const = 0;
    virtual bool                                isNotInstantiated(void) const = 0;
    virtual void                                likelihoodsNeedUpdates() = 0;                                                               //!< Tell this node that the likelihoods need to be updated
    virtual bool                                needsRecalculation(void) const = 0;                                                         //!< Does this node need to recalculate its probability or likelihood?
//    virtual void                                swapParentNode( DAGNode* oldP, DAGNode* newP) = 0;                              //!< Swap a parent node
    
protected:
    VariableInferenceNode(const RbValue<void*> &v, const std::string &n);                                                                   //!< Constructor of empty node
    VariableInferenceNode(const VariableInferenceNode &v);                                                                                  //!< Copy Constructor
    
    
    //    virtual void                                        keepMe(void) = 0;                                                           //!< Keep value of myself
    //    virtual void                                        restoreMe(void) = 0;                                                        //!< Restore value of this nodes
    //    virtual void                                        touchMe(void) = 0;                                                          //!< Touch myself (flag for recalculation)
    
    StochasticInferenceNode*                    factorRoot;
    
    // Member variables
    bool                                        touched;                                                                        //!< Is touched by move?
};

#endif

