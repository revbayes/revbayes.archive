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
  //  allocationVector_->push_back(RbDagNodePtr<RbObject> (new Integer(allocationVec[i])));
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


/* Get class information */
const VectorString& Mixture::getClass(void) const {
    
    static VectorString rbClass = VectorString(Mixture_name) + MemberObject::getClass();
    return rbClass;
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
        memberRules.push_back( new ValueRule( "numObservations", Integer_name ) );
       // memberRules->push_back( RbDagNodePtr<ArgumentRule> ( new ValueRule( "allocationVector", DagNodeContainer_name ) ) );
        memberRules.push_back( new ValueRule( "parameters", DagNodeContainer_name ) );
       // memberRules->push_back( RbDagNodePtr<ArgumentRule> ( new ValueRule( "classProbabilities", VectorRealPos_name ) ) );
      
        rulesSet = true;
    }
    
    return memberRules;
}


/** Print value for user */
void Mixture::printValue(std::ostream& o) const {
  std::cout <<"printValue"<<std::endl;
  o << "Parameter vector:\n";
  for ( size_t i = 0; i < parameters_->size(); i++ ) {
     RbDagNodePtr theNode = static_cast<const VariableSlot&>( parameters_->getElement(i) ).getDagNode();
     theNode->printValue(o) ;
     o << "\t";
  }
  o << std::endl << std::endl;

  o << "Allocation vector:\n";
/*    for ( size_t i = 0; i < allocationVector_->size(); i++ ) {
        RbDagNodePtr theNode = static_cast<const VariableSlot*>( (const RbObject*) (allocationVector_->getElement(i) ) )->getDagNode();
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
RbLanguageObject* Mixture::executeOperation(const std::string& name, Environment& args) {
    if (name == "getNumberOfClasses") {
      return new Natural(getNumberOfClasses());
    }
    else if (name == "getParameter") {      
      // get the member with given index
      const Natural& index = static_cast<const Natural&>( args[0].getValue() );
      
      if (parameters_->size() < (size_t)(index.getValue())) {
        throw RbException("Index out of bounds in Mixture::getParameter");
      }
     // (DagNodeContainer*) getParameter(index->getValue());
      return static_cast<RbLanguageObject*>( getParameter(index.getValue()).clone() );
      
//      
//      const std::string& numString = static_cast<const RbString*>( (const RbObject*)(*args)[0]->getValue() )->getValue();
//      std::stringstream ss(numString);
//      int num;
//      ss >> num;
      
   //     unsigned int num = static_cast<const Natural*>( (const RbObject*)(*args)[0]->getValue() )->getValue();
   //     return getParameter(num);
        //return RbDagNodePtr<RbLanguageObject>::getNullPtr();

    }
    else if (name == "getParameters") {
       // return getParameters();
        return NULL;
    }
    else if (name == "setParameters") {
        Environment& a = args;
        DagNodeContainer& params = static_cast<DagNodeContainer&>( a[0].getValue() );
        setParameters ( params.clone() ) ;
        return NULL;
    }
    else if (name == "setParameter") {
      Environment& a = args;
      const Natural& index = static_cast<const Natural&>( args[0].getValue() );
      DagNodeContainer& params =  static_cast<DagNodeContainer&>( a[1].getValue() );
      setParameter ( (index.getValue()), params.clone() ) ;
      return NULL;
    }

  
  
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
void Mixture::setMemberVariable(const std::string& name, Variable* var) {
    
    if ( name == "allocationVector" ) {
        allocationVector_ = static_cast<DagNodeContainer*>( var->getValue().clone() );
    }
    if ( name == "parameters" ) {
        parameters_ = static_cast<DagNodeContainer*>( var->getValue().clone() );
    }
    if ( name == "classProbabilities" ) {
        classProbabilities_ =  static_cast<VectorRealPos*>( var->getValue().clone() );
    }
    if ( name == "numObservations" ) {
      int numObservations = static_cast<Integer&>( var->getValue() ).getValue();
      if (allocationVector_ != 0) {
        throw RbException("Mixture already constructed. Cannot reconstruct it in Mixture::setMemberVariable.");
      }
      else {
        RandomNumberGenerator* rng = GLOBAL_RNG;
        if (classProbabilities_ == 0) {
          std::vector<double> v(numObservations, 1.0);
          classProbabilities_ = new VectorRealPos( RbStatistics::Dirichlet::rv(v, *rng) );
        }
        std::vector<int> allocationVec = RbStatistics::Multinomial::rv(classProbabilities_->getValue(), (int)numObservations, *rng);
        allocationVector_ = new DagNodeContainer (numObservations);
        for (size_t i = 0 ; i < numObservations ; i ++ ) {
         allocationVector_->setElement(i, new Variable(new ConstantNode( new Integer( allocationVec[i] ) ) ) );
        }
        std::cout << "Size of the vector: "<< allocationVector_->size()<<std::endl;
        
        //TEST
        std::cout <<"setMemberVariable"<<std::endl;
        const Variable& slot = static_cast<const Variable&>( allocationVector_->getElement(0) );
        std::cout <<"setMemberVariable 2"<<std::endl;
        const Integer& nat = static_cast<const Integer&>( slot.getValue() );
        std::cout <<"setMemberVariable 3"<<std::endl;
        int formerlyAssignedValue = nat.getValue();
        std::cout <<"setMemberVariable 4: "<< formerlyAssignedValue<<std::endl;

        
        
        //TEST
       /* const VariableSlot* slot = static_cast<const VariableSlot*>( allocationVector_->getElement(0) );
        std::cout <<"here"<<std::endl;
        const Variable* tmp_var = slot->getVariable();
        std::cout <<"here 2"<<std::endl;
        tmp_var->getValue();
        std::cout <<"here 3"<<std::endl;*/
        
        indexAllocationVector();
        
        std::cout << allocationVector_->size()<<std::endl;
        
        
      }
    }

    MemberObject::setMemberVariable(name, var);
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


