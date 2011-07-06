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
#include "MemberFunction.h"
#include "Natural.h"
#include "RbException.h"
#include "RbNames.h"
#include "ReferenceRule.h"
#include "Topology.h"
#include "TopologyNode.h"
#include "VectorString.h"



/* Default constructor */
Topology::Topology(void) : MemberObject( getMemberRules() ) {

}


/* Copy constructor */
Topology::Topology(const Topology& t) : MemberObject( getMemberRules() ) {

    // need to perform a deep copy of the tree nodes
}


/* Destructor */
Topology::~Topology(void) {

    for (std::vector<TopologyNode*>::iterator p = nodes.begin(); p != nodes.end(); p++)
        delete (*p);
}


/* Clone function */
Topology* Topology::clone(void) const {

    return new Topology(*this);
}


/* Get class information */
const VectorString& Topology::getClass(void) const {
    
    static VectorString rbClass = VectorString(Topology_name) + MemberObject::getClass();
    return rbClass;
}


/** Calculate and return the number of tips on the tree by going through the vector
    of nodes, querying each about its tip status. */
size_t Topology::getNumberOfTips(void) const {

    size_t n = 0;
    for (size_t i=0; i<nodes.size(); i++)
        {
        if (nodes[i]->isTip() == true)
            n++;
        }
    return n;
}


/* Map calls to member methods */
DAGNode* Topology::executeOperation(const std::string& name, ArgumentFrame& args) {
    
    if (name == "ntips") 
        {
        return ( new Natural((int)getNumberOfTips()) )->wrapIntoVariable();
        }
    else if (name == "nnodes")
        {
        return ( new Natural((int)getNumberOfNodes()) )->wrapIntoVariable();
        }

    return MemberObject::executeOperation( name, args );
}


/* Get method specifications */
const MethodTable& Topology::getMethods(void) const {
    
    static MethodTable   methods;
    static ArgumentRules ntipsArgRules;
    static ArgumentRules nnodesArgRules;
    static bool          methodsSet = false;

    if ( methodsSet == false ) 
        {
        // this must be here so the parser can distinguish between different instances of a character matrix
        ntipsArgRules.push_back(  new ReferenceRule( "", MemberObject_name ) );
        nnodesArgRules.push_back( new ReferenceRule( "", MemberObject_name ) );
        
        methods.addFunction("ntips",  new MemberFunction(Natural_name, ntipsArgRules)  );
        methods.addFunction("nnodes", new MemberFunction(Natural_name, nnodesArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( const_cast<MethodTable*>( &MemberObject::getMethods() ) );
        methodsSet = true;
        }

    return methods;
}


/* Get member rules */
const MemberRules& Topology::getMemberRules(void) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if (!rulesSet) 
        {
        rulesSet = true;
        }

    return memberRules;
}


/* Print the tree */
void Topology::printValue(std::ostream& o) const {

    o << "";
}


/* Get a lot of information about the topology */
std::string Topology::richInfo(void) const {

    std::ostringstream o;
    o <<  "Tree: ";
    printValue(o);
    return o.str();
}


