/**
 * @file
 * This file contains the implementation of Mixture, a type
 * used to hold a mixture model, where elements are assigned different parameter values.
 *
 * @brief Implementation of Mixture
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "Mixture.h"
#include "RbException.h"
#include "VectorString.h"

#include <sstream>


// Definition of the static type spec member
const TypeSpec Mixture::typeSpec_(Mixture_name);

/* Default constructor */
Mixture::Mixture(void) : MutableMemberObject( getMemberRules() ) {
    
}

/* constructor; the vector of class probabilities is filled with values 1/number_of_classes */
Mixture::Mixture(VectorNatural& allocationVector, Vector& parameters) : MutableMemberObject( getMemberRules() ) {
    
    allocationVector_ = allocationVector;
    parameters_ = parameters;
    VectorNatural numClassesAllocated = allocationVector_;
    numClassesAllocated.sort();
    numClassesAllocated.unique();
    if (parameters_.size() != numClassesAllocated.size()) {
        std::stringstream o;
        o << "ERROR: The number of classes of parameters does not correspond to the number of classes elements are allocated to in the allocation vector.";
        throw( RbException(o.str()) );        
    }
    
    for (unsigned int i = 0 ; i < parameters_.size() ; i++) {
        classProbabilities_.push_back( RealPos ( 1.0 / parameters_.size() ) );
    }
    computeNumberOfElementsInClasses();
    
}

/* constructor */
Mixture::Mixture(VectorNatural& allocationVector, Vector& parameters, VectorRealPos& classProbabilities ) {

    allocationVector_ = allocationVector;
    parameters_ = parameters;
    classProbabilities_ = classProbabilities;
    computeNumberOfElementsInClasses();
}

/* Copy constructor */
Mixture::Mixture(const Mixture& m) : MutableMemberObject( m ) {
    
    allocationVector_ = m.allocationVector_;
    parameters_ = m.parameters_;
    numberOfElementsInClasses_ = m.numberOfElementsInClasses_;
    classProbabilities_ = m.classProbabilities_;
    
}


/* Destructor */
Mixture::~Mixture(void) {

}


/* Clone function */
Mixture* Mixture::clone(void) const {
    
    return new Mixture(*this);
}


/* Get class information */
const VectorString& Mixture::getClass(void) const {
    
    static VectorString rbClass = VectorString(Mixture_name) + MemberObject::getClass();
    return rbClass;
}


/* Get method specifications */
const MethodTable& Mixture::getMethods(void) const {
    
    static MethodTable   methods;
    /*
    static ArgumentRules addvariableArgRules;
    static ArgumentRules getNodeIndexArgRules;
    static ArgumentRules getTipIndexArgRules;
    static ArgumentRules nnodesArgRules;
    static ArgumentRules nodeArgRules;
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) {
        
        // add the 'addVariable()' method
        addvariableArgRules.push_back(  new ValueRule( "name"      , RbString_name      ) );
        addvariableArgRules.push_back(  new ValueRule( ""          , RbObject_name      ) );
        addvariableArgRules.push_back(  new ValueRule( "node"      , TopologyNode_name  ) );
        
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
    */
    return methods;
}


/* Get member rules */
const MemberRules& Mixture::getMemberRules(void) const {
    
    static MemberRules memberRules;
    static bool        rulesSet = false;
    /*
    if (!rulesSet) 
    {
        TypeSpec varType(Topology_name);
        memberRules.push_back( new ValueRule( "topology" , varType ) );
        
        rulesSet = true;
    }
    */
    return memberRules;
}


/** Print value for user */
void Mixture::printValue(std::ostream& o) const {
    o << "Allocation vector:\n";
    for ( size_t i = 0; i < allocationVector_.size(); i++ ) {
        
        o << allocationVector_[i] << std::endl;

    }
    o << std::endl << std::endl;

    // TODO: print parameter values too
}

/* Get a lot of information about the Mixture */
std::string Mixture::richInfo(void) const {
    
    std::ostringstream o;
    o <<  "Mixture: ";
    printValue(o);
    return o.str();
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Mixture::getTypeSpec(void) const {
    return typeSpec_;
}


/* Map calls to member methods */
RbPtr<RbLanguageObject> Mixture::executeOperation(const std::string& name, Environment& args) {
    return NULL;
    /*
    // special handling for adding a variable
    if (name == "addVariable") {
        // get the name of the variable
        std::string varName = static_cast<const RbString*>( args[0].getValue() )->getValue();
        
        // check if a container already exists with that name
        if (!members.existsVariable(varName)) {
            // we don't have a container for this variable name yet
            // so we just create one
            members.addVariable(varName, new Variable(new ConstantNode(new DagNodeContainer(orderingTopology->getNumberOfNodes()))));
        }
        
        // get the container with the variables for this node
        DagNodeContainer *vars = static_cast<DagNodeContainer*>(members[varName].getDagNodePtr()->getValuePtr());
        
        // get the variable
        Variable* var = args[1].getVariable();
        
        // get the node we want to associate it too
        const TopologyNode *theNode = static_cast<const TopologyNode*>(args[2].getDagNodePtr()->getValue());
        
        // get the index of the node
        size_t nodeIndex = getNodeIndex(theNode);
        
        // set the variable
        vars->setElement(nodeIndex, var);
        
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
    }*/
}


/** Catch setting of the mixture variable */
void Mixture::setMemberVariable(const std::string& name, RbPtr<Variable> var) {
    /*
    if ( name == "topology" )
        orderingTopology = (Topology*)(var->getValue());
    
    MemberObject::setMemberVariable(name, var);*/
}


void Mixture::estimateClassProbabilities() {
    
    
    
}

void Mixture::computeNumberOfElementsInClasses() {

    numberOfElementsInClasses_.clear();
    std::vector <int> numberOfElementsInClasses;
    for (unsigned int i = 0 ; i <  allocationVector_.size() ; i++) {
        while ((int) allocationVector_[i]  > numberOfElementsInClasses_.size()) {
            numberOfElementsInClasses.push_back(0);
        }
        numberOfElementsInClasses[(int) allocationVector_[i] ] += 1;
        
    }
    numberOfElementsInClasses_ = VectorNatural(numberOfElementsInClasses);
}


