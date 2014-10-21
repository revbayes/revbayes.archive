/* 
 * File:   Func_tanhBranchTree.h
 * Author: nl
 *
 * Created on 25 juillet 2014, 19:56
 */

#ifndef FUNC_TANHBRANCHTREE_H
#define	FUNC_TANHBRANCHTREE_H

#include <iostream>

#include <string>

#include "ModelVector.h"
#include "RealPos.h"
#include "RlTypedFunction.h"
#include "TypedDagNode.h"


namespace RevLanguage {
    
    class Func_tanhBranchTree :  public Function {
        
    public:
        // Func_tanhBranchTree( void ) : tree(NULL), nodeval(NULL) {}

        Func_tanhBranchTree( void ) {}
        ~Func_tanhBranchTree() {}
        
                                                        // Basic utility functions
                                                        Func_tanhBranchTree(const Func_tanhBranchTree &x);                                //!< Copy constuctor

        Func_tanhBranchTree*                             clone(void) const;                                                              //!< Clone the object
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


#endif	/* FUNC_TANHBRANCHTREE_H */

