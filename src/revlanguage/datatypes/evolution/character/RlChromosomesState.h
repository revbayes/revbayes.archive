/*
 *  RlChromosomesState.h
 *  revbayes-proj
 *
 *  Created by Will Freyman on 9/8/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef RlChromosomesState_H
#define RlChromosomesState_H

#include "ChromosomesState.h"
#include "ModelObject.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class ChromosomesState : public ModelObject<RevBayesCore::ChromosomesState> {
        
    public:
        ChromosomesState(void);                                                                                 //!< Default constructor
        ChromosomesState(const RevBayesCore::ChromosomesState &d);                                              //!< Construct from ChromosomesState
        
        // Operators
        
        // Basic utility functions
        ChromosomesState*               clone(void) const;                                                      //!< Clone object
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