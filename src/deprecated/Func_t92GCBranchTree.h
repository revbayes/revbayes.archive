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
#include "RlRateMatrix.h"
#include "RlTypedFunction.h"
#include "TypedDagNode.h"


namespace RevLanguage {
    
    class Func_t92GCBranchTree : public TypedFunction< ModelVector<RateMatrix> > {
        
    public:
        // Func_t92GCBranchTree( void ) : tree(NULL), nodeval(NULL) {}

        Func_t92GCBranchTree( void ) {}
        ~Func_t92GCBranchTree() {}
        
        // Basic utility functions
        Func_t92GCBranchTree*                                                                       clone(void) const;                                                              //!< Clone the object
        static const std::string&                                                                   getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                                                                      getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                                                             getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevBayesCore::TypedFunction< RevBayesCore::RbVector< RevBayesCore::RateMatrix > >*          createFunction(void) const;                                                     //!< Create internal function object
        const ArgumentRules&                                                                        getArgumentRules(void) const;                                                   //!< Get argument rules
       

    protected:
        
        void                                                                                        printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
     //   void                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);             //!< Set member variable
        

        
    private:

        // RevPtr<const RevVariable>                         tree;
        // RevPtr<const RevVariable>                         nodeval;
    };
    
}

#endif	/* FUNC_T92GCBRANCHTREE_H */

