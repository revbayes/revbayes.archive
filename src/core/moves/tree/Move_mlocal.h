/**
 * @file
 * This file contains the declaration of Move_mlocal, which changes
 * a topology and associated branch lengths according to the LOCAL
 * mechanism.
 *
 * @reference Larget B, Simon DL. 1999. Markov chain Monte Carlo algorithms
 *            for the Bayesian analysis of phylogenetic trees. Molecular Biology
 *            and Evolution 16:750-759.
 *
 * @brief Declaration of Move_mlocal
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

#ifndef Move_mlocal_H
#define Move_mlocal_H

#include <ostream>
#include <set>
#include <string>

#include "MoveTree.h"
#include "RandomNumberFactory.h"



class Move_mlocal : public MoveTree {

    public:
                                    Move_mlocal(void);                                                      //!< Parser constructor

        // Basic utility functions
        Move_mlocal*                clone(void) const;                                                      //!< Clone object
        static const std::string&   getClassName(void);                                                                 //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                                             //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                                //!< Get language type of the object

        // Member variable inits
        const MemberRules&          getMemberRules(void) const;                                             //!< Get member rules

        // Move function
        std::vector<StochasticNode*>&       getDagNodes(void) { static std::vector<StochasticNode*> x; return x; }            //!< Get the nodes vector
    void                                replaceDagNodes(std::vector<StochasticNode*> &n) {}                                   //!< Set the nodes vector

    protected:
//        double                      perform(std::set<StochasticNode*>&      movedNodes,
//                                            std::set<StochasticNode*>&      affectedNodes,
//                                            std::vector<TopologyChange>&    topologyChanges);               //!< Perform the move
    
};

#endif

