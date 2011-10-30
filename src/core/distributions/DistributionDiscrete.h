/**
 * @file
 * This file contains the declaration of DistributionDiscrete, the
 * abstract base class for distributions on discrete variables.
 *
 * @brief Declaration of DistributionDiscrete
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-29 23:23:09 +0100 (Tis, 29 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: DistributionDiscrete.h 211 2009-12-29 22:23:09Z ronquist $
 */

#ifndef DistributionDiscrete_H
#define DistributionDiscrete_H

#include "Distribution.h"
#include "Real.h"

#include <set>
#include <string>

class ArgumentRule;
class Move;
class Simplex;
class StochasticNode;
class VectorString;


const std::string DistributionDiscrete_name = "Distribution on Discrete Random Variable";

class DistributionDiscrete: public Distribution {

    public:
	    virtual                            ~DistributionDiscrete(void) {}                                               //!< Destructor

        // Basic utility function
        virtual DistributionDiscrete*       clone(void) const = 0;                                                      //!< Clone object
        virtual const VectorString&         getClass(void) const;                                                       //!< Get class vector   

        // Member object function you have to override
        virtual RbPtr<const MemberRules>    getMemberRules(void) const = 0;                                             //!< Get member rules

        // Member object functions you should not have to override
        RbPtr<const MethodTable>            getMethods(void) const;                                                     //!< Get member methods
        RbPtr<RbLanguageObject>             executeOperation(const std::string& name, const RbPtr<Environment>& args);  //!< Direct call of member method

        // Categorical distribution functions you have to override
        virtual size_t                      getNumberOfStates(void) const = 0;                                          //!< Get number of states
        virtual RbPtr<Simplex>              getProbabilityMassVector(void) = 0;                                         //!< Get probability mass vector
        virtual const TypeSpec&             getVariableType(void) const = 0;                                            //!< Get random variable type spec
        virtual double                      lnPdf( RbPtr<const RbLanguageObject> value) const = 0;                      //!< Ln probability density
        virtual double                      pdf( RbPtr<const RbLanguageObject> value) const = 0;                        //!< Probability density
        virtual RbPtr<RbLanguageObject>     rv(void) = 0;                                                               //!< Generate a random draw

    protected:
                                            DistributionDiscrete( RbPtr<const MemberRules> memberRules);                 //!< Constructor

};

#endif

