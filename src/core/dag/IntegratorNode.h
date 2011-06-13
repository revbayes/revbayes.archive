/**
 * @file
 * This file contains the declaration of IntegratorNode, which is derived
 * from StochasticNode. IntegratorNode is used for DAG nodes integrating
 * out probabilities over a conditional likelihood vector, given the probability
 * mass vector from an appropriate categorical distribution.
 *
 * @brief Declaration of IntegratorNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-30 00:19:25 +0100 (Ons, 30 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id: IntegratorNode.h 216 2009-12-29 23:19:25Z ronquist $
 */

#ifndef IntegratorNode_H
#define IntegratorNode_H

#include "StochasticNode.h"

class Distribution;
class DeterministicMemberNode;
class RbObject;
class VectorString;


class IntegratorNode : public StochasticNode {

    public:
                                IntegratorNode(void);                                               //!< Construct empty integrator node
                                IntegratorNode(DeterministicMemberNode* condLikeVec, Distribution* dist);        //!< Construct from cond like vector and distribution
                                IntegratorNode(const IntegratorNode& x);                            //!< Copy constructor
							   ~IntegratorNode(void);                                               //!< Destructor

        // Assignment operator
        IntegratorNode&         operator=(const IntegratorNode& x);                                 //!< Assignment operator

        // Basic utility functions
        IntegratorNode*         clone(void) const;                                                  //!< Clone the stochastic node

        const VectorString&     getDAGClass(void) const;                                            //!< Get DAG node class vector
        int                     getDim(void) const { return valueDim; }                             //!< Get dim (0 for scalar, 1 for vector, etc)
        void                    printStruct(std::ostream& o) const;                                 //!< Print struct for user
        void                    printValue(std::ostream& o) const;                                  //!< Print struct for user
        std::string             richInfo(void) const;                                               //!< Complete info about object

        // IntegratorNode functions
        double                  calculateLnProbability(void);                                       //!< Calculate log conditional probability
        void                    clamp(RbObject* observedVal);                                       //!< Clamp the node with an observed value
        const Distribution*     getDistribution(void) const { return distribution; }                //!< Get distribution
        double                  getLnProbabilityRatio(void);                                        //!< Get log probability ratio of new to stored state
        bool                    isClamped(void) const { return clamped; }                           //!< Is the node clamped?
        void                    setValue(RbObject* value);                                          //!< Set value but do not clamp
        void                    unclamp(void);                                                      //!< Unclamp the node
        
        // DAG functions
        IntegratorNode*         cloneDAG(std::map<const DAGNode*, DAGNode*>& newNodes) const;       //!< Clone entire graph
        void                    keep(void);                                                         //!< Keep value of this and affected nodes
        void                    restore(void);                                                      //!< Restore value of this and affected nodes
        void                    swapParentNode(DAGNode* oldP, DAGNode* newP);                       //!< Swap a parent node
        void                    touchAffected(void) {}                                              //!< Tell affected nodes value is reset

        // Move function
        MoveSchedule*           getDefaultMoves(void);                                              //!< Get default moves

    protected:
        DeterministicMemberNode* condLikes;                                                          //!< Cond likes
};

#endif

