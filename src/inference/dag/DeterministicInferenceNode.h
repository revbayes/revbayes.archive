/**
 * @file
 * This file contains the declaration of DeterministicInferenceNode, which is derived
 * from DAGNode. DeterministicInferenceNode is used for DAG nodes associated with
 * an expression (equation) that determines their value.
 *
 * This class is a template because we store the values on pointers tp basic c++ types
 * for faster access (no indirection and casting).
 *
 * @brief Declaration of DeterministicInferenceNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-11 16:26:25 +0200 (Wed, 11 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-04-16, version 1.0
 * @extends DAGNode
 *
 * $Id: DeterministicInferenceNode.h 1375 2012-04-11 14:26:25Z hoehna $
 */

#ifndef DeterministicInferenceNode_H
#define DeterministicInferenceNode_H

#include "VariableNode.h"

#include <string>
#include <vector>

class RbFunction;

template<valueType>
class DeterministicInferenceNode : public VariableInferenceNode {
    
public:
    DeterministicInferenceNode( void );                                          //!< Constructor
    DeterministicInferenceNode(RbFunction* func);                                //!< Constructor with function
    DeterministicInferenceNode(const DeterministicInferenceNode& x);                      //!< Copy Constructor
    virtual                                ~DeterministicInferenceNode(void);                                            //!< Destructor
    
    // Utility functions implemented here
    void                                    printValue(std::ostream& o) const;                                  //!< Print value for user 
//    const RbFunction&                       getFunction(void) const;

    // DAG functions implemented here
    double                                  calculateEliminatedLnProbability(bool enforceProbabilityCalculation);//!< Calculate summed log conditional probability over all possible states
    void                                    constructSumProductSequence(std::set<VariableNode*>& nodes, std::vector<StochasticNode*>& sequence);//!< Construct the set of all nodes which are eliminated
    bool                                    isNotInstantiated(void) const;
    bool                                    isEliminated(void) const;
    void                                    likelihoodsNeedUpdates();                                           //!< Tell this node that the likelihoods need to be updated
    void                                    markForRecalculation(void);                                         //!< Flag this node for recalculation
    bool                                    needsRecalculation(void) const;                                     //!< Does this node need to recalculate its probability or likelihood?
    
protected:
    
    // Utility function you have to override
    void                                    getAffected(std::set<StochasticNode* >& affected);                  //!< Mark and get affected nodes
    void                                    keepMe(void);                                                       //!< Keep value of this and affected nodes
    void                                    restoreMe(void);                                                    //!< Restore value of this nodes
    void                                    touchMe(void);                                                      //!< Tell affected nodes value is reset
    virtual void                            update(void);                                                       //!< Update value and storedValue
    
    // Member variable
    bool                                    needsUpdate;                                                        //!< True when value after touch but before update; if then updated set to false
    RbFunction*                             function;
    
private:
    valueType*                              value;                                                               //!< Value
    
};

#endif
