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

class List;
class RandomNumberGenerator;
class StochasticNode;
class VectorString;


class MoveTree : public Move {

    public:
        virtual                        ~MoveTree(void) {}                                                       //!< Destructor

        // Basic utility functions
        virtual MoveTree*               clone(void) const = 0;                                                  //!< Clone the object
        virtual const VectorString&     getClass(void) const;                                                   //!< Get class vector

        // Member variable rules
        virtual const MemberRules&      getMemberRules(void) const;                                             //!< Get member rules

        // MoveTree functions
        void                            acceptMove(void);				                                        //!< Accept the move, update statistics
        double                          performMove(double& lnProbabilityRatio);                                //!< Call perform, calculate ratios
        void                            rejectMove(void);                                                       //!< Reject the move

	protected:
                                        MoveTree(const MemberRules& memberRules);                               //!< Parser constructor

        // Function you have to override
        virtual double                  perform(std::set<StochasticNode*>&      movedNodes,
                                                std::set<StochasticNode*>&      affectedNodes,
                                                std::vector<TopologyChange>&    topologyChanges) = 0;           //!< Perform the move

        // Functions you may want to override for additional statistics purposes, e.g.
        virtual void                    accept(void) {}                                                         //!< Accept the move
        virtual void                    reject(void) {}                                                         //!< Reject the move

        // Help functions
        const Topology*                 getTopology(void) const;                                                //!< Get topology
        std::vector<StochasticNode*>    getTreeVariable(const std::string& name) const;                         //!< Get tree variable
        void                            reconnectTreeVariables(List* treeVars, std::vector<TopologyChange>& topChanges);        //!< Reconnect tree variables

        // Member variables storing info about the move
        std::set<StochasticNode*>       moved;
        std::vector<TopologyChange>     topChanges;
};

#endif
