/**
 * @file
 * This file contains the declaration of RevLanguage wrapper of the AncestralStateTrace class.
 *
 * @brief Declaration of RlAncestralStateTrace
 *
 * (c) Copyright 2014-
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: RlAncestralStateTrace.h $
 */


#ifndef RlAncestralStateTrace_H
#define RlAncestralStateTrace_H

#include "AncestralStateTrace.h"
#include "TypedDagNode.h"
#include "WorkspaceObject.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    
    class AncestralStateTrace : public WorkspaceObject<RevBayesCore::AncestralStateTrace> {
        
    public:
        
        AncestralStateTrace(void);                                                                                              //!< Default constructor
        AncestralStateTrace(const RevBayesCore::AncestralStateTrace& x);														//!< Copy constructor                                                                                           //!< Copy constructor
        AncestralStateTrace(const AncestralStateTrace& x);
		
        // Basic utility functions
        virtual AncestralStateTrace*                          clone(void) const;                                                          //!< Clone object
        void                                        constructInternalObject(void);                                              //!< We construct the a new internal MCMC object.
        static const std::string&                   getClassType(void);                                                         //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                     //!< Get class type spec
        const MemberRules&                          getMemberRules(void) const;                                                 //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                    //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                          //!< Print value (for user)
        
        // Member method inits
        const MethodTable&                          getMethods(void) const;                                                     //!< Get methods
        RevPtr<Variable>                            executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions
        
    protected:
        
        void                                        setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
		
    };
    
}



#endif
