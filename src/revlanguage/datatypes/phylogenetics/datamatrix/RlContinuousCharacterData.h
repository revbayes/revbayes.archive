//
//  RlContinuousCharacterData.h
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-27.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef RlContinuousCharacterData_H
#define RlContinuousCharacterData_H

#include <iostream>


#include "ContinuousCharacterData.h"
#include "ModelObject.h"
#include "RlHomologousCharacterData.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class ContinuousCharacterData : public ModelObject<RevBayesCore::ContinuousCharacterData>, HomologousCharacterData {
        
    public:
        ContinuousCharacterData(void);                                                                          //!< Default constructor
        ContinuousCharacterData(const RevBayesCore::ContinuousCharacterData &d);                                //!< Constructor from core data type (need to create my own copy)
        ContinuousCharacterData(RevBayesCore::ContinuousCharacterData *d);                                      //!< Constructor from new core data type pointer
        ContinuousCharacterData(RevBayesCore::TypedDagNode<RevBayesCore::ContinuousCharacterData>*d);           //!< Constructor with DAG node
        
        // Basic utility functions
        ContinuousCharacterData*        clone(void) const;                                                      //!< Clone object
        ContinuousCharacterData*        concatenate(const RevObject& d) const;                                  //!< Concatenate two sequences
        ContinuousCharacterData*        concatenate(const ContinuousCharacterData& d) const;                    //!< Concatenate two sequences
        static const std::string&       getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&          getClassTypeSpec(void);                                                 //!< Get class type spec
        const TypeSpec&                 getTypeSpec(void) const;                                                //!< Get language type of the object

        RevPtr<RevVariable>             executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found);     //!< Execute member method

    };
    
}

#endif /* defined(__revbayes__RlContinuousCharacterData__) */
