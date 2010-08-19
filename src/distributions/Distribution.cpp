/**
 * @file
 * This file contains the partial implementation of Distribution, the atbstract
 * base class for distributions in RevBayes.
 *
 * @brief Partial implementation of Distribution
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @interface Distribution
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "DAGNode.h"
#include "Distribution.h"
#include "MemberFunction.h"
#include "RandomNumberGenerator.h"
#include "RbDouble.h"
#include "RbException.h"
#include "RbNames.h"
#include "VectorString.h"
#include "Workspace.h"
#include "WrapperRule.h"


/** Constructor with inheritance for member rules but not for method inits */
Distribution::Distribution(const  MemberRules& memberRules)
    : MemberObject(memberRules, getMethodInits()), retObject(NULL) {
}


/** Constructor with inheritance both for member rules and method inits */
Distribution::Distribution(const  MemberRules& memberRules, const MethodTable& methodInits)
    : MemberObject(memberRules, methodInits), retObject(NULL) {
}


/** Copy constructor */
Distribution::Distribution(const Distribution& x)
    : MemberObject(x), retObject(NULL) {
}


/** Destructor */
Distribution::~Distribution(void) {

    if (retObject != NULL)
        delete retObject;
}


/** Assignment operator */
Distribution& Distribution::operator=(const Distribution& x) {

    /* Do not copy retObject; keep our own workspace */
    return (*this);
}


/** Map calls to member methods */
const RbObject* Distribution::executeOperation(const std::string& name, std::vector<DAGNode*>& args) {

    if (name == "lnPdf") {
        retDouble.setValue(lnPdf(args[0]->getValue()));
    }
    else if (name == "pdf") {
        retDouble.setValue(pdf(args[0]->getValue()));
    }
    else if (name == "rv") {
        delete retObject;
        retObject = rv();
        return retObject;
    }
    else
        throw RbException("No member method called '" + name + "'");

    return &retDouble;
}


/** Get class vector describing type of object */
const VectorString& Distribution::getClass(void) const {

    static VectorString rbClass = VectorString(Distribution_name) + MemberObject::getClass();
    return rbClass;
}


/** Get member rules */
const MemberRules& Distribution::getMemberRules(void) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if (!rulesSet) {

        memberRules.push_back(new WrapperRule("rng", Workspace::globalWorkspace().getVariable("_rng")));
        rulesSet = true;
    }

    return memberRules;
}


/** Get method specifications */
const MethodTable& Distribution::getMethodInits(void) const {

    static MethodTable      methodInits;
    static ArgumentRules    lnPdfArgRules;
    static ArgumentRules    pdfArgRules;
    static ArgumentRules    rvArgRules;
    static bool             initsSet = false;

    if (!initsSet) {

        lnPdfArgRules.push_back(new ArgumentRule("x", RbObject_name));
        methodInits.addFunction("lnPdf", new MemberFunction(RbDouble_name, lnPdfArgRules));

        pdfArgRules.push_back(new ArgumentRule("x", RbObject_name));
        methodInits.addFunction("pdf",   new MemberFunction(RbDouble_name, pdfArgRules));

        methodInits.addFunction("rv",    new MemberFunction(RbObject_name, ArgumentRules()));

        initsSet = true;
    }

    return methodInits;
}

