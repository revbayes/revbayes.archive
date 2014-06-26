//
//  RlContinuousCharacterData.h
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-27.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef __revbayes__RlContinuousCharacterData__
#define __revbayes__RlContinuousCharacterData__

#include <iostream>


#include "ContinuousCharacterData.h"
#include "RlModelVariableWrapper.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class ContinuousCharacterData : public RlModelVariableWrapper<RevBayesCore::ContinuousCharacterData> {
        
    public:
        ContinuousCharacterData(void);                                                          //!< Default constructor
        ContinuousCharacterData(const RevBayesCore::ContinuousCharacterData *d);                                                            //!< Construct from AminoAcid
        
        // Operators
        
        // Basic utility functions
        ContinuousCharacterData*                       clone(void) const;                                                      //!< Clone object
        RbLanguageObject*               convertTo(const TypeSpec& type) const;                                  //!< Convert to type
        static const std::string&       getClassName(void);                                                     //!< Get class name
        static const TypeSpec&          getClassTypeSpec(void);                                                 //!< Get class type spec
        const TypeSpec&                 getTypeSpec(void) const;                                                //!< Get language type of the object
        bool                            isConvertibleTo(const TypeSpec& type) const;                            //!< Is convertible to type?
        
    };
    
}

#endif /* defined(__revbayes__RlContinuousCharacterData__) */
