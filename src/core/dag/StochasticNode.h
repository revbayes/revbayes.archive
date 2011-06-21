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


class StochasticNode : public VariableNode {

    public:
                                StochasticNode(const TypeSpec& type);                               //!< Construct empty stochastic node
                                StochasticNode(Distribution* dist);                                 //!< Construct from distribution (raw object)
                                StochasticNode(const StochasticNode& x);                            //!< Copy constructor
        virtual	               ~StochasticNode(void);                                               //!< Destructor

        // Assignment operator
        StochasticNode&         operator=(const StochasticNode& x);                                 //!< Assignment operator

        // Basic utility functions
        StochasticNode*         clone(void) const;                                                  //!< Clone the stochastic node

        const VectorString&     getDAGClass(void) const;                                            //!< Get DAG node class vector
        int                     getDim(void) const { return valueDim; }                             //!< Get dim (0 for scalar, 1 for vector, etc)
        const RbObject*         getStoredValue(void);                                               //!< Get stored value (non-const fxn because of delayed evaluation)
        const RbObject*         getValue(void);                                                     //!< Get value (non-const fxn because of delayed evaluation)
        void                    printStruct(std::ostream& o) const;                                 //!< Print struct for user
        void                    printValue(std::ostream& o);                                        //!< Print value for user  (non-const fxn because of delayed evaluation)
        std::string             richInfo(void) const;                                               //!< Complete info about object

        // StochasticNode functions
        double                  calculateLnProbability(void);                                       //!< Calculate log conditional probability
        void                    clamp(RbObject* observedVal);                                       //!< Clamp the node with an observed value
        const Distribution*     getDistribution(void) const { return distribution; }                //!< Get distribution
        double                  getLnProbabilityRatio(void);                                        //!< Get log probability ratio of new to stored state
        bool                    isClamped(void) const { return clamped; }                           //!< Is the node clamped?
        void                    setValue(RbObject* value);                                          //!< Set value but do not clamp
        void                    unclamp(void);                                                      //!< Unclamp the node
        
        // DAG functions
        StochasticNode*         cloneDAG(std::map<const DAGNode*, DAGNode*>& newNodes) const;       //!< Clone entire graph
        void                    getAffected(std::set<StochasticNode*>& affected);                   //!< Mark and get affected nodes
        void                    keep(void);                                                         //!< Keep value of this and affected nodes
        void    	            keepAffected(void);                                                 //!< Keep value of affected nodes recursively
        void                    restore(void);                                                      //!< Restore value of this and affected nodes
        void                    restoreAffected(void);                                              //!< Restore value of  affected nodes recursively
        void                    swapParentNode(DAGNode* oldP, DAGNode* newP);                       //!< Swap a parent node
        void                    touchAffected(void) {}                                              //!< Tell affected nodes value is reset

        // Move function
        MoveSchedule*           getDefaultMoves(void);                                              //!< Get default moves

    protected:
        // Help function
        virtual bool            areDistributionParamsTouched() const;                               //!< Are any distribution params touched? Important in calculating prob ratio

        // Member variables
        int                     valueDim;                                                           //!< Dimensions of value
        bool                    clamped;                                                            //!< Is the node clamped with data?
        Distribution*           distribution;                                                       //!< Distribution (density functions, random draw function)
        double                  lnProb;                                                             //!< Current log probability
        double                  storedLnProb;                                                       //!< Stored log probability
};

#endif

