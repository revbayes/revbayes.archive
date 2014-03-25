#ifndef RlGeometricDistribution_H
#define RlGeometricDistribution_H

#include "GeometricDistribution.h"
#include "Natural.h"
#include "RlTypedDistribution.h"

namespace RevLanguage {
    
    /**
     * The RevLanguage wrapper of the geometric distribution.
     *
     * The RevLanguage wrapper of the geometric distribution simply
     * manages the interactions through the Rev with our core.
     * That is, the internal distribution object can be constructed and hooked up
     * in a model graph.
     * See the GeometricDistribution.h for more details.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2012-08-08, version 1.0
     *
     */
    class GeometricDistribution :  public TypedDistribution<Natural> {
        
    public:
        GeometricDistribution( void );                                                                                                                //!< Default constructor
        
        // Basic utility functions
        GeometricDistribution*                          clone(void) const;                                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                                        //!< Get the type spec of the instance
        const MemberRules&                              getMemberRules(void) const;                                                                     //!< Get member rules (const)
        
        
        // Distribution functions you have to override
        RevBayesCore::GeometricDistribution*            createDistribution(void) const;                                                                 //!< Create the internal distribution object
        
    protected:
        
        void                                            setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var);              //!< Set member variable
        
        
    private:
        RbPtr<const Variable>                           p;                                                                                         //!< The rate of the distribution
        
    };
    
}

#endif
