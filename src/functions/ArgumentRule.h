/**
 * @file
 * This file contains the declaration of ArgumentRule, which is
 * used to describe rules for arguments passed to functions (and
 * distributions).
 *
 * @brief Declaration of ArgumentRule
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

#ifndef ArgumentRule_H
#define ArgumentRule_H

#include <string>

class ArgumentRule {

    public:
                    ArgumentRule(const std::string& lbl, const std::string& t);      //!< Constructor 
                    ArgumentRule(const ArgumentRule& a);                         //!< Copy constructor 
            virtual ~ArgumentRule() { }                                          //!< Destructor 

        // Basic utility functions
        virtual std::string         briefInfo() const;                          //!< Brief info about object
        virtual ArgumentRule*       clone() const { return new ArgumentRule(*this); } //!< Clone object
        virtual bool                equals(const RbObject* obj) const;          //!< Equals comparison
        virtual void                print(std::ostream& o) const;               //!< Print complete object info
        virtual void                printValue(std::ostream& o) const;          //!< Print value (for user)

        // Regular functions
        std::string         getLabel() const { return label; }                  //!< Get label of argument
        std::string         getType() const { return type; }                    //!< Get type of argument
        virtual bool        isArgValid(const RbObject* val) const;              //!< Is val a valid argument?

    protected:
        std::string         label;          //!< Label of argument
        std::string         type;           //!< Type of argument
};

#endif


