/**
 * @file
 * This file contains the declaration of Simulate, which is used to hold
 * information about and run a simulation given a model and monitors.
 *
 * @brief Declaration of Simulate
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface Simulate
 * @package distributions
 *
 * $Id$
 */

#ifndef Simulate_H
#define Simulate_H

#include "MemberObject.h"
#include "RbString.h"
#include "RlVector.h"

#include <set>
#include <string>

class ArgumentRule;
class DAGNode;
class Model;
class InferenceMonitor;


class Simulate: public MemberObject {
    
public:
    Simulate(void);                                                                                                     //!< Default constructor
    
    // Basic utility functions
    Simulate*                   clone(void) const;                                                                      //!< Clone object
    static const std::string&   getClassName(void);                                                                     //!< Get class name
    static const TypeSpec&      getClassTypeSpec(void);                                                                 //!< Get class type spec
    const TypeSpec&             getTypeSpec(void) const;                                                                //!< Get language type of the object 
    
    // Member variable rules
    const MemberRules&          getMemberRules(void) const;                                                             //!< Get member rules
    
    // Member method inits
    const MethodTable&          getMethods(void) const;                                                                 //!< Get methods
    
    // Simulate functions
    void                        run(size_t ndata);                                                                      //!< Simulates ndata data elements
    
    
protected:
    RbPtr<RbLanguageObject>     executeSimpleMethod(const std::string& name, const std::vector<const RbObject *>& args); //!< Execute method
    
    
private:
    void                        extractDagNodesFromModel( const Model& source );
    void                        getOrderedStochasticNodes(DAGNode* dagNode,  std::vector<StochasticNode*>& orderedStochasticNodes, std::set<DAGNode*>& visitedNodes);

    std::vector<RbPtr<DAGNode> > dagNodes;
    std::vector<Monitor*>       monitors;
};

#endif

