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


/** 
 * Change topology according to the instructions given in the vector
 * of TopologyChange structs. This allows the topology move machinery
 * to do delayed updates of the topology and use the change instructions
 * to alos update tree variable DAGs.
 *
 * @note We assume here that indices of nodes correspond to their
 *       position in the nodes vector.
 * @note No consistency checking - should probably be introduced
 */
void Topology::changeTopology( std::vector<TopologyChange>& topChanges ) {

    for ( std::vector<TopologyChange>::iterator i=topChanges.begin(); i!=topChanges.end(); i++ ) {
    
        TopologyNode* childNode = nodes[ (*i).node ];
        TopologyNode* oldParent = nodes[ (*i).oldParentNode ];
        TopologyNode* newParent = nodes[ (*i).newParentNode ];

        childNode->setParent  ( newParent );
        oldParent->removeChild( childNode );
        newParent->addChild   ( childNode );
    }
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


/** Calculate the number of interior nodes in the tree by deducing the number of
    tips from number of nodes, and then subtract 1 more if the tree is rooted. */
size_t Topology::getNumberOfInteriorNodes(void) const {

    size_t preliminaryNumIntNodes = getNumberOfNodes() - getNumberOfTips();

    if ( isRooted )
        return preliminaryNumIntNodes - 1;
    else
        return preliminaryNumIntNodes;
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


/** We provide this function to allow a caller to randomly pick one of the interior nodes.
    This version assumes that the root is always the last and the tips the first in the nodes vector. */
TopologyNode* Topology::getInteriorNode( int indx ) const {

    // TODO: Bound checking, maybe draw from downpass array instead
    return nodes[ indx + getNumberOfTips() ];
}


/** We provide this function to allow a caller to randomly pick one of the interior nodes.
    This version assumes that the tips are first in the nodes vector. */
TopologyNode* Topology::getTipNode( int indx ) const {

    // TODO: Bound checking
    return nodes[ indx ];
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


