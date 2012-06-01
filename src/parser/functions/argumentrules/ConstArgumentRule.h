///**
// * @file
// * This file contains the declaration of ConstArgumentRule, which is
// * the base class for objects used to describe rules for
// * ConstArguments passed to functions.
// *
// * @brief Declaration of ConstArgumentRule
// *
// * (c) Copyright 2009-
// * @date Last modified: $Date: 2012-04-16 17:27:08 -0700 (Mon, 16 Apr 2012) $
// * @author The RevBayes Development Core Team
// * @license GPL version 3
// * @version 1.0
// * @since 2009-11-20, version 1.0
// *
// * $Id: ConstArgumentRule.h 1378 2012-04-17 00:27:08Z hoehna $
// */
//
//#ifndef ConstArgumentRule_H
//#define ConstArgumentRule_H
//
//#include "ArgumentRule.h"
//
//#include <string>
//
//class ConstArgumentRule : public ArgumentRule {
//    
//public:
//    ConstArgumentRule(const std::string& argName, const RbPtr<RbLanguageObject> &defVal);                                           //!< Constructor of rule from default value
//    ConstArgumentRule(const std::string& argName, const TypeSpec& argTypeSp);                                                       //!< Constructor of rule without default value
//    ConstArgumentRule(const std::string& argName, const TypeSpec& argTypeSp, const RbPtr<RbLanguageObject> &defVal);                //!< Constructor of rule with default value
//    
//    // Basic utility functions
//    virtual ConstArgumentRule*  clone(void) const;                                                                                  //!< Clone object
//    static const std::string&   getClassName(void);                                                                                 //!< Get class name
//    static const TypeSpec&      getClassTypeSpec(void);                                                                             //!< Get class type spec
//    virtual const TypeSpec&     getTypeSpec(void) const;                                                                            //!< Get language type of the object
//    virtual void                printValue(std::ostream& o) const;                                                                  //!< Print value for user
//        
//    // functions overwritten in derived class
//    virtual const Variable&     getDefaultVariable(void) const;                                                                     //!< Get default ConstArgument
//    
//protected:
//    Variable                    defaultVar;
//};
//
//#endif
