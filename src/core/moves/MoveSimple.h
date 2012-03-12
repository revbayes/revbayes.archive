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
    virtual                            ~MoveSimple(void) {}                                             //!< Destructor

    // Basic utility functions
    virtual MoveSimple*                 clone(void) const = 0;                                          //!< Clone the object
    static const std::string&           getClassName(void);                                             //!< Get class name
    static const TypeSpec&              getClassTypeSpec(void);                                         //!< Get class type spec

    // Member variable rules
    virtual const MemberRules&          getMemberRules(void) const;                                     //!< Get member rules
    virtual void                        setMemberVariable(const std::string& name, const Variable* var);//!< Set member variable (ensure topologyProb is updated)

    // Move functions
    void                                acceptMove(void);				                                //!< Accept the move, update statistics
    std::vector<StochasticNode*>&       getDagNodes(void);                                              //!< Get the nodes vector
    double                              performMove(double& probRatio);                                 //!< Call perform, calculate ratios
    void                                rejectMove(void);                                               //!< Reject the move
    void                                replaceDagNodes(std::vector<StochasticNode*> &n);                                   //!< Set the nodes vector

protected:
    MoveSimple(const MemberRules& memberRules);                                                         //!< Default constructor

    // Functions you have to override
    virtual double                      perform(void) = 0;                                              //!< Perform the move and fill in the affected stochastic nodes for probability calculations
    virtual const TypeSpec              getVariableType(void) const = 0;                                //!< Get type of random variable appropriate for move

    // Functions you may want to override for e.g., additional statistics purposes
    virtual void                        accept(void) {}                                                 //!< Accept the move
    virtual void                        reject(void) {}                                                 //!< Reject the move


    // member variables
    RbVariablePtr                       node;
    
private:
    std::vector<StochasticNode*>        nodes;

};

#endif

