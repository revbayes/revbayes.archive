/**
 * @file
 * This file contains the declaration of Func_plate, which 
 * calculates the plate of a set of numbers.
 *
 * @brief Declaration of Func_plate
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-03-10 12:55:11 +0100 (Sat, 10 Mar 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func_plate.h 1327 2012-03-10 11:55:11Z hoehna $
 */

#ifndef Func_plate_H
#define Func_plate_H

#include "Environment.h"
#include "RbFunction.h"

#include <map>
#include <string>
#include <vector>

class Container;


class Func_plate :  public RbFunction {
    
public:
    Func_plate(void);                                                                       //!< default constructor
    
    // Basic utility functions
    Func_plate*                 clone(void) const;                                          //!< Clone the object
    static const std::string&   getClassName(void);                                         //!< Get class name
    static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
    const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object
    
    // Regular functions
    const RbLanguageObject&     execute(void);                                              //!< Execute function
    const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
    const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value
    
private:
    
    void                        fillPlate(const std::string &c, const std::vector<const Container *> &i, size_t l);

    // attributes
    Environment                 plate;
    
};

#endif

