/**
 * @file
 * This file contains the implementation of some common functin-
 * ality in Distribution, which is the interface used to hold
 * distribution functions in the symbol table.
 *
 * @brief Implementation of Distribution (some functions)
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-28, version 1.0
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "Distribution.h"

using namespace std;

/** Destructor: free completeArgRules */
Distribution::~Distribution() {

    delete[] completeArgRules;
}

/** Get result depending on distribution type */
RbDataType* Distribution::execute() {

    if ( functionType == d ) {
        if ( useLogs )
            return lnPdf();
        else
            return pdf();
    }
    else if ( functionType == r )
        return rv();

    return NULL;
}

/** Set complete argument rules depending on distribution type */
ArgumentRule* Distribution::getArgRules() {

    /* No processing needed if a tilde distribution */
    if ( functionType == tilde_d )
        return getTemplateArgRules();

    /* Return if complete argument rules already set */
    if ( completeArgRules )
        return completeArgRules;

    /* Get template argument rules */
    ArgumentRule* templateArgRules = getTemplateArgRules();

    /* Calculate number of argument rules */
    int numTemplateArgRules = 0;
    while (templateArgRules[numTemplateArgRules]!=ArgumentRule())
        numTemplateArgRules++;
    if ( functionType == d )
        numArgRules = numTemplateArgRules + 2;
    else if ( functionType == p )
        numArgRules = numTemplateArgRules + 3;
    else if ( functionType == q)
        numArgRules = numTemplateArgRules + 3;
    else if ( functionType == r )
        numArgRules = numTemplateArgRules + 1;

    /* Allocate space for complete argument rules */
    completeArgRules = new ArgumentRule[numArgumentRules+1];

    /* Add first argument rule */
    if ( functionType == d || functionType == p )
        completeArgRules[0] = ArgumentRule(NULL, getDataType());
    else if ( functionType == q )
        completeArgRules[0] = ArgumentRule(NULL, RbVector());
    else if ( functionType == r )
        completeArgRules[0] = ArgumentRule(NULL, RbInt());

    /* Add template argument rules */
    for (int i=0; i<numTemplateArgRules; i++)
        completeArgRules[i+1] = templateArgRules[i];

    /* Add trailing argument rules */
    if ( functionType == d )
        completeArgRules[numArgRules-1] = ArgumentRule("log", RbBool());
    else if ( functionType == p || functionType == q ) {
        completeArgRules[numArgRules-2] = ArgumentRule("lower.quantile", RbBool());
        completeArgRules[numArgRules-1] = ArgumentRule("log.p", RbBool());
    }

    /* Return complete argument rules */
    return completeArgRules;
}

/** Set type but only allow d, r and tilde_d */
void Distribution::setType(Distribution::functionT type) {

    if ( type != Distribution::p && type != Distribution::q )
        functionType = type;
}

