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
 * @author The RevBayes Development Core Team
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
    DeterministicNode(void);                                                                                    //!< Constructor
    DeterministicNode(const RbPtr<const Plate> &p);                                                             //!< Constructor
    DeterministicNode(const RbPtr<RbFunction> &func, const RbPtr<const Plate> &p);                              //!< Constructor with function
    DeterministicNode(const DeterministicNode& x);                                                              //!< Copy Constructor

    // Utility functions implemented here
    DeterministicNode*                      clone(void) const;                                                  //!< Clone this node
    std::string                             debugInfo(void) const;                                              //!< Complete info about object
    static const std::string&               getClassName(void);                                                 //!< Get class name
    static const TypeSpec&                  getClassTypeSpec(void);                                             //!< Get class type spec
    const TypeSpec&                         getTypeSpec(void) const;                                            //!< Get language type of the object
    void                                    printValue(std::ostream& o) const;                                  //!< Print value for user 

    // DAG functions implemented here
    double                                  calculateEliminatedLnProbability(bool enforceProbabilityCalculation);   //!< Calculate summed log conditional probability over all possible states
    DAGNode*                                cloneDAG(std::map<const DAGNode*, RbPtr<DAGNode> >& newNodes) const;    //!< Clone entire graph
    virtual InferenceDagNode*               createLeanDag(std::map<const DAGNode*, InferenceDagNode*>& newNodes) const; //!< Create a lean DAG from this "fat" DAG
    void                                    constructSumProductSequence(std::set<VariableNode*>& nodes, std::vector<StochasticNode*>& sequence);//!< Construct the set of all nodes which are eliminated
    void                                    expand(void);                                                       //!< Expand the current value n times. This is equivalent to dropping this node on a plate of size n.
    const RbLanguageObject&                 getValue(void) const;                                               //!< Get value (const)
    RbLanguageObject&                       getValue(void);                                                     //!< Get value (non-const)
    const RbFunction&                       getFunction(void) const;
    bool                                    isNotInstantiated(void) const;
    bool                                    isEliminated(void) const;
    void                                    likelihoodsNeedUpdates();                                           //!< Tell this node that the likelihoods need to be updated
    void                                    markForRecalculation(void);                                         //!< Flag this node for recalculation
    bool                                    needsRecalculation(void) const;                                     //!< Does this node need to recalculate its probability or likelihood?
    void                                    printStruct(std::ostream& o) const;                                 //!< Print struct for user
    void                                    swapParentNode( DAGNode* oldP, DAGNode* newP);           //!< Swap a parent node

protected:

    // Utility function you have to override
    void                                    getAffected(std::set<RbPtr<StochasticNode> >& affected);            //!< Mark and get affected nodes
    void                                    keepMe(void);                                                       //!< Keep value of this and affected nodes
    void                                    restoreMe(void);                                                    //!< Restore value of this nodes
    void                                    touchMe(void);                                                      //!< Tell affected nodes value is reset
    virtual void                            update(void);                                                       //!< Update value and storedValue

    
private:
    static const TypeSpec                   typeSpec;

    // Member variable
    bool                                    needsUpdate;                                                        //!< True when value after touch but before update; if then updated set to false
    RbPtr<RbFunction>                       function;
    RbPtr<RbLanguageObject>                 value;                                                               //!< Value

};

#endif
