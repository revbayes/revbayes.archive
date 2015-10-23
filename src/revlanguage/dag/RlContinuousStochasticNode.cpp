#include "RlContinuousStochasticNode.h"


#include "RealPos.h"

RevLanguage::ContinuousStochasticNode::ContinuousStochasticNode( const std::string& n, RevBayesCore::ContinuousDistribution* dist, ContinuousDistribution* rlDist ) :
    RevBayesCore::ContinuousStochasticNode( n, dist ),
    rlDistribution( rlDist )
{
    
    ArgumentRules* clampArgRules = new ArgumentRules();
    clampArgRules->push_back( new ArgumentRule("x", Real::getClassTypeSpec(), "The observed value.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
    this->methods.addFunction( new MemberProcedure( "clamp", RlUtils::Void, clampArgRules) );
    
    ArgumentRules* redrawArgRules = new ArgumentRules();
    this->methods.addFunction( new MemberProcedure( "redraw", RlUtils::Void, redrawArgRules) );
    
    ArgumentRules* probArgRules = new ArgumentRules();
    this->methods.addFunction( new MemberProcedure( "probability", RealPos::getClassTypeSpec(), probArgRules) );
    
    ArgumentRules* lnprobArgRules = new ArgumentRules();
    this->methods.addFunction( new MemberProcedure( "lnProbability", Real::getClassTypeSpec(), lnprobArgRules) );
    
    ArgumentRules* setValueArgRules = new ArgumentRules();
    setValueArgRules->push_back( new ArgumentRule("x", Real::getClassTypeSpec(), "The value", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
    this->methods.addFunction( new MemberProcedure( "setValue", RlUtils::Void, setValueArgRules) );
    
    ArgumentRules* unclampArgRules = new ArgumentRules();
    this->methods.addFunction( new MemberProcedure( "unclamp", RlUtils::Void, unclampArgRules) );
    
}


RevLanguage::ContinuousStochasticNode::ContinuousStochasticNode( const std::string& n, RevBayesCore::ContinuousDistribution* dist, PositiveContinuousDistribution* rlDist ) :
    RevBayesCore::ContinuousStochasticNode( n, dist ),
    rlDistribution( rlDist )
{
    
    ArgumentRules* clampArgRules = new ArgumentRules();
    clampArgRules->push_back( new ArgumentRule("x", Real::getClassTypeSpec(), "The observed value.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
    this->methods.addFunction( new MemberProcedure( "clamp", RlUtils::Void, clampArgRules) );
    
    ArgumentRules* redrawArgRules = new ArgumentRules();
    this->methods.addFunction( new MemberProcedure( "redraw", RlUtils::Void, redrawArgRules) );
    
    ArgumentRules* probArgRules = new ArgumentRules();
    this->methods.addFunction( new MemberProcedure( "probability", RealPos::getClassTypeSpec(), probArgRules) );
    
    ArgumentRules* lnprobArgRules = new ArgumentRules();
    this->methods.addFunction( new MemberProcedure( "lnProbability", Real::getClassTypeSpec(), lnprobArgRules) );
    
    ArgumentRules* setValueArgRules = new ArgumentRules();
    setValueArgRules->push_back( new ArgumentRule("x", Real::getClassTypeSpec(), "The value", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
    this->methods.addFunction( new MemberProcedure( "setValue", RlUtils::Void, setValueArgRules) );
    
    ArgumentRules* unclampArgRules = new ArgumentRules();
    this->methods.addFunction( new MemberProcedure( "unclamp", RlUtils::Void, unclampArgRules) );
    
}


RevLanguage::ContinuousStochasticNode::ContinuousStochasticNode( const std::string& n, RevBayesCore::ContinuousDistribution* dist, TypedDistribution<Probability>* rlDist ) :
    RevBayesCore::ContinuousStochasticNode( n, dist ),
    rlDistribution( rlDist )
{
    
    ArgumentRules* clampArgRules = new ArgumentRules();
    clampArgRules->push_back( new ArgumentRule("x", Real::getClassTypeSpec(), "The observed value.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
    this->methods.addFunction( new MemberProcedure( "clamp", RlUtils::Void, clampArgRules) );
    
    ArgumentRules* redrawArgRules = new ArgumentRules();
    this->methods.addFunction( new MemberProcedure( "redraw", RlUtils::Void, redrawArgRules) );
    
    ArgumentRules* probArgRules = new ArgumentRules();
    this->methods.addFunction( new MemberProcedure( "probability", RealPos::getClassTypeSpec(), probArgRules) );
    
    ArgumentRules* lnprobArgRules = new ArgumentRules();
    this->methods.addFunction( new MemberProcedure( "lnProbability", Real::getClassTypeSpec(), lnprobArgRules) );
    
    ArgumentRules* setValueArgRules = new ArgumentRules();
    setValueArgRules->push_back( new ArgumentRule("x", Real::getClassTypeSpec(), "The value", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
    this->methods.addFunction( new MemberProcedure( "setValue", RlUtils::Void, setValueArgRules) );
    
    ArgumentRules* unclampArgRules = new ArgumentRules();
    this->methods.addFunction( new MemberProcedure( "unclamp", RlUtils::Void, unclampArgRules) );
    
}


RevLanguage::ContinuousStochasticNode::ContinuousStochasticNode( const RevLanguage::ContinuousStochasticNode &n ) :
    RevBayesCore::ContinuousStochasticNode( n ),
    rlDistribution( n.rlDistribution->clone() ),
    methods( n.methods )
{
    
}


RevLanguage::ContinuousStochasticNode::~ContinuousStochasticNode( void )
{
    
    delete rlDistribution;
    
}


RevLanguage::ContinuousStochasticNode* RevLanguage::ContinuousStochasticNode::clone( void ) const
{
    
    return new ContinuousStochasticNode( *this );
}


/* Execute calls to member methods */
RevLanguage::RevPtr<RevLanguage::RevVariable> RevLanguage::ContinuousStochasticNode::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    if (name == "clamp")
    {
        
        // we found the corresponding member method
        found = true;
        
        // get the observation
        const double &observation = static_cast<const Real &>( args[0].getVariable()->getRevObject() ).getValue();
        
        // clamp
        this->clamp( new double(observation) );
        
        return NULL;
    }
    else if (name == "lnProbability")
    {
        
        // we found the corresponding member method
        found = true;
        
        return RevPtr<RevVariable>( new RevVariable( new Real( this->getLnProbability() ), "" ) );
    }
    else if (name == "probability")
    {
        
        // we found the corresponding member method
        found = true;
        
        return RevPtr<RevVariable>( new RevVariable( new RealPos( exp( this->getLnProbability() ) ), "" ) );
    }
    else if (name == "redraw")
    {
        
        // we found the corresponding member method
        found = true;
        
        // redraw the value
        this->redraw();
        
        return NULL;
    }
    else if (name == "setValue")
    {
        
        // we found the corresponding member method
        found = true;
        
        // get the observation
        const double &observation = static_cast<const Real &>( args[0].getVariable()->getRevObject() ).getValue();
        
        // set value
        this->setValue( new double(observation) );
        
        return NULL;
    }
    else if (name == "unclamp")
    {
        
        // we found the corresponding member method
        found = true;
        
        // Unclamp
        this->unclamp();
        
        return NULL;
    }
    
    found = false;
    
    return NULL;
}


/**
 * Get common member methods.
 */
const RevLanguage::MethodTable& RevLanguage::ContinuousStochasticNode::getMethods( void ) const
{
    
    // return the internal value
    return methods;
}


RevLanguage::Distribution& RevLanguage::ContinuousStochasticNode::getRlDistribution( void )
{
    
    return *rlDistribution;
}


const RevLanguage::Distribution& RevLanguage::ContinuousStochasticNode::getRlDistribution( void ) const
{
    
    return *rlDistribution;
}
