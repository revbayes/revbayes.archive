#include "ModelVector.h"
#include "Natural.h"
#include "RlTopology.h"
#include "RbUtil.h"
#include "RlString.h"
#include "RealPos.h"
#include "TopologyNode.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
Topology::Topology(void) : ModelObject<RevBayesCore::Topology>()
{

    ArgumentRules* nnodesArgRules = new ArgumentRules();
    this->methods.addFunction("nnodes", new MemberProcedure(Natural::getClassTypeSpec(),       nnodesArgRules              ) );
    
    ArgumentRules* namesArgRules = new ArgumentRules();
    this->methods.addFunction("names", new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(),  namesArgRules              ) );

}

/** Construct from core topology */
Topology::Topology(RevBayesCore::Topology *t) : ModelObject<RevBayesCore::Topology>( t )
{

    ArgumentRules* nnodesArgRules = new ArgumentRules();
    this->methods.addFunction("nnodes", new MemberProcedure(Natural::getClassTypeSpec(),       nnodesArgRules              ) );
    
    ArgumentRules* namesArgRules = new ArgumentRules();
    this->methods.addFunction("names", new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(),  namesArgRules              ) );

}

/** Construct from core topology */
Topology::Topology(const RevBayesCore::Topology &t) : ModelObject<RevBayesCore::Topology>( new RevBayesCore::Topology( t ) )
{

    ArgumentRules* nnodesArgRules = new ArgumentRules();
    this->methods.addFunction("nnodes", new MemberProcedure(Natural::getClassTypeSpec(),       nnodesArgRules              ) );
    
    ArgumentRules* namesArgRules = new ArgumentRules();
    this->methods.addFunction("names", new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(),  namesArgRules              ) );

}

/** Construct from DAG node */
Topology::Topology(RevBayesCore::TypedDagNode<RevBayesCore::Topology> *n) : ModelObject<RevBayesCore::Topology>( n )
{

    ArgumentRules* nnodesArgRules = new ArgumentRules();
    this->methods.addFunction("nnodes", new MemberProcedure(Natural::getClassTypeSpec(),       nnodesArgRules              ) );
    
    ArgumentRules* namesArgRules = new ArgumentRules();
    this->methods.addFunction("names", new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(),  namesArgRules              ) );

}


/** Clone object */
Topology* Topology::clone(void) const {
    
	return new Topology(*this);
}


/* Map calls to member methods */
RevLanguage::RevPtr<Variable> Topology::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "nnodes") {
        size_t n = this->dagNode->getValue().getNumberOfNodes();
        return new Variable( new Natural( n ) );
    }
    else if (name == "names") {
        const std::vector<std::string>& n = this->dagNode->getValue().getTipNames();
        RevBayesCore::RbVector<std::string> tmp = RevBayesCore::RbVector<std::string>(n);
        return new Variable( new ModelVector<RlString>( tmp ) );
    } 
    
    return ModelObject<RevBayesCore::Topology>::executeMethod( name, args );
}


/** Get Rev type of object */
const std::string& Topology::getClassType(void) { 
    
    static std::string revType = "Topology";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Topology::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** Get type spec */
const TypeSpec& Topology::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

