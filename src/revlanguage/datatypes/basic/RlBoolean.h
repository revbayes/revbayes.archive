/**
 * @file
 * This file contains the declaration of RlBoolean, which is
 * a RevBayes wrapper around a regular bool.
 *
 * @brief Declaration of RlBoolean
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id$
 */

#ifndef RlBoolean_H
#define RlBoolean_H

#include "ModelObject.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {

class RlBoolean : public ModelObject<bool> {

    public:
        RlBoolean(void);                                                                                        //!< Default constructor
        RlBoolean(RevBayesCore::TypedDagNode<bool> *v);                                                         //!< Constructor from DAG node
        RlBoolean(bool v);                                                                                      //!< Construct from bool

        // Operators

        // Basic utility functions
        RlBoolean*                      clone(void) const;                                                      //!< Clone object
        RevObject*                      convertTo(const TypeSpec& type) const;                                  //!< Convert to type
        static const std::string&       getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&          getClassTypeSpec(void);                                                 //!< Get class type spec
        const TypeSpec&                 getTypeSpec(void) const;                                                //!< Get language type of the object
        double                          isConvertibleTo(const TypeSpec& type, bool once) const;                 //!< Is convertible to type?
        void                            printValue(std::ostream& o) const;                                      //!< Print value (for user)
    
    };
    
}

#endif

