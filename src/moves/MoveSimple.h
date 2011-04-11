/**
 * @file
 * This file contains the declaration of MoveSimple, which is the abstract
 * base class for moves updating single stochastic dag nodes in mcmc
 * inference.
 *
 * @brief Declaration of MoveSimple
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#ifndef MoveSimple_H
#define MoveSimple_H

#include "Move.h"

#include <string>
#include <vector>

class RandomNumberGenerator;
class StochasticNode;
class VariableNode;
class VectorString;

class MoveSimple : public Move {

    public:
        virtual                    ~MoveSimple(void) {}                                                                     //!< Destructor

        // Basic utility functions
        virtual MoveSimple*         clone(void) const = 0;                                                                  //!< Clone the object
        virtual const VectorString& getClass(void) const;                                                                   //!< Get class vector

        // MoveSimple functions
        void                        acceptMove(void);				                                                        //!< Accept the move, update statistics
        void                        performMove(double& lnProbabilityRatio, double& lnHastingsRatio);                       //!< Call perform, calculate ratios
        void                        rejectMove(void);                                                                       //!< Reject the move
        void                        setNodePtr(StochasticNode* node) { nodePtr = node; }                                    //!< Set node ptr

	protected:
                                    MoveSimple(const MemberRules& memberRules);                                             //!< Parser constructor

        // Function you have to override
        virtual double              perform(std::set<StochasticNode*>& affectedNodes) = 0;                                  //!< Perform the move

        // Functions you may want to override for additional statistics purposes, e.g.
        virtual void                accept(void) {}                                                                         //!< Accept the move
        virtual void                reject(void) {}                                                                         //!< Reject the move

        // Member variable (hidden from parser)
        StochasticNode*             nodePtr;                                                                                //!< The node to change (ptr)
};

#endif

