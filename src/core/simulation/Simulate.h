/**
 * @file
 * This file contains the declaration of Simulate, which is used to hold
 * information about and run a simulation given a model and monitors.
 *
 * @brief Declaration of Simulate
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-29 23:23:09 +0100 (Tis, 29 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface Simulate
 * @package distributions
 *
 * $Id: Simulate.h 211 2009-12-14 14:31 boussau $
 */

#ifndef Simulate_H
#define Simulate_H

#include "ConstantMemberObject.h"

#include <set>
#include <string>

class ArgumentRule;
class DAGNode;
class Model;
class VectorString;

const std::string Simulate_name = "Simulate";

class Simulate: public ConstantMemberObject {
    
public:
    Simulate(void);                                                                                                 //!< Default constructor
    Simulate(const Simulate &x);                                                                                        //!< Copy Constructor
    virtual                    ~Simulate(void) {}                                                                   //!< Destructor
    
    // Basic utility functions
    Simulate*                       clone(void) const;                                                              //!< Clone object
    const VectorString&         getClass(void) const;                                                           //!< Get class vector  
    const TypeSpec&             getTypeSpec(void) const;                                                        //!< Get language type of the object 
    
    // Member variable rules
    RbPtr<const MemberRules>    getMemberRules(void) const;                                                     //!< Get member rules
    void                        setMemberVariable(const std::string& name, RbPtr<Variable> var);                //!< Only constants allowed
    
    // Member method inits
    RbPtr<const MethodTable>    getMethods(void) const;                                                         //!< Get methods
    
    // Simulate functions
    void                        run(size_t ndata);                                                               //!< Simulates ndata data elements
    
protected:
    RbPtr<RbLanguageObject>     executeOperationSimple(const std::string& name, const RbPtr<Environment>& args);//!< Execute method
    
    
private:
    static const TypeSpec       typeSpec;
    void                        getOrderedStochasticNodes(const RbPtr<DAGNode>& dagNode,  std::vector<RbPtr<StochasticNode> >& orderedStochasticNodes, std::set<RbPtr<DAGNode> >& visitedNodes);

    
};

#endif

