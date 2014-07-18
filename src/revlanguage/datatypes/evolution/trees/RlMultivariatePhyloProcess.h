/* 
 * File:   MultivariatePhyloProcess.h
 * Author: nl
 *
 * Created on 16 juillet 2014, 19:54
 */

#ifndef RLMULTIVARIATEREALPHYLOPROCESS_H
#define	RLMULTIVARIATEREALPHYLOPROCESS_H

#include "MultivariatePhyloProcess.h"
#include "ModelObject.h"

#include <set>
#include <string>
#include <vector>


namespace RevLanguage {
    
    
    class MultivariatePhyloProcess : public ModelObject<RevBayesCore::MultivariatePhyloProcess> {
        
    public:
        MultivariatePhyloProcess(void);                                                                                                                         //!< Constructor requires character type
        MultivariatePhyloProcess(RevBayesCore::MultivariatePhyloProcess *v);                                                                                                    //!< Constructor requires character type
        MultivariatePhyloProcess(const RevBayesCore::MultivariatePhyloProcess &v);                                                                                              //!< Constructor requires character type
        MultivariatePhyloProcess(RevBayesCore::TypedDagNode<RevBayesCore::MultivariatePhyloProcess> *n);                                                                        //!< Constructor requires character type
        MultivariatePhyloProcess(const MultivariatePhyloProcess& d);                                                                                                            //!< Constructor requires character type
        
        typedef RevBayesCore::MultivariatePhyloProcess valueType;
        
        // Basic utility functions
        MultivariatePhyloProcess*                   clone(void) const;                                                                                  //!< Clone object
        static const std::string&           getClassName(void);                                                                                 //!< Get class name
        static const TypeSpec&              getClassTypeSpec(void);                                                                             //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                                                            //!< Get language type of the object
        
        // Member method inits
        const MethodTable&                  getMethods(void) const;                                             //!< Get methods
        RevObject*                      executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions
        
        void                            printValue(std::ostream& o) const;                                      //!< Print value (for user)
    };
    
}

#endif	/* RLMULTIVARIATEREALPHYLOPROCESS_H */

