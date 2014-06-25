/**
 * @file
 * This file contains the declaration of the RevLanguage wrapper of the MCMC class.
 *
 * @brief Declaration of RlMcmc
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-08-06 20:14:22 +0200 (Mon, 06 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-09-06, version 1.0
 * @extends RbObject
 *
 * $Id: Real.h 1746 2012-08-06 18:14:22Z hoehna $
 */

#ifndef RlMcmc_H
#define RlMcmc_H

#include "Mcmc.h"
#include "RlControlVariableWrapper.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class Mcmc : public RlControlVariableWrapper<RevBayesCore::Mcmc> {
        
    public:
        
        Mcmc(void);                                                                                                        //!< Default constructor
        
        // Basic utility functions
        virtual Mcmc*                               clone(void) const;                                                      //!< Clone object
        void                                        constructInternalObject(void);                                          //!< We construct the a new internal MCMC object.
        static const std::string&                   getClassName(void);                                                     //!< Get class name
        static const TypeSpec&                      getClassTypeSpec(void);                                                 //!< Get class type spec
        const MemberRules&                          getMemberRules(void) const;                                             //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
        // Member method inits
        const MethodTable&                          getMethods(void) const;                                             //!< Get methods
        RbLanguageObject*                           executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions

    protected:
        
        void                                        setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var);              //!< Set member variable
        
        RbPtr<const Variable>                       model;
        RbPtr<const Variable>                       moves;
        RbPtr<const Variable>                       monitors;
        RbPtr<const Variable>                       moveSchedule;
        
    };
    
}

#endif
