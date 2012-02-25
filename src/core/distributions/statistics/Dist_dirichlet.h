/**
 * @file
 * This file contains the declaration of Dist_dirichlet, which is used to hold
 * parameters and functions related to a Dirichlet distribution.
 *
 * @brief Declaration of Dist_dirichlet
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

#ifndef Dist_dirichlet_H
#define Dist_dirichlet_H

#include "DistributionContinuous.h"
#include "Simplex.h"

#include <ostream>
#include <string>

class DAGNode;
class Real;
class Simplex;


class Dist_dirichlet: public DistributionContinuous {

    public:
                                    Dist_dirichlet(void);                                               //!< Parser constructor

        // Basic utility functions
        Dist_dirichlet*             clone(void) const;                                                  //!< Clone object
        static const std::string&   getClassName(void);                                                 //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                             //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                            //!< Get language type of the object

        // Member variable setup
        const MemberRules&          getMemberRules(void) const;                                         //!< Get member variable rules
        void                        setMemberVariable(const std::string& name, Variable* var);          //!< Catching the setting of the member variables.

        // Real-valued distribution functions
        double                      cdf(const RbLanguageObject& value);                           //!< Cumulative density
        const TypeSpec&             getVariableType(void) const;                                        //!< Get random variable type (Simplex)
        double                      lnPdf(const RbLanguageObject& value) const;                         //!< Ln probability density
        double                      pdf(const RbLanguageObject& value) const;                           //!< Probability density
        const Real&                 quantile(const double p);                                           //!< Quantile
        const RbLanguageObject&     rv(void);                                                           //!< Generate random variable
    
    private:

        // parameters
        RbVariablePtr               alpha;

        // memberfunction return variables
        Simplex                     randomVariable;

};

#endif

