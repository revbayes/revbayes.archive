/* 
 * File:   MultivariateRealNodeValTree.h
 * Author: nl
 *
 * Created on 16 juillet 2014, 19:54
 */

#ifndef RLMULTIVARIATEREALPHYLOPROCESS_H
#define	RLMULTIVARIATEREALPHYLOPROCESS_H

#include "MultivariateRealNodeContainer.h"
#include "ModelObject.h"

#include <set>
#include <string>
#include <vector>


namespace RevLanguage {
    
    
    class MultivariateRealNodeValTree : public ModelObject<RevBayesCore::MultivariateRealNodeContainer> {
        
    public:
        MultivariateRealNodeValTree(void);                                                                                                                         //!< Constructor requires character type
        MultivariateRealNodeValTree(RevBayesCore::MultivariateRealNodeContainer *v);                                                                                                    //!< Constructor requires character type
        MultivariateRealNodeValTree(const RevBayesCore::MultivariateRealNodeContainer &v);                                                                                              //!< Constructor requires character type
        MultivariateRealNodeValTree(RevBayesCore::TypedDagNode<RevBayesCore::MultivariateRealNodeContainer> *n);                                                                        //!< Constructor requires character type
        
        typedef RevBayesCore::MultivariateRealNodeContainer valueType;
        
        // Basic utility functions
        MultivariateRealNodeValTree*        clone(void) const;                                                              //!< Clone object
        static const std::string&           getClassType(void);                                                             //!< Get class name
        static const TypeSpec&              getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                                        //!< Get language type of the object
        
        // Member method functions
        RevPtr<Variable>                    executeMethod(const std::string& name, const std::vector<Argument>& args);      //!< Map member methods to internal functions
        
        void                                printValue(std::ostream& o) const;                                              //!< Print value (for user)
    };
    
}

#endif	/* RLMULTIVARIATEREALPHYLOPROCESS_H */

