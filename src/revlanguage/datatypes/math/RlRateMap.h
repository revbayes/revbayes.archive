/**
 * @file
 * This file contains the declaration of the RevLanguage wrapper of the RateMap class.
 *
 * @brief Declaration of RlRateMap
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-08-06 20:14:22 +0200 (Mon, 06 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id: Real.h 1746 2012-08-06 18:14:22Z hoehna $
 */

#ifndef RlRateMap_H
#define RlRateMap_H

#include "ModelObject.h"
#include "RateMap.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class RateMap : public ModelObject<RevBayesCore::RateMap> {
        
    public:
        
        RateMap(void);                                                                                                              //!< Default constructor
        RateMap(RevBayesCore::RateMap *m);                                                                                          //!< Construct from RC::RateMap
        RateMap(RevBayesCore::TypedDagNode<RevBayesCore::RateMap> *d);                                                              //!< Construct from DAG node
        
        // Basic utility functions
        RateMap*                            clone(void) const;                                                                      //!< Clone object
        static const std::string&           getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&              getClassTypeSpec(void);                                                                 //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                                                //!< Get language type of the object
        
        // Member method functions
        virtual RevPtr<Variable>            executeMethod(const std::string& name, const std::vector<Argument>& args, bool &f);     //!< Map member methods to internal functions
        
    };
    
}

#endif
