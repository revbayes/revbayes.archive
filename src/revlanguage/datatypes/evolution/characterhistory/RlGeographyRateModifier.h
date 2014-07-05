//
//  RlGeographyRateModifier.h
//  rb_mlandis
//
//  Created by Michael Landis on 7/4/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef RlGeographyRateModifier_H
#define RlGeographyRateModifier_H

#include "ModelObject.h"
#include "GeographicDistanceRateModifier.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class RlGeographyRateModifier : public ModelObject<RevBayesCore::GeographicDistanceRateModifier> {
        
    public:
        
        RlGeographyRateModifier(void);                                                                                  //!< Default constructor
        RlGeographyRateModifier(RevBayesCore::GeographicDistanceRateModifier *m);                                       //!< Default constructor
        RlGeographyRateModifier(RevBayesCore::TypedDagNode<RevBayesCore::GeographicDistanceRateModifier> *d);           //!< Default constructor
        
        // Basic utility functions
        RlGeographyRateModifier*            clone(void) const;                                                          //!< Clone object
        static const std::string&           getClassName(void);                                                         //!< Get class name
        static const TypeSpec&              getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                                    //!< Get language type of the object
        
        // Member method inits
        const MethodTable&                  getMethods(void) const;                                                     //!< Get methods
        RevObject*                          executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions
    };
    
}
#endif /* defined(RlGeographyRateModifier_H) */
