/**
 * @file
 * This file contains the declaration of RbObjectWrapper, which is
 * the RevBayes abstract base class for objects that wrap language
 * objects, namely DAG nodes and DAG node containers.
 *
 * @brief Declaration of RbObjectWrapper
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-11-18 01:05:57 +0100 (Ons, 18 Nov 2009) $
 * @author The REvBayes development core team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 *
 * $Id: RbObject.h 63 2009-11-18 00:05:57Z ronquist $
 */

#ifndef RbObjectWrapper_H
#define RbObjectWrapper_H

#include <string>
#include <ostream>

class RbObject;
class StringVector;
class IntVector;

class RbObjectWrapper {

    public:
		virtual						    ~RbObjectWrapper(void) {}                       //! Virtual destructor

        // Basic utility functions you HAVE TO override
        virtual RbObjectWrapper*        clone(void) const = 0;                          //!< Clone object
        virtual const StringVector&     getClass(void) const;                           //!< Get class vector
        virtual const RbObject*         getValue(void) const = 0;                       //!< Get value
        virtual void                    printStruct(std::ostream& o) const;             //!< Print struct for user
        virtual void                    printValue(std::ostream& o) const = 0;          //!< Print value for user

        // Basic utility functions you should not overrride
        bool                            isType(const std::string& type) const;          //!< Is wrapper of type?

    protected:
		                                RbObjectWrapper(void) {}                //!< No objects of this class
};

#endif
