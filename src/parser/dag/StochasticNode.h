/**
 * @file
 * This file contains the declaration of StochasticNode, which is derived
 * from VariableNode. StochasticNode is used for DAG nodes holding stochastic
 * variables with an associated distribution object.
 *
 * Stochastic nodes come in three flavors: instantiated, eliminated and summed-over.
 *
 * Instantiated: Instatiated stochastic nodes are the rather simple type. They have exactly one value
 * at each time. A clamped node can also be considered as an instantiated node, though they cannot change there values.
 * Instantiated nodes can changed their values when moves are attached to them. 
 * The probability for this value is computed as simply the pdf. Note, instantiated nodes with non-instantiated parents
 * behave differently.
 *
 * Summed-Over: Stochastic nodes on a discrete distribution can be summed over, e.g. if one is not interested
 * in the actual value. The simplest and safe solution for this is to calculate the sum of the probability of each 
 * possible value, multiply it by the likelihood of the value. For multiple nodes in a subgraph bein summed-over,
 * we need the sum-product sequence. The probability of the actual node cannot be calculated in general,
 * but the likelihood of the whole subgraph.
 *
 * Eliminated: Stochastic nodes which are summed-over can also, in some situations, be eliminated. Felsenstein's pruning
 * algorithm uses eliminated nodes. They key feature of eliminated nodes is that eliminated nodes know the likelihood 
 * of each possible value, given the current values of all other nodes. Eliminated nodes are usually at the bottom
 * of the summed-over subgraph. 
 * Any node must not have two eliminated parents. The strategy is to find as many as possible eliminated nodes and
 * set the other nodes as summed-over.
 *
 * @brief Declaration of StochasticNode
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

#ifndef StochasticNode_H
#define StochasticNode_H

#include "VariableNode.h"

class RlDistribution;
class Move;

class StochasticNode : public VariableNode {

public:
    enum VariableType                   { INSTANTIATED, SUMMED_OVER, ELIMINATED };
    
                                            StochasticNode(void);                                                                   //!< Construct empty stochastic node
                                            StochasticNode(const RbPtr<const Plate> &p);                                            //!< Construct empty stochastic node
                                            StochasticNode(const RbPtr<RlDistribution> &dist, const RbPtr<const Plate> &p);     //!< Construct from distribution (raw object) with plate holding this node
                                            StochasticNode(const StochasticNode& x);                                                //!< Copy constructor

    // Assignment operator
    StochasticNode&                         operator=(const StochasticNode& x);                                 //!< Assignment operator

    // Basic utility functions
    StochasticNode*                         clone(void) const;                                                  //!< Clone the stochastic node
    std::string                             debugInfo(void) const;                                              //!< Complete info about object
    static const std::string&               getClassName(void);                                                 //!< Get class name
    static const TypeSpec&                  getClassTypeSpec(void);                                             //!< Get class type spec
    const TypeSpec&                         getTypeSpec(void) const;                                            //!< Get language type of the object
    void                                    printValue(std::ostream& o) const;                                  //!< Print value for user 

    // DAG node function
    RbPtr<RbLanguageObject>                 executeMethod(const std::string& name, const std::vector<RlValue<const RbLanguageObject> >& args);            //!< Override to map member methods to internal functions
    const MethodTable&                      getMethods(void) const;                                                         //!< Get member methods (const)
    RlValue<const RbLanguageObject>         getValue(void) const;                                               //!< Get value (const)
    const RlValue<RbLanguageObject>&        getValue(void);                                                     //!< Get value (non-const)
    void                                    printStruct(std::ostream& o) const;                                 //!< Print struct for user

    // StochasticNode functions
    double                                  calculateLnProbability(void);                                       //!< Calculate log conditional probability
    void                                    clamp(const RlValue<RbLanguageObject> &observedVal);                  //!< Clamp the node with an observed value
    void                                    markForRecalculation(void);                                         //!< Flag this node for recalculation
    const RlDistribution&               getDistribution(void) const;                                        //!< Get distribution (const)
    RlDistribution&                     getDistribution(void);                                              //!< Get distribution (non-const)
    double                                  getLnProbabilityRatio(void);                                        //!< Get log probability ratio of new to stored state
    bool                                    isNotInstantiated(void) const;
    bool                                    isClamped(void) const { return clamped; }                           //!< Is the node clamped?
    bool                                    isEliminated() const;
    bool                                    isSummedOver() const { return type == SUMMED_OVER; }
    void                                    likelihoodsNeedUpdates(void);                                       //!< Tell this node that the likelihoods need to be updated
    bool                                    needsRecalculation(void) const;                                     //!< Does this node need to recalculate its probability or likelihood?
    void                                    setInstantiated(bool inst);                                         //!< Set whether the node is instantiated or summed over
    void                                    setSummationType(VariableType t);  
    void                                    setSumProductSequence(const std::vector<StochasticNode*> seq);      //!< Set the sum-product sequence
    void                                    setValue(const RlValue<RbLanguageObject> &value);                                  //!< Set value but do not clamp; get affected nodes
    void                                    unclamp(void);                                                      //!< Unclamp the node
    
    // DAG functions
    double                                  calculateSummedLnProbability(size_t nodeIndex);                                 //!< Calculate summed log conditional probability over all possible states
    double                                  calculateEliminatedLnProbability(bool enforceProbabilityCalculation);           //!< Calculate summed log conditional probability over all possible states
    DAGNode*                                cloneDAG(std::map<const DAGNode*, RbPtr<DAGNode> >& newNodes) const;            //!< Clone entire graph
    virtual InferenceDagNode*               createLeanDag(std::map<const DAGNode*, InferenceDagNode*>& newNodes) const;     //!< Create a lean DAG from this "fat" DAG
    void                                    constructSumProductSequence(std::set<VariableNode*>& nodes, std::vector<StochasticNode*>& sequence); //!< Construct the sum-product sequecence
    void                                    constructFactor(void);                                              //!< Construct the set of all nodes which are eliminated
    void                                    expand(void);                                                       //!< Expand the current value n times. This is equivalent to dropping this node on a plate of size n.
    void                                    swapParentNode( DAGNode* oldP, DAGNode* newP);                      //!< Swap a parent node

protected:
    // Help function
    void                                    getAffected(std::set<RbPtr<StochasticNode> >& affected);            //!< Mark and get affected nodes
    void                                    keepMe(void);                                                       //!< Keep value of this and affected nodes
    void                                    restoreMe(void);                                                    //!< Restore value of this nodes
    void                                    touchMe(void);                                                      //!< Tell affected nodes value is reset

    // Member variables
    bool                                    clamped;                                                            //!< Is the node clamped with data?
    RbPtr<RlDistribution>               distribution;                                                       //!< Distribution (density functions, random draw function)
    double                                  lnProb;                                                             //!< Current log probability
    bool                                    needsProbabilityRecalculation;                                      //!< Do we need recalculation of the ln prob?
    bool                                    needsLikelihoodRecalculation;                                       //!< Do we need recalculation of the ln likelihood?
    double                                  storedLnProb;                                                       //!< Stored log probability
    bool                                    probabilityRecalculated;                                            //!< Was the probability and/or likelihood recalculated

private:
    RlValue<RbLanguageObject>               createRV(void);
    RlValue<RbLanguageObject>               createRV(const std::vector<size_t> &plateLengths, size_t level, const std::vector<size_t> &offsets, const std::vector<RlValue<const RbLanguageObject> > &args);
    RlValue<RbLanguageObject>               createRVSingleValue(size_t plateIndex, const std::vector<size_t> &plateLengths, const std::vector<const RbObject*> &args);
    
    RlValue<RbLanguageObject>               value;                                                              //!< Value

    VariableType                            type;
        
    // probability arrays and likelihood arrays for summed out computations
    std::vector<double>                     probabilities;
    std::vector<double>                     likelihoods;
    std::vector<std::vector<double> >       partialLikelihoods;
    std::vector<double>                     storedProbabilities;
    std::vector<double>                     storedLikelihoods;
    std::vector<std::vector<double> >       storedPartialLikelihoods;
    
    std::vector<StochasticNode*>            sumProductSequence;
    
};

#endif

