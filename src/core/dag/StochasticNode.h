/**
 * @file
 * This file contains the declaration of StochasticNode, which is derived
 * from VariableNode. StochasticNode is used for DAG nodes holding stochastic
 * variables with an associated distribution object.
 *
 * @brief Declaration of StochasticNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-30 00:19:25 +0100 (Ons, 30 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id: StochasticNode.h 216 2009-12-29 23:19:25Z ronquist $
 */

#ifndef StochasticNode_H
#define StochasticNode_H

#include "VariableNode.h"

class Distribution;
class MemberNode;
class RbObject;
class VectorString;


const std::string StochasticNode_name = "Stochastic node";

class StochasticNode : public VariableNode {

public:
                                        StochasticNode(const TypeSpec& type);                               //!< Construct empty stochastic node
                                        StochasticNode(Distribution* dist);                                 //!< Construct from distribution (raw object)
                                        StochasticNode(const StochasticNode& x);                            //!< Copy constructor
    virtual                            ~StochasticNode(void);                                               //!< Destructor

    // Assignment operator
    StochasticNode&                     operator=(const StochasticNode& x);                                 //!< Assignment operator

    // Basic utility functions
    StochasticNode*                     clone(void) const;                                                  //!< Clone the stochastic node
    std::string                         debugInfo(void) const;                                               //!< Complete info about object
    const VectorString&                 getClass(void) const;                                               //!< Get class vector
    const TypeSpec&                     getTypeSpec(void) const;                                            //!< Get language type of the object
    const RbLanguageObject&             getStoredValue(void) const;                                         //!< Get stored value
    const RbLanguageObject&             getValue(void) const;                                               //!< Get value (const)
    RbLanguageObject&                   getValue(void);                                                     //!< Get value (non-const)
    const RbLanguageObject*             getValuePtr(void) const;                                                  //!< Get value pointer
    void                                printStruct(std::ostream& o) const;                                 //!< Print struct for user
    void                                printValue(std::ostream& o) const;                                  //!< Print value for user 

    // StochasticNode functions
    double                              calculateLnProbability(void);                                       //!< Calculate log conditional probability
    void                                clamp(RbLanguageObject* observedVal);                               //!< Clamp the node with an observed value
    const Distribution&                 getDistribution(void) const;                                        //!< Get distribution (const)
    Distribution&                       getDistribution(void);                                              //!< Get distribution (non-const)
    double                              getLnProbabilityRatio(void);                                        //!< Get log probability ratio of new to stored state
    bool                                isClamped(void) const { return clamped; }                           //!< Is the node clamped?
    void                                setValue(RbLanguageObject* value);                                  //!< Set value but do not clamp; get affected nodes
    void                                unclamp(void);                                                      //!< Unclamp the node
    
    // DAG functions
    RbDagNodePtr                      cloneDAG(std::map<const DAGNode*, RbDagNodePtr >& newNodes) const;//!< Clone entire graph
    void                                swapParentNode(const RbDagNodePtr& oldP, const RbDagNodePtr& newP);//!< Swap a parent node

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
    bool                                instantiated;                                                       //!< Is this node instantiated or integrated over?
    double                              lnProb;                                                             //!< Current log probability
    bool                                needsRecalculation;                                                 //!< Do we need recalculation of the ln prob?
    double                              storedLnProb;                                                       //!< Stored log probability

private:
    static const TypeSpec               typeSpec;
};

#endif

