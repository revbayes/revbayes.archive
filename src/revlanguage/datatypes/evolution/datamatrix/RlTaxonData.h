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
        DiscreteTaxonData(void);                                                                                                                    //!< Constructor requires character type
        DiscreteTaxonData(RevBayesCore::DiscreteTaxonData<typename rlCharType::valueType> *v);                                                          //!< Constructor requires character type
        
        typedef RevBayesCore::DiscreteTaxonData<typename rlCharType::valueType> valueType;
        
        // Basic utility functions
        DiscreteTaxonData*                  clone(void) const;                                                                                  //!< Clone object
        static const std::string&           getClassName(void);                                                                                 //!< Get class name
        static const TypeSpec&              getClassTypeSpec(void);                                                                             //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                                                            //!< Get language type of the object
        
        // Member method inits
        const MethodTable&                  getMethods(void) const;                                             //!< Get methods
        RevObject*                   executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions
                
    };
    
}


#include "ArgumentRule.h"
#include "MemberFunction.h"
#include "Natural.h"
#include "RlBoolean.h"
#include "Vector.h"


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
RevLanguage::RevObject* RevLanguage::DiscreteTaxonData<charType>::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if ( name == "[]") 
    {
        // get the member with give index
        const Natural &index = static_cast<const Natural &>( args[0].getVariable()->getRevObject() );
            
        if (this->dagNode->getValue().size() < (size_t)(index.getValue()) )
        {
            throw RbException("Index out of bounds in []");
        }
            
        RevObject* element = new charType( this->dagNode->getValue().getElement( size_t(index.getValue()) - 1) );
        return element;
    } 
    
    return ModelObject<RevBayesCore::DiscreteTaxonData<typename charType::valueType> >::executeMethod( name, args );
}


/* Get class name of object */
template <typename rlType>
const std::string& RevLanguage::DiscreteTaxonData<rlType>::getClassName(void) { 
    
    static std::string rbClassName = "DiscreteTaxonData";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
template <typename rlType>
const RevLanguage::TypeSpec& RevLanguage::DiscreteTaxonData<rlType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RevObject::getClassTypeSpec() ), new TypeSpec( rlType::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get the methods for this vector class */
/* Get method specifications */
template <typename rlType>
const RevLanguage::MethodTable& RevLanguage::DiscreteTaxonData<rlType>::getMethods(void) const {
    
    static MethodTable    methods                     = MethodTable();
    static bool           methodsSet                  = false;
    
    if ( methodsSet == false ) {
        
        // add method for call "x[]" as a function
        ArgumentRules* squareBracketArgRules = new ArgumentRules();
        squareBracketArgRules->push_back( new ArgumentRule( "index" , true, Natural::getClassTypeSpec() ) );
        methods.addFunction("[]",  new MemberFunction( rlType::getClassTypeSpec(), squareBracketArgRules) );
        
        
        // necessary call for proper inheritance
        methods.setParentTable( &RevObject::getMethods() );
        methodsSet = true;
    }
    
    
    return methods;
}



/** Get the type spec of this class. We return a member variable because instances might have different element types. */
template <typename rlType>
const RevLanguage::TypeSpec& RevLanguage::DiscreteTaxonData<rlType>::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}



#endif

