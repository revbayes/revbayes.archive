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
#include "RbObject.h"

class ArgumentRule : public RbObject {

    public:
        static const StringVector   rbClass;            //!< Static class attribute

                    ArgumentRule(const std::string& lbl, const std::string& t);      //!< Constructor 
                    ArgumentRule(const std::string& lbl, const std::string& t, RbObject* dv);
                    ArgumentRule(const std::string& lbl, const std::string& t, RbObject* dv, DAGNode* mnv, DAGNode* mxv);
                    ArgumentRule(const ArgumentRule& a);                         //!< Copy constructor 
            virtual ~ArgumentRule() { }                                          //!< Destructor 

        // Basic utility functions
         std::string         briefInfo() const;                          //!< Brief info about object
         ArgumentRule*       clone() const { return new ArgumentRule(*this); } //!< Clone object
         bool                equals(const RbObject* obj) const;          //!< Equals comparison
         void                print(std::ostream& o) const;               //!< Print complete object info
         void                printValue(std::ostream& o) const;          //!< Print value (for user)
         const StringVector& getClass() const { return rbClass; }        //!< Get class
        const RbObject*             getDefaultValue(void) { return defaultValue; }
        const RbObject*             getMinValue(void) { return minValue; }
        const RbObject*             getMaxValue(void) { return maxValue; }

        // overloaded operators
        RbObject&           operator=(const RbObject& o);
        ArgumentRule&       operator=(const ArgumentRule& ar);

        // Regular functions
        std::string         getLabel() const { return label; }                  //!< Get label of argument
        std::string         getType() const { return type; }                    //!< Get type of argument
        virtual bool        isArgValid(const RbObject* val) const;              //!< Is val a valid argument?
        virtual std::string         toString(void) const;                                 //!< General info on object

    protected:
        std::string         label;          //!< Label of argument
        std::string         type;           //!< Type of argument
        RbObject*           defaultValue; 
        RbObject*           minValue;
        RbObject*           maxValue;
};

#endif


