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
#include "ConstantNode.h"
#include "DagNodeContainer.h"
#include "DistributionDirichlet.h"
#include "DistributionMultinomial.h"
#include "MemberFunction.h"
#include "Natural.h"
#include "RandomNumberFactory.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "ValueRule.h"
#include "VariableNode.h"
#include "VectorString.h"


#include <sstream>

/* Default constructor */
Mixture::Mixture(void) : MutableMemberObject( getMemberRules() ) {

}

/* constructor; the vector of class probabilities is filled with values 1/number_of_classes */
Mixture::Mixture(DagNodeContainer* allocationVector, DagNodeContainer* parameters) : MutableMemberObject( getMemberRules() ) {

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
    
    if (classProbabilities_->size() == 0 ) {
        for (unsigned int i = 0 ; i < parameters_->size() ; i++) {
            classProbabilities_->push_back( RealPos ( 1.0 / parameters_->size() ) );
        }
    }
    computeNumberOfElementsInClasses();
    
}


/* constructor */
Mixture::Mixture(DagNodeContainer* allocationVector, DagNodeContainer* parameters, Simplex* classProbabilities ) : MutableMemberObject( getMemberRules() ) {

    allocationVector_ = allocationVector;
    parameters_ = parameters;
    classProbabilities_ = classProbabilities;
    computeNumberOfElementsInClasses();
}


