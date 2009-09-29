/**
 * @file
 * This file contains the declaration of RbObject, which is
 * the REvBayes abstract base class for all objects.
 *
 * @brief Declaration of RbObject
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 *
 * $Id$
 */

#ifndef RbObject_H
#define RbObject_H

#include <string>
#include <ostream>

/* Forward declaration */
class RbDumpState;

class RbObject {
    public:
        virtual ~RbObject();                              //! Virtual destructor because of virtual functions
        std::string getName(void) const; //!< Get name of object
        void setName(std::string& n); //!< Set name of object
        void setName(const char* n); //!< Set name of object
        virtual bool operator==(RbObject& o) const =0; //!< Comparison
        virtual std::string toString(void) const; //!< Information about object for debugging, printing etc
        virtual RbObject* clone() = 0;
        virtual void print(std::ostream& c) const; //!< Print this object for debugging, printing etc
        virtual void dump(std::ostream& c)=0; //!< Dump to ostream c
        virtual void resurrect(const RbDumpState& x)=0; //!< Resurrect from dumped state

    protected:
        RbObject() {
        } //!< Make it impossible to create objects
        RbObject(const char* n); //!< Constructor from type name (C style)
        RbObject(const std::string& n); //!< Constructor from type name (string)

        std::string name; //!< Name of object

    private:
};

#endif
