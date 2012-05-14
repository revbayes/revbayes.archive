/**
 * @file
 * This file contains the implementation of a TreePlate.
 *
 * @brief Implementation of TreePlate
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends MemberObject
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "ConstArgumentRule.h"
#include "DAGNode.h"
#include "DagNodeContainer.h"
#include "MemberFunction.h"
#include "MethodTable.h"
#include "Natural.h"
#include "RbBoolean.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbPtr.h"
#include "RbUtil.h"
#include "RbString.h"
#include "Real.h"
#include "Topology.h"
#include "TopologyNode.h"
#include "TreePlate.h"
#include "VariableSlot.h"
#include "Variable.h"


/* Default constructor */
TreePlate::TreePlate(void) : MemberObject( getMemberRules() ), orderingTopology( NULL ) {
    
//    orderingTopology = NULL;
}


/* Copy constructor */
TreePlate::TreePlate(const TreePlate& t) : MemberObject( t ), orderingTopology( t.orderingTopology ) {
    
    // for now we just do a shallow copy
    nodeVariableNames = t.nodeVariableNames;
    
}


/* Destructor */
TreePlate::~TreePlate(void) {
    
}


/* Build newick string */
std::string TreePlate::buildNewickString(const TopologyNode& node) const {

    // create the newick string
    std::string newick;
    
    // test whether this is a internal or external node
    if (node.isTip()) {
        // this is a tip so we just return the name of the node
        newick = node.getName();
    }
    else {
        newick = "(";
        for (size_t i=0; i<(node.getNumberOfChildren()-1); i++) {
            newick += buildNewickString(node.getChild(i)) + ",";
        }
        newick += buildNewickString(node.getChild(node.getNumberOfChildren()-1)) + ")";
    }
    
    // check whether we have additional information for the node
    if (nodeVariableNames.size() > 0) {
        newick += "[";
        
        // add each variable as a name-value pari
        for (std::vector<std::string>::const_iterator it = nodeVariableNames.begin(); it != nodeVariableNames.end(); it++) {
            
            // get the name of the variable
            const std::string &varName = *it;
            
            // get the container with the variables for this node
            const Container *vars = static_cast<const Container *>( (const RbLanguageObject *) memberVariables[varName].getDagNode()->getValue() );
            
            // get the index of the node
            size_t nodeIndex = getNodeIndex(node) - 1;
            
            // get the variable
            const RbObject& obj = vars->getElement(nodeIndex);
            const Variable& var = static_cast<const VariableSlot&>( obj ).getVariable();
            
            // check if this node also has the parameter (already) set and only if so, add it
            if (var.getDagNode() != NULL) {
                // for every except the first element we need to add a delimiter
                if (it != nodeVariableNames.begin()) {
                    newick += ",";
                }
                std::ostringstream ss;
                var.getDagNode()->getValue()->printValue(ss);
                newick += varName + ":" + ss.str();
            }
        }
        
        newick += "]";
    }
    
    return newick;
}


/* Clone function */
TreePlate* TreePlate::clone(void) const {
    
    return new TreePlate(*this);
}


/** Get class name of object */
const std::string& TreePlate::getClassName(void) { 
    
    static std::string rbClassName = "Tree Plate";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& TreePlate::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( MemberObject::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& TreePlate::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}




/* Map calls to member methods */
RbPtr<RbLanguageObject> TreePlate::executeOperationSimple(const std::string& name, const std::vector<RbPtr<Argument> >& args) {
    
    return MemberObject::executeOperationSimple( name, args );

}

/* Get method specifications */
const MethodTable& TreePlate::getMethods(void) const {
    
    static MethodTable methods = MethodTable();
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) {
        
        // necessary call for proper inheritance
        methods.setParentTable( &MemberObject::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}


/* Get member rules */
const MemberRules& TreePlate::getMemberRules(void) const {
    
    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;
    
    if (!rulesSet) 
    {
        memberRules.push_back( new ConstArgumentRule( "topology" , Topology::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}





/** Find the index of the node */
size_t TreePlate::getNodeIndex(const TopologyNode& theNode) const {
    const std::vector<const TopologyNode*>& nodes = orderingTopology->getNodes();
    
    size_t index = 0;
    for (; index<nodes.size(); index++) {
        const TopologyNode* node = nodes[index];
        if (theNode.equals( *node ) ) {
            break;
        }
    }
    
    if (index == nodes.size()) {
        std::cerr << "Ooops, could not find node!" << std::endl;
        for (size_t idx = 0; idx<nodes.size(); idx++) {
            const TopologyNode& node = *nodes[idx];
            if (theNode.equals( node ) ) {
                break;
            }
        }
    }
    
    // return -1 if the node does not exist in the tree
    return (index < nodes.size() ? index + 1 : 0);
}


/** Find the tip-index of the node */
size_t TreePlate::getTipIndex(const TopologyNode& theNode) const {
    
    size_t index = 0;
    for (; index<orderingTopology->getNumberOfTips(); index++) {
        const TopologyNode& theTip = orderingTopology->getTipNode(index);
        if (theNode.equals( theTip ) ) {
            break;
        }
    }
    if (index == orderingTopology->getNumberOfTips()) {
        if (theNode.isTip()){
            std::cout << "Ooops ..." << std::endl;
        }
        if (theNode.equals(orderingTopology->getTipNode(index))) {
            std::cout << "Hm, wrong numbering scheme ... ?" << std::endl;
        }
    }
    
    // return -1 if the node does not exist in the tree
    return (index < orderingTopology->getNumberOfTips() ? index + 1 : 0);
}



const Topology& TreePlate::getTopology(void) const {
    return *orderingTopology;
}


/* Print the tree */
void TreePlate::printValue(std::ostream& o) const {
    
    o << buildNewickString( orderingTopology->getRoot() );
    
}



/** Catch setting of the topology variable */
void TreePlate::setMemberVariable(const std::string& name, const RbPtr<RbLanguageObject> &var) {
    
    if ( name == "topology" ) {
        orderingTopology = static_cast<Topology*>( (RbLanguageObject*)var );
    }
    else {
        MemberObject::setMemberVariable(name, var);
    }
}


/** Set the variable with identifier for a node */
void TreePlate::setNodeVariable(const TopologyNode &node, std::string const &name, RbLanguageObject *value) {
    
    setNodeVariable(node, name, Variable( new ConstantNode(value) ) );
}


/** Set the variable with identifier for a node */
void TreePlate::setNodeVariable(const TopologyNode &node, std::string const &name, const Variable& value) {

    // check if a container already exists with that name
    if (!memberVariables.existsVariable( name )) 
    {
        // we don't have a container for this variable name yet
        // so we just create one
        Variable* var = new Variable( new ConstantNode( new DagNodeContainer( orderingTopology->getNumberOfNodes() ) ) );
        
        memberVariables.addVariable(name, var );
        
        // and we add it to our names list
        nodeVariableNames.push_back(name);
    }
    
    // get the container with the variables for this node
    DagNodeContainer *vars = static_cast<DagNodeContainer *>( (RbLanguageObject *) memberVariables[name].getValue() );
    
    // get the index of the node
    size_t nodeIndex = getNodeIndex(node);
    
    // set the variable
    vars->setElement(nodeIndex - 1, value.clone() );
}

