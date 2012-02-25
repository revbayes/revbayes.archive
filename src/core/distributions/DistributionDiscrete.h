/**
 * @file
 * This file contains the declaration of DistributionDiscrete, the
 * abstract base class for distributions on discrete variables.
 *
 * @brief Declaration of DistributionDiscrete
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef DistributionDiscrete_H
#define DistributionDiscrete_H

#include "Distribution.h"
#include "Natural.h"
#include "Simplex.h"
#include "Real.h"

#include <set>
#include <string>

class ArgumentRule;
class Move;
class Simplex;
class StochasticNode;
class VectorString;


class DistributionDiscrete: public Distribution {

    public:
	    virtual                            ~DistributionDiscrete(void) {}                                                       //!< Destructor

        // Basic utility function
        virtual DistributionDiscrete*       clone(void) const = 0;                                                              //!< Clone object
        static const std::string&           getClassName(void);                                                                 //!< Get class name
        static const TypeSpec&              getClassTypeSpec(void);                                                             //!< Get class type spec

        // Member object function you have to override
        virtual const MemberRules&          getMemberRules(void) const = 0;                                                     //!< Get member rules

        // Member object functions you should not have to override
        const MethodTable&                  getMethods(void) const;                                                             //!< Get member methods

        // Categorical distribution functions you have to override
        virtual size_t                      getNumberOfStates(void) const = 0;                                                  //!< Get number of states
        virtual const Simplex&              getProbabilityMassVector(void) = 0;                                                 //!< Get probability mass vector
        virtual const TypeSpec&             getVariableType(void) const = 0;                                                    //!< Get random variable type spec
        virtual double                      lnPdf( const RbLanguageObject& value) const = 0;                                    //!< Ln probability density
        virtual double                      pdf( const RbLanguageObject& value) const = 0;                                      //!< Probability density
        virtual const RbLanguageObject&     rv(void) = 0;                                                                       //!< Generate a random draw

    protected:
                                            DistributionDiscrete( const MemberRules& memberRules);                              //!< Constructor
        const RbLanguageObject&             executeOperationSimple(const std::string& name, const std::vector<Argument>& args); //!< Direct call of member method

    
        // member function return values
        Simplex                             probMassVector;
        Natural                             numStates;

};

#endif

