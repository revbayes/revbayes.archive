//
//  Func_structureDispersalExtinction.h
//  revbayes-proj
//
//  Created by Michael Landis on 3/3/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#ifndef __revbayes_proj__Func_structureDispersalExtinction__
#define __revbayes_proj__Func_structureDispersalExtinction__


#include "RlTypedFunction.h"
#include "RbVector.h"
#include "ModelVector.h"
#include "RealPos.h"

#include <map>
#include <string>
#include <vector>

namespace RevLanguage {
    
    class Func_structureDispersalExtinction :  public TypedFunction<ModelVector<RealPos> > {
        
    public:
        Func_structureDispersalExtinction( void );
        
        // Basic utility functions
        Func_structureDispersalExtinction*                              clone(void) const;                                                              //!< Clone the object
        static const std::string&                                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevBayesCore::TypedFunction< RevBayesCore::RbVector<double> >*  createFunction(void) const;                                                                  //!< Execute function
        const ArgumentRules&                                            getArgumentRules(void) const;                                                   //!< Get argument rules
        
    };
    
}

#endif /* defined(__revbayes_proj__Func_structureDispersalExtinction__) */
