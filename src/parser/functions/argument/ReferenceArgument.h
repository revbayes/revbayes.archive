///**
// * @file
// * This file contains the declaration of a constant Argument, which is
// * used to hold a potentially labeled argument passed to a
// * function. Function used in Models need constant arguments, i.e.
// * it is guaranteed that they wont change the arguments.
// *
// * @brief Declaration of ReferenceArgument
// *
// * (c) Copyright 2009-
// * @date Last modified: $Date: 2012-03-07 15:57:47 +0100 (Wed, 07 Mar 2012) $
// * @author The RevBayes Development Core Team
// * @license GPL version 3
// * @version 1.0
// * @since 2009-11-20, version 1.0
// *
// * $Id: Argument.h 1325 2012-03-07 14:57:47Z hoehna $
// */
//
//#ifndef ReferenceArgument_H
//#define ReferenceArgument_H
//
//#include "Argument.h"
//
//#include <ostream>
//#include <string>
//
//class ReferenceArgument : public Argument {
//    
//public:
//    ReferenceArgument(const RbPtr<Variable> &arg, const std::string& argLabel = "");                               //!< Constructor 
//    
//    // Basic utility functions
//    ReferenceArgument*                  clone(void) const;                                                          //!< Clone object
//    static const std::string&           getClassName(void);                                                         //!< Get class name
//    static const TypeSpec&              getClassTypeSpec(void);                                                     //!< Get class type spec
//    const TypeSpec&                     getTypeSpec(void) const;                                                    //!< Get language type of the object
//    void                                printValue(std::ostream& o) const;                                          //!< Complete info about object
//    
//    // Regular functions
//    const RbPtr<const Variable>&        getVariable(void) const;                                                    //!< Get the variable contained in this argument
//    const RbPtr<Variable>&              getReferenceVariable(void) const;
//    
//private:
//    RbPtr<Variable>                     var;                                                                        //!< Pointer to the variable slot containing the variable (and value)
//    
//};
//
//#endif
//
