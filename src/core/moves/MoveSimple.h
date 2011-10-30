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

const std::string MoveSimple_name = "MoveSimple";

class MoveSimple : public Move {

public:
    virtual                            ~MoveSimple(void) {}                                             //!< Destructor

    // Basic utility functions
    virtual MoveSimple*                 clone(void) const = 0;                                          //!< Clone the object
    virtual const VectorString&         getClass(void) const;                                           //!< Get class vector

    // Member variable rules
    virtual RbPtr<const MemberRules>    getMemberRules(void) const;                                     //!< Get member rules

    // MoveSimple functions
    void                                acceptMove(void);				                                //!< Accept the move, update statistics
    double                              performMove(double& lnProbabilityRatio);                        //!< Call perform, calculate ratios
    void                                rejectMove(void);                                               //!< Reject the move

protected:
    MoveSimple(RbPtr<const MemberRules> memberRules);                                                   //!< Default constructor
    MoveSimple(const MoveSimple &ms);                                                                   //!< Copy constructor

    // Functions you have to override
    virtual double                      perform(std::set<RbPtr<StochasticNode> >& affectedNodes) = 0;   //!< Perform the move and fill in the affected stochastic nodes for probability calculations
    virtual const TypeSpec              getVariableType(void) const = 0;                                //!< Get type of random variable appropriate for move

    // Functions you may want to override for additional statistics purposes, e.g.
    virtual void                        accept(void) {}                                                 //!< Accept the move
    virtual void                        reject(void) {}                                                 //!< Reject the move
};

#endif

