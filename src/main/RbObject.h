/**
 * @file
 * This file contains the declaration of RbObject, which is
 * the RevBayes abstract base class for all objects.
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

/* Forward declarations */
// class RbDumpState;
class RbList;

class RbObject {

    public:
            virtual ~RbObject();            //! Virtual destructor because of virtual functions

        static const RbList     rbClass;                //!< Static class attribute

        virtual RbObject*       clone() const = 0;              //!< Clone object
        virtual const RbList*   getClass() const;               //!< Get class
        virtual bool            operator==(const RbObject& o) const = 0;   //!< Equals operator
        virtual void            print(std::ostream& o) const;   //!< Print the object

        //TODO I am commenting these out for now, they need to be implemented later -- Fredrik
     // virtual void        dump(std::ostream& o) = 0;              //!< Dump to ostream c
     // virtual void        resurrect(const RbDumpState& x) = 0;    //!< Resurrect from dumped state

        //TODO I do not use these at the moment but I keep them to please others -- Fredrik
        std::string         getName(void) const;        //!< Get name of object
        void                setName(std::string& n);    //!< Set name of object
        void                setName(const char* n);     //!< Set name of object
        virtual std::string toString(void) const;       //!< General info about object

    protected:
            RbObject() {}               //!< Make it impossible to create objects

            //TODO I do not use these at the moment but keep them to please others -- Fredrik
            RbObject(const char* n); //!< Constructor from type name (C style)
            RbObject(const std::string& n); //!< Constructor from type name (string)

        //TODO I do not use this at the moment but keep it to please others -- Fredrik
        std::string     name; //!< Name of object
};

#endif
