#ifndef RlMemberFunction_H
#define RlMemberFunction_H

#include "AbstractMemberFunction.h"

namespace RevLanguage {
    
    
    
    template <typename memberObjectType, typename retType>
    class MemberFunction :  public AbstractMemberFunction {
        
    public:
        MemberFunction(const memberObjectType* o, ArgumentRules* argRules);                                             //!< Constructor
        
        // Basic utility functions
        MemberFunction*                     clone(void) const;                                                          //!< Clone the object
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
RevLanguage::MemberFunction<memberObjectType, retType>::MemberFunction( const memberObjectType* o, ArgumentRules* ar ) : AbstractMemberFunction( retType::getClassTypeSpec(), ar ),
    theMemberObject( o )
{
    
}


/** Clone object */
template <typename memberObjectType, typename retType>
RevLanguage::MemberFunction<memberObjectType, retType>* RevLanguage::MemberFunction<memberObjectType, retType>::clone( void ) const
{
    
    return new MemberFunction<memberObjectType, retType>( *this );
}


template <typename memberObjectType, typename retType>
RevLanguage::RevPtr<RevLanguage::Variable> RevLanguage::MemberFunction<memberObjectType, retType>::execute( void )
{
    
//    RevBayesCore::TypedDagNode<typename firstValType::valueType>* firstArg = static_cast<const firstValType &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
//    RevBayesCore::TypedDagNode<typename secondValType::valueType>* secondArg = static_cast<const secondValType &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    
    std::vector<const RevBayesCore::DagNode*> argNodes;
    for ( size_t i=0; i<args.size(); ++i)
    {
        argNodes.push_back( this->args[0].getVariable()->getRevObject().getDagNode() );
    }
    
    const RevBayesCore::TypedDagNode<typename memberObjectType::valueType>* o = theMemberObject->getDagNode();
//    const typename memberObjectType::valueType &rbMemberObject = theMemberObject->getValue();
//    o = dynamic_cast<const RevBayesCore::MemberObject<typename retType::valueType>* >( &rbMemberObject );
    if ( o == NULL )
    {
        throw RbException("Could not cast the member object.");
    }
    
    RevBayesCore::MemberFunction<typename memberObjectType::valueType, typename retType::valueType> *func = new RevBayesCore::MemberFunction<typename memberObjectType::valueType, typename retType::valueType>(name, o, argNodes);
    RevBayesCore::TypedDagNode<typename retType::valueType>* d  = new RevBayesCore::DeterministicNode<typename retType::valueType>("", func);
    
    return new Variable( new retType(d) );
}


template <typename memberObjectType, typename retType>
const std::string& RevLanguage::MemberFunction<memberObjectType, retType>::getClassType(void)
{
    
    static std::string revClassType = "MemberFunction<" + memberObjectType::getClassType() + "," + retType::getClassType() + ">";
    
	return revClassType;
}

/* Get class type spec describing type of object */
template <typename memberObjectType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::MemberFunction<memberObjectType, retType>::getClassTypeSpec(void)
{
    
    static TypeSpec revClassTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revClassTypeSpec;
}


template <typename memberObjectType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::MemberFunction<memberObjectType, retType>::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


#endif

