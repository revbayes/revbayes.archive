
#ifndef RlClade_H
#define RlClade_H

#include "Clade.h"
#include "ModelObject.h"

#include <set>
#include <string>
#include <vector>


namespace RevLanguage {
    /**
    * @file
    * This file contains the declaration of a Clade, which is
    * the class that contains the names of the taxa that build the clade.
    *
    * @brief Declaration of Clade
    *
    * (c) Copyright 2009-
    * @date Last modified: $Date: $
    * @author The RevBayes Development Core Team
    * @license GPL version 3
    *
    * 
    */

    
    class Clade : public ModelObject<RevBayesCore::Clade> {
        
    public:
        Clade(void);                                                                                                                    //!< Constructor requires character type
        Clade(RevBayesCore::Clade *v);                                                                                                  //!< Constructor requires character type
        Clade(const RevBayesCore::Clade &v);                                                                                            //!< Constructor requires character type
        Clade(RevBayesCore::TypedDagNode<RevBayesCore::Clade> *n);                                                                      //!< Constructor requires character type
        Clade(const Clade& d);                                                                                                          //!< Constructor requires character type
        
        typedef RevBayesCore::Clade valueType;
        
        // Basic utility functions
        Clade*                                  clone(void) const;                                                                      //!< Clone object
        void                                    constructInternalObject(void);                                                          //!< We construct the a new internal MCMC object.
        static const std::string&               getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&                  getClassTypeSpec(void);                                                                 //!< Get class type spec
        const MemberRules&                      getMemberRules(void) const;                                                             //!< Get member rules (const)
        const TypeSpec&                         getTypeSpec(void) const;                                                                //!< Get language type of the object
               
    protected:
        void                                    setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
        std::vector<RevPtr<const Variable> >    names;

    };
    
}

#endif
