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
		virtual						    ~RbObjectWrapper(void) {}                                   //! Virtual destructor

        // Functions you have to override
        virtual RbObjectWrapper*        clone(void) const = 0;                                      //!< Clone object
        virtual const StringVector&     getAtomicClass(void) const = 0;                             //!< Get atomic class vector
        virtual const StringVector&     getClass(void) const;                                       //!< Get class vector
        virtual int                     getDim(void) const = 0;                                     //!< Get dim (value + wrap)
        virtual const RbObject*         getValue(void) const = 0;                                   //!< Get value
        virtual const RbObject*         getValElement(const IntVector& index) const = 0;            //!< Get value element
        virtual void                    printStruct(std::ostream& o) const;                         //!< Print struct for user
        virtual void                    printValue(std::ostream& o) const = 0;                      //!< Print value for user
        virtual void                    setElement(const IntVector& index, RbObject* val);          //!< Set element value

        // Functions you may want to overrride
        virtual const RbObjectWrapper*  getVarElement(const IntVector& index) const;                //!< Get variable element
        virtual int                     getWrapperDim(void) { return 0; }                           //!< Get wrapper dim
        virtual void                    setElement(const IntVector& index, RbObjectWrapper* var);   //!< Set variable element

        // Functions you should not overrride
        bool                            isType(const std::string& type) const;                      //!< Is wrapper of type?

    protected:
		                                RbObjectWrapper(void) {}                                    //!< No objects of this class
};

#endif
