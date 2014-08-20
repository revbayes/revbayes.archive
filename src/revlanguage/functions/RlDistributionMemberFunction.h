#ifndef RlDistributionMemberFunction_H
#define RlDistributionMemberFunction_H

#include "AbstractMemberFunction.h"

namespace RevLanguage {
    
    
    
    template <typename memberObjectType, typename retType>
    class DistributionMemberFunction :  public AbstractMemberFunction {
        
    public:
        DistributionMemberFunction(const memberObjectType* o, ArgumentRules* argRules);                                 //!< Constructor
        
        // Basic utility functions
        DistributionMemberFunction*         clone(void) const;                                              //!< Clone the object
        static const std::string&           getClassType(void);                                                         //!< Get class name
        static const TypeSpec&              getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                                    //!< Get language type of the object
        
        // pure virtual functions
        RevPtr<Variable>                    execute(void);                                                              //!< Execute function
        
    private:
        
        const memberObjectType*             theMemberObject;
    };
    
}


#include "MemberFunction.h"
#include "RbException.h"

/** default constructor */
template <typename memberObjectType, typename retType>
RevLanguage::DistributionMemberFunction<memberObjectType, retType>::DistributionMemberFunction( const memberObjectType* o, ArgumentRules* ar ) : AbstractMemberFunction( retType::getClassTypeSpec(), ar ),
    theMemberObject( o )
{
    
}


/** Clone object */
template <typename memberObjectType, typename retType>
RevLanguage::DistributionMemberFunction<memberObjectType, retType>* RevLanguage::DistributionMemberFunction<memberObjectType, retType>::clone( void ) const
{
    
    return new DistributionMemberFunction<memberObjectType, retType>( *this );
}


template <typename memberObjectType, typename retType>
RevLanguage::RevPtr<RevLanguage::Variable> RevLanguage::DistributionMemberFunction<memberObjectType, retType>::execute( void )
{
    
    std::vector<const RevBayesCore::DagNode*> argNodes;
    for ( size_t i=0; i<args.size(); ++i)
    {
        argNodes.push_back( this->args[i].getVariable()->getRevObject().getDagNode() );
    }
    
//    RevBayesCore::MemberFunction<typename memberObjectType::valueType, typename retType::valueType> *func = new RevBayesCore::MemberFunction<typename memberObjectType::valueType, typename retType::valueType>(name, o, argNodes);
//    RevBayesCore::TypedDagNode<typename retType::valueType>* d  = new RevBayesCore::DeterministicNode<typename retType::valueType>("", func);
    
//    return new Variable( new retType(d) );
    
    return NULL;
}


template <typename memberObjectType, typename retType>
const std::string& RevLanguage::DistributionMemberFunction<memberObjectType, retType>::getClassType(void)
{
    
    static std::string revClassType = "DistributionMemberFunction<" + memberObjectType::getClassType() + "," + retType::getClassType() + ">";
    
	return revClassType;
}

/* Get class type spec describing type of object */
template <typename memberObjectType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::DistributionMemberFunction<memberObjectType, retType>::getClassTypeSpec(void)
{
    
    static TypeSpec revClassTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revClassTypeSpec;
}


template <typename memberObjectType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::DistributionMemberFunction<memberObjectType, retType>::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


#endif

