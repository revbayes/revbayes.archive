//#ifndef RlHomologousDiscreteCharacterData_H
//#define RlHomologousDiscreteCharacterData_H
//
//#include "HomologousDiscreteCharacterData.h"
//#include "ModelObject.h"
//#include "RlAbstractHomologousDiscreteCharacterData.h"
//
//#include <set>
//#include <string>
//#include <vector>
//
//
//namespace RevLanguage {
//    
//
//    /**
//     * The RevLanguage wrapper of the HomologousDiscreteCharacterData class.
//     *
//     * The RevLanguage wrapper of the discrete character data is our
//     * data object that stores discrete character data such as character matrices (alignments).
//     * Please read the HomologousDiscreteCharacterData.h for more info.
//     *
//     *
//     * @copyright Copyright 2009-
//     * @author The RevBayes Development Core Team (Sebastian Hoehna)
//     * @since 2014-02-16, version 1.0
//     *
//     */
//    template <class rlCharType>
//    class HomologousDiscreteCharacterData : public AbstractHomologousDiscreteCharacterData {
//    
//    public:
//        HomologousDiscreteCharacterData(void);                                                                                                            //!< Default constructor
//        HomologousDiscreteCharacterData(RevBayesCore::HomologousDiscreteCharacterData<typename rlCharType::valueType> *v);                                          //!< Constructor with core object
//        HomologousDiscreteCharacterData(RevBayesCore::TypedDagNode< RevBayesCore::AbstractHomologousDiscreteCharacterData >*d);                                     //!< Constructor with DAG node
//        
//        typedef RevBayesCore::HomologousDiscreteCharacterData<typename rlCharType::valueType> valueType;
//    
//        // Basic utility functions
//        HomologousDiscreteCharacterData*    clone(void) const;                                                                                  //!< Clone object
//        static const std::string&           getClassType(void);                                                                                 //!< Get class name
//        static const TypeSpec&              getClassTypeSpec(void);                                                                             //!< Get class type spec
//        const TypeSpec&                     getTypeSpec(void) const;                                                                            //!< Get language type of the object
//         
//        // Member method inits
//        virtual RevPtr<RevVariable>         executeMethod(const std::string& name, const std::vector<Argument>& args, bool &f);                 //!< Override to map member methods to internal functions
//        
//    };
//    
//}
//
//
//#include "ArgumentRule.h"
//#include "MemberProcedure.h"
//#include "Natural.h"
//#include "RlBoolean.h"
//#include "RlString.h"
//#include "RlDiscreteTaxonData.h"
//
//
//template <class rlType>
//RevLanguage::HomologousDiscreteCharacterData<rlType>::HomologousDiscreteCharacterData(void) : AbstractHomologousDiscreteCharacterData()
//{
//
//    // Add method for call "x[]" as a function
//    ArgumentRules* squareBracketArgRules = new ArgumentRules();
//    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
//    methods.addFunction("[]",  new MemberProcedure( DiscreteTaxonData<rlType>::getClassTypeSpec(), squareBracketArgRules) );
//
//}
//
//
//template <class rlType>
//RevLanguage::HomologousDiscreteCharacterData<rlType>::HomologousDiscreteCharacterData( RevBayesCore::HomologousDiscreteCharacterData<typename rlType::valueType> *v) :
//    AbstractHomologousDiscreteCharacterData( v )
//{
//
//    // Add method for call "x[]" as a function
//    ArgumentRules* squareBracketArgRules = new ArgumentRules();
//    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
//    methods.addFunction("[]",  new MemberProcedure( DiscreteTaxonData<rlType>::getClassTypeSpec(), squareBracketArgRules) );
//
//}
//
//
//template <class rlType>
//RevLanguage::HomologousDiscreteCharacterData<rlType>::HomologousDiscreteCharacterData( RevBayesCore::TypedDagNode< RevBayesCore::AbstractHomologousDiscreteCharacterData > *d) :
//    AbstractHomologousDiscreteCharacterData( d )
//{
//
//    // Add method for call "x[]" as a function
//    ArgumentRules* squareBracketArgRules = new ArgumentRules();
//    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
//    methods.addFunction("[]",  new MemberProcedure( DiscreteTaxonData<rlType>::getClassTypeSpec(), squareBracketArgRules) );
//
//}
//
//
//
//template <typename charType>
//RevLanguage::HomologousDiscreteCharacterData<charType>* RevLanguage::HomologousDiscreteCharacterData<charType>::clone() const {
//    
//    return new HomologousDiscreteCharacterData<charType>( *this );
//}
//
//
///* Map calls to member methods */
//template <typename rlType>
//RevLanguage::RevPtr<RevLanguage::RevVariable> RevLanguage::HomologousDiscreteCharacterData<rlType>::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
//{
//    
//    if (name == "[]") 
//    {
//        found = true;
//        
//        // get the member with give index
//        const Natural& index = static_cast<const Natural&>( args[0].getVariable()->getRevObject() );
//            
//        if (this->dagNode->getValue().getNumberOfTaxa() < (size_t)(index.getValue()) )
//        {
//            throw RbException("Index out of bounds in []");
//        }
//            
//        const RevBayesCore::DiscreteTaxonData<typename rlType::valueType>& element = static_cast< RevBayesCore::HomologousDiscreteCharacterData<typename rlType::valueType>& >( this->dagNode->getValue() ).getTaxonData(size_t(index.getValue()) - 1);
//    
//        return new RevVariable( new DiscreteTaxonData<rlType>( new RevBayesCore::DiscreteTaxonData<typename rlType::valueType>( element ) ) );
//    }
//    
//    return AbstractHomologousDiscreteCharacterData::executeMethod( name, args, found );
//}
//
//
///* Get class name of object */
//template <typename rlType>
//const std::string& RevLanguage::HomologousDiscreteCharacterData<rlType>::getClassType(void)
//{
//    
//    static std::string revClassType = "HomologousDiscreteCharacterData<" + rlType::getClassType() + ">";
//    
//	return revClassType; 
//}
//
///* Get class type spec describing type of object */
//template <typename rlType>
//const RevLanguage::TypeSpec& RevLanguage::HomologousDiscreteCharacterData<rlType>::getClassTypeSpec(void)
//{
//    
//    static TypeSpec revClassTypeSpec = TypeSpec( getClassType(), new TypeSpec( AbstractHomologousDiscreteCharacterData::getClassTypeSpec() ), new TypeSpec( rlType::getClassTypeSpec() ) );
//    
//	return revClassTypeSpec; 
//}
//
//
///** Get the type spec of this class. We return a member variable because instances might have different element types. */
//template <typename rlType>
//const RevLanguage::TypeSpec& RevLanguage::HomologousDiscreteCharacterData<rlType>::getTypeSpec(void) const {
//    
//    static TypeSpec typeSpec = getClassTypeSpec();
//    return typeSpec;
//}
//
//
//
//#endif
//
