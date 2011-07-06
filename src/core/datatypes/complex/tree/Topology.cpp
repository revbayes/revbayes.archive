/**
 * @file
 * This file contains the implementation of a Topology.
 *
 * @brief Implementation of Topology
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends RbComplex
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "ContainerNode.h"
#include "Integer.h"
#include "MemberFunction.h"
#include "RbException.h"
#include "RbNames.h"
#include "Topology.h"
#include "TopologyNode.h"
#include "VectorString.h"



/** Default constructor */
Topology::Topology(void) : MemberObject( getMemberRules() ) {

    topologyChanged = true;     // make sure we calculate the newick string at the first call
}


/** constructor */
Topology::Topology(const MemberRules& memberRules) : MemberObject( memberRules ) {

    topologyChanged = true;     // make sure we calculate the newick string at the first call
    
}

/** copy constructor */
Topology::Topology(const Topology& t) : MemberObject( getMemberRules() ) {

    topologyChanged = true;     // make sure we calculate the newick string at the first call
}


/** recompute the newick string */
void Topology::computeNewickString() {

    // TODO: implement this!!!
    newick = "computation of newick string not yet implemented";
}


const VectorString& Topology::getClass(void) const {
    
    static VectorString rbClass = VectorString(Topology_name) + MemberObject::getClass();
    return rbClass;
}


/** Do not allow the user to set the index */
void Topology::setVariable(const std::string& name, DAGNode* var) {
    
    MemberObject::setVariable(name, var);
}


/** Map calls to member methods */
DAGNode* Topology::executeOperation(const std::string& name, ArgumentFrame& args) {
    
    if (name == "getNodes") 
        {
        ContainerNode* plate = new ContainerNode(TopologyNode_name, int(nodes.size()));
        for (size_t i=0; i<nodes.size(); i++) 
            {
//            plate->setValue(nodes[i]);
            }
        return plate;
        }
    else if (name == "getNumberOfNodes") 
        {
        return new ConstantNode( new Integer((int)nodes.size()));
        }
    else if (name == "getRoot") 
        {
        //return new ConstantNode( root );
        }
    else if (name == "getTips") 
        {
        ContainerNode* plate = new ContainerNode(TopologyNode_name,tips.size());
        size_t j = 0;
        for (size_t i=0; i<nodes.size(); i++) 
            {
            if ( nodes[i]->isTip() ) 
                {
 //               plate[j].setValue(nodes[i]);
                j++;
                }
            }
        return plate;
        }
    else
        throw RbException("No member method called '" + name + "' for object of type Topology");
    
    return NULL;
}


/** Get method specifications */
const MethodTable& Topology::getMethods(void) const {
    
    static MethodTable      methodInits;
    static ArgumentRules    isTipRules;
    static bool             initsSet = false;
    
    if (!initsSet) 
        {
        // add a function returning all nodes of the Topology
        methodInits.addFunction("getNodes", new MemberFunction(TopologyNode_name, ArgumentRules()));
        
        // add a function returning the number of nodes
        methodInits.addFunction("getNumberOfNodes", new MemberFunction(Boolean_name, ArgumentRules()));
        
        // add a function returning the root of the Topology
        methodInits.addFunction("getRoot", new MemberFunction(TopologyNode_name, ArgumentRules()));
        
        // add a function returning all the tips
        methodInits.addFunction("getTips", new MemberFunction(Boolean_name, ArgumentRules()));
        
        
        //        methodInits.addFunction("getNumberOfNodes", new MemberFunction(Boolean_name, ArgumentRules()));
        
        initsSet = true;
        }
    
    return methodInits;
}


const MemberRules& Topology::getMemberRules(void) const {
    
    static MemberRules memberRules;
    static bool        rulesSet = false;
    
    if (!rulesSet) 
        {
        
        rulesSet = true;
        }
    
    return memberRules;
}


void Topology::printValue(std::ostream& o) const {

    o << newick;
}


std::string Topology::richInfo(void) const {

    return newick;
}