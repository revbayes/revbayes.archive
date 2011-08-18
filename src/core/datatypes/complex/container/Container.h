/**
 * @file
 * This file contains the declaration of Container, which is
 * a generic container of RbObject objects.
 *
 * @brief Declaration of Container
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-05, version 1.0
 *
 * $Id$
 */

#ifndef Container_H
#define Container_H

#include "ConstantMemberObject.h"
#include "ContainerIterator.h"
#include "Natural.h"

#include <ostream>
#include <string>
#include <vector>

class RbObject;
class VectorIndex;
class VectorInteger;
class VectorString;


/**
 * This class is used for all complex data types seen by the parser
 * as being multidimensional. 
 *
 */
const std::string Container_name = "container";

class Container : public ConstantMemberObject {

    public:
        virtual                        ~Container(void) {}                                                  //!< Virtual destructor

        // Basic utility functions you have to override (also getClass()!)
        virtual Container*              clone(void) const = 0;                                              //!< Clone object
        virtual const VectorString&     getClass(void) const;                                               //!< Get class
        virtual void                    printValue(std::ostream& o) const = 0;                              //!< Print value for user
        virtual std::string             richInfo(void) const = 0;                                           //!< Complete info about object

        // Container functions and basic utility functions you should not have to override
        ContainerIterator               begin(void) const;                                                  //!< Begin iterator
        ContainerIterator               end(void) const;                                                    //!< End iterator
    
        bool                            empty(void) { return length == 0; }                                 //!< Test whether the container is empty
        const std::string&              getElementType(void) const { return elementType; }                  //!< Get element type
//        size_t                          getIndexOfName(const std::string& s) const;                         //!< Get index corresponding to name s
        size_t                          getLength(void) const { return length; }                            //!< Get length of the container
        virtual bool                    supportsIndex(void) const { return true; }                          //!< Does object support index operator?

        // Container functions you have to override
        virtual void                    clear(void) = 0;                                                    //!< Clear
        virtual void                    resize(const size_t len) = 0;                                       //!< Resize
        virtual RbObject*               getElement(size_t index) = 0;                                       //!< Get element or subcontainer (not constant because of potential resize)

protected:
                                        Container(const TypeSpec& typeSpec);                                //!< Default constructor (set type of elements of container)
                                        Container(const TypeSpec& typeSpec, const MemberRules& memberRules);//!< Default constructor (set type of elements of container)

        // Help functions you do not have to override
        Container&                      operator=(const Container& x);                                      //!< Assignment operator checks type and dimension (but length can differ)

//        size_t                          getOffset(const Natural& index) const;                              //!< Get offset in elements vector


        // Member variables
        const std::string               elementType;                                                        //!< Type of elements
        size_t                          length;                                                             //!< Length in each dimension (size_t because this is natural (STL) type for sizes)
//        std::vector<std::string>        names;                                                              //!< Names for string indexing
};  

#endif

