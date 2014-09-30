/**
 * @file
 * This file contains the declaration of a Tree.
 *
 * @brief Declaration of Tree
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef RlTree_H
#define RlTree_H

#include "Tree.h"
#include "ModelObject.h"

#include <set>
#include <string>
#include <vector>


namespace RevLanguage {
    
    
    class Tree : public ModelObject<RevBayesCore::Tree> {
        
    public:
        Tree(void);                                                                                                                         //!< Constructor requires character type
        Tree(RevBayesCore::Tree *v);                                                                                                    //!< Constructor requires character type
        Tree(const RevBayesCore::Tree &v);                                                                                              //!< Constructor requires character type
        Tree(RevBayesCore::TypedDagNode<RevBayesCore::Tree> *n);                                                                        //!< Constructor requires character type
        Tree(const Tree& d);                                                                                                            //!< Constructor requires character type
        
        typedef RevBayesCore::Tree valueType;
        
        // Basic utility functions
        Tree*                   clone(void) const;                                                                                  //!< Clone object
        static const std::string&           getClassType(void);                                                                                 //!< Get Rev type
        static const TypeSpec&              getClassTypeSpec(void);                                                                             //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                                                            //!< Get language type of the object
        
        // Member method functions
        const MethodTable&                  getMethods(void) const;                                                     //!< Get member methods
        MethodTable                         makeMethods(void) const;                                                    //!< Make member methods
        RevPtr<Variable>                    executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Map member methods to internal functions
        
    };
    
}

#endif
