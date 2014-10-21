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
    
    class Func_expBranchTree :  public Function {
        
    public:
        // Func_expBranchTree( void ) : tree(NULL), nodeval(NULL) {}

        Func_expBranchTree( void ) {}
        ~Func_expBranchTree() {}
        
                                                        // Basic utility functions
                                                        Func_expBranchTree(const Func_expBranchTree &x);                                //!< Copy constuctor

        Func_expBranchTree*                             clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
                                                        // Function functions you have to override
        RevPtr<Variable>                                execute(void);                                                                  //!< Execute function
        const ArgumentRules&                            getArgumentRules(void) const;                                                   //!< Get argument rules
        const TypeSpec&                                 getReturnType(void) const;                                                      //!< Get type of return value


    protected:
        
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
     //   void                                            setConstParameter(const std::string& name, const RevPtr<const Variable> &var);             //!< Set member variable
        

        
    private:

        // RevPtr<const Variable>                         tree;
        // RevPtr<const Variable>                         nodeval;
    };
    
}

/*

namespace RevLanguage {
    
    class Func_expBranchTree : public TypedFunction<ModelVector<RealPos> > {
        
    public:
        Func_expBranchTree(void);
        //        Func_expBranchTree(RevBayesCore::TypedDagNode<RevBayesCore::Func_expBranchTree> *n);
        virtual                                         ~Func_expBranchTree(void);                                                  //!< Destructor
        Func_expBranchTree(const Func_expBranchTree &x);                                                                            //!< Copy constuctor
        
        // the value type definition
        //         typedef RevBayesCore::Func_expBranchTree valueType;
        
        // Basic utility functions you have to override
        virtual Func_expBranchTree*                     clone(void) const;                                                          //!< Clone object
        static const std::string&                       getClassType(void);                                                         //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                     //!< Get class type spec
        virtual const ArgumentRules&                    getArgumentRules(void) const;                                                   //!< Get argument rules
        virtual const TypeSpec&                         getReturnType(void) const;                                                      //!< Get type of return value
        virtual const TypeSpec&                         getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        
        // Distribution functions you have to override
        virtual RevBayesCore::Func_expBranchTree*       createFunction(void) const;                                                 //!< Create a random variable from this distribution
        
        
    protected:                                                                                                 //!< Basic constructor
        
    };
    
}
*/
#endif /* defined(__revbayes__RlExponentialBranchTree__) */
