/**
 * @file
 * This file contains the declaration of Container, a container type
 * that acts as a base class for all constant Containers.
 *
 * @brief Declaration of Container
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 *
 * $Id$
 */

#ifndef Container_H
#define Container_H

#include "RevObject.h"
#include "TypeSpec.h"

#include <iostream>
#include <vector>

namespace RevLanguage {

    class Container : public RevObject {
    
    public:
    
        // Basic utility functions you have to override
        virtual RevObject*                          convertTo(const TypeSpec& type) const;                              //!< Convert to type
        virtual Container*                          clone(void) const = 0;                                              //!< Clone object
        static const std::string&                   getClassName(void);                                                 //!< Get class name
        static const TypeSpec&                      getClassTypeSpec(void);                                             //!< Get class type spec
        virtual bool                                isConvertibleTo(const TypeSpec& type) const;                        //!< Is convertible to type?
        virtual void                                printStructure(std::ostream& o) const;                              //!< Print structure of language object for user
        virtual void                                printValue(std::ostream& o) const = 0;                              //!< Print value for user
    
        // Container functions you have to override
        virtual size_t                              size(void) const = 0;                                               //!< get the number of elements in the Container
    
        virtual void                                clear(void) = 0;                                                    //!< Clear
        virtual RevObject*                          getElement(size_t index) = 0;                                       //!< Get element (non-const to return non-const element)
    
        // Member method inits
        virtual const MethodTable&                  getMethods(void) const;                                             //!< Get methods
        virtual RevObject*                          executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions

        // Container functions you do not have to override
        const TypeSpec&                             getElementType(void) const { return elementType; }                  //!< Get element type
        bool                                        empty(void) const { return size() == 0; }                           //!< Test whether the container is empty
    
    protected:
        Container(const TypeSpec &elemType);                                                                    //!< Set type spec of container from type of elements
        Container(const TypeSpec &elemType, const MemberRules& memberRules);                                    //!< Set type spec of container from type of elements
    
        Container&                                  operator=(const Container& x);                                          //!< Assignment operator

        // Member variables
        const TypeSpec                              elementType;                                                            //!< Type of elements

    };

}
    
#endif

