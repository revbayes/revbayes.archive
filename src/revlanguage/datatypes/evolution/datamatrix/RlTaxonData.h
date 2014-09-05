#ifndef RlDiscreteTaxonData_H
#define RlDiscreteTaxonData_H

#include "DiscreteTaxonData.h"
#include "ModelObject.h"

#include <set>
#include <string>
#include <vector>


namespace RevLanguage {
    
    
    template <class rlCharType>
    class DiscreteTaxonData : public ModelObject<RevBayesCore::DiscreteTaxonData<typename rlCharType::valueType> > {
        
    public:
        DiscreteTaxonData(void);                                                                                        //!< Constructor requires character type
        DiscreteTaxonData(RevBayesCore::DiscreteTaxonData<typename rlCharType::valueType> *v);                          //!< Constructor requires character type
        
        typedef RevBayesCore::DiscreteTaxonData<typename rlCharType::valueType> valueType;
        
        // Basic utility functions
        DiscreteTaxonData*                  clone(void) const;                                                          //!< Clone object
        static const std::string&           getClassType(void);                                                         //!< Get Rev type
        static const TypeSpec&              getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                                    //!< Get language type of the object
        
        // Member method inits
        const MethodTable&                  getMethods(void) const;                                                     //!< Get member methods
        MethodTable                         makeMethods(void) const;                                                    //!< Make member methods
        RevPtr<Variable>                    executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions
                
    };
    
}


#include "ArgumentRule.h"
#include "MemberProcedure.h"
#include "Natural.h"
#include "RlBoolean.h"


template <class rlCharType>
RevLanguage::DiscreteTaxonData<rlCharType>::DiscreteTaxonData(void) : ModelObject<RevBayesCore::DiscreteTaxonData<typename rlCharType::valueType> >() {
    
}


template <class rlCharType>
RevLanguage::DiscreteTaxonData<rlCharType>::DiscreteTaxonData( RevBayesCore::DiscreteTaxonData<typename rlCharType::valueType> *v) : ModelObject<RevBayesCore::DiscreteTaxonData<typename rlCharType::valueType> >( v ) {
    
}



template <typename charType>
RevLanguage::DiscreteTaxonData<charType>* RevLanguage::DiscreteTaxonData<charType>::clone() const {
    return new DiscreteTaxonData<charType>( *this );
}


/* Map calls to member methods */
template <typename charType>
RevLanguage::RevPtr<RevLanguage::Variable> RevLanguage::DiscreteTaxonData<charType>::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if ( name == "[]") 
    {
        // get the member with give index
        const Natural &index = static_cast<const Natural &>( args[0].getVariable()->getRevObject() );
            
        if (this->dagNode->getValue().size() < (size_t)(index.getValue()) )
        {
            throw RbException("Index out of bounds in []");
        }
            
        RevObject* element = new charType( this->dagNode->getValue().getElement( size_t(index.getValue()) - 1) );
        return new Variable( element );
    } 
    
    return ModelObject<RevBayesCore::DiscreteTaxonData<typename charType::valueType> >::executeMethod( name, args );
}


/* Get Rev type of object */
template <typename rlType>
const std::string& RevLanguage::DiscreteTaxonData<rlType>::getClassType(void) { 
    
    static std::string revType = "DiscreteTaxonData";
    
	return revType; 
}


/* Get class type spec describing type of object */
template <typename rlType>
const RevLanguage::TypeSpec& RevLanguage::DiscreteTaxonData<rlType>::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ), new TypeSpec( rlType::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/**
 * Get member methods. We construct the appropriate static member
 * function table here.
 */
template <typename rlType>
const RevLanguage::MethodTable& RevLanguage::DiscreteTaxonData<rlType>::getMethods( void ) const
{
    static MethodTable  myMethods   = MethodTable();
    static bool         methodsSet  = false;
    
    if ( !methodsSet )
    {
        myMethods = makeMethods();
        methodsSet = true;
    }
    
    return myMethods;
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
template <typename rlType>
const RevLanguage::TypeSpec& RevLanguage::DiscreteTaxonData<rlType>::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}


/** Make member methods for this class */
template <typename rlType>
RevLanguage::MethodTable RevLanguage::DiscreteTaxonData<rlType>::makeMethods( void ) const
{
    MethodTable methods = MethodTable();
    
    // Add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("[]",  new MemberProcedure( rlType::getClassTypeSpec(), squareBracketArgRules) );
    
    // Insert inherited methods
    methods.insertInheritedMethods( ModelObject<RevBayesCore::DiscreteTaxonData<typename rlType::valueType> >::makeMethods() );
    
    return methods;
}


#endif

