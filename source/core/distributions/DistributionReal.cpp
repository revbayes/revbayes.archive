/**
 * @file
 * This file contains the partial implementation of DistributionReal, the
 * abstract base class for distributions on real-valued variables.
 *
 * @brief Partial implementation of DistributionReal
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @interface DistributionReal
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "DAGNode.h"
#include "DistributionReal.h"
#include "MemberFunction.h"
#include "RbException.h"
#include "Real.h"
#include "RbNames.h"
#include "VectorString.h"

#include <cfloat>


/** Constructor passes member rules and method inits to base class */
DistributionReal::DistributionReal(const MemberRules& memberRules)
    : Distribution(memberRules) {
}


/** Map calls to member methods */
DAGNode* DistributionReal::executeOperation(const std::string& name, std::vector<VariableSlot>& args) {

    /* Manage all calls here, for simplicity, instead of relying on inheritance */
    if (name == "cdf") {
        retDouble.setValue(cdf(((Real*)(args[0].getValue()))->getValue()));
    }
    else if (name == "lnPdf") {
        retDouble.setValue(lnPdf(args[0].getValue()));
    }
    else if (name == "pdf") {
        retDouble.setValue(pdf(args[0].getValue()));
    }
    else if (name == "quantile") {
        retDouble.setValue(quantile(((Real*)(args[0].getValue()))->getValue()));
    }
    else if (name == "rv") {
        if (getVariableType() == RealPos_name)
            return new ConstantNode(rv());
        Real* randomVal = (Real*)(rv());
        retDouble.setValue(*randomVal);
        delete randomVal;
    }
    else
        throw RbException("No member method called '" + name + "'");

    return new ConstantNode(&retDouble);
}


/** Get class vector describing type of object */
const VectorString& DistributionReal::getClass(void) const {

    static VectorString rbClass = VectorString(DistributionReal_name) + Distribution::getClass();
    return rbClass;
}


/** Get max value of distribution */
const Real* DistributionReal::getMax(void) {

    static Real rbMax = Real(DBL_MAX);
    return &rbMax;
}


/** Get min value of distribution */
const Real* DistributionReal::getMin(void) {

    static Real rbMin = Real(DBL_MIN);
    return &rbMin;
}


/** Get method specifications */
const MethodTable& DistributionReal::getMethodInits(void) const {

    static MethodTable   methodInits;
    static ArgumentRules cdfArgRules;
    static ArgumentRules quantileArgRules;
    static bool          initsSet = false;

    if (!initsSet) {

        /* We inherit lnPdf, pdf and rv functions */
//        methodInits = Distribution::getMethodInits();
        
//        cdfArgRules.push_back(new ArgumentRule("q", RealPos_name));
        methodInits.addFunction("cdf", new MemberFunction(Real_name, cdfArgRules));

//        quantileArgRules.push_back(new ArgumentRule("p", RealPos_name));
        methodInits.addFunction("quantile", new MemberFunction(Real_name, quantileArgRules));

        initsSet = true;
    }

    return methodInits;
}


/** Get variable type */
const TypeSpec DistributionReal::getVariableType(void) const {
    
    return TypeSpec( Real_name );
}


