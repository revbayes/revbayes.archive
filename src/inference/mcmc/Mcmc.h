/**
 * @file
 * This file contains the declaration of Mcmc, which is used to hold
 * information about and run an mcmc analysis.
 *
 * @brief Declaration of Mcmc
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-29 23:23:09 +0100 (Tis, 29 Dec 2009) $
 * @author The RevBayes core development team
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

#include "MemberObject.h"

#include <set>
#include <string>

class ArgumentRule;
class DAGNode;
class Model;
class VectorString;

class Mcmc: public MemberObject {

    public:
								    Mcmc(void);                                                                        //!< Parser constructor
									Mcmc(Model* model, int ngen, int printfreq, int samplefreq, std::string filename); //!< Internal constructor
	    virtual                    ~Mcmc(void) {}                                                                      //!< Destructor

        // Basic utility functions
        Mcmc*                       clone(void) const;                                                                 //!< Clone object
        const VectorString&         getClass(void) const;                                                              //!< Get class vector   

        // Member variable rules
        const MemberRules&          getMemberRules(void) const;                                                        //!< Get member rules
        void                        setVariable(const std::string& name, DAGNode* var);                                //!< Only constants allowed

        // Member method inits
        const MethodTable&          getMethodInits(void) const;                                                        //!< Get method inits
        
        // Mcmc functions
        void                        update(void);                                                                      //!< Update the chain

	protected:
        DAGNode*                    executeOperation(const std::string& name, std::vector<DAGNode*>& args);            //!< Execute method
};

#endif

