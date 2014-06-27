#ifndef RlDiscreteCharacterData_H
#define RlDiscreteCharacterData_H

#include "DiscreteCharacterData.h"
#include "ModelObject.h"
#include "RlAbstractCharacterData.h"

#include <set>
#include <string>
#include <vector>


namespace RevLanguage {
    

    /**
     * The RevLanguage wrapper of the DiscreteCharacterData class.
     *
     * The RevLanguage wrapper of the discrete character data is our
     * data object that stores discrete character data such as character matrices (alignments).
     * Please read the DiscreteCharacterData.h for more info.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-02-16, version 1.0
     *
     */
    template <class rlCharType>
    class DiscreteCharacterData : public AbstractCharacterData {
    
    public:
        DiscreteCharacterData(void);                                                                                                            //!< Default constructor
        DiscreteCharacterData(RevBayesCore::DiscreteCharacterData<typename rlCharType::valueType> *v);                                          //!< Constructor with core object
        DiscreteCharacterData(RevBayesCore::TypedDagNode< RevBayesCore::AbstractCharacterData >*d);                                             //!< Constructor with DAG node
        
        typedef RevBayesCore::DiscreteCharacterData<typename rlCharType::valueType> valueType;
    
        // Basic utility functions
        DiscreteCharacterData*              clone(void) const;                                                                                  //!< Clone object
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
#include "RlString.h"
#include "RlTaxonData.h"
#include "Vector.h"


template <class rlCharType>
RevLanguage::DiscreteCharacterData<rlCharType>::DiscreteCharacterData(void) : AbstractCharacterData() {
    
}


template <class rlCharType>
RevLanguage::DiscreteCharacterData<rlCharType>::DiscreteCharacterData( RevBayesCore::DiscreteCharacterData<typename rlCharType::valueType> *v) : AbstractCharacterData( v ) {
    
}


template <class rlCharType>
RevLanguage::DiscreteCharacterData<rlCharType>::DiscreteCharacterData( RevBayesCore::TypedDagNode< RevBayesCore::AbstractCharacterData > *d) : AbstractCharacterData( d ) {

}



template <typename charType>
RevLanguage::DiscreteCharacterData<charType>* RevLanguage::DiscreteCharacterData<charType>::clone() const {
    
    return new DiscreteCharacterData<charType>( *this );
}


/* Map calls to member methods */
template <typename charType>
RevLanguage::RevObject* RevLanguage::DiscreteCharacterData<charType>::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "[]") 
    {
        // get the member with give index
        const Natural& index = static_cast<const Natural&>( args[0].getVariable()->getValue() );
            
        if (this->value->getValue().getNumberOfTaxa() < (size_t)(index.getValue()) ) 
        {
            throw RbException("Index out of bounds in []");
        }
            
        const RevBayesCore::DiscreteTaxonData<typename charType::valueType>& element = static_cast< RevBayesCore::DiscreteCharacterData<typename charType::valueType>& >( this->value->getValue() ).getTaxonData(size_t(index.getValue()) - 1);
    
        return new DiscreteTaxonData<charType>( new RevBayesCore::DiscreteTaxonData<typename charType::valueType>( element ) );
    }
    
    return AbstractCharacterData::executeMethod( name, args );
}


/* Get class name of object */
template <typename rlType>
const std::string& RevLanguage::DiscreteCharacterData<rlType>::getClassName(void) { 
    
    static std::string rbClassName = "DiscreteCharacterData";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
template <typename rlType>
const RevLanguage::TypeSpec& RevLanguage::DiscreteCharacterData<rlType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( AbstractCharacterData::getClassTypeSpec() ), new TypeSpec( rlType::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get the methods for this vector class */
/* Get method specifications */
template <typename rlType>
const RevLanguage::MethodTable& RevLanguage::DiscreteCharacterData<rlType>::getMethods(void) const {
    
    static MethodTable    myMethods                   = MethodTable();
    static bool           methodsSet                  = false;
    
    if ( methodsSet == false ) 
    {
                
        // add method for call "x[]" as a function
        ArgumentRules* squareBracketArgRules = new ArgumentRules();
        squareBracketArgRules->push_back( new ArgumentRule( "index" , true, Natural::getClassTypeSpec() ) );
        myMethods.addFunction("[]",  new MemberFunction( DiscreteTaxonData<rlType>::getClassTypeSpec(), squareBracketArgRules) );
        
                
        // necessary call for proper inheritance
        myMethods.setParentTable( &AbstractCharacterData::getMethods() );
        methodsSet = true;
    }
    
    
    return myMethods;
}



/** Get the type spec of this class. We return a member variable because instances might have different element types. */
template <typename rlType>
const RevLanguage::TypeSpec& RevLanguage::DiscreteCharacterData<rlType>::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}



#endif

