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
#include "RbString.h"
#include "Vector.h"

#include <set>
#include <string>

class ArgumentRule;
class DAGNode;
class Model;
class VectorString;


class Simulate: public ConstantMemberObject {
    
public:
    Simulate(void);                                                                                                     //!< Default constructor
    Simulate(const Simulate &x);                                                                                        //!< Copy Constructor
    virtual                    ~Simulate(void) {}                                                                       //!< Destructor
    
    // Basic utility functions
    Simulate*                   clone(void) const;                                                                      //!< Clone object
    static const std::string&   getClassName(void);                                                                     //!< Get class name
    static const TypeSpec&      getClassTypeSpec(void);                                                                 //!< Get class type spec
    const TypeSpec&             getTypeSpec(void) const;                                                                //!< Get language type of the object 
    
    // Member variable rules
    const MemberRules&          getMemberRules(void) const;                                                             //!< Get member rules
    void                        setMemberVariable(const std::string& name, Variable* var);                              //!< Only constants allowed
    
    // Member method inits
    const MethodTable&          getMethods(void) const;                                                                 //!< Get methods
    
    // Simulate functions
    void                        run(size_t ndata);                                                                      //!< Simulates ndata data elements
    
    
protected:
    const RbLanguageObject&     executeOperationSimple(const std::string& name, const std::vector<Argument>& args);     //!< Execute method
    
    
private:
    void                        getOrderedStochasticNodes(DAGNode* dagNode,  std::vector<StochasticNode*>& orderedStochasticNodes, std::set<DAGNode*>& visitedNodes);

    // parameters
    RbVariablePtr               model;
    RbVariablePtr               monitors;
};

#endif

