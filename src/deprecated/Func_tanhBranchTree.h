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
#include "Probability.h"
#include "RlTypedFunction.h"
#include "TypedDagNode.h"


namespace RevLanguage {
    
    class Func_tanhBranchTree :  public TypedFunction< ModelVector<Probability> > {
        
    public:
        // Func_tanhBranchTree( void ) : tree(NULL), nodeval(NULL) {}

        Func_tanhBranchTree( void ) {}
        ~Func_tanhBranchTree() {}
        
        // Basic utility functions
        Func_tanhBranchTree*                                                clone(void) const;                                                      //!< Clone the object
        static const std::string&                                           getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&                                              getClassTypeSpec(void);                                                 //!< Get class type spec
        const TypeSpec&                                                     getTypeSpec(void) const;                                                //!< Get the type spec of the instance
        
        
        // Function functions you have to override
        RevBayesCore::TypedFunction< RevBayesCore::RbVector<double > >*     createFunction(void) const;                                             //!< Create internal function object
        const ArgumentRules&                                                getArgumentRules(void) const;                                           //!< Get argument rules
        

    protected:
        
        void                                                                printValue(std::ostream& o) const;                                      //!< Print the general information on the function ('usage')
        
        
     //   void                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);             //!< Set member variable
        

        
    private:

        // RevPtr<const RevVariable>                         tree;
        // RevPtr<const RevVariable>                         nodeval;
    };
    
}


#endif	/* FUNC_TANHBRANCHTREE_H */

