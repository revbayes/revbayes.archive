/**
 * @file
 * This file contains the declaration of ArgRule, which is
 * used to describe rules for arguments passed to functions (and
 * distributions).
 *
 * @brief Declaration of ArgRule
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id$
 */

#ifndef ArgRule_H
#define ArgRule_H

#include <string>

#include "ObjectSlot.h"
#include "RbObject.h"

class ArgRule : public RbObject {

    public:

                    ArgRule(const std::string& label, ObjectSlot* slot);    //!< Constructor 
            virtual ~ArgRule() { delete objectSlot; }                       //!< Destructor 

        static const StringVector   rbClass;            //!< Static class attribute

        // Basic utility functions
        virtual std::string         briefInfo() const;                          //!< Brief info about object
        virtual ArgRule*            clone() const { return new ArgRule(*this); }//!< Clone object
        virtual bool                equals(const RbObject* obj) const;          //!< Equals comparison
        virtual const StringVector& getClass() const { return rbClass; }        //!< Get class
        virtual void                print(std::ostream& o) const;               //!< Print complete object info
        virtual void                printValue(std::ostream& o) const;          //!< Print value (for user)

        // Regular functions
        std::string         getLabel() const { return label; }                  //!< Get label of argument
        std::string         getType() const { return objectSlot->getType(); }   //!< Get type of argument
        const RbObject*     getValue() const { return objectSlot->getValue(); } //!< Get value of argument
        virtual bool        isArgValid(const RbObject* val) const;              //!< Is val a valid argument?

    protected:
        const std::string   label;          //!< Label of argument
        ObjectSlot*         objectSlot;     //!< Object slot of argument (type, value)
};

#endif


