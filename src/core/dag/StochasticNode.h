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
                                        StochasticNode(RbPtr<Distribution> dist);                           //!< Construct from distribution (raw object)
                                        StochasticNode(const StochasticNode& x);                            //!< Copy constructor
    virtual                            ~StochasticNode(void);                                               //!< Destructor

    // Assignment operator
    StochasticNode&                     operator=(const StochasticNode& x);                                 //!< Assignment operator

    // Basic utility functions
    StochasticNode*                     clone(void) const;                                                  //!< Clone the stochastic node

    const VectorString&                 getClass(void) const;                                               //!< Get class vector
    const TypeSpec&                     getTypeSpec(void) const;                                            //!< Get language type of the object
    const RbPtr<RbLanguageObject>       getStoredValue(void);                                               //!< Get stored value (non-const fxn because of delayed evaluation)
    const RbPtr<RbLanguageObject>       getValue(void);                                                     //!< Get value (non-const fxn because of delayed evaluation)
    RbPtr<RbLanguageObject>             getValuePtr(void);                                                  //!< Get value pointer (non-const fxn because of delayed evaluation)
    void                                printStruct(std::ostream& o) const;                                 //!< Print struct for user
    void                                printValue(std::ostream& o);                                        //!< Print value for user  (non-const fxn because of delayed evaluation)
    std::string                         richInfo(void) const;                                               //!< Complete info about object

    // StochasticNode functions
    double                              calculateLnProbability(void);                                       //!< Calculate log conditional probability
    void                                clamp(RbPtr<RbLanguageObject> observedVal);             //!< Clamp the node with an observed value
    const RbPtr<Distribution>           getDistribution(void) const { return distribution; }                //!< Get distribution
    double                              getLnProbabilityRatio(void);                                        //!< Get log probability ratio of new to stored state
    bool                                isClamped(void) const { return clamped; }                           //!< Is the node clamped?
    void                                setValue(RbPtr<RbLanguageObject> value, std::set<RbPtr<StochasticNode> >& affected);     //!< Set value but do not clamp; get affected nodes
    void                                unclamp(void);                                                      //!< Unclamp the node
    
    // DAG functions
    StochasticNode*                     cloneDAG(std::map<const DAGNode*, DAGNode*>& newNodes) const;       //!< Clone entire graph
    void                                getAffected(std::set<RbPtr<StochasticNode> >& affected);            //!< Mark and get affected nodes
    void                                keep(void);                                                         //!< Keep value of this and affected nodes
    void                                keepAffected(void);                                                 //!< Keep value of affected nodes recursively
    void                                restore(void);                                                      //!< Restore value of this and affected nodes
    void                                restoreAffected(void);                                              //!< Restore value of  affected nodes recursively
    void                                swapParentNode(RbPtr<DAGNode> oldP, RbPtr<DAGNode> newP);           //!< Swap a parent node
    void                                touchAffected(void) {}                                              //!< Tell affected nodes value is reset

protected:
    // Help function
    virtual bool                        areDistributionParamsTouched() const;                               //!< Are any distribution params touched? Important in calculating prob ratio

    // Member variables
    bool                                clamped;                                                            //!< Is the node clamped with data?
    RbPtr<Distribution>                 distribution;                                                       //!< Distribution (density functions, random draw function)
    double                              lnProb;                                                             //!< Current log probability
    double                              storedLnProb;                                                       //!< Stored log probability

private:
    static const TypeSpec               typeSpec;
};

#endif

