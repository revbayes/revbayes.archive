/**
 * @file
 * This file contains the declaration of RbComplex, which is
 * the RevBayes abstract base class for complex objects.
 *
 * @brief Declaration of RbComplex
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-11-18 01:05:57 +0100 (Ons, 18 Nov 2009) $
 * @author The REvBayes development core team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 * @extends RbObject
 *
 * $Id: RbComplex.h 63 2009-11-18 00:05:57Z ronquist $
 */

#ifndef RbComplex_H
#define RbComplex_H

#include "RbObject.h"

#include <ostream>
#include <string>
#include <vector>

class Argument;
class IntVector;
class MethodTable;
class RbObjectWrapper;
class StringVector;

class RbComplex : public RbObject {

    public:
		virtual                         ~RbComplex(void) {}                                                     //!< Virtual destructor 

        // Basic utility functions you have to override
        virtual RbObject*               clone(void) const = 0;                                                  //!< Clone object
        virtual bool                    equals(const RbObject* obj) const = 0;                                  //!< Equals comparison
		virtual const StringVector&     getClass(void) const;                                                   //!< Get class vector
		virtual void                    printValue(std::ostream& o) const = 0;                                  //!< Print value for user
		virtual std::string             toString(void) const = 0;                                               //!< Complete info 

        // Member variable functions: override if object contains member variables
        virtual const std::vector<std::string>& getMembers(void) const;                                         //!< Get member names
        virtual const RbObject*         getValue(const std::string& name) const;                                //!< Get member value
        virtual const RbObjectWrapper*  getVariable(const std::string& name) const;                             //!< Get member variable
        virtual void                    setValue(const std::string& name, RbObject* val);                       //!< Set member value
        virtual void                    setVariable(const std::string& name, RbObjectWrapper* var);             //!< Set member variable

        // Member method functions: override if object contains member functions
        virtual const RbObject*         executeMethod(int funcId);                                              //!< Execute function with preprocessed args
        virtual const RbObject*         executeMethod(const std::string& name, std::vector<Argument>& args);    //!< Execute function
        virtual const MethodTable&      getMethodTable(void) const;                                             //!< Get method descriptions
        virtual int                     setArguments(const std::string& name, std::vector<Argument>& args);     //!< Set arguments and get funcId back 

		// Element access functions: override if object contains elements
        virtual int                     getDim(void) { return 0; }                                              //!< Get subscript dimensions
        virtual const StringVector&     getAtomicClass(void) { return getClass() ; }                            //!< Get atomic (element) class
        virtual const RbObject*         getElement(const IntVector& index) const;                               //!< Get element (read-only)
        virtual const IntVector&        getElementLength(void) const;                                           //!< Get length in each dim
        virtual void                    resize(const IntVector& len);                                           //!< Resize
        virtual void                    setElement(const IntVector& index, RbObject* val);                      //!< Set element
        virtual void                    setElementLength(const IntVector& len);                                 //!< Get length in each dim

    protected:
                                        RbComplex(void) : RbObject() {}                                         //!< No objects of this class

        // Override these functions to provide friend classes with modify access to members or elements
        virtual RbObject*               getMemberRef(const std::string& name);                                  //!< Allow modify access to member
        virtual RbObject*               getElementRef(const IntVector& index);                                  //!< Allow modify access to element 

        // These are friends that can try to modify members or elements
        friend class                    SyntaxVariable;                                                         //!< The parser class dealing with variables 
};

#endif
