/**
 * @file
 * This file contains the declaration of Dist_topologyunif, which holds
 * the parameters and functions related to a uniform distribution on
 * topologies (unrooted or rooted, binary or polytomous).
 *
 * @brief Declaration of Dist_topologyunif
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id$
 */

#ifndef Dist_topologyunif_H
#define Dist_topologyunif_H

#include "DistributionDiscrete.h"
#include "Topology.h"

#include <ostream>
#include <string>

class VectorString;
class TopologyNode;

const std::string Dist_topologyunif_name = "Dist_topologyunif";

class Dist_topologyunif: public DistributionDiscrete {

    public:
                                        Dist_topologyunif(void);                                            //!< Parser constructor

        // Basic utility functions
        Dist_topologyunif*              clone(void) const;                                                  //!< Clone object
        const VectorString&             getClass(void) const;                                               //!< Get class vector
        const TypeSpec&                 getTypeSpec(void) const;                                                //!< Get language type of the object
    
        // Member variable setup
        RbPtr<const MemberRules>        getMemberRules(void) const;                                         //!< Get member variable rules
        void                            setMemberVariable(const std::string& name, RbPtr<Variable> var);    //!< Set member variable (ensure topologyProb is updated)

        // Discrete distribution functions
        size_t                          getNumberOfStates(void) const;                                      //!< Get number of states
        virtual RbPtr<Simplex>          getProbabilityMassVector(void);                                     //!< Get probability mass vector
        const TypeSpec&                 getVariableType(void) const;                                        //!< Get random variable type (Simplex)
        double                          lnPdf(const RbLanguageObject *value) const;                   //!< Ln probability density
        double                          pdf(const RbLanguageObject *value) const;                     //!< Probability density
        RbPtr<RbLanguageObject>         rv(void);                                                           //!< Generate random variable

    private:
        void                            buildRandomBinaryTree(std::vector<RbPtr<TopologyNode> >& tips, size_t numTaxa);    //!< Build a random binary tree recursively until we have numTaxa tips
        void                            calculateTopologyProb(void);                                        //!< Calculate and set probability member variables
        void                            calculateNumberOfStates(void);                                      //!< Calculate and set number of states member variables

        double                          topologyProb;                                                       //!< The probability of a topology
        double                          lnTopologyProb;                                                     //!< The log probability of a topology
        size_t                          numberOfStates;                                                     //!< The number of states for this topology distribution
    
        static const TypeSpec           typeSpec;
        static const TypeSpec           varTypeSpec;
};

#endif

