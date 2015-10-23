#ifndef RlStochasticNode_H
#define RlStochasticNode_H

#include "StochasticNode.h"
#include "RevMemberObject.h"

namespace RevLanguage {
    
    template<class valueType>
    class StochasticNode : public RevBayesCore::StochasticNode<valueType>, public RevMemberObject {
        
    public:
        StochasticNode(const std::string& n, RevBayesCore::TypedDistribution<valueType>* dist, Distribution* rlDist);
        StochasticNode(const StochasticNode<valueType> &n);
        virtual                            ~StochasticNode(void);
        
        // public methods
        StochasticNode<valueType>*          clone(void) const;                                                                              //!< Clone the node
        virtual RevPtr<RevVariable>         executeMethod(const std::string& name, const std::vector<Argument>& args, bool &found);         //!< Execute member method (if applicable)
        const MethodTable&                  getMethods( void ) const;                                                                       //!< Get the member methods
        Distribution&                       getRlDistribution(void);                                                                        //!< Get the Rev distribution
        const Distribution&                 getRlDistribution(void) const;                                                                  //!< Get the Rev distribution (const)

    private:
        
        Distribution*                       rlDistribution;                                                                                 //!< Rev distribution
        MethodTable                         methods;
        
        
    };
    
}


#include "RealPos.h"

template<class valueType>
RevLanguage::StochasticNode<valueType>::StochasticNode( const std::string& n, RevBayesCore::TypedDistribution<valueType>* dist, Distribution* rlDist ) :
    RevBayesCore::StochasticNode<valueType>( n, dist ),
    rlDistribution( rlDist )
{
    
    ArgumentRules* clampArgRules = new ArgumentRules();
    clampArgRules->push_back( new ArgumentRule("x", rlDistribution->getVariableTypeSpec(), "The observed value.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
    this->methods.addFunction( new MemberProcedure( "clamp",  RlUtils::Void, clampArgRules) );
    
    ArgumentRules* redrawArgRules = new ArgumentRules();
    this->methods.addFunction( new MemberProcedure( "redraw", RlUtils::Void, redrawArgRules) );
    
    ArgumentRules* probArgRules = new ArgumentRules();
    this->methods.addFunction( new MemberProcedure( "probability", RealPos::getClassTypeSpec(), probArgRules) );
    
    ArgumentRules* lnprobArgRules = new ArgumentRules();
    this->methods.addFunction( new MemberProcedure( "lnProbability", Real::getClassTypeSpec(), lnprobArgRules) );
    
    ArgumentRules* setValueArgRules = new ArgumentRules();
    setValueArgRules->push_back( new ArgumentRule("x", rlDistribution->getVariableTypeSpec(), "The value.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
    this->methods.addFunction( new MemberProcedure( "setValue", RlUtils::Void, setValueArgRules) );
    
    ArgumentRules* unclampArgRules = new ArgumentRules();
    this->methods.addFunction( new MemberProcedure( "unclamp", RlUtils::Void, unclampArgRules) );
    
    // add the distribution member methods
    RevMemberObject* mo = dynamic_cast<RevMemberObject*>( rlDistribution );
    if ( mo != NULL)
    {
        const MethodTable &distMethods = mo->getMethods();
        methods.insertInheritedMethods( distMethods );
    }
    
}


template<class valueType>
RevLanguage::StochasticNode<valueType>::StochasticNode( const RevLanguage::StochasticNode<valueType> &n ) :
    RevBayesCore::StochasticNode<valueType>( n ),
    rlDistribution( n.rlDistribution->clone() ),
    methods( n.methods )
{
    
}


template<class valueType>
RevLanguage::StochasticNode<valueType>::~StochasticNode( void )
{
    
    delete rlDistribution;
    
}


template<class valueType>
RevLanguage::StochasticNode<valueType>* RevLanguage::StochasticNode<valueType>::clone( void ) const
{
    
    return new StochasticNode<valueType>( *this );
}


/* Execute calls to member methods */
template <typename valueType>
RevLanguage::RevPtr<RevLanguage::RevVariable> RevLanguage::StochasticNode<valueType>::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    // execute the distribution member methods
    RevMemberObject* mo = dynamic_cast<RevMemberObject*>( rlDistribution );
    if ( mo != NULL)
    {
        RevPtr<RevVariable> retVal = mo->executeMethod(name, args, found);
        
        if ( found == true )
        {
            return retVal;
        }
    }
    
    std::vector<RevBayesCore::DagNode*> distArgs;
    for (size_t i = 0; i < args.size(); ++i)
    {
        try
        {
            distArgs.push_back( args[i].getVariable()->getRevObject().getDagNode() );
        } catch ( ... )
        {
            // nothing to throw, just keep going
        }
    }
    this->distribution->executeProcedure(name, distArgs, found);
    if ( found == true )
    {
        return NULL;
    }

    
    if (name == "clamp")
    {
        
        // we found the corresponding member method
        found = true;
        
        // get the observation
        const valueType &observation = static_cast<const ModelObject<valueType> &>( args[0].getVariable()->getRevObject() ).getValue();
        
        // clamp
        this->clamp( RevBayesCore::Cloner<valueType, IsDerivedFrom<valueType, RevBayesCore::Cloneable>::Is >::createClone( observation ) );
        
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
        const valueType &observation = static_cast<const ModelObject<valueType> &>( args[0].getVariable()->getRevObject() ).getValue();
        
        // set value
        this->setValue( RevBayesCore::Cloner<valueType, IsDerivedFrom<valueType, RevBayesCore::Cloneable>::Is >::createClone( observation ) );
        
        // mark this node to ignore redraws
        this->setIgnoreRedraw( true );
        
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
template<class valueType>
const RevLanguage::MethodTable& RevLanguage::StochasticNode<valueType>::getMethods( void ) const
{
    
    // return the internal value
    return methods;
}


template<class valueType>
RevLanguage::Distribution& RevLanguage::StochasticNode<valueType>::getRlDistribution( void )
{
    
    return *rlDistribution;
}


template<class valueType>
const RevLanguage::Distribution& RevLanguage::StochasticNode<valueType>::getRlDistribution( void ) const
{
    
    return *rlDistribution;
}


#endif

