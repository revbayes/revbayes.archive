/**
 * @file
 * This file contains the implementation of RbFunction_c, the
 * c() function.
 *
 * @brief Implementation of RbFunction_c
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author Fredrik Ronquist and the REvBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-26, version 1.0
 *
 * $Id$
 */

#include "RbFunction_c.h"
#include "RbTypeConverter.h"
#include "SymbolTable.h"
#include "SyntaxElement.h"


/* No argument rules needed */

/** Add to symbol table */
static bool fxn_c = SymbolTable::globalTable().add("c", new RbFunction_c());

/** Default constructor, allocate workspace (NULL because data type is not known) */
RbFunction_c::RbFunction_c()
    : RbStandardFxn(), resultVec(NULL) {
}

/** Copy constructor (set result vector to NULL because data type not necessarily known) */
RbFunction_c::RbFunction_c(const RbFunction_c& s)
    : RbStandardFxn(s), resultVec(NULL) {
}

/** Destructor, delete workspace */
RbFunction_c::~RbFunction_c() {

    delete resultVec;
}

/** Return copy */
RbFunction_c* RbFunction_c::copy() const {

    return new RbFunction_c(*this);
}

/** Execute function */
RbDataType* RbFunction_c::execute(void) {

    if (!resultVec)
        resultVec = RbTypeConverter::makeCommonType(arguments);
    else if (resultVec->getType() != RbTypeConverter::getCommonType(arguments))
        resultVec = RbTypeConverter::makeCommonType(arguments);

    resultVec->erase();
    for (vector<SyntaxElement*>::iterator i=arguments.begin(); i!=arguments.end(); i++) {
        resultVec->addElements(*((*i)->getValue()));
    }
    return resultVec;
}

/** Get data type for type checking */
const std::string& RbFunction_c::getDataType() const {

    if ( arguments.empty() )
        return RbAbstractDataType::dataType;
    else
        return resultVec->getType();
}

/** Set and check arguments */
bool RbFunction_c::setArguments(vector<SyntaxLabeledExpr*> args) {

    /* check that all arguments lack a label */
    for (std::vector<SyntaxLabeledExpr*>::iterator i=args.begin(); i!= args.end(); i++) {
        if ( (*i)->getLabel() != "" ) {
            std::cerr << "Not expecting labeled arguments." << std::endl;
            return false;
        }
        if ( args.size() == 0 ) {
            std::cerr <<  "Expecting at least one argument." << std::endl;
            return false;
        }
    }

    /* set arguments */
    for (std::vector<SyntaxLabeledExpr*>::iterator i=args.begin(); i!=args.end(); i++) {
        SyntaxElement *expr = (*i)->getExpression();
        arguments.push_back(expr);
    }

    /* set vector of correct data type to hold results */
    resultVec = RbTypeConverter::makeCommonType(arguments);
    if (!resultVec) {
        arguments.clear();
        std::cerr << "Arguments cannot be coerced to the same type" << std::endl;
        return false;
    }

    /* Success */
    return true;
}

/** Print some info */
void RbFunction_c::print(std::ostream& c) const {

    c << "RbFunction_c: args=";
    for (std::vector<SyntaxElement*>::const_iterator i=arguments.begin(); i!=arguments.end(); i++)
        (*i)->print(c);
    c << " -- result=";
    resultVec->print(c);
    c << std::endl;
}