/* constructor */
Mixture::Mixture(const size_t numObservations, DagNodeContainer* parameters) : MutableMemberObject( getMemberRules() ) {
    
  parameters_ = parameters;
  std::vector<double> v(parameters->size(), 1.0);
  RandomNumberGenerator* rng = GLOBAL_RNG;
  classProbabilities_ = new Simplex( RbStatistics::Dirichlet::rv(v, *rng) );
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
    
    static ArgumentRules* getNumberOfClassesArgRules = new ArgumentRules();
    static ArgumentRules* getParameterArgRules = new ArgumentRules();
    static ArgumentRules* getParametersArgRules = new ArgumentRules();
    static ArgumentRules* setParametersArgRules = new ArgumentRules();
    static ArgumentRules* setParameterArgRules = new ArgumentRules();
  static ArgumentRules* getAllocationVectorArgRules = new ArgumentRules();
  static ArgumentRules* setAllocationVectorArgRules = new ArgumentRules();
  static ArgumentRules* allocateElementArgRules = new ArgumentRules();
  static ArgumentRules* getElementAllocationArgRules = new ArgumentRules();
  static ArgumentRules* setClassProbabilitiesArgRules = new ArgumentRules();
  static ArgumentRules* getClassProbabilitiesArgRules = new ArgumentRules();
  static ArgumentRules* getParameterForElemArgRules = new ArgumentRules();

    static bool          methodsSet = false;
    
    if ( methodsSet == false ) {
        
      //add the 'getNumberOfClasses()' method
      methods.addFunction("getNumberOfClasses", new MemberFunction(Natural::getClassTypeSpec(), getNumberOfClassesArgRules) );

      //add the 'getParameter(i)' method
      getParameterArgRules->push_back(  new ValueRule( "index" , Natural::getClassTypeSpec()   ) );
      methods.addFunction("getParameter", new MemberFunction(DagNodeContainer::getClassTypeSpec(), getParameterArgRules) );

      //add the 'getParameters()' method
      methods.addFunction("getParameters", new MemberFunction(DagNodeContainer::getClassTypeSpec(), getParametersArgRules) );

      //add the 'setParameters(d)' method
      setParametersArgRules->push_back(  new ValueRule( "parameters" , DagNodeContainer::getClassTypeSpec()   ) );
      methods.addFunction("setParameters", new MemberFunction(RbVoid_name, setParametersArgRules) );
      
      //add the 'setParameter(i,d)' method
      setParameterArgRules->push_back(  new ValueRule( "index" , Natural::getClassTypeSpec()   ) );
      setParameterArgRules->push_back(  new ValueRule( "parameters" , DagNodeContainer::getClassTypeSpec()   ) );
      methods.addFunction("setParameter", new MemberFunction(RbVoid_name, setParameterArgRules) );

      //add the 'getAllocationVector()' method
      methods.addFunction("getAllocationVector", new MemberFunction(DagNodeContainer::getClassTypeSpec(), getAllocationVectorArgRules) );
      
      //add the 'setAllocationVector(v)' method
      setAllocationVectorArgRules->push_back(  new ValueRule( "vector" , DagNodeContainer::getClassTypeSpec()   ) );
      methods.addFunction("setAllocationVector", new MemberFunction(RbVoid_name, setAllocationVectorArgRules) );

      //add the 'allocateElement(i,c)' method
      allocateElementArgRules->push_back(  new ValueRule( "index" , Natural::getClassTypeSpec()   ) );
      allocateElementArgRules->push_back(  new ValueRule( "class" , Natural::getClassTypeSpec()   ) );
      methods.addFunction("allocateElement", new MemberFunction(RbVoid_name, allocateElementArgRules) );

      //add the 'getElementAllocation(i)' method
      getElementAllocationArgRules->push_back(  new ValueRule( "index" , Natural::getClassTypeSpec()   ) );
      methods.addFunction("getElementAllocation", new MemberFunction(Natural::getClassTypeSpec(), getElementAllocationArgRules) );

      //add the 'setClassProbabilities(p)' method
      setClassProbabilitiesArgRules->push_back(  new ValueRule( "vector" , Simplex::getClassTypeSpec()   ) );
      methods.addFunction("setClassProbabilities", new MemberFunction(RbVoid_name, setClassProbabilitiesArgRules) );

      //add the 'getClassProbabilities()' method
      methods.addFunction("getClassProbabilities", new MemberFunction(Simplex::getClassTypeSpec(), getClassProbabilitiesArgRules) );

      //add the 'getParameterForElem(i)' method
      getParameterForElemArgRules->push_back(  new ValueRule( "index" , Natural::getClassTypeSpec()   ) );
      methods.addFunction("getParameterForElem", new MemberFunction(DagNodeContainer::getClassTypeSpec(), getParameterForElemArgRules) );

      
        // necessary call for proper inheritance
        methods.setParentTable( const_cast<MethodTable*>( &MemberObject::getMethods() ) );
        methodsSet = true;
    }
    
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
        memberRules.push_back( new ValueRule( "classProbabilities", Simplex::getClassTypeSpec() ) );

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

    o << "Class probabilities:\n";
    classProbabilities_->printValue(o) ;
  /*  for ( size_t i = 0; i < classProbabilities_->size(); i++ ) {
        classProbabilities_[i].printValue(o) ;
        o << "\t";
    }*/
    
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
        return getParameters();
        //return RbNullObject::getInstance();
    }
    else if (name == "setParameters") {
        Environment& a = args;
        DagNodeContainer& params = static_cast<DagNodeContainer&>( a[0].getValue() );
        setParameters ( params.clone() ) ;
        return RbNullObject::getInstance();
    }
    else if (name == "setParameter") {
      const Natural& index = static_cast<const Natural&>( args[0].getValue() );
      DagNodeContainer& params =  static_cast<DagNodeContainer&>( args[1].getValue() );
      setParameter ( (index.getValue()), params.clone() ) ;
      return RbNullObject::getInstance();
    }
    else if (name == "getAllocationVector") {
        return getAllocationVector();
    }
    else if (name == "setAllocationVector") {
        Environment& a = args;
        DagNodeContainer& params = static_cast<DagNodeContainer&>( a[0].getValue() );
        setAllocationVector ( params.clone() ) ;
        return RbNullObject::getInstance();
    }
    else if (name == "allocateElement") {
        const Natural& index = static_cast<const Natural&>( args[0].getValue() );
        const Natural& classId = static_cast<const Natural&>( args[1].getValue() );
        allocateElement ((index.getValue()), (classId.getValue()));
        return RbNullObject::getInstance();
    }
    else if (name == "getElementAllocation") {
        const Natural& index = static_cast<const Natural&>( args[0].getValue() );
        return getElementAllocation(index.getValue() );
    }
    else if (name == "setClassProbabilities") {
        Environment& a = args;
        Simplex& params = static_cast<Simplex&>( a[0].getValue() );
        setClassProbabilities ( params.clone() ) ;
        return RbNullObject::getInstance();
    }
    else if (name == "getClassProbabilities") {
        return getClassProbabilities();
    }
    else if (name == "getParameterForElem") {
      // get the member with given index
      const Natural& index = static_cast<const Natural&>( args[0].getVariable().getValue() );
      
      if (allocationVector_->size() < (size_t)(index.getValue())) {
        throw RbException("Index out of bounds in Mixture::getParameterForElem");
      }
      // (DagNodeContainer*) getParameter(index->getValue());
      return  getParameterForElem(index.getValue());
    }
    return RbNullObject::getInstance();

}


