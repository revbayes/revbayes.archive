//
//  Func_readTSVBitsetData.h
//  revbayes-proj
//
//  Created by Michael Landis on 3/4/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#ifndef __revbayes_proj__Func_readTSVBitsetData__
#define __revbayes_proj__Func_readTSVBitsetData__


#include "Procedure.h"
#include "RbFileManager.h"

#include <map>
#include <string>
#include <vector>


namespace RevLanguage {
    
    class Func_readTSVBitsetData :  public Procedure {
        
    public:
        // Basic utility functions
        Func_readTSVBitsetData*     clone(void) const;                                                      //!< Clone the object
        static const std::string&   getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&      getClassTypeSpec(void);                                                 //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                                //!< Get language type of the object
        
        // Regular functions
        RevPtr<RevVariable>         execute(void);                                                          //!< Execute function
        const ArgumentRules&        getArgumentRules(void) const;                                           //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                              //!< Get type of return value
        
    private:
        std::string                 bitToState(std::string s);
        
    };
    
}

#endif /* defined(__revbayes_proj__Func_readTSVBitsetData__) */
