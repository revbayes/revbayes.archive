#include "RlPosteriorPredictiveSimulation.h"
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

PosteriorPredictiveSimulation::PosteriorPredictiveSimulation() : WorkspaceToCoreWrapperObject<RevBayesCore::PosteriorPredictiveSimulation>()
{
    
    ArgumentRules* runArgRules = new ArgumentRules();
    runArgRules->push_back( new ArgumentRule("thinning", Natural::getClassTypeSpec(), "The number of samples to jump over.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(1)) );
    this->methods.addFunction( new MemberProcedure( "run", RlUtils::Void, runArgRules) );
    
}


PosteriorPredictiveSimulation::PosteriorPredictiveSimulation(const RevBayesCore::PosteriorPredictiveSimulation &m) : WorkspaceToCoreWrapperObject<RevBayesCore::PosteriorPredictiveSimulation>( new RevBayesCore::PosteriorPredictiveSimulation( m ) )
{
    
    ArgumentRules* runArgRules = new ArgumentRules();
    runArgRules->push_back( new ArgumentRule("thinning", Natural::getClassTypeSpec(), "The number of samples to jump over.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(1)) );
    this->methods.addFunction( new MemberProcedure( "run", RlUtils::Void, runArgRules) );
    
    
}


/** Clone object */
PosteriorPredictiveSimulation* RevLanguage::PosteriorPredictiveSimulation::clone(void) const
{
    
    return new PosteriorPredictiveSimulation(*this);
}


void PosteriorPredictiveSimulation::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new PosteriorPredictiveSimulation object
    const RevBayesCore::Model&                              mdl     = static_cast<const Model &>( model->getRevObject() ).getValue();
    
    RevBayesCore::RbVector<RevBayesCore::ModelTrace>      pt;
    const WorkspaceVector<ModelTrace> &      tmp_pt     = static_cast<const WorkspaceVector<ModelTrace> &>( trace->getRevObject() );
    for ( size_t i=0; i<tmp_pt.size(); ++i)
    {
        const RevBayesCore::ModelTrace &tr = tmp_pt.getElement( i )->getValue();
        pt.push_back( tr );
    }
    
    const std::string &    dir   = static_cast<const RlString &>( directory->getRevObject() ).getValue();
    
    value = new RevBayesCore::PosteriorPredictiveSimulation(mdl, dir, pt);
    
}


/* Map calls to member methods */
RevPtr<RevLanguage::RevVariable> RevLanguage::PosteriorPredictiveSimulation::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
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
const std::string& RevLanguage::PosteriorPredictiveSimulation::getClassType(void)
{
    
    //    static std::string revType = "PosteriorPredictiveSimulation<" + treeType::getClassType() + ">";
    static std::string revType = "PosteriorPredictiveSimulation";
    
    return revType;
}

/** Get class type spec describing type of object */
const RevLanguage::TypeSpec& RevLanguage::PosteriorPredictiveSimulation::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( WorkspaceToCoreWrapperObject<RevBayesCore::PosteriorPredictiveSimulation>::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the Rev name for the constructor function.
 *
 * \return Rev name of constructor function.
 */
std::string PosteriorPredictiveSimulation::getConstructorFunctionName( void ) const
{
    // create a constructor function name variable that is the same for all instance of this class
    std::string c_name = "posteriorPredictiveSimulation";
    
    return c_name;
}


/** Return member rules (no members) */
const MemberRules& PosteriorPredictiveSimulation::getParameterRules(void) const
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
const TypeSpec& PosteriorPredictiveSimulation::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void PosteriorPredictiveSimulation::printValue(std::ostream &o) const
{
    
    o << "PosteriorPredictiveSimulation";
}


/** Set a member variable */
void PosteriorPredictiveSimulation::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
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
