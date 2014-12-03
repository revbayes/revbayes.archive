//#include "ArgumentRule.h"
//#include "ArgumentRules.h"
//#include "OptionRule.h"
//#include "Mntr_AncestralState.h"
//#include "Model.h"
//#include "AncestralStateMonitor.h"
//#include "RlMonitor.h"
//#include "ModelVector.h"
//#include "Ellipsis.h"
//#include "Natural.h"
//#include "RbException.h"
//#include "RevObject.h"
//#include "RlModel.h"
//#include "RlTree.h"
//#include "RlTimeTree.h"
//#include "RlBranchLengthTree.h"
//#include "Tree.h"
//#include "RlString.h"
//#include "TypeSpec.h"
//#include "ChromosomesState.h"
//#include "NaturalNumbersState.h"
//#include "DnaState.h"
//
//using namespace RevLanguage;
//
//template <class treeType>
//Mntr_AncestralState<treeType>::Mntr_AncestralState(void) : Monitor() 
//{
//    
//}
//
//
///** Clone object */
//template <class treeType>
//Mntr_AncestralState<treeType>* Mntr_AncestralState<treeType>::clone(void) const 
//{
//    
//	return new Mntr_AncestralState<treeType>(*this);
//}
//
//
//template <class treeType>
//void Mntr_AncestralState<treeType>::constructInternalObject( void ) 
//{
//    const std::string&                  fn      = static_cast<const RlString &>( filename->getRevObject() ).getValue();
//    const std::string&                  sep     = static_cast<const RlString &>( separator->getRevObject() ).getValue();
//    int                                 g       = static_cast<const Natural  &>( printgen->getRevObject() ).getValue();
//	std::cout << "OK\n";
//	RevBayesCore::TypedDagNode<RevBayesCore::Tree> *t = static_cast<const Tree &>( tree->getRevObject() ).getDagNode();
//	std::cout << "OK2\n";
//    RevBayesCore::DagNode*				ch		= character->getRevObject().getDagNode();
//	bool                                ap      = static_cast<const RlBoolean &>( append->getRevObject() ).getValue();
//	std::string							character = static_cast<const RlString &>( monitorType->getRevObject() ).getValue();
//    
//	delete value;
//	if (character == "Chromosomes") {
//		
//		if (treeType == "non-clock") {
//			RevBayesCore::AncestralStateMonitor<RevBayesCore::ChromosomesState, RevBayesCore::BranchLengthTree> *m = new RevBayesCore::AncestralStateMonitor<RevBayesCore::ChromosomesState, RevBayesCore::BranchLengthTree>(t, ch, (unsigned long)g, fn, sep);
//			m->setAppend( ap );
//			value = m;		
//		} else {
//			RevBayesCore::AncestralStateMonitor<RevBayesCore::ChromosomesState, RevBayesCore::TimeTree> *m = new RevBayesCore::AncestralStateMonitor<RevBayesCore::ChromosomesState, RevBayesCore::TimeTree>(t, ch, (unsigned long)g, fn, sep);
//			m->setAppend( ap );
//			value = m;	
//		}
//
//	} else if (character == "NaturalNumbers") {
//			
//		if (treeType == "non-clock") {
//			RevBayesCore::AncestralStateMonitor<RevBayesCore::NaturalNumbersState, RevBayesCore::BranchLengthTree> *m = new RevBayesCore::AncestralStateMonitor<RevBayesCore::NaturalNumbersState, RevBayesCore::BranchLengthTree>(t, ch, (unsigned long)g, fn, sep);
//			m->setAppend( ap );
//			value = m;		
//		} else { 
//			RevBayesCore::AncestralStateMonitor<RevBayesCore::NaturalNumbersState, RevBayesCore::TimeTree> *m = new RevBayesCore::AncestralStateMonitor<RevBayesCore::NaturalNumbersState, RevBayesCore::TimeTree>(t, ch, (unsigned long)g, fn, sep);		
//			m->setAppend( ap );
//			value = m;		
//		}
//			
//	} else if (character == "DNA") {
//		
//		if (treeType == "non-clock") {
//			RevBayesCore::AncestralStateMonitor<RevBayesCore::DnaState, RevBayesCore::BranchLengthTree> *m = new RevBayesCore::AncestralStateMonitor<RevBayesCore::DnaState, RevBayesCore::BranchLengthTree>(t, ch, (unsigned long)g, fn, sep);
//			m->setAppend( ap );
//			value = m;		
//		} else { 
//			RevBayesCore::AncestralStateMonitor<RevBayesCore::DnaState, RevBayesCore::TimeTree> *m = new RevBayesCore::AncestralStateMonitor<RevBayesCore::DnaState, RevBayesCore::TimeTree>(t, ch, (unsigned long)g, fn, sep);
//			m->setAppend( ap );
//			value = m;		
//		}
//		
//	} else {
//		throw RbException( "Incorrect character type specified. Valid options are: NaturalNumbers, DNA, Chromosomes" );
//	}
//
//    
//	
//
//}
//
//
///** Get Rev type of object */
//template <class treeType>
//const std::string& Mntr_AncestralState<treeType>::getClassType(void) 
//{ 
//    
//    static std::string revType = "Mntr_AncestralState";
//    
//	return revType; 
//}
//
///** Get class type spec describing type of object */
//template <class treeType>
//const TypeSpec& Mntr_AncestralState<treeType>::getClassTypeSpec(void) 
//{ 
//    
//    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Monitor::getClassTypeSpec() ) );
//    
//	return revTypeSpec; 
//}
//
//
//
///** Return member rules (no members) */
//template <class treeType>
//const MemberRules& Mntr_AncestralState<treeType>::getMemberRules(void) const {
//    
//    static MemberRules modelMonitorMemberRules;
//    static bool rulesSet = false;
//    
//    if ( !rulesSet ) 
//    {
//		modelMonitorMemberRules.push_back( new ArgumentRule("tree"           , treeType::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
//		modelMonitorMemberRules.push_back( new ArgumentRule("character"    , RevObject::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
//        modelMonitorMemberRules.push_back( new ArgumentRule("filename"      , RlString::getClassTypeSpec() , ArgumentRule::BY_VALUE ) );
//		modelMonitorMemberRules.push_back( new ArgumentRule("type"      , RlString::getClassTypeSpec() , ArgumentRule::BY_VALUE ) );
//        modelMonitorMemberRules.push_back( new ArgumentRule("printgen"      , Natural::getClassTypeSpec()  , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(1) ) );
//        modelMonitorMemberRules.push_back( new ArgumentRule("separator"     , RlString::getClassTypeSpec() , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString("\t") ) );
//        modelMonitorMemberRules.push_back( new ArgumentRule("append"        , RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
//        
//        rulesSet = true;
//    }
//    
//    return modelMonitorMemberRules;
//}
//
///** Get type spec */
//template <class treeType>
//const TypeSpec& Mntr_AncestralState<treeType>::getTypeSpec( void ) const 
//{
//    
//    static TypeSpec typeSpec = getClassTypeSpec();
//    
//    return typeSpec;
//}
//
//
///** Get type spec */
//template <class treeType>
//void Mntr_AncestralState<treeType>::printValue(std::ostream &o) const 
//{
//    
//    o << "Mntr_AncestralState";
//}
//
//
///** Set a member variable */
//template <class treeType>
//void Mntr_AncestralStat<treeType>e::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) 
//{
//    
//    if ( name == "" ) {
//        vars.push_back( var );
//    }
//	else if ( name == "filename" ) 
//    {
//        filename = var;
//    }
//    else if ( name == "separator" ) 
//    {
//        separator = var;
//    }
//	else if ( name == "tree" ) {
//			tree = var;
//    }
//	else if ( name == "type" ) 
//    {
//        monitorType = var;
//    }
//	else if ( name == "character" ) 
//    {
//        character = var;
//    }
//    else if ( name == "printgen" ) 
//    {
//        printgen = var;
//    }
//    else if ( name == "append" ) 
//    {
//        append = var;
//    }
//    else 
//    {
//        Monitor::setConstMemberVariable(name, var);
//    }
//    
//}
