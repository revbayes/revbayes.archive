#ifndef Dist_bdpTopology_H
#define Dist_bdpTopology_H

#include "ConstantRateBirthDeathProcessTopology.h"
#include "RlBirthDeathProcess.h"
#include "RlTimeTree.h"

namespace RevLanguage {
	
	/**
	 * The RevLanguage wrapper of the Dist_bdpTopology
	 *
	 * The RevLanguage wrapper of the constant-rate birth-death process (with topology) connects
	 * the variables/parameters of the process and creates the internal Dist_bdpTopology object.
	 * Please read the ConstantRateBirthDeathProcessTopology.h for more info.
	 *
	 *
	 * @copyright Copyright 2009-
	 * @author The RevBayes Development Core Team (Sebastian Hoehna)
	 * @since 2014-01-26, version 1.0
	 *
	 */
	class Dist_bdpTopology : public BirthDeathProcess {
		
	public:
		Dist_bdpTopology( void );
		
		// Basic utility functions
		Dist_bdpTopology*                               clone(void) const;                                                                      //!< Clone the object
		static const std::string&                       getClassType(void);                                                                     //!< Get Rev type
		static const TypeSpec&                          getClassTypeSpec(void);                                                                 //!< Get class type spec
        std::string                                     getDistributionFunctionName(void) const;                                                //!< Get the Rev-name for this distribution.
        const TypeSpec&                                 getTypeSpec(void) const;                                                                //!< Get the type spec of the instance
		const MemberRules&                              getParameterRules(void) const;                                                          //!< Get member rules (const)
		
		
		// Distribution functions you have to override
		RevBayesCore::ConstantRateBirthDeathProcess*    createDistribution(void) const;
		
	protected:
		
		void                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);       //!< Set member variable
		
		
	private:
		
		RevPtr<const RevVariable>                          lambda;                                                                                 //!< The speciation rate
		RevPtr<const RevVariable>                          mu;                                                                                     //!< The extinction rate
		
	};
	
}

#endif
