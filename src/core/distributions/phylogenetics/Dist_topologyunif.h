/**
 * @file
 * This file contains the declaration of Dist_topologyunif, which holds
 * the parameters and functions related to a uniform distribution on
 * topologies (unrooted or rooted, binary or polytomous).
 *
 * @brief Declaration of Dist_topologyunif
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-14 12:43:32 +0100 (Mån, 14 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id: MemberObject.h 194 2009-12-14 11:43:32Z ronquist $
 */

#ifndef Dist_topologyunif_H
#define Dist_topologyunif_H

#include "DistributionDiscrete.h"
#include "Topology.h"

#include <ostream>
#include <string>

class VectorString;

class Dist_topologyunif: public DistributionDiscrete {

    public:
                                    Dist_topologyunif(void);                                            //!< Parser constructor
                                    Dist_topologyunif(int numTaxa, bool isRooted, bool isBinary=true);  //!< Constructor for internal use

        // Basic utility functions
        Dist_topologyunif*          clone(void) const;                                                  //!< Clone object
        const VectorString&         getClass(void) const;                                               //!< Get class vector

        // Member variable setup
        const MemberRules&          getMemberRules(void) const;                                         //!< Get member variable rules
        void                        setVariable(const std::string& name, DAGNode* var);                 //!< Set member variable (ensure topologyProb is updated)

        // Discrete distribution functions
        Move*                       getDefaultMove(StochasticNode* node);                               //!< Get default move
        int                         getNumStates(void) const;                                           //!< Get number of states
        virtual const Simplex*      getProbabilityMassVector(void);                                     //!< Get probability mass vector
        const TypeSpec              getVariableType(void) const;                                        //!< Get random variable type (Simplex)
        double                      lnPdf(const RbObject* value);                                       //!< Ln probability density
        double                      pdf(const RbObject* value);                                         //!< Probability density
        Topology*                   rv(void);                                                           //!< Generate random variable

    private:
        void                        calculateTopologyProb(void);                                        //!< Calculate and set probability member variables

        double                      topologyProb;                                                       //!< The probability of a topology
        double                      lnTopologyProb;                                                     //!< The log probability of a topology
};

#endif

