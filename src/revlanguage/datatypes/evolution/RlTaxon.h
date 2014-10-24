
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
        Taxon(void);                                                                                                                //!< Constructor requires character type
        Taxon(RevBayesCore::Taxon *v);                                                                                              //!< Constructor requires character type
        Taxon(const RevBayesCore::Taxon &v);                                                                                        //!< Constructor requires character type
        Taxon(RevBayesCore::TypedDagNode<RevBayesCore::Taxon> *n);                                                                  //!< Constructor requires character type
        
        typedef RevBayesCore::Taxon valueType;
        
        // Basic utility functions
        Taxon*                              clone(void) const;                                                                      //!< Clone object
        void                                constructInternalObject(void);                                                          //!< We construct the a new internal MCMC object.
        static const std::string&           getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&              getClassTypeSpec(void);                                                                 //!< Get class type spec
        const MemberRules&                  getParameterRules(void) const;                                                             //!< Get member rules (const)
        const TypeSpec&                     getTypeSpec(void) const;                                                                //!< Get language type of the object
        
        // Member method inits
        virtual RevPtr<Variable>            executeMethod(const std::string& name, const std::vector<Argument>& args);              //!< Map member methods to internal functions
        
    protected:
        void                                setConstParameter(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
        RevPtr<const Variable>              taxon;
        RevPtr<const Variable>              species;
        RevPtr<const Variable>              date;

    };
    
}

#endif
