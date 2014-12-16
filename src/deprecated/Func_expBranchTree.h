//
//  Func_expBranchTree.h
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-24.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef __revbayes__RlExponentialBranchTree__
#define __revbayes__RlExponentialBranchTree__

#include <iostream>

#include <string>

#include "ModelVector.h"
#include "RealPos.h"
// #include "Func_expBranchTree.h"
#include "RlTypedFunction.h"
#include "TypedDagNode.h"


namespace RevLanguage {
    
    class Func_expBranchTree : public TypedFunction< ModelVector<RealPos> > {
        
    public:
        // Func_expBranchTree( void ) : tree(NULL), nodeval(NULL) {}

        Func_expBranchTree( void ) {}
        ~Func_expBranchTree() {}
        
        
        Func_expBranchTree*                                                         clone(void) const;                                                              //!< Clone the object
        static const std::string&                                                   getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                                                      getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                                             getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevBayesCore::TypedFunction< RevBayesCore::RbVector< double > >*            createFunction(void) const;                                                     //!< Create internal function object
        const ArgumentRules&                                                        getArgumentRules(void) const;                                                   //!< Get argument rules
        

    protected:
        
        void                                                                        printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
     //   void                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);             //!< Set member variable
        

        
    private:

        // RevPtr<const RevVariable>                         tree;
        // RevPtr<const RevVariable>                         nodeval;
    };
    
}


#endif /* defined(__revbayes__RlExponentialBranchTree__) */
