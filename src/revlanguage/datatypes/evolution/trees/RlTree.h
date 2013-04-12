///**
// * @file
// * This file contains the declaration of a Tree, which is
// * the base class of all trees.
// *
// * @brief Declaration of Tree
// *
// * (c) Copyright 2009-
// * @date Last modified: $Date: $
// * @author The RevBayes Development Core Team
// * @license GPL version 3
// *
// * $Id$
// */
//
//#ifndef RlTree_H
//#define RlTree_H
//
//#include "Tree.h"
//#include "RlModelVariableWrapper.h"
//
//#include <set>
//#include <string>
//#include <vector>
//
//
//namespace RevLanguage {
//    
//    
//    class Tree : public RlModelVariableWrapper<RevBayesCore::Tree> {
//        
//    public:
//        Tree(void);                                                                                                                         //!< Constructor requires character type
//        Tree(RevBayesCore::Tree *v);                                                                                                    //!< Constructor requires character type
//        Tree(const RevBayesCore::Tree &v);                                                                                              //!< Constructor requires character type
//        Tree(RevBayesCore::TypedDagNode<RevBayesCore::Tree> *n);                                                                        //!< Constructor requires character type
//        Tree(const Tree& d);                                                                                                            //!< Constructor requires character type
//        
//        typedef RevBayesCore::Tree valueType;
//        
//        // Basic utility functions
//        Tree*                               clone(void) const;                                                                                  //!< Clone object
//        static const std::string&           getClassName(void);                                                                                 //!< Get class name
//        static const TypeSpec&              getClassTypeSpec(void);                                                                             //!< Get class type spec
//        const TypeSpec&                     getTypeSpec(void) const;                                                                            //!< Get language type of the object
//        
//        // Member method inits
//        const MethodTable&                  getMethods(void) const;                                             //!< Get methods
//        RbLanguageObject*                   executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions
//        
//    };
//    
//}
//
//#endif
