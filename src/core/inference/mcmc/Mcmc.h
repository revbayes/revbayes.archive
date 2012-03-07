/**
 * @file
 * This file contains the declaration of Mcmc, which is used to hold
 * information about and run an mcmc analysis.
 *
 * @brief Declaration of Mcmc
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface Mcmc
 * @package distributions
 *
 * $Id$
 */

#ifndef Mcmc_H
#define Mcmc_H

#include "MemberObject.h"
#include "Variable.h"

#include <set>
#include <string>

class ArgumentRule;
class DAGNode;
class Model;
class VectorString;


class Mcmc: public MemberObject {

public:
    Mcmc(void);                                                                                                         //!< Default constructor
    Mcmc(const Mcmc &x);                                                                                                //!< Copy Constructor
    virtual                    ~Mcmc(void) {}                                                                           //!< Destructor

    // Basic utility functions
    Mcmc*                       clone(void) const;                                                                      //!< Clone object
    static const std::string&   getClassName(void);                                                                     //!< Get class name
    static const TypeSpec&      getClassTypeSpec(void);                                                                 //!< Get class type spec
    const TypeSpec&             getTypeSpec(void) const;                                                                //!< Get language type of the object 

    // Member variable rules
    const MemberRules&          getMemberRules(void) const;                                                             //!< Get member rules
    void                        setMemberVariable(const std::string& name, Variable* var);                              //!< Only constants allowed

    // Member method inits
    const MethodTable&          getMethods(void) const;                                                                 //!< Get methods
        
    // Mcmc functions
    void                        run(size_t ngen);                                                                       //!< Update the chain

protected:
    const RbLanguageObject&     executeOperationSimple(const std::string& name, const std::vector<Argument>& args);     //!< Execute method

 
private:
    
    // parameters
    RbVariablePtr               model;
    RbVariablePtr               moves;
    RbVariablePtr               monitors;
};

#endif

