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
#include "MemberFunction.h"
#include "Natural.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "Topology.h"
#include "TopologyNode.h"
#include "TreePlate.h"
#include "ValueRule.h"
#include "VectorString.h"



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
    
}


/* Destructor */
TreePlate::~TreePlate(void) {

    // delete the ordering topology
    if (orderingTopology != NULL) {
        orderingTopology->release();
        if (orderingTopology->isUnreferenced()) {
            delete orderingTopology;
        }
    }
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


/* Map calls to member methods */
RbLanguageObject* TreePlate::executeOperation(const std::string& name, Environment& args) {
    
    // special handling for adding a variable
    if (name == "addVariable") {
        // get the name of the variable
        std::string varName = static_cast<const RbString*>( args[0].getValue() )->getValue();
        
        // test whether this variable already exists
        if (hasMember(name)) {
            throw RbException("Variable with name \"" + name + "\" already exists.");
        }
        
        // get the replication parameter
        bool repl = static_cast<const RbBoolean*>( args[2].getValue() )->getValue();
        
        if (repl) {
            if (orderingTopology == NULL) {
                // we don't have a topology and we initialize the member with an empty list
                throw RbException("Cannot add variable with name \"" + name + "\" replicated for each node if the topology was not set.");
            }
            else {
                // we have a tree
                size_t nNodes = orderingTopology->getNumberOfNodes();
                
                // prepare the variable and slot for the variable
                DAGNode* var = args[1].getDagNodePtr();
                ValueRule* varTypeRule = new ValueRule( "", var->getValueType() );
                VariableSlot* slot = new VariableSlot(varName, RbObject_name );
                for (int i=0; i<nNodes; i++) {
 //                   members
 //                   members.addVariable( varName, slot->clone() );
 //                   members[ members.size() - 1 ].setVariable( var );
                }
                
                delete slot;
            }
        } else {
            DAGNode* var = args[1].getDagNodePtr();
            
            // Add an empty variable with type
            members.addVariable( varName, RbObject_name );
            
            // set the variable
            members[ varName ].getVariable()->setDagNode( var );
        }
        
        return NULL;
    }
    else if (name == "nnodes") {
        return new Natural( orderingTopology->getNumberOfNodes() );
    }
    else if (name == "node") {
        // we assume that the node indices in the RevLanguage are from 1:nnodes()
        int index = dynamic_cast<const Natural *>(args.getValue("index"))->getValue() - 1;
        
        return orderingTopology->getNodes()[index];
    }
    else if (name == "index") {
        TopologyNode *theNode = dynamic_cast<TopologyNode*>(args[0].getDagNodePtr()->getValuePtr());
        return new Natural(getNodeIndex(theNode));
    }
    else if (name == "tipIndex") {
        TopologyNode *theNode = dynamic_cast<TopologyNode*>(args[0].getDagNodePtr()->getValuePtr());
        return new Natural(getTipIndex(theNode));
    }
    else {
        return MemberObject::executeOperation( name, args );
    }
}


/** Find the index of the node */
size_t TreePlate::getNodeIndex(TopologyNode *theNode) {
    std::vector<TopologyNode*>& nodes = orderingTopology->getNodes();
    
    size_t index = 0;
    for (; index<nodes.size(); index++) {
        if (theNode->equals( nodes[index] ) ) {
            break;
        }
    }
    
    // return -1 if the node does not exist in the tree
    return (index < nodes.size() ? index + 1 : 0);
}


/** Find the tip-index of the node */
size_t TreePlate::getTipIndex(TopologyNode *theNode) {
    
    size_t index = 0;
    for (; index<orderingTopology->getNumberOfTips(); index++) {
        TopologyNode *theTip = orderingTopology->getTipNode(index);
        if (theNode->equals( theTip ) ) {
            break;
        }
    }
    
    // return -1 if the node does not exist in the tree
    return (index < orderingTopology->getNumberOfTips() ? index + 1 : 0);
}

/* Get method specifications */
const MethodTable& TreePlate::getMethods(void) const {
    
    static MethodTable   methods;
    static ArgumentRules addvariableArgRules;
    static ArgumentRules getNodeIndexArgRules;
    static ArgumentRules getTipIndexArgRules;
    static ArgumentRules nnodesArgRules;
    static ArgumentRules nodeArgRules;
    static bool          methodsSet = false;

    if ( methodsSet == false ) {
            
        // add the 'addVariable()' method
        addvariableArgRules.push_back(  new ValueRule( "name"      , RbString_name  ) );
        addvariableArgRules.push_back(  new ValueRule( ""          , RbObject_name ) );
        addvariableArgRules.push_back(  new ValueRule( "replicate" , Boolean_name  ) );
        
        methods.addFunction("addVariable",  new MemberFunction(RbVoid_name, addvariableArgRules)  );
        
        // add the 'index(node)' method
        getNodeIndexArgRules.push_back( new ValueRule( "node", TopologyNode_name ));
        
        methods.addFunction("index", new MemberFunction(Natural_name, getNodeIndexArgRules));
        
        // add the 'tipIndex(node)' method
        getTipIndexArgRules.push_back( new ValueRule( "node", TopologyNode_name ));
        
        methods.addFunction("tipIndex", new MemberFunction(Natural_name, getTipIndexArgRules));
        
        // add the 'node(i)' method
        nodeArgRules.push_back(  new ValueRule( "index" , Natural_name  ) );
        
        methods.addFunction("node", new MemberFunction(TopologyNode_name, nodeArgRules) );
        
        // add the 'nnodes()' method
        methods.addFunction("nnodes", new MemberFunction(Natural_name, nnodesArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( const_cast<MethodTable*>( &MemberObject::getMethods() ) );
        methodsSet = true;
    }

    return methods;
}


/* Get member rules */
const MemberRules& TreePlate::getMemberRules(void) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if (!rulesSet) 
    {
        TypeSpec varType(Topology_name);
        memberRules.push_back( new ValueRule( "topology" , varType ) );
        
        rulesSet = true;
        }

    return memberRules;
}


/* Print the tree */
void TreePlate::printValue(std::ostream& o) const {

    o << "Tree Plate:\n";
    orderingTopology->printValue(o);
    
    // TODO: print other member too
}


/* Get a lot of information about the TreePlate */
std::string TreePlate::richInfo(void) const {

    std::ostringstream o;
    o <<  "Tree Plate: ";
    printValue(o);
    return o.str();
}

/** Catch setting of the topology variable */
void TreePlate::setMemberVariable(const std::string& name, Variable* var) {
    
    if ( name == "topology" )
        orderingTopology = (Topology*)(var->getValue());
    
    MemberObject::setMemberVariable(name, var);
}


