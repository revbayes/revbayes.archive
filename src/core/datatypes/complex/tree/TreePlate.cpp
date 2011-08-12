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
#include "Boolean.h"
#include "ConstantNode.h"
#include "List.h"
#include "MemberFunction.h"
#include "Natural.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "Topology.h"
#include "TreePlate.h"
#include "ValueRule.h"
#include "VectorString.h"



/* Default constructor */
TreePlate::TreePlate(void) : MutableMemberObject( getMemberRules() ) {

    orderingTopology = NULL;
    treePlateVariableTypeRule = new ValueRule( "", TypeSpec( List_name ) );
}

/* constructor */
TreePlate::TreePlate(Topology* top) : MutableMemberObject( getMemberRules() ) {
    
    orderingTopology = top;
    treePlateVariableTypeRule = new ValueRule( "", TypeSpec( List_name ) );
}


/* Copy constructor */
TreePlate::TreePlate(const TreePlate& t) : MutableMemberObject( t ) {

    if (t.orderingTopology != NULL)
        orderingTopology = t.orderingTopology->clone();
    
    treePlateVariableTypeRule = t.treePlateVariableTypeRule->clone();
}


/* Destructor */
TreePlate::~TreePlate(void) {

//    delete orderingTopology;
//    delete treePlateVariableTypeRule;
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
        bool repl = static_cast<const Boolean*>( args[2].getValue() )->getValue();
        
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
    else {
        return MemberObject::executeOperation( name, args );
    }
}


/* Get method specifications */
const MethodTable& TreePlate::getMethods(void) const {
    
    static MethodTable   methods;
    static ArgumentRules addvariableArgRules;
    static bool          methodsSet = false;

    if ( methodsSet == false ) 
        {
            
        addvariableArgRules.push_back(  new ValueRule( "name"      , RbString_name  ) );
        addvariableArgRules.push_back(  new ValueRule( ""          , RbObject_name ) );
        addvariableArgRules.push_back(  new ValueRule( "replicate" , Boolean_name  ) );
        
        methods.addFunction("addVariable",  new MemberFunction(RbVoid_name, addvariableArgRules)  );
        
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


