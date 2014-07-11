
#ifndef RlBppAlphabet_H
#define RlBppAlphabet_H

#include "ModelObject.h"
#include "BppAlphabet.h"

#include <set>
#include <string>
#include <vector>


namespace RevLanguage {
    
    /**
    * @file
    * This file contains the declaration of a BppAlphabet, which is
    * the class that contains the name of a taxon with associated species and date.
    *
    * @brief Declaration of BppAlphabet
    *
    * (c) Copyright 2009-
    * @date Last modified: $Date: $
    * @author The RevBayes Development Core Team
    * @license GPL version 3
    *
    *
    */
    
    class BppAlphabet : public ModelObject<RevBayesCore::BppAlphabet> {
        
    public:
        BppAlphabet(void);                                                                                                                //!< Constructor requires character type
        BppAlphabet(RevBayesCore::BppAlphabet *v);                                                                                              //!< Constructor requires character type
        BppAlphabet(const RevBayesCore::BppAlphabet &v);                                                                                        //!< Constructor requires character type
        BppAlphabet(RevBayesCore::TypedDagNode<RevBayesCore::BppAlphabet> *n);                                                                  //!< Constructor requires character type
        BppAlphabet(const BppAlphabet& d);                                                                                                      //!< Constructor requires character type
        
        typedef RevBayesCore::BppAlphabet valueType;
        
        // Basic utility functions
        BppAlphabet*                              clone(void) const;                                                                      //!< Clone object
        void                                constructInternalObject(void);                                                          //!< We construct the a new internal MCMC object.
        static const std::string&           getClassName(void);                                                                     //!< Get class name
        static const TypeSpec&              getClassTypeSpec(void);                                                                 //!< Get class type spec
        const MemberRules&                  getMemberRules(void) const;                                                             //!< Get member rules (const)
        const TypeSpec&                     getTypeSpec(void) const;                                                                //!< Get language type of the object
        
        // Member method inits
        const MethodTable&                  getMethods(void) const;                                                                 //!< Get methods
        RevObject*                          executeMethod(const std::string& name, const std::vector<Argument>& args);              //!< Override to map member methods to internal functions
        
    protected:
        void                                setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
    };
    
}

#endif
