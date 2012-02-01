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
#include "ValueRule.h"
#include "DagNodeContainer.h"
#include "DistributionDirichlet.h"
#include "DistributionMultinomial.h"
#include "RandomNumberFactory.h"

#include <sstream>


// Definition of the static type spec member
const TypeSpec Mixture::typeSpec_(Mixture_name);

/* Default constructor */
Mixture::Mixture(void) : MutableMemberObject( getMemberRules() ) {

}

/* constructor; the vector of class probabilities is filled with values 1/number_of_classes */
Mixture::Mixture(RbPtr<DagNodeContainer>& allocationVector, RbPtr<DagNodeContainer>& parameters) : MutableMemberObject( getMemberRules() ) {
  std::cout << "constructor 1"<<std::endl;

    allocationVector_ = allocationVector;
    parameters_ = parameters;
    RbPtr<DagNodeContainer> numClassesAllocated = allocationVector_->clone();
    numClassesAllocated->sort();
    numClassesAllocated->unique();
    if (parameters_->size() != numClassesAllocated->size()) {
        std::stringstream o;
        o << "ERROR: The number of classes of parameters does not correspond to the number of classes elements are allocated to in the allocation vector.";
        throw( RbException(o.str()) );        
    }
    
    for (unsigned int i = 0 ; i < parameters_->size() ; i++) {
        classProbabilities_.push_back( RealPos ( 1.0 / parameters_->size() ) );
    }
    computeNumberOfElementsInClasses();
    
}


/* constructor */
Mixture::Mixture(RbPtr<DagNodeContainer>& allocationVector, RbPtr<DagNodeContainer>& parameters, VectorRealPos& classProbabilities ) : MutableMemberObject( getMemberRules() ) {
  std::cout << "constructor 2 "<<std::endl;

    allocationVector_ = allocationVector;
    parameters_ = parameters;
    classProbabilities_ = classProbabilities;
    computeNumberOfElementsInClasses();
}


