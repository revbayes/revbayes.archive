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
#include "ModelObject.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class ContinuousCharacterData : public ModelObject<RevBayesCore::ContinuousCharacterData> {
        
    public:
        ContinuousCharacterData(void);                                                                          //!< Default constructor
        ContinuousCharacterData(const RevBayesCore::ContinuousCharacterData &d);                                //!< Constructor from core data type (need to create my own copy)
        ContinuousCharacterData(RevBayesCore::ContinuousCharacterData *d);                                      //!< Constructor from new core data type pointer
        ContinuousCharacterData(RevBayesCore::TypedDagNode<RevBayesCore::ContinuousCharacterData>*d);           //!< Constructor with DAG node

        // Operators
        
        // Basic utility functions
        ContinuousCharacterData*        add(const RevObject& d) const;                                          //!< Addition operator used for example in '+=' statements
        ContinuousCharacterData*        add(const ContinuousCharacterData& d) const;                            //!< Addition operator used for example in '+=' statements

        ContinuousCharacterData*        clone(void) const;                                                      //!< Clone object
        RevObject*                      convertTo(const TypeSpec& type) const;                                  //!< Convert to type
        static const std::string&       getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&          getClassTypeSpec(void);                                                 //!< Get class type spec
        const TypeSpec&                 getTypeSpec(void) const;                                                //!< Get language type of the object
        bool                            isConvertibleTo(const TypeSpec& type, bool once) const;                 //!< Is convertible to type?
        
    };
    
}

#endif /* defined(__revbayes__RlContinuousCharacterData__) */