/** Get the vector containing elements on which the mixture operates*/
/*RbDagNodePtr<DagNodeContainer> Mixture::getObservations() {
    
    return observations_;
}*/

/** Set the vector containing elements on which the mixture operates*/
/*void Mixture::setObservations(RbDagNodePtr<DagNodeContainer>& observations) {
    observations_ = observations;
}*/

/** Re-number the classes in the allocation vector so that they start from 0 and end at number_of_classes - 1*/
void Mixture::indexAllocationVector() {
  std::map<int, int> rvToNumber;
  unsigned int maxIntSeen = 0;
  //Renumber the allocation vector
  for (unsigned int i = 0 ; i < allocationVector_->size() ; i++ ) { 
 //   allocationVector_->setElement(i, RbDagNodePtr<Variable> ( (static_cast< VariableSlot*>( ( RbObject*) (new Integer(allocationVec[i] ) ) )  )->getVariable() ) );
   /* const RbObject* variab = (const  RbObject*) (allocationVector_->getElement(i) );
   const  RbLanguageObject* lango = (const RbLanguageObject*)static_cast<const VariableSlot*>( variab);
    const Natural* nat = static_cast<const Natural*>(lango);
    int formerlyAssignedValue = nat->getValue();*/
    
    //TEST
    const VariableSlot& slot = static_cast<const VariableSlot&>( allocationVector_->getElement(i) );
    std::cout <<"indexAllocationVector"<<std::endl;
    const Variable& tmp_var = slot.getVariable();
    std::cout <<"indexAllocationVector 2"<<std::endl;
    const  RbLanguageObject& lango = (const RbLanguageObject&)( tmp_var);
    const Natural& nat = static_cast<const Natural&>(lango);
    int formerlyAssignedValue = nat.getValue();
    std::cout <<"indexAllocationVector 3"<<std::endl;
    
    
    
  //  int formerlyAssignedValue = static_cast<const Natural*>( (const RbLanguageObject*)static_cast<const VariableSlot*>( (const RbObject*) (allocationVector_->getElement(i) ) )->getVariable() ) ->getValue();
    
 //   int formerlyAssignedValue = static_cast<const Natural*>( (const RbLanguageObject*)static_cast<const VariableSlot*>( (const RbObject*) (allocationVector_->getElement(i) ) )->getValue() ) ->getValue();
    if (rvToNumber.find(formerlyAssignedValue) != rvToNumber.end())
      allocationVector_->setElement(i, new Integer(rvToNumber[formerlyAssignedValue]) );
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
  //Renumber the classProbabilities_ vector
  VectorRealPos& copy = *(classProbabilities_->clone());
  for (unsigned int i = 0 ; i < classProbabilities_->size() ; i++ ) { 
      classProbabilities_->AbstractVector::setElement(i, copy.getElement(rvToNumber[i] ).clone() );
  }
}






