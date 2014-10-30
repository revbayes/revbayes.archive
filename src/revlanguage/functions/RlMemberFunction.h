#ifndef RlMemberFunction_H
#define RlMemberFunction_H

#include "RlMemberMethod.h"
#include "RlTypedFunction.h"


namespace RevLanguage {
    
    template <typename memberObjectType, typename retType>
    class MemberFunction : public TypedFunction<retType>, public MemberMethod {
        
    public:
        MemberFunction(const memberObjectType* o, ArgumentRules* argRules);                                             //!< Constructor
        
        // Basic utility functions
        MemberFunction*                                                 clone(void) const;                              //!< Clone the object
        static const std::string&                                       getClassType(void);                             //!< Get class name
        static const TypeSpec&                                          getClassTypeSpec(void);                         //!< Get class type spec
        const TypeSpec&                                                 getTypeSpec(void) const;                        //!< Get language type of the object
        
        // Regular functions
        RevBayesCore::TypedFunction< typename retType::valueType>*      createFunction(void) const;                     //!< Execute function
        void                                                            setMemberObject(const RevPtr<Variable> &obj);   //!< Set the member object to which this function belongs
        const ArgumentRules&                                            getArgumentRules(void) const;                   //!< Get argument rules
        void                                                            setMethodName(const std::string& name);         //!< Set name of member method
        
        
    private:
        
        const ArgumentRules*                                            argumentRules;                                  //!< Argument rules (different for different member functions)
        std::string                                                     funcName;                                       //!< Name of member method
        RevPtr<Variable>                                                object;
        const memberObjectType*                                         theMemberObject;
    };
    
}


#include "MemberFunction.h"
#include "RbException.h"

/** default constructor */
template <typename memberObjectType, typename retType>
RevLanguage::MemberFunction<memberObjectType, retType>::MemberFunction( const memberObjectType* o, ArgumentRules* ar ) : TypedFunction<retType>(),
    argumentRules( ar ),
    object( NULL ),
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
RevBayesCore::TypedFunction< typename retType::valueType >* RevLanguage::MemberFunction<memberObjectType, retType>::createFunction( void ) const
{
        
    std::vector<const RevBayesCore::DagNode*> argNodes;
    for ( size_t i=0; i<this->args.size(); ++i)
    {
        argNodes.push_back( this->args[i].getVariable()->getRevObject().getDagNode() );
    }
    
    const RevBayesCore::TypedDagNode<typename memberObjectType::valueType>* o = theMemberObject->getDagNode();
    if ( o == NULL )
    {
        throw RbException("Could not cast the member object.");
    }
    
    RevBayesCore::MemberFunction<typename memberObjectType::valueType, typename retType::valueType> *func = new RevBayesCore::MemberFunction<typename memberObjectType::valueType, typename retType::valueType>(this->name, o, argNodes);
    
    return func;
}


/** Get argument rules */
template <typename memberObjectType, typename retType>
const RevLanguage::ArgumentRules& RevLanguage::MemberFunction<memberObjectType, retType>::getArgumentRules(void) const {
    
    return *argumentRules;
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



template <typename memberObjectType, typename retType>
void RevLanguage::MemberFunction<memberObjectType, retType>::setMemberObject( const RevPtr<Variable> &obj) {
    
    // we do not own the object itself because one object can have multiple member functions
    object = obj;
    theMemberObject = static_cast< const memberObjectType *>( &(obj->getRevObject()) );
}



template <typename memberObjectType, typename retType>
void RevLanguage::MemberFunction<memberObjectType, retType>::setMethodName(std::string const &name) {
    
    funcName = name;
}


#endif

