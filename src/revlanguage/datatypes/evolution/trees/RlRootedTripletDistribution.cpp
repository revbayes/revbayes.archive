#include "ModelVector.h"
#include "Natural.h"
#include "RbUtil.h"
#include "RlRootedTripletDistribution.h"
#include "RlMemberFunction.h"
#include "RlString.h"
#include "RlTaxon.h"
#include "RlTimeTree.h"
#include "RealPos.h"
#include "RootedTripletDistribution.h"
#include "Taxon.h"
#include "TreeUtilities.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
RootedTripletDistribution::RootedTripletDistribution(void) : ModelObject<RevBayesCore::RootedTripletDistribution>() {
    
}

/** Construct from bool */
RootedTripletDistribution::RootedTripletDistribution(RevBayesCore::RootedTripletDistribution *t) : ModelObject<RevBayesCore::RootedTripletDistribution>( t ) {
    
}

/** Construct from bool */
RootedTripletDistribution::RootedTripletDistribution(const RevBayesCore::RootedTripletDistribution &t) : ModelObject<RevBayesCore::RootedTripletDistribution>( new RevBayesCore::RootedTripletDistribution( t ) ) {
    
}

/** Construct from bool */
RootedTripletDistribution::RootedTripletDistribution(RevBayesCore::TypedDagNode<RevBayesCore::RootedTripletDistribution> *n) : ModelObject<RevBayesCore::RootedTripletDistribution>( n ) {
    
}



/** Construct from bool */
RootedTripletDistribution::RootedTripletDistribution(const RootedTripletDistribution &t) : ModelObject<RevBayesCore::RootedTripletDistribution>( t ) {
    
}


/** Clone object */
RootedTripletDistribution* RootedTripletDistribution::clone(void) const {
    
	return new RootedTripletDistribution(*this);
}


/* Map calls to member methods */
RevLanguage::RevPtr<RevLanguage::Variable> RootedTripletDistribution::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "nTriplets")
    {
        size_t n = this->dagNode->getValue().getNumberOfTriplets();
        return new Variable( new Natural( n ) );
    }
    else if (name == "nTrees")
    {
        size_t n = this->dagNode->getValue().getNumberOfTrees();
        return new Variable( new Natural( n ) );
    }
    else if (name == "species")
    {
        const std::vector<std::string>& n = this->dagNode->getValue().getSpecies();
        return new Variable( new ModelVector<RlString>( n ) );
    }
    else if (name == "taxa")
    {
        const std::vector<RevBayesCore::Taxon>& n = this->dagNode->getValue().getTaxa();
        return new Variable( new ModelVector<Taxon>( n ) );
    }
    else if (name == "setTaxa")
    {
        std::vector<RevBayesCore::Taxon> t = static_cast<const ModelVector<Taxon>  &>( args[0].getVariable()->getRevObject() ).getValue();
        RevBayesCore::RootedTripletDistribution &dist = dagNode->getValue();
        dist.setTaxa(t);
        
        return NULL;
    }
    else if (name == "setSpecies")
    {
        std::vector<std::string> t = static_cast<const ModelVector<RlString>  &>( args[0].getVariable()->getRevObject() ).getValue();
        RevBayesCore::RootedTripletDistribution &dist = dagNode->getValue();
        dist.setSpecies(t);
        
        return NULL;
    }
/*    else if (name == "extractTriplets")
    {
        std::vector<RevBayesCore::TimeTree> t = static_cast<const ModelVector<TimeTree>  &>( args[0].getVariable()->getRevObject() ).getValue();
        RevBayesCore::RootedTripletDistribution &dist = dagNode->getValue();
        dist.extractTriplets(t);
        
        return NULL;
    }*/

    return ModelObject<RevBayesCore::RootedTripletDistribution>::executeMethod( name, args );
}


/** Get Rev type of object */
const std::string& RootedTripletDistribution::getClassType(void) {
    
    static std::string revType = "RootedTripletDistribution";
    
	return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& RootedTripletDistribution::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/** Get type spec */
const TypeSpec& RootedTripletDistribution::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Make member methods for this class */
void RootedTripletDistribution::initializeMethods( void ) const
{
    
    // Insert inherited methods
    ModelObject<RevBayesCore::RootedTripletDistribution>::initializeMethods();

    ArgumentRules* nTripletsArgRules = new ArgumentRules();
    methods.addFunction("nTriplets", new MemberProcedure(Natural::getClassTypeSpec(),         nTripletsArgRules   ) );
    
    ArgumentRules* nTreesArgRules = new ArgumentRules();
    methods.addFunction("nTrees", new MemberProcedure(Natural::getClassTypeSpec(),          nTreesArgRules   ) );
    
    ArgumentRules* SpeciesArgRules = new ArgumentRules();
    methods.addFunction("species", new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(),          SpeciesArgRules   ) );
    
    ArgumentRules* TaxaArgRules = new ArgumentRules();
    methods.addFunction("taxa", new MemberProcedure(ModelVector<Taxon>::getClassTypeSpec(),  TaxaArgRules    ) );
    
    ArgumentRules* setTaxaArgRules = new ArgumentRules();
    setTaxaArgRules->push_back( new ArgumentRule( "taxa", ModelVector<Taxon>::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("setTaxa", new MemberProcedure(RlUtils::Void,                       setTaxaArgRules  ) );

    ArgumentRules* setSpeciesArgRules = new ArgumentRules();
    setSpeciesArgRules->push_back( new ArgumentRule( "species", ModelVector<RlString>::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("setSpecies", new MemberProcedure(RlUtils::Void,                      setSpeciesArgRules  ) );
    
    ArgumentRules* extractTripletsArgRules = new ArgumentRules();
    extractTripletsArgRules->push_back( new ArgumentRule( "trees", ModelVector<TimeTree>::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("extractTriplets", new MemberProcedure(RlUtils::Void,            extractTripletsArgRules  ) );
    
}

