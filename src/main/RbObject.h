/**
 * @file
 * This file contains the declaration of RbObject, which is
 * the REvBayes abstrct base class for all objects.
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

class RbObject{

    public:
        virtual ~RbObject() {}  //! Virtual destructor because of virtual functions

        virtual RbObject*   clone() const = 0;                      //!< Make copy
	    virtual void        dump(std::ostream& c) const = 0;        //!< Dump to ostream c
        const std::string&  getName() const;                        //!< Get name of object
	    virtual bool        operator==(RbObject& o) const = 0;     //!< Comparison
        virtual void        print(std::ostream& c) const;           //!< Print this object
	    virtual void        resurrect(const RbDumpState& x) = 0;    //!< Resurrect from dumped state
        void                setName(std::string& n);                //!< Set name of object
        virtual std::string toString() const;                       //!< Information about object

    protected:
	        RbObject() {}   //!< Make it impossible to create objects

    private:
	    std::string name;   //!< Name of object

};

#endif
