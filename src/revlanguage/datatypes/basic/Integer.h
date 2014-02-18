/**
 * @file
 * This file contains the declaration of Integer, which is
 * a RevBayes wrapper around a regular int.
 *
 * @brief Declaration of Integer
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

#ifndef Integer_H
#define Integer_H

#include "RlModelVariableWrapper.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class Real;

    class Integer : public RlModelVariableWrapper<int> {

    public:
        Integer(void);                                                                                          //!< Default constructor
        Integer(RevBayesCore::TypedDagNode<int> *v);                                                            //!< Constructor from DAG node
        Integer(int v);                                                                                         //!< Constructor from int
        Integer(unsigned int v);                                                                                //!< Constructor from unsigned int

        // Basic operator functions
        virtual RbLanguageObject*       add(const RbLanguageObject &rhs) const;                                 //!< Addition operator used for example in '+=' statements
        Integer*                        add(const Integer &rhs) const;                                             //!< Addition operator used for example in '+=' statements
        Real*                           add(const Real &rhs) const;                                             //!< Addition operator used for example in '+=' statements

        // Basic utility functions
        virtual Integer*                clone(void) const;                                                      //!< Clone object
        virtual RbLanguageObject*       convertTo(const TypeSpec& type) const;                                  //!< Convert to type
        static const std::string&       getClassName(void);                                                     //!< Get class name
        static const TypeSpec&          getClassTypeSpec(void);                                                 //!< Get class type spec
        virtual const TypeSpec&         getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual bool                    isConvertibleTo(const TypeSpec& type) const;                            //!< Is convertible to type?
    
    
    };
    
}

#endif

