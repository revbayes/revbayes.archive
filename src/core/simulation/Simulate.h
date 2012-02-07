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

const std::string Simulate_name = "Simulate";

class Simulate: public ConstantMemberObject {
    
public:
    Simulate(void);                                                                                                 //!< Default constructor
    Simulate(const Simulate &x);                                                                                        //!< Copy Constructor
    virtual                    ~Simulate(void) {}                                                                   //!< Destructor
    
    // Basic utility functions
    Simulate*                   clone(void) const;                                                              //!< Clone object
    const VectorString&         getClass(void) const;                                                           //!< Get class vector  
    const TypeSpec&             getTypeSpec(void) const;                                                        //!< Get language type of the object 
    
    // Member variable rules
    const MemberRules*          getMemberRules(void) const;                                                     //!< Get member rules
    void                        setMemberVariable(const std::string& name, Variable* var);                //!< Only constants allowed
    
    // Member method inits
    const MethodTable*          getMethods(void) const;                                                         //!< Get methods
    
    // Simulate functions
    void                        run(size_t ndata);                                                               //!< Simulates ndata data elements
    Vector*                     getValues(RbString varName);                                                  //!< Get the values for variable with name varName
    
    
protected:
    RbLanguageObject*           executeOperationSimple(const std::string& name, Environment* args);//!< Execute method
    
    
private:
    static const TypeSpec       typeSpec;
    void                        getOrderedStochasticNodes(DAGNode* dagNode,  std::vector<StochasticNode*>& orderedStochasticNodes, std::set<DAGNode*>& visitedNodes);

    
};

#endif

