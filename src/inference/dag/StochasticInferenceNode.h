/**
 * @file
 * This file contains the declaration of StochasticInferenceNode, which is derived
 * from VariableNode. StochasticInferenceNode is used for DAG nodes holding stochastic
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
 * @brief Declaration of StochasticInferenceNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-11 16:26:25 +0200 (Wed, 11 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-04-16, version 1.0
 * @extends DAGNode
 *
 * $Id: StochasticInferenceNode.h 1375 2012-04-11 14:26:25Z hoehna $
 */

#ifndef StochasticInferenceNode_H
#define StochasticInferenceNode_H

#include "VariableInferenceNode.h"

class Distribution;

template<valueType>
class StochasticInferenceNode : public VariableInferenceNode {
    
public:
    enum VariableType                   { INSTANTIATED, SUMMED_OVER, ELIMINATED };
    
    StochasticInferenceNode(void);                                                                          //!< Construct empty stochastic node
    StochasticInferenceNode(Distribution* dist);                                                            //!< Construct from distribution (raw object)
    StochasticInferenceNode(const StochasticInferenceNode& x);                                              //!< Copy constructor
    virtual                            ~StochasticInferenceNode(void);                                      //!< Destructor
    
    // Assignment operator
    StochasticInferenceNode&            operator=(const StochasticInferenceNode& x);                        //!< Assignment operator
    
    // Basic utility functions
    void                                printValue(std::ostream& o) const;                                  //!< Print value for user 
        
    // StochasticInferenceNode functions
    double                              calculateLnProbability(void);                                       //!< Calculate log conditional probability
    void                                clamp(valueType* observedVal);                                      //!< Clamp the node with an observed value
    double                              getLnProbabilityRatio(void);                                        //!< Get log probability ratio of new to stored state
    bool                                isClamped(void) const { return clamped; }                           //!< Is the node clamped?
//    void                                setValue(valueType* value);                                         //!< Set value but do not clamp; get affected nodes
    void                                unclamp(void);                                                      //!< Unclamp the node
    
    // Stochastic DAG Node functions for efficient computations
    double                              calculateSummedLnProbability(size_t nodeIndex);                     //!< Calculate summed log conditional probability over all possible states
    double                              calculateEliminatedLnProbability(bool enforceProbabilityCalculation);//!< Calculate summed log conditional probability over all possible states
//    DAGNode*                            cloneDAG(std::map<const DAGNode*, RbDagNodePtr>& newNodes) const;   //!< Clone entire graph
    void                                constructSumProductSequence(std::set<VariableNode*>& nodes, std::vector<StochasticInferenceNode*>& sequence); //!< Construct the sum-product sequecence
    void                                constructFactor(void);                                              //!< Construct the set of all nodes which are eliminated
    bool                                isEliminated() const;
    bool                                isNotInstantiated(void) const;
    bool                                isSummedOver() const { return type == SUMMED_OVER; }
    void                                likelihoodsNeedUpdates(void);                                       //!< Tell this node that the likelihoods need to be updated
    void                                markForRecalculation(void);                                         //!< Flag this node for recalculation
    bool                                needsRecalculation(void) const;                                     //!< Does this node need to recalculate its probability or likelihood?
    void                                setInstantiated(bool inst);                                         //!< Set whether the node is instantiated or summed over
    void                                setSummationType(VariableType t);  
    void                                setSumProductSequence(const std::vector<StochasticInferenceNode*> seq);      //!< Set the sum-product sequence
    
protected:
    // Help function
//    virtual bool                        areDistributionParamsTouched() const;                               //!< Are any distribution params touched? Important in calculating prob ratio
    void                                getAffected(std::set<StochasticInferenceNode* >& affected);                  //!< Mark and get affected nodes
    void                                keepMe(void);                                                       //!< Keep value of this and affected nodes
    void                                restoreMe(void);                                                    //!< Restore value of this nodes
    void                                touchMe(void);                                                      //!< Tell affected nodes value is reset
    
private:
    valueType*                          value;                                                              //!< Value
    
    VariableType                        type;
    
//    std::vector<Move*>                  moves;

    // Member variables
    bool                                clamped;                                                            //!< Is the node clamped with data?
    Distribution*                       distribution;                                                       //!< Distribution (density functions, random draw function)
    double                              lnProb;                                                             //!< Current log probability
    bool                                needsProbabilityRecalculation;                                      //!< Do we need recalculation of the ln prob?
    bool                                needsLikelihoodRecalculation;                                       //!< Do we need recalculation of the ln likelihood?
    double                              storedLnProb;                                                       //!< Stored log probability
    bool                                probabilityRecalculated;                                            //!< Was the probability and/or likelihood recalculated

    // probability arrays and likelihood arrays for summed out computations
    std::vector<double>                 probabilities;
    std::vector<double>                 likelihoods;
    std::vector<std::vector<double> >   partialLikelihoods;
    std::vector<double>                 storedProbabilities;
    std::vector<double>                 storedLikelihoods;
    std::vector<std::vector<double> >   storedPartialLikelihoods;
    
    std::vector<StochasticInferenceNode*>        sumProductSequence;
    
};

#endif

