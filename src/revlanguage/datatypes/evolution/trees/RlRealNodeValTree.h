/* 
 * File:   RlRealNodeValTree.h
 * Author: nl
 *
 * Created on 26 juillet 2014, 13:03
 */

#ifndef RLREALNODEVALTREE_H
#define	RLREALNODEVALTREE_H


#include "RealNodeContainer.h"
#include "ModelObject.h"

#include <set>
#include <string>
#include <vector>


namespace RevLanguage {
    
    
    class RealNodeValTree : public ModelObject<RevBayesCore::RealNodeContainer> {
        
    public:
        RealNodeValTree(void);                                                                                                      //!< Constructor requires character type
        RealNodeValTree(RevBayesCore::RealNodeContainer *v);                                                                        //!< Constructor requires character type
        RealNodeValTree(const RevBayesCore::RealNodeContainer &v);                                                                  //!< Constructor requires character type
        RealNodeValTree(RevBayesCore::TypedDagNode<RevBayesCore::RealNodeContainer> *n);                                            //!< Constructor requires character type
        
        typedef RevBayesCore::RealNodeContainer valueType;
        
        // Basic utility functions
        RealNodeValTree*           clone(void) const;                                                                               //!< Clone object
        static const std::string&           getClassType(void);                                                                     //!< Get class name
        static const TypeSpec&              getClassTypeSpec(void);                                                                 //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                                                //!< Get language type of the object
        
        // Member method functions
        virtual RevPtr<RevVariable>            executeMethod(const std::string& name, const std::vector<Argument>& args, bool &f);     //!< Map member methods to internal functions
        
        void                                printValue(std::ostream& o) const;                                                      //!< Print value (for user)
    };
    
}

#endif	/* RLREALNODEVALTREE_H */

