/**
 * @file
 * This file contains the declaration of Mcmc, which is used to hold
 * information about and run an mcmc analysis.
 *
 * @brief Declaration of Mcmc
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-29 23:23:09 +0100 (Tis, 29 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface Mcmc
 * @package distributions
 *
 * $Id: Mcmc.h 211 2009-12-29 22:23:09Z ronquist $
 */

#ifndef Mcmc_H
#define Mcmc_H

#include "ConstantMemberObject.h"

#include <set>
#include <string>

class ArgumentRule;
class DAGNode;
class Model;
class VectorString;

const std::string Mcmc_name = "MCMC";

class Mcmc: public ConstantMemberObject {

public:
    Mcmc(void);                                                                                                 //!< Default constructor
    Mcmc(const Mcmc &x);                                                                                        //!< Copy Constructor
    virtual                    ~Mcmc(void) {}                                                                   //!< Destructor

    // Basic utility functions
    Mcmc*                       clone(void) const;                                                              //!< Clone object
    const VectorString&         getClass(void) const;                                                           //!< Get class vector  
    const TypeSpec&             getTypeSpec(void) const;                                                        //!< Get language type of the object 

    // Member variable rules
    RbPtr<const MemberRules>    getMemberRules(void) const;                                                     //!< Get member rules
    void                        setMemberVariable(const std::string& name, RbPtr<Variable> var);                //!< Only constants allowed

    // Member method inits
    RbPtr<const MethodTable>    getMethods(void) const;                                                         //!< Get methods
        
    // Mcmc functions
    void                        run(size_t ngen);                                                               //!< Update the chain

protected:
    RbPtr<RbLanguageObject>     executeOperationSimple(const std::string& name, const RbPtr<Environment>& args);//!< Execute method

    
private:
    static const TypeSpec       typeSpec;
    
};

#endif

