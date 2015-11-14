#include "RlPosteriorPredictiveCharacterDataSimulation.h"
#include "ArgumentRules.h"
#include "MemberProcedure.h"
#include "MethodTable.h"
#include "Natural.h"
#include "Probability.h"
#include "RlModel.h"
#include "RlString.h"
#include "RlModelTrace.h"
#include "RlUtils.h"
#include "TreeSummary.h"
#include "WorkspaceVector.h"

using namespace RevLanguage;

PosteriorPredictiveCharacterDataSimulation::PosteriorPredictiveCharacterDataSimulation() : WorkspaceToCoreWrapperObject<RevBayesCore::PosteriorPredictiveCharacterDataSimulation>()
{
    
    ArgumentRules* runArgRules = new ArgumentRules();
    runArgRules->push_back( new ArgumentRule("thinning", Natural::getClassTypeSpec(), "The number of samples to jump over.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(1)) );
    this->methods.addFunction( new MemberProcedure( "run", RlUtils::Void, runArgRules) );
    
}


PosteriorPredictiveCharacterDataSimulation::PosteriorPredictiveCharacterDataSimulation(const RevBayesCore::PosteriorPredictiveCharacterDataSimulation &m) : WorkspaceToCoreWrapperObject<RevBayesCore::PosteriorPredictiveCharacterDataSimulation>( new RevBayesCore::PosteriorPredictiveCharacterDataSimulation( m ) )
{
    
    ArgumentRules* runArgRules = new ArgumentRules();
    runArgRules->push_back( new ArgumentRule("thinning", Natural::getClassTypeSpec(), "The number of samples to jump over.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(1)) );
    this->methods.addFunction( new MemberProcedure( "run", RlUtils::Void, runArgRules) );
    
    
}


/** Clone object */
PosteriorPredictiveCharacterDataSimulation* RevLanguage::PosteriorPredictiveCharacterDataSimulation::clone(void) const
{
    
    return new PosteriorPredictiveCharacterDataSimulation(*this);
}


void PosteriorPredictiveCharacterDataSimulation::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new PosteriorPredictiveCharacterDataSimulation object
    const RevBayesCore::Model&                              mdl     = static_cast<const Model &>( model->getRevObject() ).getValue();
    RevBayesCore::RbVector<RevBayesCore::ModelTrace>      pt;
    const WorkspaceVector<ModelTrace> &      tmp_pt     = static_cast<const WorkspaceVector<ModelTrace> &>( trace->getRevObject() );
    for ( size_t i=0; i<tmp_pt.size(); ++i)
    {
        const RevBayesCore::ModelTrace &tr = tmp_pt.getElement( i )->getValue();
        pt.push_back( tr );
    }
    const std::string &    dir   = static_cast<const RlString &>( directory->getRevObject() ).getValue();
    
    value = new RevBayesCore::PosteriorPredictiveCharacterDataSimulation(mdl, dir, pt);
    
}


/* Map calls to member methods */
RevPtr<RevLanguage::RevVariable> RevLanguage::PosteriorPredictiveCharacterDataSimulation::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    if (name == "run")
    {
        found = true;
        
        double t            = static_cast<const Natural &>( args[0].getVariable()->getRevObject() ).getValue();
        this->value->run( t );
        
        return NULL;
    }
    
    return RevObject::executeMethod( name, args, found );
}


/** Get Rev type of object */
const std::string& RevLanguage::PosteriorPredictiveCharacterDataSimulation::getClassType(void)
{
    
    //    static std::string revType = "PosteriorPredictiveCharacterDataSimulation<" + treeType::getClassType() + ">";
    static std::string revType = "PosteriorPredictiveCharacterDataSimulation";
    
    return revType;
}

/** Get class type spec describing type of object */
const RevLanguage::TypeSpec& RevLanguage::PosteriorPredictiveCharacterDataSimulation::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( WorkspaceToCoreWrapperObject<RevBayesCore::PosteriorPredictiveCharacterDataSimulation>::getClassTypeSpec() ) );
    
    return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& PosteriorPredictiveCharacterDataSimulation::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        memberRules.push_back( new ArgumentRule("model", Model::getClassTypeSpec(), "The reference model instance.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule("directory", RlString::getClassTypeSpec(), "The name of the directory where we store the simulations.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule("trace", WorkspaceVector<ModelTrace>::getClassTypeSpec(), "The sample trace object.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


/** Get type spec */
const TypeSpec& PosteriorPredictiveCharacterDataSimulation::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void PosteriorPredictiveCharacterDataSimulation::printValue(std::ostream &o) const
{
    
    o << "PosteriorPredictiveCharacterDataSimulation";
}


/** Set a member variable */
void PosteriorPredictiveCharacterDataSimulation::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "model")
    {
        model = var;
    }
    else if ( name == "filetype" )
    {
        filetype = var;
    }
    else if ( name == "directory" )
    {
        directory = var;
    }
    else if ( name == "trace" )
    {
        trace = var;
    }
    else
    {
        RevObject::setConstParameter(name, var);
    }
}
