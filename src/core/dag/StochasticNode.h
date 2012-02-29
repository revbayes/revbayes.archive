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

class Distribution;
class MemberNode;
class RbObject;
class VectorString;


class StochasticNode : public VariableNode {

public:
    enum VariableType                   { INSTANTIATED, SUMMED_OVER, ELIMINATED };
    
                                        StochasticNode(void);                                               //!< Construct empty stochastic node
                                        StochasticNode(Distribution* dist);                                 //!< Construct from distribution (raw object)
                                        StochasticNode(const StochasticNode& x);                            //!< Copy constructor
    virtual                            ~StochasticNode(void);                                               //!< Destructor

    // Assignment operator
    StochasticNode&                     operator=(const StochasticNode& x);                                 //!< Assignment operator

    // Basic utility functions
    StochasticNode*                     clone(void) const;                                                  //!< Clone the stochastic node
    std::string                         debugInfo(void) const;                                              //!< Complete info about object
    static const std::string&           getClassName(void);                                                 //!< Get class name
    static const TypeSpec&              getClassTypeSpec(void);                                             //!< Get class type spec
    const TypeSpec&                     getTypeSpec(void) const;                                            //!< Get language type of the object
    const RbLanguageObject&             getStoredValue(void) const;                                         //!< Get stored value
    const RbLanguageObject&             getValue(void) const;                                               //!< Get value (const)
    RbLanguageObject&                   getValue(void);                                                     //!< Get value (non-const)
//    const RbLanguageObject*             getValuePtr(void) const;                                                  //!< Get value pointer
    void                                printStruct(std::ostream& o) const;                                 //!< Print struct for user
    void                                printValue(std::ostream& o) const;                                  //!< Print value for user 

    // StochasticNode functions
    double                              calculateLnProbability(void);                                       //!< Calculate log conditional probability
    void                                clamp(RbLanguageObject* observedVal);                               //!< Clamp the node with an observed value
    const Distribution&                 getDistribution(void) const;                                        //!< Get distribution (const)
    Distribution&                       getDistribution(void);                                              //!< Get distribution (non-const)
    double                              getLnProbabilityRatio(void);                                        //!< Get log probability ratio of new to stored state
    bool                                isEliminated(void) const;
    bool                                isClamped(void) const { return clamped; }                           //!< Is the node clamped?
    void                                likelihoodsNeedUpdates(void);                                       //!< Tell this node that the likelihoods need to be updated
    void                                setInstantiated(bool inst);                                         //!< Set whether the node is instantiated or summed over
    void                                setSumProductSequence(const std::vector<StochasticNode*> seq);      //!< Set the sum-product sequence
    void                                setValue(RbLanguageObject* value);                                  //!< Set value but do not clamp; get affected nodes
    void                                unclamp(void);                                                      //!< Unclamp the node
    
    // DAG functions
    double                              calculateSummedLnProbability(size_t nodeIndex);                     //!< Calculate summed log conditional probability over all possible states
    double                              calculateEliminatedLnProbability(void);                             //!< Calculate summed log conditional probability over all possible states
    DAGNode*                            cloneDAG(std::map<const DAGNode*, RbDagNodePtr>& newNodes) const;   //!< Clone entire graph
    void                                constructSumProductSequence(std::set<VariableNode*>& nodes, std::vector<StochasticNode*>& sequence); //!< Construct the sum-product sequecence
    void                                constructFactor(void);                                              //!< Construct the set of all nodes which are eliminated
    void                                swapParentNode( DAGNode* oldP, DAGNode* newP);                      //!< Swap a parent node

protected:
    // Help function
    virtual bool                        areDistributionParamsTouched() const;                               //!< Are any distribution params touched? Important in calculating prob ratio
    void                                getAffected(std::set<StochasticNode* >& affected);                  //!< Mark and get affected nodes
    void                                keepMe(void);                                                       //!< Keep value of this and affected nodes
    void                                restoreMe(void);                                                    //!< Restore value of this nodes
    void                                touchMe(void);                                                      //!< Tell affected nodes value is reset

    // Member variables
    bool                                clamped;                                                            //!< Is the node clamped with data?
    Distribution*                       distribution;                                                       //!< Distribution (density functions, random draw function)
    double                              lnProb;                                                             //!< Current log probability
    bool                                needsProbabilityRecalculation;                                      //!< Do we need recalculation of the ln prob?
    bool                                needsLikelihoodRecalculation;                                       //!< Do we need recalculation of the ln likelihood?
    double                              storedLnProb;                                                       //!< Stored log probability

private:
    static const TypeSpec               typeSpec;
    RbLanguageObject*                   value;                                                              //!< Value
    RbLanguageObject*                   storedValue;                                                                    //!< Stored value

    VariableType                        type;
    
    // probability arrays and likelihood arrays for summed out computations
    std::vector<double>                 probabilities;
    std::vector<double>                 likelihoods;
    std::vector<std::vector<double> >   partialLikelihoods;
    std::vector<double>                 storedProbabilities;
    std::vector<double>                 storedLikelihoods;
    std::vector<std::vector<double> >   storedPartialLikelihoods;
    
    std::vector<StochasticNode*>        sumProductSequence;
    
};

#endif

