#include "ConstantNode.h"
#include "ModelVector.h"
#include "RlTaxon.h"
#include "TimeAndDate.h"
#include "RbUtil.h"
#include "RlString.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
Taxon::Taxon(void) : ModelObject<RevBayesCore::Taxon>() {
    
}

/** Construct from core Taxon */
Taxon::Taxon(RevBayesCore::Taxon *c) : ModelObject<RevBayesCore::Taxon>( c ) {
    
}

/** Construct from core Taxon */
Taxon::Taxon(const RevBayesCore::Taxon &t) : ModelObject<RevBayesCore::Taxon>( new RevBayesCore::Taxon( t ) ) {
    
}

/** Construct from DAG node */
Taxon::Taxon(RevBayesCore::TypedDagNode<RevBayesCore::Taxon> *n) : ModelObject<RevBayesCore::Taxon>( n ) {
    
}



/** Construct */
Taxon::Taxon(const Taxon &t) : ModelObject<RevBayesCore::Taxon>( t ) {
    
}


/** Clone object */
Taxon* Taxon::clone(void) const {
    
    return new Taxon(*this);
}


void Taxon::constructInternalObject( void ) 
{
    // we free the memory first
    if ( dagNode != NULL )
    {
        if ( dagNode->decrementReferenceCount() == 0 )
        {
            delete dagNode;
        }
    }
    
    // now allocate a new Taxon
    std::string taxonName = static_cast<const RlString &>( (taxon)->getRevObject() ).getValue() ;
    std::string taxonSpecies = static_cast<const RlString &>( (species)->getRevObject() ).getValue() ;
//    std::string taxonDate = static_cast<const RlDate &>( (date)->getRevObject() ).getValue() ;
//    RevBayesCore::TimeAndDate d = RevBayesCore::TimeAndDate();
    
    dagNode = new RevBayesCore::ConstantNode<RevBayesCore::Taxon>("", new RevBayesCore::Taxon( taxonName, taxonSpecies ) );
    dagNode->incrementReferenceCount();
    
}


/* Map calls to member methods */
RevLanguage::RevPtr<RevLanguage::Variable> Taxon::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "getSpeciesName") {
        std::string n = this->dagNode->getValue().getSpeciesName();
        return RevPtr<Variable>( new Variable( new RlString( n ) ) );
    }
//    else if (name == "names") {
//        const std::vector<std::string>& n = this->value->getValue().getNames();
//        return new ModelVector<RlString>( n );
//    } 
    
    return ModelObject<RevBayesCore::Taxon>::executeMethod( name, args );
}



/** Return member rules (no members) */
const MemberRules& Taxon::getMemberRules(void) const {
    
    static MemberRules modelMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        modelMemberRules.push_back( new ArgumentRule("taxonName", true, RlString::getClassTypeSpec() ) );
        modelMemberRules.push_back( new ArgumentRule("speciesName", true, RlString::getClassTypeSpec() ) );
      //  modelMemberRules.push_back( new ArgumentRule("date", true, RlDate::getClassTypeSpec() ) );
                
        rulesSet = true;
    }
    
    return modelMemberRules;
}


/** Get Rev type of object */
const std::string& Taxon::getClassType(void) { 
    
    static std::string revType = "Taxon";
    
    return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Taxon::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
    return revTypeSpec; 
}


<<<<<<< HEAD
/**
 * Get member methods. We construct the appropriate static member
 * function table here.
 */
const RevLanguage::MethodTable& Taxon::getMethods( void ) const
{
    static MethodTable  myMethods   = MethodTable();
    static bool         methodsSet  = false;
    
    if ( !methodsSet )
    {
        myMethods = makeMethods();
=======
/* Get method specifications */
const RevLanguage::MethodTable& Taxon::getMethods(void) const {
    
    static MethodTable    methods                     = MethodTable();
    static bool           methodsSet                  = false;
    
    if ( methodsSet == false ) {
        
        ArgumentRules* speciesNameArgRules = new ArgumentRules();
        methods.addFunction("getSpeciesName", new MemberProcedure(RlString::getClassTypeSpec(),       speciesNameArgRules              ) );
//
//        ArgumentRules* namesArgRules = new ArgumentRules();
//        methods.addFunction("names", new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(),  namesArgRules              ) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &RevObject::getMethods() );
>>>>>>> 65ec182a54adf3e576d25ddf723d8318bd66d281
        methodsSet = true;
    }
    
    return myMethods;
}


/** Get type spec */
const TypeSpec& Taxon::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Set a member variable */
void Taxon::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "taxonName") 
    {
        taxon = var ;
    } 
    else if ( name == "speciesName") 
    {
        species = var ;
    } 
    else if ( name == "date") 
    {
        date = var ;
    } 
    else {
        RevObject::setConstMemberVariable(name, var);
    }
}


/** Make methods for this class */
RevLanguage::MethodTable Taxon::makeMethods(void) const
{ 
    MethodTable methods = MethodTable();
        
    ArgumentRules* speciesNameArgRules = new ArgumentRules();
    methods.addFunction("getSpeciesName", new MemberProcedure(RlString::getClassTypeSpec(),       speciesNameArgRules              ) );

//    ArgumentRules* namesArgRules = new ArgumentRules();
//    methods.addFunction("names", new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(),  namesArgRules              ) );
        
    // Insert inherited methods
    methods.insertInheritedMethods( ModelObject<RevBayesCore::Taxon>::makeMethods() );

    return methods;
}


