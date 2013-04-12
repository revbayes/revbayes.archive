/**
 * @file
 * This file contains the declaration of a TimeTree, which is
 * class that holds a tree with nodes having times attached.
 *
 * @brief Declaration of TimeTree
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef RlTimeTree_H
#define RlTimeTree_H

#include "TimeTree.h"
#include "RlModelVariableWrapper.h"

#include <set>
#include <string>
#include <vector>


namespace RevLanguage {
    
    
    class TimeTree : public RlModelVariableWrapper<RevBayesCore::TimeTree> {
        
    public:
        TimeTree(void);                                                                                                                         //!< Constructor requires character type
        TimeTree(RevBayesCore::TimeTree *v);                                                                                                    //!< Constructor requires character type
        TimeTree(const RevBayesCore::TimeTree &v);                                                                                              //!< Constructor requires character type
        TimeTree(RevBayesCore::TypedDagNode<RevBayesCore::TimeTree> *n);                                                                        //!< Constructor requires character type
        TimeTree(const TimeTree& d);                                                                                                            //!< Constructor requires character type
        
        typedef RevBayesCore::TimeTree valueType;
        
        // Basic utility functions
        TimeTree*                           clone(void) const;                                                                                  //!< Clone object
        static const std::string&           getClassName(void);                                                                                 //!< Get class name
        static const TypeSpec&              getClassTypeSpec(void);                                                                             //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                                                            //!< Get language type of the object
        
        // Member method inits
        const MethodTable&                  getMethods(void) const;                                             //!< Get methods
        RbLanguageObject*                   executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions
        
    };
    
}

#endif
