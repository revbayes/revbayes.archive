/**
 * @file
 * This file contains the declaration of the RevLanguage wrapper of the base class of all monitors.
 *
 * @brief Declaration of RlMonitor
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

#ifndef RlMonitor_H
#define RlMonitor_H

#include "Monitor.h"
#include "TypedDagNode.h"
#include "WorkspaceToCoreWrapperObject.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class Monitor : public WorkspaceToCoreWrapperObject<RevBayesCore::Monitor> {
        
    public:
        
        Monitor(void);                                                                                                      //!< Default constructor (0.0)
        Monitor(RevBayesCore::Monitor *m);                                                                                  //!< constructor with internal value
        Monitor(const Monitor& x);                                                                                          //!< Copy constructor
        
        // Overloaded operators
        
        // Basic utility functions
        virtual Monitor*                            clone(void) const = 0;                                                  //!< Clone object
        static const std::string&                   getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                 //!< Get class type spec
                
    };
    
}

#endif
