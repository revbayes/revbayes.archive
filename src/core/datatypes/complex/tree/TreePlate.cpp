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


// Definition of the static type spec member
const TypeSpec TreePlate::typeSpec(TreePlate_name);

/* Default constructor */
TreePlate::TreePlate(void) : MutableMemberObject( getMemberRules() ) {
    
    orderingTopology = NULL;
}

/* constructor */
TreePlate::TreePlate(Topology* top) : MutableMemberObject( getMemberRules() ) {
    
    orderingTopology = top;
}


/* Copy constructor */
TreePlate::TreePlate(const TreePlate& t) : MutableMemberObject( t ) {
    
    if (t.orderingTopology != NULL)
        orderingTopology = t.orderingTopology->clone();
    else
        orderingTopology = NULL;
}


/* Destructor */
TreePlate::~TreePlate(void) {
    
    if (orderingTopology != NULL) {
        delete orderingTopology;
    }
}


/* Build newick string */
std::string TreePlate::buildNewickString(const TopologyNode* node) const {
    // create the newick string
    std::string newick;
    
    // test whether this is a internal or external node
    if (node->isTip()) {
        // this is a tip so we just return the name of the node
        newick = node->getName();
    }
    else {
        newick = "(";
        for (size_t i=0; i<(node->getNumberOfChildren()-1); i++) {
            newick += buildNewickString(node->getChild(i)) + ",";
        }
        newick += buildNewickString(node->getChild(node->getNumberOfChildren()-1)) + ")";
    }
    
    // check whether we have additional information for the node
    if (nodeVariableNames.size() > 0) {
        newick += "[";
        
        // add each variable as a name-value pari
        for (std::vector<std::string>::const_iterator it = nodeVariableNames.begin(); it != nodeVariableNames.end(); it++) {
            
            // get the name of the variable
            const std::string &varName = *it;
            
            // get the container with the variables for this node
            const RbObject& vars = (*members)[varName]->getDagNode()->getValue();
            
            // get the index of the node
            size_t nodeIndex = getNodeIndex(node) - 1;
            
            // get the variable
            const RbObject& obj = vars.getElement(nodeIndex);
            const Variable* var = static_cast<const VariableSlot&>( obj ).getVariable();
            
            // check if this node also has the parameter (already) set and only if so, add it
            if (var->getDagNode() != NULL) {
                // for every except the first element we need to add a delimiter
                if (it != nodeVariableNames.begin()) {
                    newick += ",";
                }
                std::ostringstream ss;
                var->getDagNode()->getValue().printValue(ss);
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


/* Get class information */
const VectorString& TreePlate::getClass(void) const {
    
    static VectorString rbClass = VectorString(TreePlate_name) + MemberObject::getClass();
    return rbClass;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& TreePlate::getTypeSpec(void) const {
    return typeSpec;
}


/* Map calls to member methods */
RbLanguageObject* TreePlate::executeOperation(const std::string& name, Environment* args) {

    if (name == "getVariable") {
        // get the name of the variable
        const std::string& varName = static_cast<const RbString*>( (const RbObject*)(*args)[0]->getValue() )->getValue();
        
        // check if a container already exists with that name
        if (!members->existsVariable(varName)) {
            // we don't have a container for this variable name yet
            // so we need to throw an error
            
            throw RbException("Variable with name \"" + varName + "\" does not exist.");
        }
        else {
            // get the container with the variables for this node
            DagNodeContainer *vars = static_cast<DagNodeContainer*>( (RbObject*)(*members)[varName]->getValue() );
            
            // get the node we want to associate it too
            const TopologyNode* theNode = static_cast<const TopologyNode*>( (*args)[1]->getValue() );
            
            // get the index of the node
            size_t nodeIndex = getNodeIndex(theNode) - 1;
            
            // get the variable
            Variable* var = static_cast<VariableSlot&>( vars->getElement(nodeIndex) ).getVariable();
            
            return static_cast<RbLanguageObject*>( var->getDagNode() );
        }
    }

    
    return MutableMemberObject::executeOperation(name, args);
}

/* Map calls to member methods */
RbLanguageObject* TreePlate::executeOperationSimple(const std::string& name, Environment* args) {
    
    // special handling for adding a variable
    if (name == "addVariable") {
        // get the name of the variable
        const std::string& varName = static_cast<const RbString*>( (*args)[0]->getValue() )->getValue();
        
        // check if a container already exists with that name
        if (!members->existsVariable(varName)) {
            // we don't have a container for this variable name yet
            // so we just create one
            Variable* var = new Variable( new ConstantNode( new DagNodeContainer(orderingTopology->getNumberOfNodes() ) ) );
            
            members->addVariable(varName, var );
            
            // and we add it to our names list
            nodeVariableNames.push_back(varName);
        }
        
        // get the container with the variables for this node
        DagNodeContainer *vars = static_cast<DagNodeContainer*>( (*members)[varName]->getValue() );
        
        // get the variable
        Environment* args2 = args;
        Variable* var = (*args2)[1]->getVariable();
        
        // get the node we want to associate it too
        const TopologyNode* theNode = static_cast<const TopologyNode*>( (*args)[2]->getValue() );
        
        // get the index of the node
        size_t nodeIndex = getNodeIndex(theNode);
        
        // set the variable
        vars->setElement(nodeIndex - 1, var );
        
        return NULL;
    }
    else if (name == "nnodes") {
        return new Natural( orderingTopology->getNumberOfNodes() );
    }
    else if (name == "node") {
        // we assume that the node indices in the RevLanguage are from 1:nnodes()
        int index = dynamic_cast<const Natural *>( args->getValue("index") )->getValue() - 1;
        
        return orderingTopology->getNodes()[index];
    }
    else if (name == "index") {
        const TopologyNode* theNode = dynamic_cast<const TopologyNode*>( (*args)[0]->getValue() );
        return new Natural(getNodeIndex(theNode) );
    }
    else if (name == "tipIndex") {
        const TopologyNode* theNode = dynamic_cast<const TopologyNode*>( (*args)[0]->getValue() );
        size_t tIndex = getTipIndex(theNode);
        return new Natural(tIndex);
    }
    else {
        return MemberObject::executeOperationSimple( name, args );
    }
}

/* Get method specifications */
const MethodTable* TreePlate::getMethods(void) const {
    
    static MethodTable* methods = new MethodTable();
    static ArgumentRules* addVariableArgRules = new ArgumentRules();
    static ArgumentRules* getVariableArgRules = new ArgumentRules();
    static ArgumentRules* getNodeIndexArgRules = new ArgumentRules();
    static ArgumentRules* getTipIndexArgRules = new ArgumentRules();
    static ArgumentRules* nnodesArgRules = new ArgumentRules();
    static ArgumentRules* nodeArgRules = new ArgumentRules();
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) {
        
        // add the 'addVariable()' method
        addVariableArgRules->push_back( new ValueRule( "name"      , RbString_name )     );
        addVariableArgRules->push_back( new ValueRule( ""          , RbObject_name )     );
        addVariableArgRules->push_back( new ValueRule( "node"      , TopologyNode_name ) );
        
        methods->addFunction("addVariable", new MemberFunction(RbVoid_name, addVariableArgRules) );
        
        // add the 'getVariable()' method
        getVariableArgRules->push_back( new ValueRule( "name"      , RbString_name )     );
        getVariableArgRules->push_back( new ValueRule( "node"      , TopologyNode_name ) );
        
        methods->addFunction("getVariable", new MemberFunction(RbObject_name, getVariableArgRules) );
        
        // add the 'index(node)' method
        getNodeIndexArgRules->push_back( new ValueRule( "node", TopologyNode_name ) );
        
        methods->addFunction("index", new MemberFunction(Natural_name, getNodeIndexArgRules) );
        
        // add the 'tipIndex(node)' method
        getTipIndexArgRules->push_back( new ValueRule( "node", TopologyNode_name ) );
        
        methods->addFunction("tipIndex", new MemberFunction(Natural_name, getTipIndexArgRules) );
        
        // add the 'node(i)' method
        nodeArgRules->push_back( new ValueRule( "index" , Natural_name  ) );
        
        methods->addFunction("node", new MemberFunction(TopologyNode_name, nodeArgRules) );
        
        // add the 'nnodes()' method
        methods->addFunction("nnodes", new MemberFunction(Natural_name, nnodesArgRules) );
        
        // necessary call for proper inheritance
        methods->setParentTable( MemberObject::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}


/* Get member rules */
const MemberRules* TreePlate::getMemberRules(void) const {
    
    static MemberRules* memberRules = new MemberRules();
    static bool        rulesSet = false;
    
    if (!rulesSet) 
    {
        TypeSpec varType(Topology_name);
        memberRules->push_back( new ValueRule( "topology" , varType ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}





/** Find the index of the node */
size_t TreePlate::getNodeIndex(const TopologyNode* theNode) const {
    const std::vector<TopologyNode* >& nodes = orderingTopology->getNodes();
    
    size_t index = 0;
    for (; index<nodes.size(); index++) {
        const TopologyNode* node = nodes[index];
        if (theNode->equals( node ) ) {
            break;
        }
    }
    
    // return -1 if the node does not exist in the tree
    return (index < nodes.size() ? index + 1 : 0);
}


/** Find the tip-index of the node */
size_t TreePlate::getTipIndex(const TopologyNode* theNode) const {
    
    size_t index = 0;
    for (; index<orderingTopology->getNumberOfTips(); index++) {
        const TopologyNode* theTip = orderingTopology->getTipNode(index);
        if (theNode->equals( theTip ) ) {
            break;
        }
    }
    if (index == orderingTopology->getNumberOfTips()) {
        if (theNode->isTip()){
            std::cout << "Ooops ..." << std::endl;
        }
        if (theNode->equals(orderingTopology->getTipNode(index))) {
            std::cout << "Hm, wrong numbering scheme ... ?" << std::endl;
        }
    }
    
    // return -1 if the node does not exist in the tree
    return (index < orderingTopology->getNumberOfTips() ? index + 1 : 0);
}



const Topology* TreePlate::getTopology(void) const {
    return orderingTopology;
}


/* Print the tree */
void TreePlate::printValue(std::ostream& o) const {
    
    o << buildNewickString(orderingTopology->getRoot());
    
}



/** Catch setting of the topology variable */
void TreePlate::setMemberVariable(const std::string& name, Variable* var) {
    
    if ( name == "topology" ) {
        RbLanguageObject& obj = var->getValue();
        orderingTopology = static_cast<Topology*>( obj.clone() );
    }
    
    MemberObject::setMemberVariable(name, var);
}


