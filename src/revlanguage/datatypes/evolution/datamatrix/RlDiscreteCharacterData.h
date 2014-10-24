#ifndef RlDiscreteCharacterData_H
#define RlDiscreteCharacterData_H

#include "DiscreteCharacterData.h"
#include "ModelObject.h"
#include "RlAbstractDiscreteCharacterData.h"

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
    class DiscreteCharacterData : public AbstractDiscreteCharacterData {
    
    public:
        DiscreteCharacterData(void);                                                                                                            //!< Default constructor
        DiscreteCharacterData(RevBayesCore::DiscreteCharacterData<typename rlCharType::valueType> *v);                                          //!< Constructor with core object
        DiscreteCharacterData(RevBayesCore::TypedDagNode< RevBayesCore::AbstractDiscreteCharacterData >*d);                                     //!< Constructor with DAG node
        
        typedef RevBayesCore::DiscreteCharacterData<typename rlCharType::valueType> valueType;
    
        // Basic utility functions
        DiscreteCharacterData*              clone(void) const;                                                                                  //!< Clone object
        static const std::string&           getClassType(void);                                                                                 //!< Get class name
        static const TypeSpec&              getClassTypeSpec(void);                                                                             //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                                                            //!< Get language type of the object
         
        // Member method inits
        virtual RevPtr<Variable>            executeMethod(const std::string& name, const std::vector<Argument>& args);                          //!< Override to map member methods to internal functions
        
    };
    
}


#include "ArgumentRule.h"
#include "MemberProcedure.h"
#include "Natural.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "RlTaxonData.h"


template <class rlType>
RevLanguage::DiscreteCharacterData<rlType>::DiscreteCharacterData(void) : AbstractDiscreteCharacterData()
{

    // Add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("[]",  new MemberProcedure( DiscreteTaxonData<rlType>::getClassTypeSpec(), squareBracketArgRules) );

}


template <class rlType>
RevLanguage::DiscreteCharacterData<rlType>::DiscreteCharacterData( RevBayesCore::DiscreteCharacterData<typename rlType::valueType> *v) :
    AbstractDiscreteCharacterData( v )
{

    // Add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("[]",  new MemberProcedure( DiscreteTaxonData<rlType>::getClassTypeSpec(), squareBracketArgRules) );

}


template <class rlType>
RevLanguage::DiscreteCharacterData<rlType>::DiscreteCharacterData( RevBayesCore::TypedDagNode< RevBayesCore::AbstractDiscreteCharacterData > *d) :
    AbstractDiscreteCharacterData( d )
{

    // Add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("[]",  new MemberProcedure( DiscreteTaxonData<rlType>::getClassTypeSpec(), squareBracketArgRules) );

}



template <typename charType>
RevLanguage::DiscreteCharacterData<charType>* RevLanguage::DiscreteCharacterData<charType>::clone() const {
    
    return new DiscreteCharacterData<charType>( *this );
}


/* Map calls to member methods */
template <typename rlType>
RevLanguage::RevPtr<RevLanguage::Variable> RevLanguage::DiscreteCharacterData<rlType>::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "[]") 
    {
        // get the member with give index
        const Natural& index = static_cast<const Natural&>( args[0].getVariable()->getRevObject() );
            
        if (this->dagNode->getValue().getNumberOfTaxa() < (size_t)(index.getValue()) )
        {
            throw RbException("Index out of bounds in []");
        }
            
        const RevBayesCore::DiscreteTaxonData<typename rlType::valueType>& element = static_cast< RevBayesCore::DiscreteCharacterData<typename rlType::valueType>& >( this->dagNode->getValue() ).getTaxonData(size_t(index.getValue()) - 1);
    
        return new Variable( new DiscreteTaxonData<rlType>( new RevBayesCore::DiscreteTaxonData<typename rlType::valueType>( element ) ) );
    }
    
    return AbstractDiscreteCharacterData::executeMethod( name, args );
}


/* Get class name of object */
template <typename rlType>
const std::string& RevLanguage::DiscreteCharacterData<rlType>::getClassType(void) { 
    
    static std::string revClassType = "DiscreteCharacterData<" + rlType::getClassType() + ">";
    
	return revClassType; 
}

/* Get class type spec describing type of object */
template <typename rlType>
const RevLanguage::TypeSpec& RevLanguage::DiscreteCharacterData<rlType>::getClassTypeSpec(void) { 
    
    static TypeSpec revClassTypeSpec = TypeSpec( getClassType(), new TypeSpec( AbstractDiscreteCharacterData::getClassTypeSpec() ), new TypeSpec( rlType::getClassTypeSpec() ) );
    
	return revClassTypeSpec; 
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
template <typename rlType>
const RevLanguage::TypeSpec& RevLanguage::DiscreteCharacterData<rlType>::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}



#endif

