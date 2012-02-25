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
#include "ConstantNode.h"
#include "VariableNode.h"
#include "DagNodeContainer.h"
#include "DistributionDirichlet.h"
#include "DistributionMultinomial.h"
#include "RandomNumberFactory.h"
#include "RbNullObject.h"

#include <sstream>

/* Default constructor */
Mixture::Mixture(void) : MutableMemberObject( getMemberRules() ) {

}

/* constructor; the vector of class probabilities is filled with values 1/number_of_classes */
Mixture::Mixture(DagNodeContainer* allocationVector, DagNodeContainer* parameters) : MutableMemberObject( getMemberRules() ) {
  std::cout << "constructor 1"<<std::endl;

    allocationVector_ = allocationVector;
    parameters_ = parameters;
    DagNodeContainer* numClassesAllocated = allocationVector_->clone();
    numClassesAllocated->sort();
    numClassesAllocated->unique();
    if (parameters_->size() != numClassesAllocated->size()) {
        std::stringstream o;
        o << "ERROR: The number of classes of parameters does not correspond to the number of classes elements are allocated to in the allocation vector.";
        throw( RbException(o.str()) );        
    }
    
    for (unsigned int i = 0 ; i < parameters_->size() ; i++) {
        classProbabilities_->push_back( RealPos ( 1.0 / parameters_->size() ) );
    }
    computeNumberOfElementsInClasses();
    
}


/* constructor */
Mixture::Mixture(DagNodeContainer* allocationVector, DagNodeContainer* parameters, VectorRealPos* classProbabilities ) : MutableMemberObject( getMemberRules() ) {
  std::cout << "constructor 2 "<<std::endl;

    allocationVector_ = allocationVector;
    parameters_ = parameters;
    classProbabilities_ = classProbabilities;
    computeNumberOfElementsInClasses();
}


