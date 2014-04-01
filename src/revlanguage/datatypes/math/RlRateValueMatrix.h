//
//  RlRateValueMatrix.h
//  rb_mlandis
//
//  Created by Michael Landis on 3/30/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__RlRateValueMatrix__
#define __rb_mlandis__RlRateValueMatrix__

#include "RateValueMatrix.h"
#include "RlModelVariableWrapper.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class RateValueMatrix : public RlModelVariableWrapper<RevBayesCore::RateValueMatrix> {
        
    public:
        
        RateValueMatrix(void);                                                                                                        //!< Default constructor
        RateValueMatrix(RevBayesCore::RateValueMatrix *m);                                                                                                        //!< Default constructor
        RateValueMatrix(RevBayesCore::TypedDagNode<RevBayesCore::RateValueMatrix> *d);                                                                                                        //!< Default constructor
        
        // Basic utility functions
        RateValueMatrix*                         clone(void) const;                                                                                  //!< Clone object
        static const std::string&           getClassName(void);                                                                                 //!< Get class name
        static const TypeSpec&              getClassTypeSpec(void);                                                                             //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                                                            //!< Get language type of the object
        
        // Member method inits
        const MethodTable&                  getMethods(void) const;                                             //!< Get methods
        RbLanguageObject*                   executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions
        
        
    };
    
}


#endif /* defined(__rb_mlandis__RlRateValueMatrix__) */
