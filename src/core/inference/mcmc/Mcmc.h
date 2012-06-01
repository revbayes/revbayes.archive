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
#include "Model.h"
#include "RlVector.h"
#include "Variable.h"

#include <set>
#include <string>

class ArgumentRule;
class DAGNode;
class InferenceMonitor;
class InferenceMove;


class Mcmc: public MemberObject {

public:
    Mcmc(void);                                                                                                         //!< Default constructor
    Mcmc(const Mcmc &x);                                                                                                //!< Copy Constructor
    virtual                    ~Mcmc(void) {}                                                                           //!< Destructor

    // operator overloading
    Mcmc&                           operator=(const Mcmc &m);
    
    // Basic utility functions
    Mcmc*                           clone(void) const;                                                                      //!< Clone object
    static const std::string&       getClassName(void);                                                                     //!< Get class name
    static const TypeSpec&          getClassTypeSpec(void);                                                                 //!< Get class type spec
    const TypeSpec&                 getTypeSpec(void) const;                                                                //!< Get language type of the object 
    void                            printValue(std::ostream& o) const;                                                      //!< Print value for user

    // Member variable rules
    const MemberRules&              getMemberRules(void) const;                                                             //!< Get member rules
    void                            setConstMemberVariable(const std::string &name, const RbPtr<const Variable> &var);
    // Member method inits
    const MethodTable&              getMethods(void) const;                                                                 //!< Get methods
        
    // Mcmc functions
    void                            run(size_t ngen);                                                                       //!< Update the chain

protected:
    RbPtr<RbLanguageObject>         executeSimpleMethod(const std::string& name, const std::vector<const RbObject *>& args); //!< Execute method

 
private:
    void                            addMove(const DAGNode* m);
    void                            addMove(const InferenceMove* m);
    void                            addMonitor(const DAGNode* m);
    void                            addMonitor(const InferenceMonitor* m);
    void                            extractDagNodesFromModel( const Model& source );
    
    // parameters
    Model                           model;

    // Member variables
    std::vector<InferenceDagNode*>  dagNodes;                
    std::vector<InferenceMove*>     moves;
    std::vector<InferenceMonitor*>  monitors;

};

#endif

