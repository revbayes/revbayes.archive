#include "ConstantNode.h"
#include "RlBppAlphabet.h"
#include "TimeAndDate.h"
#include "RbUtil.h"
#include "RlString.h"
#include "TypeSpec.h"
#include "Vector.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
BppAlphabet::BppAlphabet(void) : ModelObject<RevBayesCore::BppAlphabet>() {
    
}

/** Construct from core BppAlphabet */
BppAlphabet::BppAlphabet(RevBayesCore::BppAlphabet *c) : ModelObject<RevBayesCore::BppAlphabet>( c ) {
}

/** Construct from core BppAlphabet */
BppAlphabet::BppAlphabet(const RevBayesCore::BppAlphabet &t) : ModelObject<RevBayesCore::BppAlphabet>( new RevBayesCore::BppAlphabet( t ) ) {
    
}

/** Construct from DAG node */
BppAlphabet::BppAlphabet(RevBayesCore::TypedDagNode<RevBayesCore::BppAlphabet> *n) : ModelObject<RevBayesCore::BppAlphabet>( n ) {
    
}



/** Construct */
BppAlphabet::BppAlphabet(const BppAlphabet &t) : ModelObject<RevBayesCore::BppAlphabet>( t ) {
    
}


/** Clone object */
BppAlphabet* BppAlphabet::clone(void) const
{
  return new BppAlphabet(*this);
}


void BppAlphabet::constructInternalObject( void ) 
{
    // we free the memory first
  if ( dagNode != NULL )
  {
    if ( dagNode->decrementReferenceCount() == 0 ) 
    {
      delete dagNode;
    }
  }
    
  // now allocate a new BppAlphabet
  const bpp::Alphabet* alph = getValue().getAlphabet();
  
  dagNode = new RevBayesCore::ConstantNode<RevBayesCore::BppAlphabet>("", new RevBayesCore::BppAlphabet( alph ) );
  dagNode->incrementReferenceCount();
}


/* Map calls to member methods */
RevLanguage::RevObject* BppAlphabet::executeMethod(std::string const &name, const std::vector<Argument> &args) {
  return ModelObject<RevBayesCore::BppAlphabet>::executeMethod( name, args );
}



/** Return member rules (no members) */
const MemberRules& BppAlphabet::getMemberRules(void) const {
    
    static MemberRules modelMemberRules;
    // static bool rulesSet = false;
    
    // if ( !rulesSet ) {
    //     modelMemberRules.push_back( new ArgumentRule("taxonName", true, RlString::getClassTypeSpec() ) );
    //     modelMemberRules.push_back( new ArgumentRule("speciesName", true, RlString::getClassTypeSpec() ) );
    //   //  modelMemberRules.push_back( new ArgumentRule("date", true, RlDate::getClassTypeSpec() ) );
                
    //     rulesSet = true;
    // }
    
    return modelMemberRules;
}


/** Get class name of object */
const std::string& BppAlphabet::getClassName(void) { 
    
    static std::string rbClassName = "BppAlphabet";
    
    return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& BppAlphabet::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
    return rbClass; 
}


/* Get method specifications */
const RevLanguage::MethodTable& BppAlphabet::getMethods(void) const {
    
    static MethodTable    methods                     = MethodTable();
    static bool           methodsSet                  = false;
    
    if ( methodsSet == false ) {
        
//        ArgumentRules* nnodesArgRules = new ArgumentRules();
//        methods.addFunction("nnodes", new MemberFunction(Natural::getClassTypeSpec(),       nnodesArgRules              ) );
//        
//        ArgumentRules* namesArgRules = new ArgumentRules();
//        methods.addFunction("names", new MemberFunction(Vector<RlString>::getClassTypeSpec(),  namesArgRules              ) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &RevObject::getMethods() );
        methodsSet = true;
    }
    
    
    return methods;
}


/** Get type spec */
const TypeSpec& BppAlphabet::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Set a member variable */
void BppAlphabet::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var)
{
  
}

