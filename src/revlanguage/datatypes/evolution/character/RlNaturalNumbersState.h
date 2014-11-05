/*
 *  RlNaturalNumbersState.h
 *  revbayes-proj
 *
 *  Created by Will Freyman on 9/8/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef RlNaturalNumbersState_H
#define RlNaturalNumbersState_H

#include "NaturalNumbersState.h"
#include "ModelObject.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class NaturalNumbersState : public ModelObject<RevBayesCore::NaturalNumbersState> {
        
    public:
        NaturalNumbersState(void);                                                                                 //!< Default constructor
        NaturalNumbersState(RevBayesCore::TypedDagNode<RevBayesCore::NaturalNumbersState> *v);                                                            //!< Constructor from DAG node
        NaturalNumbersState(const RevBayesCore::NaturalNumbersState &d);                                              //!< Construct from NaturalNumbersState
        
        // Operators
        
        // Basic utility functions
        NaturalNumbersState*               clone(void) const;                                                      //!< Clone object
        RevObject*                      convertTo(const TypeSpec& type) const;                                  //!< Convert to type
        static const std::string&       getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&          getClassTypeSpec(void);                                                 //!< Get class type spec
        const TypeSpec&                 getTypeSpec(void) const;                                                //!< Get language type of the object
        bool                            isConvertibleTo(const TypeSpec& type, bool once) const;                 //!< Is convertible to type?
        
        // Member methods
        const MethodTable&              getMethods(void) const;                                                 //!< Get member methods
        
    };
    
}

#endif