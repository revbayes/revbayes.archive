/* 
 * File:   Func_t92GCBranchTree.h
 * Author: nl
 *
 * Created on 25 juillet 2014, 20:59
 */

#ifndef FUNC_T92GCBRANCHTREE_H
#define	FUNC_T92GCBRANCHTREE_H

#include <iostream>

#include <string>

#include "ModelVector.h"
#include "RlTypedFunction.h"
#include "TypedDagNode.h"


namespace RevLanguage {
    
    class Func_t92GCBranchTree :  public Function {
        
    public:
        // Func_t92GCBranchTree( void ) : tree(NULL), nodeval(NULL) {}

        Func_t92GCBranchTree( void ) {}
        ~Func_t92GCBranchTree() {}
        
                                                        // Basic utility functions
                                                        Func_t92GCBranchTree(const Func_t92GCBranchTree &x);                                //!< Copy constuctor

        Func_t92GCBranchTree*                             clone(void) const;                                                              //!< Clone the object
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

#endif	/* FUNC_T92GCBRANCHTREE_H */

