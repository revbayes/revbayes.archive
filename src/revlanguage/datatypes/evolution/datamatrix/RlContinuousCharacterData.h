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
#include "RlAbstractCharacterData.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class ContinuousCharacterData : public AbstractCharacterData  { //ModelObject<RevBayesCore::ContinuousCharacterData> {
        
    public:
        ContinuousCharacterData(void);                                                                          //!< Default constructor
        ContinuousCharacterData( RevBayesCore::ContinuousCharacterData *d);                                //!< Constructor 
        ContinuousCharacterData( RevBayesCore::TypedDagNode<RevBayesCore::AbstractCharacterData> *d);                                //!< Constructor requires character type

        typedef RevBayesCore::AbstractCharacterData valueType;
        
        // Basic utility functions
        ContinuousCharacterData*              add(const RevObject& d) const;                                                          //!< Addition operator used for example in '+=' statements
        ContinuousCharacterData*              add(const ContinuousCharacterData& d) const;                                              //!< Addition operator used for example in '+=' statements

        ContinuousCharacterData*        clone(void) const;                                                      //!< Clone object
        RevObject*                      convertTo(const TypeSpec& type) const;                                  //!< Convert to type
        static const std::string&       getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&          getClassTypeSpec(void);                                                 //!< Get class type spec
        const TypeSpec&                 getTypeSpec(void) const;                                                //!< Get language type of the object
        bool                            isConvertibleTo(const TypeSpec& type, bool once) const;                 //!< Is convertible to type?
        
        const RevBayesCore::ContinuousCharacterData& getValue( void ) const;                                  //!< Get value as base class reference
        
        // Member method functions
        const MethodTable&              getMethods(void) const;                                                 //!< Get member methods
        
    };
    
}

#endif /* defined(__revbayes__RlContinuousCharacterData__) */
