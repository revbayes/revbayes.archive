/*
 * RbFunction_GTR.h
 *
 *  Created on: 23 dec 2009
 *      Author: Sebastian
 */

#ifndef RBFUNCTION_GTR_H_
#define RBFUNCTION_GTR_H_


#include "ArgumentRule.h"
#include "RbDouble.h"
#include "RbFunction.h"
#include "RbObject.h"
#include <iostream>
#include <string>
#include <vector>

class RbDumpState;
class DAGNode;

class RbFunction_GTR : public RbFunction {
    public:
        RbFunction_GTR();
        RbFunction_GTR(const RbFunction_GTR& gtr);
        virtual ~RbFunction_GTR();


        RbObject*                   clone(void) const ;                                             //!< clone this object
        const StringVector&         getClass(void) const;                                           //!< Get class
        std::string                 toString(void) const;                                           //!< General info on object

        const RbObject*             executeOperation(const std::vector<RbObjectWrapper*>& arguments) const; //!< Get result
        const ArgumentRule**        getArgumentRules(void) const;                                   //!< Get the number of argument rules
        const std::string           getReturnType(void) const;

};

#endif /* RBFUNCTION_GTR_H_ */
