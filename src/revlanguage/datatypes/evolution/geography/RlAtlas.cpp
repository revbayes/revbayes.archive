#include "ArgumentRule.h"
#include "TimeAtlas.h"
#include "GeographicArea.h"
#include "ModelVector.h"
#include "MemberProcedure.h"
#include "Natural.h"
#include "OptionRule.h"
#include "RealPos.h"
#include "RlAtlas.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "RlTaxonData.h"
#include "TimeAtlas.h"
#include "Variable.h"
#include <vector>

using namespace RevLanguage;

RlAtlas::RlAtlas(void) : ModelObject<RevBayesCore::TimeAtlas>( )
{

    ArgumentRules* nAreasArgRules               = new ArgumentRules();
    ArgumentRules* nEpochsArgRules              = new ArgumentRules();
    ArgumentRules* namesArgRules                = new ArgumentRules();
    
    methods.addFunction("names",               new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(), namesArgRules           ) );
    methods.addFunction("nAreas",              new MemberProcedure(Natural::getClassTypeSpec(),               nAreasArgRules          ) );
    methods.addFunction("nEpochs",             new MemberProcedure(Natural::getClassTypeSpec(),               nEpochsArgRules         ) );

    ArgumentRules* adjacentArgRules             = new ArgumentRules();
    std::vector<std::string> optionsElements;
    optionsElements.push_back( "off-diagonal" );
    optionsElements.push_back( "diagonal" );
    optionsElements.push_back( "upper" );
    adjacentArgRules->push_back( new OptionRule( "elements", new RlString( "off-diagonal" ), optionsElements ) );
    adjacentArgRules->push_back( new ArgumentRule("symmetric", RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
    methods.addFunction("getEpochValues",         new MemberProcedure( RlUtils::Void, adjacentArgRules) );

    // Add method for call "size" as a function
    ArgumentRules* sizeArgRules = new ArgumentRules();
    methods.addFunction("size",  new MemberProcedure( Natural::getClassTypeSpec(), sizeArgRules) );

}


RlAtlas::RlAtlas( RevBayesCore::TimeAtlas *v) : ModelObject<RevBayesCore::TimeAtlas>( v ),
atlas(v)
{

    ArgumentRules* nAreasArgRules               = new ArgumentRules();
    ArgumentRules* nEpochsArgRules              = new ArgumentRules();
    ArgumentRules* namesArgRules                = new ArgumentRules();
    
    methods.addFunction("names",               new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(), namesArgRules           ) );
    methods.addFunction("nAreas",              new MemberProcedure(Natural::getClassTypeSpec(),               nAreasArgRules          ) );
    methods.addFunction("nEpochs",             new MemberProcedure(Natural::getClassTypeSpec(),               nEpochsArgRules         ) );
    
    ArgumentRules* adjacentArgRules             = new ArgumentRules();
    std::vector<std::string> optionsElements;
    optionsElements.push_back( "off-diagonal" );
    optionsElements.push_back( "diagonal" );
    optionsElements.push_back( "upper" );
    adjacentArgRules->push_back( new OptionRule( "elements", new RlString( "off-diagonal" ), optionsElements ) );
    adjacentArgRules->push_back( new ArgumentRule("symmetric", RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
    methods.addFunction("getEpochValues",         new MemberProcedure( RlUtils::Void, adjacentArgRules) );

    
    // Add method for call "size" as a function
    ArgumentRules* sizeArgRules = new ArgumentRules();
    methods.addFunction("size",  new MemberProcedure( Natural::getClassTypeSpec(), sizeArgRules) );

}


RlAtlas::RlAtlas( RevBayesCore::TypedDagNode<RevBayesCore::TimeAtlas> *m) : ModelObject<RevBayesCore::TimeAtlas>( m ),
atlas(&m->getValue())
{

    ArgumentRules* nAreasArgRules               = new ArgumentRules();
    ArgumentRules* nEpochsArgRules              = new ArgumentRules();
    ArgumentRules* namesArgRules                = new ArgumentRules();
    
    methods.addFunction("names",               new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(), namesArgRules           ) );
    methods.addFunction("nAreas",              new MemberProcedure(Natural::getClassTypeSpec(),               nAreasArgRules          ) );
    methods.addFunction("nEpochs",             new MemberProcedure(Natural::getClassTypeSpec(),               nEpochsArgRules         ) );

    ArgumentRules* adjacentArgRules             = new ArgumentRules();
    std::vector<std::string> optionsElements;
    optionsElements.push_back( "off-diagonal" );
    optionsElements.push_back( "diagonal" );
    optionsElements.push_back( "upper" );
    adjacentArgRules->push_back( new OptionRule( "elements", new RlString( "off-diagonal" ), optionsElements ) );
    adjacentArgRules->push_back( new ArgumentRule("symmetric", RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
    methods.addFunction("getEpochValues",         new MemberProcedure( RlUtils::Void, adjacentArgRules) );
    
    // Add method for call "size" as a function
    ArgumentRules* sizeArgRules = new ArgumentRules();
    methods.addFunction("size",  new MemberProcedure( Natural::getClassTypeSpec(), sizeArgRules) );

}


RlAtlas* RlAtlas::clone() const {
    return new RlAtlas( *this );
}


/* Map calls to member methods */
RevPtr<Variable> RlAtlas::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    if (name == "nAreas")
    {
        found = true;
        
        return new Variable(new Natural((int)this->dagNode->getValue().getNumAreas())) ;
    }
    else if (name == "nEpochs")
    {
        found = true;
        
        return new Variable(new Natural((int)this->dagNode->getValue().getNumEpochs())) ;
    }
    else if (name == "names")
    {
        found = true;
        
        ModelVector<RlString> *n = new ModelVector<RlString>();
        const std::vector<std::vector<RevBayesCore::GeographicArea*> >& areas = this->dagNode->getValue().getAreas();
        for (size_t i = 0; i < areas[0].size(); ++i)
        {
            std::string name = areas[0][i]->getName();
            n->push_back( name );
        }
        return new Variable( n );
    }
    else if (name == "epochs")
    {
        found = true;
        ModelVector<RealPos> *n = new ModelVector<RealPos>( this->dagNode->getValue().getEpochs() );
        return new Variable( n );
    }
    else if (name == "getEpochValues")
    {
        found = true;
        
        // get the member with give index
        std::string elements = static_cast<const RlString &>( args[0].getVariable()->getRevObject() ).getValue();
        bool symmetric = static_cast<const RlBoolean &>( args[1].getVariable()->getRevObject() ).getValue();

        std::vector<std::vector<RevBayesCore::GeographicArea*> > areas = this->dagNode->getValue().getAreas();
        
        ModelVector<ModelVector<RealPos> > *f = new ModelVector<ModelVector<RealPos> >();
        for (size_t i = 0; i < areas.size(); i++)
        {
            ModelVector<RealPos> v;
            for (size_t j = 0; j < areas[i].size(); j++)
            {
                std::vector<double> a = areas[i][j]->getDispersalValues();
                if (elements == "diagonal")
                {
                    v.push_back(a[j]);
                    continue;
                }
                for (size_t k = 0; k < areas[i].size(); k++)
                {
                    if (j == k && elements == "upper")
                        v.push_back(a[k]);
                    
                    if (j < k || symmetric)
                    {
                        v.push_back(a[k]);
                    }
                }
            }
            f->push_back(v);
        }
        
        return new Variable(f);
    }
    
    return ModelObject<RevBayesCore::TimeAtlas>::executeMethod( name, args, found );
}


/* Get Rev type of object */
const std::string& RlAtlas::getClassType(void) {
    
    static std::string revType = "RlAtlas";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& RlAtlas::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& RlAtlas::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}

