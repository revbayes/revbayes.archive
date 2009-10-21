/*!
 * \file
 * This file contains the declaration of ArgumentRule, which is
 * used to describe rules for arguments passed to functions (and
 * distributions).
 *
 * \brief Declaration of ArgumentRule
 *
 * (c) Copyright 2009-
 * \date Last modified: $Date$
 * \author The REvBayes core team
 * \license GPL version 3.0
 *
 * $Id$
 */

#ifndef ArgumentRule_H
#define ArgumentRule_H

#include <string>
#include "RbDataType.h"
#include "RbTypeInfo.h"
#include "RbObject.h"

/*! This is the class used to describe labels and permissible types for arguments
 *  of functions and distributions, the latter being derived functions.
 */
class ArgumentRule : public RbObject {

    public:
        ArgumentRule(const std::string& key, const RbTypeInfo& type, const RbDataType& defVal = NULL);  //!< Constructor for keyword=value arguments
        ArgumentRule(const ArgumentRule& ar);
//        ArgumentRule (const std::string& key, const std::string& type, const RbDataType* defVal = NULL);  //!< Constructor for keyword=value arguments
        /*
        ArgumentRule (const string label, vector<string> options);  //!< Constructor for keyword=options argument
        ArgumentRule (const string label, const string datatype, const RbDataType *min, const RbDatatype *max, const int dim=1);    //!< constructor for argument with bounds
        */
        virtual ~ArgumentRule();                //!< Delete default value

        // implemented abstract/virtual functions from base classes
        RbObject*           clone(void) const ;                                 //!< clone this object
        void                print(std::ostream &c) const;                       //!< Print the value to ostream c
        void                dump(std::ostream& c);                              //!< Dump to ostream c
        void                resurrect(const RbDumpState& x);                    //!< Resurrect from dumped state
        bool                operator==(const RbObject& o) const;                //!< Comparison
        bool                operator==(const ArgumentRule& o) const;            //!< Comparison

        const RbTypeInfo&   getReturnType() const ;                             //!< Get data type
        RbDataType&         getDefaultValue() const ;                           //!< Get a default data type object
        const std::string&  getLabel() const ;                                  //!< Get label
        bool                isTypeValid(const RbTypeInfo& x) const ;            //!< Is type valid?

    protected:
        const RbTypeInfo&   returnType;     //!< Data type of argument
        const RbDataType&   defaultValue;   //!< Default value
        const std::string   label;          //!< Label of argument

        /*
        RbDataType*     minValue;       //!< Minimum value
        RbDataType*     maxValue;       //!< Maximum value
        */
};

/* Operator overloading */
bool operator!=(const ArgumentRule& A, const ArgumentRule& B);

#endif
