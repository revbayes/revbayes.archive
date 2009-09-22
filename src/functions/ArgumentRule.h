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
#include "../datatypes/RbDataType.h"
#include "../main/RbObject.h"

using namespace std;

/*! This is the class used to describe labels and permissible types for arguments
 *  of functions and distributions, the latter being derived functions.
 */
class ArgumentRule : public RbObject {

    public:
            ArgumentRule() : dataType(""), defaultValue(NULL), label("") {} //!< Default constructor
            ArgumentRule (const string& key, const RbDataType& type, const RbDataType* defVal = NULL);  //!< Constructor for keyword=value arguments
            ArgumentRule (const string& key, const string& type, const RbDataType* defVal = NULL);  //!< Constructor for keyword=value arguments
            /*
            ArgumentRule (const string label, vector<string> options);  //!< Constructor for keyword=options argument
            ArgumentRule (const string label, const string datatype, const RbDataType *min, const RbDatatype *max, const int dim=1);    //!< constructor for argument with bounds
            */
            virtual ~ArgumentRule() { delete defaultValue; }    //!< Delete default value

//has a argument rule a datatype?!?        const string&   getDataType() const { return dataType; }        //!< Get data type
        RbDataType*     getDefaultValue() const { return defaultValue->copy(); }   //!< Get a default data type object
        const string&   getLabel() const { return label; }              //!< Get label
        bool            isValid(const string& x) const { return x == dataType; }  //!< Is type valid?

    protected:
//has a argument rule a datatype?!?        const string        dataType;       //!< Data type of argument
        const RbDataType*   defaultValue;   //!< Default value
        const string        label;          //!< Label of argument

        /*
        RbDataType*     minValue;       //!< Minimum value
        RbDataType*     maxValue;       //!< Maximum value
        */
};

/* Operator overloading */
bool operator!=(const ArgumentRule& A, const ArgumentRule& B);

#endif
