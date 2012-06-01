///**
// * @file
// * This file contains the declaration of a constant Argument, which is
// * used to hold a potentially labeled argument passed to a
// * function. Function used in Models need constant arguments, i.e.
// * it is guaranteed that they wont change the arguments.
// *
// * @brief Declaration of ConstArgument
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
//#ifndef ConstArgument_H
//#define ConstArgument_H
//
//#include "Argument.h"
//
//#include <ostream>
//#include <string>
//
//class ConstArgument : public Argument {
//    
//public:
//    ConstArgument(const RbPtr<const Variable> &arg, const std::string& argLabel = "");                              //!< Constructor 
//    
//    // Basic utility functions
//    ConstArgument*                      clone(void) const;                                                          //!< Clone object
//    static const std::string&           getClassName(void);                                                         //!< Get class name
//    static const TypeSpec&              getClassTypeSpec(void);                                                     //!< Get class type spec
//    const TypeSpec&                     getTypeSpec(void) const;                                                    //!< Get language type of the object
//    void                                printValue(std::ostream& o) const;                                          //!< Complete info about object
//    
//    // Regular functions
//    const RbPtr<const Variable>&        getVariable(void) const;                                                    //!< Get the variable contained in this argument
//    
//private:
//    RbPtr<const Variable>               var;                                                                        //!< Pointer to the variable slot containing the variable (and value)
//    
//};
//
//#endif
//