/* constructor */
Mixture::Mixture(const size_t numObservations, RbPtr<DagNodeContainer>& parameters) : MutableMemberObject( getMemberRules() ) {
  std::cout << "constructor 3 "<<std::endl;

  parameters_ = parameters;
  std::vector<double> v(numObservations, 1.0);
  RbPtr<RandomNumberGenerator> rng = GLOBAL_RNG;
  classProbabilities_ = RbStatistics::Dirichlet::rv(v, rng) ;
  std::vector<int> allocationVec = RbStatistics::Multinomial::rv(classProbabilities_.getValue(), (int)numObservations, rng);
  allocationVector_ = RbPtr< DagNodeContainer >(new DagNodeContainer (numObservations) );
  for (size_t i = 0 ; i < numObservations ; i ++ ) {
  //  allocationVector_->push_back(RbPtr<RbObject> (new Integer(allocationVec[i])));
    allocationVector_->setElement(i, RbPtr<RbObject> (new Integer(allocationVec[i] ) ) );
  }
  indexAllocationVector();
  
  std::cout << allocationVector_->size()<<std::endl;
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
RbPtr<const MethodTable> Mixture::getMethods(void) const {
    
    static RbPtr<MethodTable>   methods( new MethodTable() );
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
    return RbPtr<const MethodTable>( methods) ;
}


/* Get member rules */
RbPtr<const MemberRules> Mixture::getMemberRules(void) const {
    
    static RbPtr<MemberRules> memberRules( new MemberRules() );
    static bool        rulesSet = false;
    if (!rulesSet) 
    {
        memberRules->push_back( RbPtr<ArgumentRule> ( new ValueRule( "numObservations", Integer_name ) ) );
       // memberRules->push_back( RbPtr<ArgumentRule> ( new ValueRule( "allocationVector", DagNodeContainer_name ) ) );
        memberRules->push_back( RbPtr<ArgumentRule> ( new ValueRule( "parameters", DagNodeContainer_name ) ) );
       // memberRules->push_back( RbPtr<ArgumentRule> ( new ValueRule( "classProbabilities", VectorRealPos_name ) ) );
      
        rulesSet = true;
    }
    
    return RbPtr<const MemberRules>( memberRules );
}


/** Print value for user */
void Mixture::printValue(std::ostream& o) const {
  std::cout <<"printValue"<<std::endl;
  o << "Parameter vector:\n";
  for ( size_t i = 0; i < parameters_->size(); i++ ) {
     RbPtr<const DAGNode> theNode = static_cast<const VariableSlot*>( (const RbObject*) (parameters_->getElement(i) ) )->getDagNode();
     theNode->printValue(o) ;
     o << "\t";
  }
  o << std::endl << std::endl;

  o << "Allocation vector:\n";
/*    for ( size_t i = 0; i < allocationVector_->size(); i++ ) {
        RbPtr<const DAGNode> theNode = static_cast<const VariableSlot*>( (const RbObject*) (allocationVector_->getElement(i) ) )->getDagNode();
        theNode->printValue(o) ;
       o << "\t";
  }*/
    o << std::endl << std::endl;

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
RbPtr<RbLanguageObject> Mixture::executeOperation(const std::string& name, const RbPtr<Environment>& args) {
    if (name == "getNumberOfClasses") {
      return new Natural(getNumberOfClasses());
    }
    else if (name == "getParameter") {      
      // get the member with given index
      RbPtr<const Natural> index( static_cast<const Natural*>( (const RbObject*)(*args)[0]->getValue()) );
      
      if (parameters_->size() < (size_t)(index->getValue())) {
        throw RbException("Index out of bounds in Mixture::getParameter");
      }
     // (DagNodeContainer*) getParameter(index->getValue());
      return RbPtr<RbLanguageObject>( static_cast<RbLanguageObject*>( (DagNodeContainer*) getParameter(index->getValue()) ) );
      
//      
//      const std::string& numString = static_cast<const RbString*>( (const RbObject*)(*args)[0]->getValue() )->getValue();
//      std::stringstream ss(numString);
//      int num;
//      ss >> num;
      
   //     unsigned int num = static_cast<const Natural*>( (const RbObject*)(*args)[0]->getValue() )->getValue();
   //     return getParameter(num);
        //return RbPtr<RbLanguageObject>::getNullPtr();

    }
    else if (name == "getParameters") {
       // return getParameters();
        return RbPtr<RbLanguageObject>::getNullPtr();
    }
    else if (name == "setParameters") {
        RbPtr<Environment> a = args;
        const RbPtr< DagNodeContainer >& params =  RbPtr< DagNodeContainer > ( static_cast<DagNodeContainer*>( (RbObject*)(*a)[0]->getValue() ) );
        setParameters ( params ) ;
        return RbPtr<RbLanguageObject>::getNullPtr();
    }
    else if (name == "setParameter") {
      RbPtr<Environment> a = args;
      RbPtr<const Natural> index( static_cast<const Natural*>( (const RbObject*)(*args)[0]->getValue()) );
      const RbPtr< DagNodeContainer >& params =  RbPtr< DagNodeContainer > ( static_cast<DagNodeContainer*>( (RbObject*)(*a)[1]->getValue() ) );
      setParameter ( (index->getValue()), params ) ;
      return RbPtr<RbLanguageObject>::getNullPtr();
    }

  
  
    return RbPtr<RbLanguageObject>::getNullPtr();
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
    
    if ( name == "allocationVector" ) {
        allocationVector_ = RbPtr< DagNodeContainer >(static_cast<DagNodeContainer*>( (RbLanguageObject*)var->getValue() ) );
    }
    if ( name == "parameters" ) {
        parameters_ = RbPtr< DagNodeContainer >(static_cast<DagNodeContainer*>( (RbLanguageObject*)var->getValue() ) );
    }

    MemberObject::setMemberVariable(name, var);
}


void Mixture::estimateClassProbabilities() {
    for (unsigned int i = 0 ; i <  numberOfElementsInClasses_.size() ; i++) {
        if (classProbabilities_.size() <= i ) {
            classProbabilities_.push_back( (double) numberOfElementsInClasses_[i] / (double) allocationVector_->size() );
        }
        else {
        classProbabilities_.setElement(i, new RealPos ( (double) numberOfElementsInClasses_[i] / (double) allocationVector_->size() ) );
        }
    }
}

void Mixture::computeNumberOfElementsInClasses() {

    numberOfElementsInClasses_.clear();
    std::vector <int> numberOfElementsInClasses;
    for (unsigned int i = 0 ; i <  allocationVector_->size() ; i++) {
       int index =  static_cast<const Natural*>( (const RbLanguageObject*)static_cast<const VariableSlot*>( (const RbObject*) (allocationVector_->getElement(i) ) )->getValue() ) ->getValue() ;
        while (index   > numberOfElementsInClasses_.size()) {
            numberOfElementsInClasses.push_back(0);
        }
        numberOfElementsInClasses[index ] += 1;
        
    }
    numberOfElementsInClasses_ = VectorNatural(numberOfElementsInClasses);
}


/** Get the number of classes in the mixture */
size_t Mixture::getNumberOfClasses() {
    return parameters_->size();
}

/** Add a new class to the mixture */
void Mixture::addClass(RbPtr<DagNodeContainer>& parameter) {                                                            
  numberOfElementsInClasses_.push_back(0);
  classProbabilities_.push_back(0.0);
  parameters_->push_back(RbPtr<RbObject> ( static_cast<RbObject*> ( parameter ) ));
}

/* Remove a class from the mixture */
void Mixture::removeClass(unsigned int classId) {
  if (numberOfElementsInClasses_[classId] > 0 ) {
    throw RbException("Trying to remove a non-empty class in Mixture::removeClass");
  }
  std::map <int, int> oldToNew;
  for (unsigned int i = 0 ; i < numberOfElementsInClasses_.size() - 1; i++) {
    if (i < classId) {
      oldToNew[i] = i;
    }
    else {
      if (i > classId) 
        oldToNew[i] = i -1 ;
      numberOfElementsInClasses_.getElement(i) = numberOfElementsInClasses_.getElement(i+1);
    }
  }
  numberOfElementsInClasses_.pop_back();
  VectorRealPos copy = classProbabilities_;
  double multiplier = 1.0/ ( 1.0 - classProbabilities_[classId] );
  classProbabilities_.clear();
  for (unsigned int i = 0 ; i < copy.size(); i++) {
    if (i != classId)
      classProbabilities_.push_back(copy[i] * multiplier);
  }

  RbPtr<DagNodeContainer> copyParam = parameters_->clone();
  parameters_->clear();
  for (unsigned int i = 0 ; i < copy.size(); i++) {
    if (i != classId)
      parameters_->push_back(copyParam[i].clone());
  }

}


/** Set the vector of parameter values associated to the classes of the mixture*/
void Mixture::setParameters(const RbPtr< DagNodeContainer>& parameters) {
    parameters_ = parameters;
}

/**Set the value of a parameter associated to a particular class*/
void Mixture::setParameter(unsigned int classId, const RbPtr<DagNodeContainer>& parameter) {
  parameters_->setElement(classId, RbPtr< RbObject >(parameter));
}

/** Set the vector of parameter values associated to the classes of the mixture*/
const RbPtr<DagNodeContainer>& Mixture::getParameter(unsigned int classId) {
  return ( RbPtr<DagNodeContainer> ( &(parameters_[classId]) ) );
}


/** Get the vector containing elements on which the mixture operates*/
/*RbPtr<DagNodeContainer> Mixture::getObservations() {
    
    return observations_;
}*/

/** Set the vector containing elements on which the mixture operates*/
/*void Mixture::setObservations(RbPtr<DagNodeContainer>& observations) {
    observations_ = observations;
}*/

/** Re-number the classes in the allocation vector so that they start from 0 and end at number_of_classes - 1*/
void Mixture::indexAllocationVector() {
  std::map<int, int> rvToNumber;
  unsigned int maxIntSeen = 0;
  //Renumber the allocation vector
  for (unsigned int i = 0 ; i < allocationVector_->size() ; i++ ) { 
    int formerlyAssignedValue = static_cast<const Natural*>( (const RbLanguageObject*)static_cast<const VariableSlot*>( (const RbObject*) (allocationVector_->getElement(i) ) )->getValue() ) ->getValue();
    if (rvToNumber.find(formerlyAssignedValue) != rvToNumber.end())
      allocationVector_->setElement(i, RbPtr< RbObject >(new Integer(rvToNumber[formerlyAssignedValue])) );
    else {
      rvToNumber[static_cast<const Natural*>( (const RbLanguageObject*)static_cast<const VariableSlot*>( (const RbObject*) (allocationVector_->getElement(i) ) )->getValue() ) ->getValue()] = maxIntSeen;
      maxIntSeen = maxIntSeen +1;
    }
  }
  //Renumber the classProbabilities_ vector
  VectorRealPos copy = classProbabilities_;
  for (unsigned int i = 0 ; i < classProbabilities_.size() ; i++ ) { 
    classProbabilities_.getElement(i) = copy.getElement(rvToNumber[i] );
  }
}