/* Map calls to member methods */
const RbLanguageObject& Mixture::executeOperationSimple(const std::string& name, const std::vector<Argument>& args) {
  if (name == "getNumberOfClasses") {
    numClasses.setValue( getNumberOfClasses() );
    return numClasses;
  }
  else if (name == "getParameter") {      
    // get the member with given index
    const Natural& index = static_cast<const Natural&>( args[0].getVariable().getValue() );
    
    if (parameters_->size() < (size_t)(index.getValue())) {
      throw RbException("Index out of bounds in Mixture::getParameter");
    }
    // (DagNodeContainer*) getParameter(index->getValue());
    return  getParameter(index.getValue());
  }
  else if (name == "getParameters") {
    return getParameters();
    //return RbNullObject::getInstance();
  }
  else if (name == "setParameters") {
    const DagNodeContainer& params = static_cast<const DagNodeContainer&>( args[0].getVariable().getValue() );
    setParameters ( params.clone() ) ;
    return RbNullObject::getInstance();
  }
  else if (name == "setParameter") {
    const Natural& index = static_cast<const Natural&>( args[0].getVariable().getValue() );
    const DagNodeContainer& params =  static_cast<const DagNodeContainer&>( args[1].getVariable().getValue() );
    setParameter ( (index.getValue()), params.clone() ) ;
    return RbNullObject::getInstance();
  }
  else if (name == "getAllocationVector") {
    return getAllocationVector();
  }
  else if (name == "setAllocationVector") {
    const DagNodeContainer& params = static_cast<const DagNodeContainer&>( args[0].getVariable().getValue() );
    setAllocationVector ( params.clone() ) ;
    return RbNullObject::getInstance();
  }
  else if (name == "allocateElement") {
    const Natural& index = static_cast<const Natural&>( args[0].getVariable().getValue() );
    const Natural& classId = static_cast<const Natural&>( args[1].getVariable().getValue() );
    allocateElement ((index.getValue()), (classId.getValue()));
    return RbNullObject::getInstance();
  }
  else if (name == "getElementAllocation") {
    const Natural& index = static_cast<const Natural&>( args[0].getVariable().getValue() );
    return getElementAllocation(index.getValue() );
  }
  else if (name == "setClassProbabilities") {
    const Simplex& params = static_cast<const Simplex&>( args[0].getVariable().getValue() );
    setClassProbabilities ( params.clone() ) ;
    return RbNullObject::getInstance();
  }
  else if (name == "getClassProbabilities") {
    return getClassProbabilities();
  }
  else if (name == "getParameterForElem") {
    // get the member with given index
    const Natural& index = static_cast<const Natural&>( args[0].getVariable().getValue() );
    
    if (allocationVector_->size() < (size_t)(index.getValue())) {
      throw RbException("Index out of bounds in Mixture::getParameterForElem");
    }
    // (DagNodeContainer*) getParameter(index->getValue());
    return  getParameterForElem(index.getValue());

  }
  else 
  {
    return MutableMemberObject::executeOperationSimple( name, args );
  }
}




