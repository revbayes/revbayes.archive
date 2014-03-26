#include "ConstantNode.h"
#include "RlTaxon.h"
#include "TimeAndDate.h"
#include "RbUtil.h"
#include "RlString.h"
#include "TypeSpec.h"
#include "Vector.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
Taxon::Taxon(void) : RlModelVariableWrapper<RevBayesCore::Taxon>() {
    
}

/** Construct from core Taxon */
Taxon::Taxon(RevBayesCore::Taxon *c) : RlModelVariableWrapper<RevBayesCore::Taxon>( c ) {
    
}

/** Construct from core Taxon */
Taxon::Taxon(const RevBayesCore::Taxon &t) : RlModelVariableWrapper<RevBayesCore::Taxon>( new RevBayesCore::Taxon( t ) ) {
    
}

/** Construct from DAG node */
Taxon::Taxon(RevBayesCore::TypedDagNode<RevBayesCore::Taxon> *n) : RlModelVariableWrapper<RevBayesCore::Taxon>( n ) {
    
}



/** Construct */
Taxon::Taxon(const Taxon &t) : RlModelVariableWrapper<RevBayesCore::Taxon>( t ) {
    
}


/** Clone object */
Taxon* Taxon::clone(void) const {
    
    return new Taxon(*this);
}


void Taxon::constructInternalObject( void ) 
{
    // we free the memory first
    if ( value != NULL )
    {
        if ( value->decrementReferenceCount() == 0 ) 
        {
            delete value;
        }
    }
    
    // now allocate a new Taxon
    std::string taxonName = static_cast<const RlString &>( (taxon)->getValue() ).getValue() ;
    std::string taxonSpecies = static_cast<const RlString &>( (species)->getValue() ).getValue() ;
  //  std::string taxonDate = static_cast<const RlDate &>( (date)->getValue() ).getValue() ;
    RevBayesCore::TimeAndDate d = RevBayesCore::TimeAndDate();
    
    value = new RevBayesCore::ConstantNode<RevBayesCore::Taxon>("", new RevBayesCore::Taxon( taxonName, taxonSpecies ) );
    value->incrementReferenceCount();
    
}


/* Map calls to member methods */
RevLanguage::RbLanguageObject* Taxon::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
//    if (name == "nnodes") {
//        size_t n = this->value->getValue().getNumberOfNodes();
//        return new Natural( n );
//    }
//    else if (name == "names") {
//        const std::vector<std::string>& n = this->value->getValue().getNames();
//        return new Vector<RlString>( n );
//    } 
    
    return RlModelVariableWrapper<RevBayesCore::Taxon>::executeMethod( name, args );
}



/** Return member rules (no members) */
const MemberRules& Taxon::getMemberRules(void) const {
    
    static MemberRules modelMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        modelMemberRules.push_back( new ArgumentRule("taxonName", true, RlString::getClassTypeSpec() ) );
        modelMemberRules.push_back( new ArgumentRule("speciesName", true, RlString::getClassTypeSpec() ) );
      //  modelMemberRules.push_back( new ArgumentRule("date", true, RlDate::getClassTypeSpec() ) );
                
        rulesSet = true;
    }
    
    return modelMemberRules;
}


/** Get class name of object */
const std::string& Taxon::getClassName(void) { 
    
    static std::string rbClassName = "Taxon";
    
    return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Taxon::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbLanguageObject::getClassTypeSpec() ) );
    
    return rbClass; 
}


/* Get method specifications */
const RevLanguage::MethodTable& Taxon::getMethods(void) const {
    
    static MethodTable    methods                     = MethodTable();
    static bool           methodsSet                  = false;
    
    if ( methodsSet == false ) {
        
//        ArgumentRules* nnodesArgRules = new ArgumentRules();
//        methods.addFunction("nnodes", new MemberFunction(Natural::getClassTypeSpec(),       nnodesArgRules              ) );
//        
//        ArgumentRules* namesArgRules = new ArgumentRules();
//        methods.addFunction("names", new MemberFunction(Vector<RlString>::getClassTypeSpec(),  namesArgRules              ) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &RbLanguageObject::getMethods() );
        methodsSet = true;
    }
    
    
    return methods;
}


/** Get type spec */
const TypeSpec& Taxon::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Set a member variable */
void Taxon::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
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
        RbLanguageObject::setConstMemberVariable(name, var);
    }
}