/* constructor */
Mixture::Mixture(const size_t numObservations, DagNodeContainer* parameters) : MutableMemberObject( getMemberRules() ) {
  std::cout << "constructor 3 "<<std::endl;

  parameters_ = parameters;
  std::vector<double> v(numObservations, 1.0);
  RandomNumberGenerator* rng = GLOBAL_RNG;
  classProbabilities_ = new VectorRealPos( RbStatistics::Dirichlet::rv(v, *rng) );
  std::vector<int> allocationVec = RbStatistics::Multinomial::rv(classProbabilities_->getValue(), (int)numObservations, *rng);
  allocationVector_ = new DagNodeContainer(numObservations);
  for (size_t i = 0 ; i < numObservations ; i ++ ) {
    allocationVector_->setElement(i, new Integer(allocationVec[i]) );
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


/** Get class name of object */
const std::string& Mixture::getClassName(void) { 
    
    static std::string rbClassName = "Mixture";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Mixture::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( MutableMemberObject::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Mixture::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/* Get method specifications */
const MethodTable& Mixture::getMethods(void) const {
    
    static MethodTable   methods = MethodTable();
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
    
    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;
    if (!rulesSet) 
    {
        memberRules.push_back( new ValueRule( "numObservations", Integer::getClassTypeSpec() ) );
        memberRules.push_back( new ValueRule( "parameters", DagNodeContainer::getClassTypeSpec() ) );
      
        rulesSet = true;
    }
    
    return memberRules;
}


/** Print value for user */
void Mixture::printValue(std::ostream& o) const {

    o << "Parameter vector:\n";
    for ( size_t i = 0; i < parameters_->size(); i++ ) {
        DAGNode* theNode = static_cast<const VariableSlot&>( parameters_->getElement(i) ).getDagNode();
        theNode->printValue(o) ;
        o << "\t";
    }
    o << std::endl << std::endl;

    o << "Allocation vector:\n";
    for ( size_t i = 0; i < allocationVector_->size(); i++ ) {
        DAGNode* theNode = static_cast<const VariableSlot&>( allocationVector_->getElement(i) ).getDagNode();
        theNode->printValue(o) ;
        o << "\t";
    }

    o << std::endl << std::endl;

}


/* Map calls to member methods */
const RbLanguageObject& Mixture::executeOperation(const std::string& name, Environment& args) {
    if (name == "getNumberOfClasses") {
        numClasses.setValue( getNumberOfClasses() );
        return numClasses;
    }
    else if (name == "getParameter") {      
      // get the member with given index
      const Natural& index = static_cast<const Natural&>( args[0].getValue() );
      
      if (parameters_->size() < (size_t)(index.getValue())) {
        throw RbException("Index out of bounds in Mixture::getParameter");
      }
     // (DagNodeContainer*) getParameter(index->getValue());
      return  getParameter(index.getValue());
    }
    else if (name == "getParameters") {
       // return getParameters();
        return RbNullObject::getInstance();
    }
    else if (name == "setParameters") {
        Environment& a = args;
        DagNodeContainer& params = static_cast<DagNodeContainer&>( a[0].getValue() );
        setParameters ( params.clone() ) ;
        return RbNullObject::getInstance();
    }
    else if (name == "setParameter") {
      Environment& a = args;
      const Natural& index = static_cast<const Natural&>( args[0].getValue() );
      DagNodeContainer& params =  static_cast<DagNodeContainer&>( a[1].getValue() );
      setParameter ( (index.getValue()), params.clone() ) ;
      return RbNullObject::getInstance();
    }

  
  
    return RbNullObject::getInstance();
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
void Mixture::setMemberVariable(const std::string& name, Variable* var) {
    
    if ( name == "allocationVector" ) {
        allocationVector_ = static_cast<DagNodeContainer*>( var->getValue().clone() );
    }
    else if ( name == "parameters" ) {
        parameters_ = static_cast<DagNodeContainer*>( var->getValue().clone() );
        if (classProbabilities_ == 0) {
          std::vector<double> v(parameters_->size(), 1.0);
          RandomNumberGenerator* rng = GLOBAL_RNG;
          classProbabilities_ = new VectorRealPos( RbStatistics::Dirichlet::rv(v, *rng) );
          std::cout << "classProbabilities_->size() "<< classProbabilities_->size() <<std::endl;
        }
      if (allocationVector_ == NULL ) {
        
      }

    }
    else if ( name == "classProbabilities" ) {
        classProbabilities_ =  static_cast<VectorRealPos*>( var->getValue().clone() );
    }
    else if ( name == "numObservations" ) {
      int numObservations = static_cast<Integer&>( var->getValue() ).getValue();
      if (allocationVector_ != NULL ) {
        throw RbException("Mixture already constructed. Cannot reconstruct it in Mixture::setMemberVariable.");
      }
      else {
        std::cout << "numObservations: "<< numObservations<<std::endl;
        RandomNumberGenerator* rng = GLOBAL_RNG;
        if (classProbabilities_ != NULL) {
        std::vector<int> allocationVec = RbStatistics::Multinomial::rv(classProbabilities_->getValue(), (int)numObservations, *rng);
        allocationVector_ = new DagNodeContainer (numObservations);
        for (size_t i = 0 ; i < numObservations ; i ++ ) {
          std::cout <<"allocationVec[i] "<<allocationVec[i] <<std::endl;
         allocationVector_->setElement(i, new Variable(new ConstantNode( new Integer( allocationVec[i] ) ) ) );
    //      allocationVector_->setElement(i, new ConstantNode( new Integer( allocationVec[i] ) ) ) ;
        }
        }
        else {
          VectorRealPos* prop = new VectorRealPos (1);
          prop->setElement(0,  new Real( 1.0 ) );
          std::vector<int> allocationVec = RbStatistics::Multinomial::rv(prop->getValue(), (int)numObservations, *rng);
          allocationVector_ = new DagNodeContainer (numObservations);
          for (size_t i = 0 ; i < numObservations ; i ++ ) {
            std::cout <<"allocationVec[i] "<<allocationVec[i] <<std::endl;
            allocationVector_->setElement(i, new Variable(new ConstantNode( new Integer( allocationVec[i] ) ) ) );
            //      allocationVector_->setElement(i, new ConstantNode( new Integer( allocationVec[i] ) ) ) ;
          }

          
          
        }
        std::cout << "Size of the vector: "<< allocationVector_->size()<<std::endl;
        
        
        RbObject& elemPtr = allocationVector_->getElement(0);
        DAGNode* theNode = static_cast<VariableSlot&>( elemPtr ).getVariable().getDagNode();
        const Integer& nat = static_cast<const Integer&>( theNode->getValue() );
        int formerlyAssignedValue = nat.getValue();
        std::cout <<"setMemberVariable 4: "<< formerlyAssignedValue<<std::endl;
        
        
        for ( size_t i = 0; i < allocationVector_->size(); i++ ) {
          DAGNode* theNode = static_cast<const VariableSlot&>( allocationVector_->getElement(i) ).getDagNode();
          theNode->printValue(std::cout) ;
          std::cout << "\t";
        }

        
        indexAllocationVector();
        
        for ( size_t i = 0; i < allocationVector_->size(); i++ ) {
          DAGNode* theNode = static_cast<const VariableSlot&>( allocationVector_->getElement(i) ).getDagNode();
          theNode->printValue(std::cout) ;
          std::cout << "\t";
        }

        
        
        std::cout << allocationVector_->size()<<std::endl;
        
        
      }
    }
    else {
      MemberObject::setMemberVariable(name, var);
    }
}


void Mixture::estimateClassProbabilities() {
    for (unsigned int i = 0 ; i <  numberOfElementsInClasses_.size() ; i++) {
        if (classProbabilities_->size() <= i ) {
            classProbabilities_->push_back( (double) numberOfElementsInClasses_[i] / (double) allocationVector_->size() );
        }
        else {
        classProbabilities_->setElement(i, new RealPos ( (double) numberOfElementsInClasses_[i] / (double) allocationVector_->size() ) );
        }
    }
}

void Mixture::computeNumberOfElementsInClasses() {

    numberOfElementsInClasses_.clear();
    std::vector <int> numberOfElementsInClasses;
    for (unsigned int i = 0 ; i <  allocationVector_->size() ; i++) {
       int index =  static_cast<const Natural&>( static_cast<const VariableSlot&>( allocationVector_->getElement(i) ).getValue() ).getValue() ;
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
void Mixture::addClass(DagNodeContainer* parameter) {                                                            
  numberOfElementsInClasses_.push_back(new Natural(0));
  classProbabilities_->push_back(new RealPos(0.0));
  parameters_->push_back( parameter );
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
        numberOfElementsInClasses_.AbstractVector::setElement(i, numberOfElementsInClasses_.getElement(i+1).clone() );
    }
  }
  numberOfElementsInClasses_.pop_back();
  VectorRealPos copy = *(classProbabilities_->clone());
  double classProba =  *(static_cast<const RealPos*>( (const RbObject*)classProbabilities_->getElement(classId).clone() ) );
  //double classProba =  *(static_cast<RealPos*> ( (static_cast<VectorRealPos*> (classProbabilities_) )->getElement(classId) ) );
  double multiplier = 1.0/ ( 1.0 - classProba);
  classProbabilities_->clear();
  for (unsigned int i = 0 ; i < copy.size(); i++) {
    if (i != classId)
      classProbabilities_->push_back(copy[i] * multiplier);
  }

  DagNodeContainer* copyParam = parameters_->clone();
  parameters_->clear();
  for (unsigned int i = 0 ; i < copy.size(); i++) {
    if (i != classId)
      parameters_->push_back(copyParam[i].clone());
  }

}


/** Set the vector of parameter values associated to the classes of the mixture*/
void Mixture::setParameters(DagNodeContainer* parameters) {
    parameters_ = parameters;
}

/**Set the value of a parameter associated to a particular class*/
void Mixture::setParameter(unsigned int classId, DagNodeContainer* parameter) {
  parameters_->setElement(classId, parameter);
}

/** Set the vector of parameter values associated to the classes of the mixture*/
DagNodeContainer& Mixture::getParameter(unsigned int classId) {
  return static_cast<DagNodeContainer&>( parameters_->getElement(classId) );
}

/** Re-number the classes in the allocation vector so that they start from 0 and end at number_of_classes - 1*/
void Mixture::indexAllocationVector() {
  std::map<int, int> rvToNumber;
  unsigned int maxIntSeen = 0;
  //Renumber the allocation vector
  for (unsigned int i = 0 ; i < allocationVector_->size() ; i++ ) { 
   
    RbObject& elemPtr = allocationVector_->getElement(i);
    DAGNode* theNode = static_cast<VariableSlot&>( elemPtr ).getVariable().getDagNode();
    const Integer& nat = static_cast<const Integer&>( theNode->getValue() );
    int formerlyAssignedValue = nat.getValue();
    std::cout <<"indexAllocationVector: "<< formerlyAssignedValue<<std::endl;

    if (rvToNumber.find(formerlyAssignedValue) != rvToNumber.end())
      allocationVector_->setElement(i, new Variable( new ConstantNode( new Integer(rvToNumber[formerlyAssignedValue]) ) ) );
    else {
      const VariableSlot& slot = static_cast<const VariableSlot&>( (allocationVector_->getElement(i) ) );
      std::cout <<"indexAllocationVector 4"<<std::endl;
      const Variable& var = slot.getVariable();
      std::cout <<"indexAllocationVector 5"<<std::endl;
      var.getValue();
      std::cout <<"indexAllocationVector 6"<<std::endl;

      const Integer& j = static_cast<const  Integer&> (  (var.getValue() ) );
      j.printValue(std::cout);
      std::cout <<std::endl;
      
      rvToNumber[static_cast<const Natural&>( static_cast<const VariableSlot&>( (allocationVector_->getElement(i) ) ).getValue() ).getValue()] = maxIntSeen;
      maxIntSeen = maxIntSeen +1;
    }
  }
  std::cout <<"indexAllocationVector 7"<<std::endl;
  //Renumber the classProbabilities_ vector
  VectorRealPos& copy = *(classProbabilities_->clone());
 // std::cout <<"indexAllocationVector 8: classProbabilities_->size(): "<< classProbabilities_->size() <<" copy.size(): "<< copy.size()  <<std::endl;
  
  for (unsigned int i = 0 ; i < copy.size() ; i++ ) { 
    std::cout <<"indexAllocationVector 9"<<std::endl;
      classProbabilities_->AbstractVector::setElement(i, copy.getElement(rvToNumber[i] ).clone() );
  }
  std::cout <<"indexAllocationVector 10"<<std::endl;
}






