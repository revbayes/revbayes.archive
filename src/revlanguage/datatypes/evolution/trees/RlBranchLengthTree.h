/**
 * @file
 * This file contains the declaration of a BranchLengthTree, which is
 * class that holds a tree with nodes having BranchLengths attached.
 *
 * @brief Declaration of BranchLengthTree
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef RlBranchLengthTree_H
#define RlBranchLengthTree_H

#include "BranchLengthTree.h"
#include "ModelObject.h"

#include <set>
#include <string>
#include <vector>


namespace RevLanguage {
    
    
    class BranchLengthTree : public ModelObject<RevBayesCore::BranchLengthTree> {
        
    public:
        BranchLengthTree(void);                                                                                                                         //!< Constructor requires character type
        BranchLengthTree(RevBayesCore::BranchLengthTree *v);                                                                                                    //!< Constructor requires character type
        BranchLengthTree(const RevBayesCore::BranchLengthTree &v);                                                                                              //!< Constructor requires character type
        BranchLengthTree(RevBayesCore::TypedDagNode<RevBayesCore::BranchLengthTree> *n);                                                                        //!< Constructor requires character type
        
        typedef RevBayesCore::BranchLengthTree valueType;
        
        // Basic utility functions
        BranchLengthTree*                   clone(void) const;                                                                                  //!< Clone object
        static const std::string&           getClassType(void);                                                                                 //!< Get Rev type
        static const TypeSpec&              getClassTypeSpec(void);                                                                             //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                                                            //!< Get language type of the object
        
        // Member method functions
        RevPtr<Variable>                    executeMethod(const std::string& name, const std::vector<Argument>& args, bool &f);                 //!< Map member methods to internal functions
        
    };
    
}

#endif
