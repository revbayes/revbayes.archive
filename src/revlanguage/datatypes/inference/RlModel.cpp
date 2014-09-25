
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "Ellipsis.h"
#include "Model.h"
#include "ModelVector.h"
#include "RevObject.h"
#include "RbException.h"
#include "RlModel.h"
#include "RlString.h"
#include "TypeSpec.h"

#include <vector>
#include <fstream>
#include <algorithm>
#include <string>

using namespace RevLanguage;

Model::Model() : WorkspaceObject<RevBayesCore::Model>() {
    
}


/** Clone object */
Model* Model::clone(void) const {
    
	return new Model(*this);
}


void Model::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a model
    std::set<const RevBayesCore::DagNode*> s;
    for (std::set<RevPtr<const Variable> >::iterator it = sources.begin(); it != sources.end(); ++it) {
        RevBayesCore::DagNode* n = (*it)->getRevObject().getDagNode();
        s.insert( n );
    }
    value = new RevBayesCore::Model( s );
//    printModelDotGraph();
}

/* Map calls to member methods */
RevPtr<Variable> Model::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "writeModelGVFile") {
        const std::string&   fn      = static_cast<const RlString &>( args[0].getVariable()->getRevObject() ).getValue();
        printModelDotGraph(fn);
        
        return NULL;
    }
    
    return RevObject::executeMethod( name, args );
}

/** Get Rev type of object */
const std::string& Model::getClassType(void) { 
    
    static std::string revType = "Model";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Model::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( WorkspaceObject<RevBayesCore::Model>::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** Return member rules (no members) */
const MemberRules& Model::getMemberRules(void) const {
    
    static MemberRules modelMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        modelMemberRules.push_back( new ArgumentRule("x", RevObject::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        modelMemberRules.push_back( new Ellipsis( RevObject::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return modelMemberRules;
}

/* Get method specifications */
const MethodTable& Model::getMethods(void) const {
    
    static MethodTable   methods    = MethodTable();
    static bool          methodsSet = false;
    
    if ( methodsSet == false )
    {
        
        ArgumentRules* dotArgRules = new ArgumentRules();
        dotArgRules->push_back( new ArgumentRule("file", RlString::getClassTypeSpec()  , ArgumentRule::BY_VALUE ) );
        methods.addFunction("writeModelGVFile", new MemberProcedure( RlUtils::Void, dotArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &RevObject::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}

/** Get type spec */
const TypeSpec& Model::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Print a simplified representation of the model for the user. */
void Model::printValue(std::ostream &o) const {
    
    const std::vector<RevBayesCore::DagNode*>& theNodes = value->getDagNodes();
    std::vector<RevBayesCore::DagNode*>::const_iterator it;

    o << std::endl;
    std::stringstream s;
    s << "Model with " << theNodes.size() << " nodes";
    o << s.str() << std::endl;
    for ( size_t i = 0; i < s.str().size(); ++i )
        o << "=";
    o << std::endl << std::endl;
    
    for ( it=theNodes.begin(); it!=theNodes.end(); ++it )
    {
        if ( (*it)->getName() != "" )
            o << (*it)->getName() <<  " <" << (*it) << "> :" << std::endl;
        else
            o << "<" << (*it) << "> :" << std::endl;
        
        o << "_value        = ";
        std::ostringstream o1;
        (*it)->printValue( o1, ", " );
        o << StringUtilities::oneLiner( o1.str(), 54 ) << std::endl;

#if defined (DEBUG_STRUCTURE)
        (*it)->printStructureInfo( o, true );
#else
        (*it)->printStructureInfo( o, false );
#endif

        o << std::endl;
    }
}


/** Set a member variable */
void Model::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {

    if ( name == "" || name == "x") {
        sources.insert( var );
    }
    else {
        RevObject::setConstMemberVariable(name, var);
    }
}

/* Write a file in DOT format for viewing the model DAG in graphviz */
//   This requires the user to have graphviz installed, or they can paste the file contents
//   into http://graphviz-dev.appspot.com/
void Model::printModelDotGraph(const std::string &fn){
    
    const std::vector<RevBayesCore::DagNode*>& theNodes = value->getDagNodes();
    std::vector<RevBayesCore::DagNode*>::const_iterator it;
    
    std::ofstream o;
    o.open(fn.c_str());
	o << "digraph REVDAG {\n";
    for ( it=theNodes.begin(); it!=theNodes.end(); ++it ){
        std::stringstream nname;
        if ( (*it)->getName() != "" )
            nname << (*it)->getName();
        else
            nname << (*it);
        std::string stname = nname.str();
        std::replace( stname.begin(), stname.end(), '[', '_');
        stname.erase(std::remove(stname.begin(), stname.end(), ']'), stname.end());  
              
        std::stringstream rl;
        rl << nname.str() ;        
       
        o << "   n_" << stname;
        o << " [shape=";
        // only print values of constant nodes (only simple numeric values)
        if((*it)->getDagNodeType() == "constant"){
            o << "record, style=filled, fillcolor=white, ";
            rl << "|";
            if((*it)->isSimpleNumeric())  
                (*it)->printValue(rl," ");
            else 
                rl << " ... ";
            o << "label=\"{" << rl.str() << "}\"]\n";
        }
        else if((*it)->getDagNodeType() == "deterministic"){
            std::stringstream strss;
            (*it)->printStructureInfo(strss);
            std::string w;
            while(strss >> w){
                if(w == "_function"){
                    strss >> w;
                    strss >> w;
                    strss >> w;
                    rl << "\\n[ " << w << "() ]";
                }
            }
            //std::cout << strss.str() << std::endl;
            o << "oval, style=\"dashed,filled\", fillcolor=white, label=\"" << rl.str() << "\"]\n";
        }
        else if((*it)->getDagNodeType() == "stochastic"){
            o << "oval, ";
            if((*it)->isClamped()){
                o << "style=filled, fillcolor=gray, ";
            }
            else 
                o << "style=filled, fillcolor=white, ";
            o << "label=\"" << rl.str() << "\"]\n";
        }
    }
    for ( it=theNodes.begin(); it!=theNodes.end(); ++it ){
        std::stringstream nname;
        if ( (*it)->getName() != "" )
            nname << (*it)->getName() ;
        else
            nname << (*it);
        std::string stname = nname.str();
        std::replace( stname.begin(), stname.end(), '[', '_');
        stname.erase(std::remove(stname.begin(), stname.end(), ']'), stname.end());  

        if((*it)->getNumberOfChildren() > 0){
            const std::set<RevBayesCore::DagNode*>& childeren = (*it)->getChildren();
            std::set<RevBayesCore::DagNode*>::const_iterator ci;
            for ( ci=childeren.begin(); ci!=childeren.end(); ++ci ){
                
                std::stringstream cn;
                if ( (*ci)->getName() != "" )
                    cn << (*ci)->getName();
                else
                    cn << (*ci);
                std::string stcn = cn.str();
                std::replace( stcn.begin(), stcn.end(), '[', '_');
                stcn.erase(std::remove(stcn.begin(), stcn.end(), ']'), stcn.end());  

                o << "   n_" << stname << " -> n_";
                o << stcn;
                if((*ci)->getDagNodeType() == "deterministic")
                    o << "[style=dashed]";
                o << "\n";
            }
        }
        
    }
    o << "}";
    o.close();
    
    
}
