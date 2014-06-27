
#ifndef RlTaxon_H
#define RlTaxon_H

#include "ModelObject.h"
#include "Taxon.h"

#include <set>
#include <string>
#include <vector>


namespace RevLanguage {
    
    /**
    * @file
    * This file contains the declaration of a Taxon, which is
    * the class that contains the name of a taxon with associated species and date.
    *
    * @brief Declaration of Taxon
    *
    * (c) Copyright 2009-
    * @date Last modified: $Date: $
    * @author The RevBayes Development Core Team
    * @license GPL version 3
    *
    *
    */
    
    class Taxon : public ModelObject<RevBayesCore::Taxon> {
        
    public:
        Taxon(void);                                                                                                                         //!< Constructor requires character type
        Taxon(RevBayesCore::Taxon *v);                                                                                                    //!< Constructor requires character type
        Taxon(const RevBayesCore::Taxon &v);                                                                                              //!< Constructor requires character type
        Taxon(RevBayesCore::TypedDagNode<RevBayesCore::Taxon> *n);                                                                        //!< Constructor requires character type
        Taxon(const Taxon& d);                                                                                                            //!< Constructor requires character type
        
        typedef RevBayesCore::Taxon valueType;
        
        // Basic utility functions
        Taxon*                              clone(void) const;                                                                                  //!< Clone object
        void                                constructInternalObject(void);                                          //!< We construct the a new internal MCMC object.
        static const std::string&           getClassName(void);                                                                                 //!< Get class name
        static const TypeSpec&              getClassTypeSpec(void);                                                                             //!< Get class type spec
        const MemberRules&                  getMemberRules(void) const;                                             //!< Get member rules (const)
        const TypeSpec&                     getTypeSpec(void) const;                                                                            //!< Get language type of the object
        
        // Member method inits
        const MethodTable&                  getMethods(void) const;                                             //!< Get methods
        RevObject*                   executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions
        
    protected:
        void                                setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var);              //!< Set member variable
        
        RbPtr<const Variable> taxon;
        RbPtr<const Variable> species;
        RbPtr<const Variable> date;

                

    };
    
}

#endif
