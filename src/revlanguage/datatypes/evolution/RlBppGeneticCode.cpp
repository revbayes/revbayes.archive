#include "ConstantNode.h"
#include "RlBppGeneticCode.h"
#include "TimeAndDate.h"
#include "RbUtil.h"
#include "RlString.h"
#include "TypeSpec.h"
#include "Vector.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
BppGeneticCode::BppGeneticCode(void) : ModelObject<RevBayesCore::BppGeneticCode>() {
    
}

/** Construct from core BppGeneticCode */
BppGeneticCode::BppGeneticCode(RevBayesCore::BppGeneticCode *c) : ModelObject<RevBayesCore::BppGeneticCode>( c ) {
    
}

/** Construct from core BppGeneticCode */
BppGeneticCode::BppGeneticCode(const RevBayesCore::BppGeneticCode &t) : ModelObject<RevBayesCore::BppGeneticCode>( new RevBayesCore::BppGeneticCode( t ) ) {
    
}

/** Construct from DAG node */
BppGeneticCode::BppGeneticCode(RevBayesCore::TypedDagNode<RevBayesCore::BppGeneticCode> *n) : ModelObject<RevBayesCore::BppGeneticCode>( n ) {
    
}



/** Construct */
BppGeneticCode::BppGeneticCode(const BppGeneticCode &t) : ModelObject<RevBayesCore::BppGeneticCode>( t ) {
    
}


/** Clone object */
BppGeneticCode* BppGeneticCode::clone(void) const {
    
    return new BppGeneticCode(*this);
}


void BppGeneticCode::constructInternalObject( void ) 
{
    // we free the memory first
    if ( value != NULL )
    {
        if ( value->decrementReferenceCount() == 0 ) 
        {
            delete value;
        }
    }
    
    // now allocate a new BppGeneticCode
    const bpp::GeneticCode* pgc = getValue().getGeneticCode();
    
    value = new RevBayesCore::ConstantNode<RevBayesCore::BppGeneticCode>("", new RevBayesCore::BppGeneticCode( pgc ) );
    value->incrementReferenceCount();
}


/* Map calls to member methods */
RevLanguage::RevObject* BppGeneticCode::executeMethod(std::string const &name, const std::vector<Argument> &args) {
  return ModelObject<RevBayesCore::BppGeneticCode>::executeMethod( name, args );
}



/** Return member rules (no members) */
const MemberRules& BppGeneticCode::getMemberRules(void) const {
    
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
const std::string& BppGeneticCode::getClassName(void) { 
    
    static std::string rbClassName = "BppGeneticCode";
    
    return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& BppGeneticCode::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
    return rbClass; 
}


/* Get method specifications */
const RevLanguage::MethodTable& BppGeneticCode::getMethods(void) const {
    
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
const TypeSpec& BppGeneticCode::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Set a member variable */
void BppGeneticCode::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var)
{
  
}

