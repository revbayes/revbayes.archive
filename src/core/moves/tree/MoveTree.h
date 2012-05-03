/**
 * @file
 * This file contains the declaration of MoveTree, which is the abstract
 * base class for moves updating trees during mcmc inference.
 *
 * @brief Declaration of MoveTree
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

#ifndef MoveTree_H
#define MoveTree_H

#include "Move.h"
#include "Topology.h"

#include <string>
#include <vector>

class Container;
class RandomNumberGenerator;
class StochasticNode;


class MoveTree : public Move {

    public:
        virtual                        ~MoveTree(void) {}                                                       //!< Destructor

        // Basic utility functions
        virtual MoveTree*                   clone(void) const = 0;                                                  //!< Clone the object
        static const std::string&           getClassName(void);                                                                 //!< Get class name
        static const TypeSpec&              getClassTypeSpec(void);                                                             //!< Get class type spec

        // Member variable rules
        virtual const MemberRules&          getMemberRules(void) const;                                             //!< Get member rules

        // MoveTree functions
        void                                acceptMove(void);				                                        //!< Accept the move, update statistics
        double                              performMove(double& lnProbabilityRatio);                                //!< Call perform, calculate ratios
        void                                rejectMove(void);                                                       //!< Reject the move

	protected:
                                            MoveTree(const MemberRules& memberRules);                               //!< Parser constructor

        // Function you have to override
//        virtual double                  perform(std::set<StochasticNode*>&      movedNodes,
//                                                std::set<StochasticNode*>&      affectedNodes,
//                                                std::vector<TopologyChange>&    topologyChanges) = 0;           //!< Perform the move

        // Functions you may want to override for additional statistics purposes, e.g.
        virtual void                        accept(void) {}                                                         //!< Accept the move
        virtual void                        reject(void) {}                                                         //!< Reject the move

        // Help functions
        const Topology&                     getTopology(void) const;                                                //!< Get topology

        // parameters
        RbVariablePtr                       tree;
};

#endif
