#include "ModelVector.h"
#include "Natural.h"
#include "RbUtil.h"
#include "RlBoolean.h"
#include "RlClade.h"
#include "RlTree.h"
#include "RlMemberFunction.h"
#include "RlString.h"
#include "RealPos.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
Tree::Tree(void) : ModelObject<RevBayesCore::Tree>()
{
    
    ArgumentRules* isInternalArgRules = new ArgumentRules();
    isInternalArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "isInternal", RlBoolean::getClassTypeSpec(), isInternalArgRules ) );
    
    ArgumentRules* nnodesArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "nnodes", Natural::getClassTypeSpec(), nnodesArgRules ) );
    
    ArgumentRules* ntipsArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "ntips", Natural::getClassTypeSpec(), ntipsArgRules ) );
    
    ArgumentRules* namesArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "names", ModelVector<RlString>::getClassTypeSpec(), namesArgRules ) );
    
    ArgumentRules* nodeNameArgRules = new ArgumentRules();
    nodeNameArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "nodeName", RlString::getClassTypeSpec(),  nodeNameArgRules ) );
    
    ArgumentRules* rescaleArgRules = new ArgumentRules();
    rescaleArgRules->push_back( new ArgumentRule( "factor", RealPos::getClassTypeSpec(), "The scaling factor.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "rescale", RlUtils::Void, rescaleArgRules ) );
    
    
    // member functions
    ArgumentRules* parentArgRules = new ArgumentRules();
    parentArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<Tree, Natural>( "parent", this, parentArgRules   ) );
    
    ArgumentRules* branchLengthArgRules = new ArgumentRules();
    branchLengthArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<Tree, RealPos>( "branchLength", this, branchLengthArgRules   ) );
    
    ArgumentRules* containedInCaldeArgRules = new ArgumentRules();
    containedInCaldeArgRules->push_back( new ArgumentRule( "node" , Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    containedInCaldeArgRules->push_back( new ArgumentRule( "clade", Clade::getClassTypeSpec()  , "The embracing clade.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<Tree, RlBoolean>( "isContainedInClade", this, containedInCaldeArgRules ) );
    
}

/** Construct from core pointer */
Tree::Tree(RevBayesCore::Tree *t) : ModelObject<RevBayesCore::Tree>( t )
{
    ArgumentRules* isInternalArgRules = new ArgumentRules();
    isInternalArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "isInternal", RlBoolean::getClassTypeSpec(), isInternalArgRules ) );
    
    ArgumentRules* nnodesArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "nnodes", Natural::getClassTypeSpec(), nnodesArgRules ) );
    
    ArgumentRules* ntipsArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "ntips", Natural::getClassTypeSpec(), ntipsArgRules ) );
    
    ArgumentRules* namesArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "names", ModelVector<RlString>::getClassTypeSpec(), namesArgRules ) );
    
    ArgumentRules* nodeNameArgRules = new ArgumentRules();
    nodeNameArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "nodeName", RlString::getClassTypeSpec(),  nodeNameArgRules ) );
    
    ArgumentRules* rescaleArgRules = new ArgumentRules();
    rescaleArgRules->push_back( new ArgumentRule( "factor", RealPos::getClassTypeSpec(), "The scaling factor.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "rescale", RlUtils::Void, rescaleArgRules ) );
    
    
    // member functions
    ArgumentRules* parentArgRules = new ArgumentRules();
    parentArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<Tree, Natural>( "parent", this, parentArgRules   ) );
    
    ArgumentRules* branchLengthArgRules = new ArgumentRules();
    branchLengthArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<Tree, RealPos>( "branchLength", this, branchLengthArgRules   ) );
    
    ArgumentRules* containedInCaldeArgRules = new ArgumentRules();
    containedInCaldeArgRules->push_back( new ArgumentRule( "node" , Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    containedInCaldeArgRules->push_back( new ArgumentRule( "clade", Clade::getClassTypeSpec()  , "The embracing clade.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<Tree, RlBoolean>( "isContainedInClade", this, containedInCaldeArgRules ) );

}

/** Construct from core reference */
Tree::Tree(const RevBayesCore::Tree &t) : ModelObject<RevBayesCore::Tree>( new RevBayesCore::Tree( t ) )
{
    ArgumentRules* isInternalArgRules = new ArgumentRules();
    isInternalArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "isInternal", RlBoolean::getClassTypeSpec(), isInternalArgRules ) );
    
    ArgumentRules* nnodesArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "nnodes", Natural::getClassTypeSpec(), nnodesArgRules ) );
    
    ArgumentRules* ntipsArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "ntips", Natural::getClassTypeSpec(), ntipsArgRules ) );
    
    ArgumentRules* namesArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "names", ModelVector<RlString>::getClassTypeSpec(), namesArgRules ) );
    
    ArgumentRules* nodeNameArgRules = new ArgumentRules();
    nodeNameArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "nodeName", RlString::getClassTypeSpec(),  nodeNameArgRules ) );
    
    ArgumentRules* rescaleArgRules = new ArgumentRules();
    rescaleArgRules->push_back( new ArgumentRule( "factor", RealPos::getClassTypeSpec(), "The scaling factor.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "rescale", RlUtils::Void, rescaleArgRules ) );
    
    
    // member functions
    ArgumentRules* parentArgRules = new ArgumentRules();
    parentArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<Tree, Natural>( "parent", this, parentArgRules   ) );
    
    ArgumentRules* branchLengthArgRules = new ArgumentRules();
    branchLengthArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<Tree, RealPos>( "branchLength", this, branchLengthArgRules   ) );
    
    ArgumentRules* containedInCaldeArgRules = new ArgumentRules();
    containedInCaldeArgRules->push_back( new ArgumentRule( "node" , Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    containedInCaldeArgRules->push_back( new ArgumentRule( "clade", Clade::getClassTypeSpec()  , "The embracing clade.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<Tree, RlBoolean>( "isContainedInClade", this, containedInCaldeArgRules ) );

}

/** Construct from core DAG node */
Tree::Tree(RevBayesCore::TypedDagNode<RevBayesCore::Tree> *n) : ModelObject<RevBayesCore::Tree>( n )
{
    
    ArgumentRules* isInternalArgRules = new ArgumentRules();
    isInternalArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "isInternal", RlBoolean::getClassTypeSpec(), isInternalArgRules ) );
    
    ArgumentRules* nnodesArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "nnodes", Natural::getClassTypeSpec(), nnodesArgRules ) );
    
    ArgumentRules* ntipsArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "ntips", Natural::getClassTypeSpec(), ntipsArgRules ) );
    
    ArgumentRules* namesArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "names", ModelVector<RlString>::getClassTypeSpec(), namesArgRules ) );
    
    ArgumentRules* nodeNameArgRules = new ArgumentRules();
    nodeNameArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "nodeName", RlString::getClassTypeSpec(),  nodeNameArgRules ) );
    
    ArgumentRules* rescaleArgRules = new ArgumentRules();
    rescaleArgRules->push_back( new ArgumentRule( "factor", RealPos::getClassTypeSpec(), "The scaling factor.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "rescale", RlUtils::Void, rescaleArgRules ) );
    
    
    // member functions
    ArgumentRules* parentArgRules = new ArgumentRules();
    parentArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<Tree, Natural>( "parent", this, parentArgRules   ) );
    
    ArgumentRules* branchLengthArgRules = new ArgumentRules();
    branchLengthArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<Tree, RealPos>( "branchLength", this, branchLengthArgRules   ) );
    
    ArgumentRules* containedInCaldeArgRules = new ArgumentRules();
    containedInCaldeArgRules->push_back( new ArgumentRule( "node" , Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    containedInCaldeArgRules->push_back( new ArgumentRule( "clade", Clade::getClassTypeSpec()  , "The embracing clade.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<Tree, RlBoolean>( "isContainedInClade", this, containedInCaldeArgRules ) );
    
}


/** Clone object */
Tree* Tree::clone(void) const
{
    
    return new Tree(*this);
}


/* Map calls to member methods */
RevLanguage::RevPtr<RevLanguage::RevVariable> Tree::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    if (name == "isInternal")
    {
        found = true;
        
        int index = static_cast<const Natural&>( args[0].getVariable()->getRevObject() ).getValue() - 1;
        
        bool tf = this->dagNode->getValue().getNode((size_t)index).isInternal();
        return new RevVariable( new RlBoolean( tf ) );
    }
    else if (name == "nnodes")
    {
        found = true;
        
        size_t n = this->dagNode->getValue().getNumberOfNodes();
        return new RevVariable( new Natural( n ) );
    }
    else if (name == "ntips")
    {
        found = true;
        
        size_t n = this->dagNode->getValue().getNumberOfTips();
        return new RevVariable( new Natural( n ) );
    }
    else if (name == "names")
    {
        found = true;
        
        const std::vector<std::string>& n = this->dagNode->getValue().getTipNames();
        return new RevVariable( new ModelVector<RlString>( n ) );
    }
    else if (name == "nodeName")
    {
        found = true;
        
        int index = static_cast<const Natural&>( args[0].getVariable()->getRevObject() ).getValue() - 1;
        const std::string& n = this->dagNode->getValue().getNode((size_t)index).getName();
        return new RevVariable( new RlString( n ) );
    }
    else if (name == "rescale")
    {
        found = true;
        
        double f = static_cast<const RealPos&>( args[0].getVariable()->getRevObject() ).getValue();
        RevBayesCore::Tree &tree = dagNode->getValue();
        RevBayesCore::TreeUtilities::rescaleTree(&tree, &tree.getRoot(), f);
        
        return NULL;
    }
    
    return ModelObject<RevBayesCore::Tree>::executeMethod( name, args, found );
}


/** Get Rev type of object */
const std::string& Tree::getClassType(void)
{
    
    static std::string revType = "Tree";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Tree::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/** Get type spec */
const TypeSpec& Tree::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


