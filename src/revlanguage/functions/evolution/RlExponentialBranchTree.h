//
//  RlExponentialBranchTree.h
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-24.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef __revbayes__RlExponentialBranchTree__
#define __revbayes__RlExponentialBranchTree__

#include <iostream>

#include <string>

#include "Vector.h"
#include "RealPos.h"
#include "ExponentialBranchTree.h"
#include "RlTypedFunction.h"
#include "TypedDagNode.h"


namespace RevLanguage {
    
    class ExponentialBranchTree :  public Function {
        
    public:
        // ExponentialBranchTree( void ) : tree(NULL), nodeval(NULL) {}

        ExponentialBranchTree( void ) {}
        ~ExponentialBranchTree() {}
        
                                                        // Basic utility functions
                                                        ExponentialBranchTree(const ExponentialBranchTree &x);                          //!< Copy constuctor

        ExponentialBranchTree*                          clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
                                                        // Function functions you have to override
        RevObject*                                      execute(void);                                                                  //!< Execute function
        const ArgumentRules&                            getArgumentRules(void) const;                                                   //!< Get argument rules
        const TypeSpec&                                 getReturnType(void) const;                                                      //!< Get type of return value


    protected:
        
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
     //   void                                            setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);             //!< Set member variable
        

        
    private:

        // RevPtr<const Variable>                         tree;
        // RevPtr<const Variable>                         nodeval;
    };
    
}

/*

namespace RevLanguage {
    
    class ExponentialBranchTree : public TypedFunction<Vector<RealPos> > {
        
    public:
        ExponentialBranchTree(void);
        //        ExponentialBranchTree(RevBayesCore::TypedDagNode<RevBayesCore::ExponentialBranchTree> *n);
        virtual                                         ~ExponentialBranchTree(void);                                                  //!< Destructor
        ExponentialBranchTree(const ExponentialBranchTree &x);                                                                            //!< Copy constuctor
        
        // the value type definition
        //         typedef RevBayesCore::ExponentialBranchTree valueType;
        
        // Basic utility functions you have to override
        virtual ExponentialBranchTree*                     clone(void) const;                                                          //!< Clone object
        static const std::string&                       getClassName(void);                                                         //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                     //!< Get class type spec
        virtual const ArgumentRules&                    getArgumentRules(void) const;                                                   //!< Get argument rules
        virtual const TypeSpec&                         getReturnType(void) const;                                                      //!< Get type of return value
        virtual const TypeSpec&                         getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        
        // Distribution functions you have to override
        virtual RevBayesCore::ExponentialBranchTree*       createFunction(void) const;                                                 //!< Create a random variable from this distribution
        
        
    protected:                                                                                                 //!< Basic constructor
        
    };
    
}
*/
#endif /* defined(__revbayes__RlExponentialBranchTree__) */
