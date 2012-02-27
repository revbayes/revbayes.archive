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
#include "RbBoolean.h"
#include "ConstantNode.h"
#include "DagNodeContainer.h"
#include "MemberFunction.h"
#include "Natural.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "RbString.h"
#include "Real.h"
#include "Topology.h"
#include "TopologyNode.h"
#include "TreePlate.h"
#include "ValueRule.h"
#include "VariableSlot.h"
#include "Variable.h"
#include "VectorString.h"


/* Default constructor */
TreePlate::TreePlate(void) : MutableMemberObject( getMemberRules() ), orderingTopology( NULL ) {
    
//    orderingTopology = NULL;
}


/* Copy constructor */
TreePlate::TreePlate(const TreePlate& t) : MutableMemberObject( t ), orderingTopology( t.orderingTopology ) {
    
    // for now we just do a shallow copy
    nodeVariableNames = t.nodeVariableNames;
    
    
//    if (t.orderingTopology != NULL)
//        orderingTopology = t.orderingTopology->clone();
//    else
//        orderingTopology = NULL;
    
    // clear the member variables because we need to set the correct matches
//    memberVariables.clear();
}


/* Destructor */
TreePlate::~TreePlate(void) {
    
//    if (orderingTopology != NULL) {
//        delete orderingTopology;
//    }
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
            const RbObject& vars = memberVariables[varName].getDagNode()->getValue();
            
            // get the index of the node
            size_t nodeIndex = getNodeIndex(node) - 1;
            
            // get the variable
            const RbObject& obj = vars.getElement(nodeIndex);
            const Variable& var = static_cast<const VariableSlot&>( obj ).getVariable();
            
            // check if this node also has the parameter (already) set and only if so, add it
            if (var.getDagNode() != NULL) {
                // for every except the first element we need to add a delimiter
                if (it != nodeVariableNames.begin()) {
                    newick += ",";
                }
                std::ostringstream ss;
                var.getDagNode()->getValue().printValue(ss);
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
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( MutableMemberObject::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& TreePlate::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/* Map calls to member methods */
const RbLanguageObject& TreePlate::executeOperation(const std::string& name, const std::vector<Argument>& args) {

    if (name == "getVariable") {
        // get the name of the variable
        const std::string& varName = static_cast<const RbString&>( args[0].getVariable().getValue() ).getValue();
        
        // check if a container already exists with that name
        if (!memberVariables.existsVariable(varName)) {
            // we don't have a container for this variable name yet
            // so we need to throw an error
            
            throw RbException("Variable with name \"" + varName + "\" does not exist.");
        }
        else {
            // get the container with the variables for this node
            DagNodeContainer& vars = static_cast<DagNodeContainer&>( memberVariables[varName].getValue() );
            
            // get the node we want to associate it too
            const TopologyNode& theNode = static_cast<const TopologyNode&>( args[1].getVariable().getValue() );
            
            // get the index of the node
            size_t nodeIndex = getNodeIndex(theNode) - 1;
            
            // get the variable
            Variable& var = static_cast<VariableSlot&>( vars.getElement(nodeIndex) ).getVariable();
            
//            return static_cast<RbLanguageObject*>( var.getDagNode() );
            return RbNullObject::getInstance();
        }
    }

    
    return MutableMemberObject::executeOperation(name, args);
}

/* Map calls to member methods */
const RbLanguageObject& TreePlate::executeOperationSimple(const std::string& name, const std::vector<Argument>& args) {
    
    // special handling for adding a variable
    if (name == "addVariable") 
        {
        // get the name of the variable
        const std::string& varName  = static_cast<const RbString&>( args[0].getVariable().getValue() ).getValue();
        const RbVariablePtr& theVar = args[1].getVariablePtr();
        const TopologyNode& theNode = static_cast<const TopologyNode&>( args[2].getVariable().getValue() );
        
        setNodeVariable(theNode, varName, theVar);
        
        return RbNullObject::getInstance();
        }
    else if (name == "nnodes") 
        {
            numNodes.setValue( static_cast<const Topology&>( orderingTopology->getValue() ).getNumberOfNodes() );
        return numNodes;
        }
    else if (name == "node") 
        {
        // we assume that the node indices in the RevLanguage are from 1:nnodes()
        int index = dynamic_cast<const Natural &>( args[0].getVariable().getValue() ).getValue() - 1;
        
        return *static_cast<const Topology&>( orderingTopology->getValue() ).getNodes()[index];
        }
    else if (name == "index") 
        {
        const TopologyNode& theNode = dynamic_cast<const TopologyNode&>( args[0].getVariable().getValue() );
        nodeIndex.setValue( getNodeIndex( theNode ) );
        return nodeIndex;
        }
    else if (name == "tipIndex") 
        {
        const TopologyNode& theNode = dynamic_cast<const TopologyNode&>( args[0].getVariable().getValue() );
        size_t tIndex = getTipIndex(theNode);
        tipIndex.setValue( tIndex );
        return tipIndex;
        }
    else 
        {
        return MutableMemberObject::executeOperationSimple( name, args );
        }
}

/* Get method specifications */
const MethodTable& TreePlate::getMethods(void) const {
    
    static MethodTable methods = MethodTable();
    static ArgumentRules* addVariableArgRules = new ArgumentRules();
    static ArgumentRules* getVariableArgRules = new ArgumentRules();
    static ArgumentRules* getNodeIndexArgRules = new ArgumentRules();
    static ArgumentRules* getTipIndexArgRules = new ArgumentRules();
    static ArgumentRules* nnodesArgRules = new ArgumentRules();
    static ArgumentRules* nodeArgRules = new ArgumentRules();
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) {
        
        // add the 'addVariable()' method
        addVariableArgRules->push_back( new ValueRule( "name"      , RbString::getClassTypeSpec() )     );
        addVariableArgRules->push_back( new ValueRule( "value"     , RbObject::getClassTypeSpec() )     );
        addVariableArgRules->push_back( new ValueRule( "node"      , TopologyNode::getClassTypeSpec() ) );
        
        methods.addFunction("addVariable", new MemberFunction(RbVoid_name, addVariableArgRules) );
        
        // add the 'getVariable()' method
        getVariableArgRules->push_back( new ValueRule( "name"      , RbString::getClassTypeSpec() )     );
        getVariableArgRules->push_back( new ValueRule( "node"      , TopologyNode::getClassTypeSpec() ) );
        
        methods.addFunction("getVariable", new MemberFunction(RbObject::getClassTypeSpec() , getVariableArgRules) );
        
        // add the 'index(node)' method
        getNodeIndexArgRules->push_back( new ValueRule( "node", TopologyNode::getClassTypeSpec() ) );
        
        methods.addFunction("index", new MemberFunction(Natural::getClassTypeSpec(), getNodeIndexArgRules) );
        
        // add the 'tipIndex(node)' method
        getTipIndexArgRules->push_back( new ValueRule( "node", TopologyNode::getClassTypeSpec() ) );
        
        methods.addFunction("tipIndex", new MemberFunction(Natural::getClassTypeSpec(), getTipIndexArgRules) );
        
        // add the 'node(i)' method
        nodeArgRules->push_back( new ValueRule( "index" , Natural::getClassTypeSpec()  ) );
        
        methods.addFunction("node", new MemberFunction(TopologyNode::getClassTypeSpec(), nodeArgRules) );
        
        // add the 'nnodes()' method
        methods.addFunction("nnodes", new MemberFunction(Natural::getClassTypeSpec(), nnodesArgRules) );
        
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
        memberRules.push_back( new ValueRule( "topology" , Topology::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}





/** Find the index of the node */
size_t TreePlate::getNodeIndex(const TopologyNode& theNode) const {
    const std::vector<const TopologyNode*>& nodes = static_cast<const Topology&>( orderingTopology->getValue() ).getNodes();
    
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
    for (; index<static_cast<const Topology&>( orderingTopology->getValue() ).getNumberOfTips(); index++) {
        const TopologyNode& theTip = static_cast<const Topology&>( orderingTopology->getValue() ).getTipNode(index);
        if (theNode.equals( theTip ) ) {
            break;
        }
    }
    if (index == static_cast<const Topology&>( orderingTopology->getValue() ).getNumberOfTips()) {
        if (theNode.isTip()){
            std::cout << "Ooops ..." << std::endl;
        }
        if (theNode.equals(static_cast<const Topology&>( orderingTopology->getValue() ).getTipNode(index))) {
            std::cout << "Hm, wrong numbering scheme ... ?" << std::endl;
        }
    }
    
    // return -1 if the node does not exist in the tree
    return (index < static_cast<const Topology&>( orderingTopology->getValue() ).getNumberOfTips() ? index + 1 : 0);
}



const Topology& TreePlate::getTopology(void) const {
    return static_cast<const Topology&>( orderingTopology->getValue() );
}


/* Print the tree */
void TreePlate::printValue(std::ostream& o) const {
    
    o << buildNewickString(static_cast<const Topology&>( orderingTopology->getValue() ).getRoot());
    
}



/** Catch setting of the topology variable */
void TreePlate::setMemberVariable(const std::string& name, Variable* var) {
    
    if ( name == "topology" ) {
        orderingTopology = var;
    }
    else {
        MemberObject::setMemberVariable(name, var);
    }
}


/** Set the variable with identifier for a node */
void TreePlate::setNodeVariable(const TopologyNode &node, std::string const &name, RbLanguageObject *value) {
    
    setNodeVariable(node, name, new Variable( new ConstantNode(value) ) );
}


/** Set the variable with identifier for a node */
void TreePlate::setNodeVariable(const TopologyNode &node, std::string const &name, Variable *value) {
    // check if a container already exists with that name
    if (!memberVariables.existsVariable( name )) 
    {
        // we don't have a container for this variable name yet
        // so we just create one
        Variable* var = new Variable( new ConstantNode( new DagNodeContainer( static_cast<const Topology&>( orderingTopology->getValue() ).getNumberOfNodes() ) ) );
        
        memberVariables.addVariable(name, var );
        
        // and we add it to our names list
        nodeVariableNames.push_back(name);
    }
    
    // get the container with the variables for this node
    DagNodeContainer& vars = static_cast<DagNodeContainer&>( memberVariables[name].getValue() );
    
    // get the index of the node
    size_t nodeIndex = getNodeIndex(node);
    
    // set the variable
    vars.setElement(nodeIndex - 1, value->clone() );
}