/** Catch setting of the mixture variable */
void Mixture::setMemberVariable(const std::string& name, Variable* var) {
    
    if ( name == "allocationVector" ) {
        allocationVector_ = static_cast<DagNodeContainer*>( var->getValue().clone() );
    }
    else if ( name == "parameters" ) {
        parameters_ = static_cast<DagNodeContainer*>( var->getValue().clone() );
        RandomNumberGenerator* rng = GLOBAL_RNG;
        if (classProbabilities_ == 0) {
          std::vector<double> v(parameters_->size(), 1.0);
          classProbabilities_ = new Simplex( RbStatistics::Dirichlet::rv(v, *rng) );
        }
        else {
          if (classProbabilities_->size() != parameters_->size() ) {
            throw RbException("Class probability vector and parameter vector of different sizes. Cannot construct the mixture in Mixture::setMemberVariable.");
          }
            /*std::vector<double> v(parameters_->size(), 1.0);
            delete classProbabilities_;
            classProbabilities_ = new Simplex( RbStatistics::Dirichlet::rv(v, *rng) );*/
        }
      if (allocationVector_ != NULL ) { //we re-draw the allocation vector
          std::vector<int> allocationVec = RbStatistics::Multinomial::rv(classProbabilities_->getValue(), (int)allocationVector_->size(), *rng);
          for (size_t i = 0 ; i < allocationVector_->size() ; i++ ) {
              allocationVector_->setElement(i, new Variable(new ConstantNode( new Integer( allocationVec[i] ) ) ) );
          }
          indexAllocationVector();
      }

    }
    else if ( name == "classProbabilities" ) {
        classProbabilities_ =  static_cast<Simplex*>( var->getValue().clone() );
    }
    else if ( name == "numObservations" ) {
        int numObservations = static_cast<Integer&>( var->getValue() ).getValue();
        if (allocationVector_ != NULL ) {
            throw RbException("Mixture already constructed. Cannot reconstruct it in Mixture::setMemberVariable.");
        }
        else {
            RandomNumberGenerator* rng = GLOBAL_RNG;
            if (classProbabilities_ != NULL) {
                std::vector<int> allocationVec = RbStatistics::Multinomial::rv(classProbabilities_->getValue(), (int)numObservations, *rng);
                allocationVector_ = new DagNodeContainer (numObservations);
                for (size_t i = 0 ; i < numObservations ; i ++ ) {
                    allocationVector_->setElement(i, new Variable(new ConstantNode( new Integer( allocationVec[i] ) ) ) );
                }
                indexAllocationVector();
            }
            else {
                allocationVector_ = new DagNodeContainer (numObservations);
                for (size_t i = 0 ; i < numObservations ; i ++ ) {
                    allocationVector_->setElement(i, new Variable(new ConstantNode( new Integer( 0 ) ) ) );
                }
            }
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


/** Get the number of elements in the mixture */
size_t Mixture::getNumberOfElements() {
    return (allocationVector_->size());
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
      if (rvToNumber.find(formerlyAssignedValue) != rvToNumber.end()) {//Value met before
          allocationVector_->setElement(i, new Variable( new ConstantNode( new Integer(rvToNumber[formerlyAssignedValue]) ) ) );
      }
      else { //Value never met before
          const VariableSlot& slot = static_cast<const VariableSlot&>( (allocationVector_->getElement(i) ) );
          const Variable& var = slot.getVariable();
          var.getValue();
          const Integer& j = static_cast<const  Integer&> (  (var.getValue() ) );      
          rvToNumber[j] = maxIntSeen;
          allocationVector_->setElement(i, new Variable( new ConstantNode( new Integer(rvToNumber[formerlyAssignedValue]) ) ) );
          maxIntSeen = maxIntSeen +1;
      }
  }
  //Renumber the classProbabilities_ vector
  VectorRealPos& copy = *(classProbabilities_->clone());
 // std::cout <<"indexAllocationVector 8: classProbabilities_->size(): "<< classProbabilities_->size() <<" copy.size(): "<< copy.size()  <<std::endl;
    classProbabilities_->clear();
  for (unsigned int i = 0 ; i < copy.size() ; i++ ) { 
      classProbabilities_->AbstractVector::setElement(i, copy.getElement(rvToNumber[i] ).clone() );
  }    
}


/** Get the allocation vector associating class indices to elements */
DagNodeContainer& Mixture::getAllocationVector() {
    return *(allocationVector_ ) ;
}


/** Change the class of a particular element */
void Mixture::allocateElement (int elementId, int classId) {
    allocationVector_->setElement(elementId, new Variable( new ConstantNode( new Integer(classId) ) ) );
    return;
}


/** Get the class of a particular element */
Integer Mixture::getElementAllocation (int elementId) {
    const VariableSlot& slot = static_cast<const VariableSlot&>( (allocationVector_->getElement(elementId) ) );
    const Variable& var = slot.getVariable();
    var.getValue();
    const Integer& j = static_cast<const  Integer&> (  (var.getValue() ) );    
    return (j); 
}


/** Get the vector of parameter values associated to the classes of the mixture */
DagNodeContainer& Mixture::getParameters() {
    return *parameters_;
}

/** Set the allocation vector associating class indices to elements */

void Mixture::setAllocationVector(DagNodeContainer*  allocationVector) {
    allocationVector_ = allocationVector;
    return;
}


/** Set the vector containing class probabilities */
void Mixture::setClassProbabilities(Simplex* proba) {
    classProbabilities_ = proba->clone();
}


/** Get the vector containing class probabilities */
const Simplex& Mixture::getClassProbabilities() {
    return static_cast<const VectorRealPos&>( *classProbabilities_ );
}

/** Get the vector of parameter values associated to the element index */
DagNodeContainer& Mixture::getParameterForElem(unsigned int index) {
  const VariableSlot& slot = static_cast<const VariableSlot&>( (allocationVector_->getElement(index) ) );
  const Variable& var = slot.getVariable();
  var.getValue();
  const Integer& j = static_cast<const  Integer&> (  (var.getValue() ) );      
  return (getParameter(j.getValue() ) );
}

