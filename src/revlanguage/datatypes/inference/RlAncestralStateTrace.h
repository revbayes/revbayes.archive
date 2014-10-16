/*
 *  RlAncestralStateTrace.h
 *  revbayes-proj
 *
 *  Created by mucho mass on 10/16/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
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
        
        AncestralStateTrace(void);                                                                                                        //!< Default constructor
        AncestralStateTrace(const RevBayesCore::AncestralStateTrace& x);                                                                 //!< Copy constructor
        
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
