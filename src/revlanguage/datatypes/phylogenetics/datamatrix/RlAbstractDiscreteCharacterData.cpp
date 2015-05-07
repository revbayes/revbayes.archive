#include "RlAbstractDiscreteCharacterData.h"
#include "ArgumentRule.h"
#include "MemberProcedure.h"
#include "ModelVector.h"
#include "Natural.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "RlDiscreteTaxonData.h"
#include "RlSimplex.h"


using namespace RevLanguage;

AbstractDiscreteCharacterData::AbstractDiscreteCharacterData(void) :
    ModelObject<RevBayesCore::AbstractDiscreteCharacterData>(),
    AbstractCharacterData( NULL )
{
    
    ArgumentRules* chartypeArgRules            = new ArgumentRules();
    ArgumentRules* ishomologousArgRules        = new ArgumentRules();
    ArgumentRules* empiricalBaseArgRules       = new ArgumentRules();
    ArgumentRules* invSitesArgRules            = new ArgumentRules();
    ArgumentRules* setCodonPartitionArgRules   = new ArgumentRules();
    ArgumentRules* setCodonPartitionArgRules2  = new ArgumentRules();
    ArgumentRules* setNumStatesPartitionArgRules   = new ArgumentRules();
    setCodonPartitionArgRules->push_back(  new ArgumentRule("", Natural::getClassTypeSpec()              , ArgumentRule::BY_VALUE) );
    setCodonPartitionArgRules2->push_back( new ArgumentRule("", ModelVector<Natural>::getClassTypeSpec() , ArgumentRule::BY_VALUE) );
    setNumStatesPartitionArgRules->push_back(  new ArgumentRule("", Natural::getClassTypeSpec()              , ArgumentRule::BY_VALUE) );
    
    
    methods.addFunction("chartype",            new MemberProcedure(RlString::getClassTypeSpec(),      chartypeArgRules           ) );
    methods.addFunction("setCodonPartition",   new MemberProcedure(RlUtils::Void,        setCodonPartitionArgRules  ) );
    methods.addFunction("setCodonPartition",   new MemberProcedure(RlUtils::Void,        setCodonPartitionArgRules2 ) );
    methods.addFunction("setNumStatesPartition",   new MemberProcedure(RlUtils::Void,        setNumStatesPartitionArgRules  ) );
    methods.addFunction("isHomologous",        new MemberProcedure(RlBoolean::getClassTypeSpec(),     ishomologousArgRules       ) );
    methods.addFunction("getEmpiricalBaseFrequencies", new MemberProcedure(Simplex::getClassTypeSpec(),     empiricalBaseArgRules       ) );
    methods.addFunction("getNumInvariantSites", new MemberProcedure(Natural::getClassTypeSpec(),     invSitesArgRules       ) );

}


AbstractDiscreteCharacterData::AbstractDiscreteCharacterData( RevBayesCore::AbstractDiscreteCharacterData *d) :
    ModelObject<RevBayesCore::AbstractDiscreteCharacterData>( d ),
    AbstractCharacterData( NULL )
{
    // set the internal value pointer
    setCharacterDataObject( &this->getDagNode()->getValue() );
    
    // insert the character data specific methods
    MethodTable charDataMethods = getCharacterDataMethods();
    methods.insertInheritedMethods( charDataMethods );
    
    ArgumentRules* chartypeArgRules            = new ArgumentRules();
    ArgumentRules* ishomologousArgRules        = new ArgumentRules();
    ArgumentRules* empiricalBaseArgRules       = new ArgumentRules();
    ArgumentRules* invSitesArgRules            = new ArgumentRules();
    ArgumentRules* setCodonPartitionArgRules   = new ArgumentRules();
    ArgumentRules* setCodonPartitionArgRules2  = new ArgumentRules();
    ArgumentRules* setNumStatesPartitionArgRules   = new ArgumentRules();
    setCodonPartitionArgRules->push_back(  new ArgumentRule(""           , Natural::getClassTypeSpec()              , ArgumentRule::BY_VALUE) );
    setCodonPartitionArgRules2->push_back( new ArgumentRule(""           , ModelVector<Natural>::getClassTypeSpec() , ArgumentRule::BY_VALUE) );
    setNumStatesPartitionArgRules->push_back(  new ArgumentRule(""           , Natural::getClassTypeSpec()              , ArgumentRule::BY_VALUE) );
    
    
    methods.addFunction("chartype",            new MemberProcedure(RlString::getClassTypeSpec(),      chartypeArgRules           ) );
    methods.addFunction("setCodonPartition",   new MemberProcedure(RlUtils::Void,        setCodonPartitionArgRules  ) );
    methods.addFunction("setCodonPartition",   new MemberProcedure(RlUtils::Void,        setCodonPartitionArgRules2 ) );
    methods.addFunction("setNumStatesPartition",   new MemberProcedure(RlUtils::Void,        setNumStatesPartitionArgRules  ) );
    methods.addFunction("isHomologous",        new MemberProcedure(RlBoolean::getClassTypeSpec(),     ishomologousArgRules       ) );
    methods.addFunction("getEmpiricalBaseFrequencies", new MemberProcedure(Simplex::getClassTypeSpec(),     empiricalBaseArgRules       ) );
    methods.addFunction("getNumInvariantSites", new MemberProcedure(Natural::getClassTypeSpec(),     invSitesArgRules       ) );

}


AbstractDiscreteCharacterData::AbstractDiscreteCharacterData( const RevBayesCore::AbstractDiscreteCharacterData &d) :
    ModelObject<RevBayesCore::AbstractDiscreteCharacterData>( d.clone() ),
    AbstractCharacterData( NULL )
{
    // set the internal value pointer
    setCharacterDataObject( &this->getDagNode()->getValue() );
    
    // insert the character data specific methods
    MethodTable charDataMethods = getCharacterDataMethods();
    methods.insertInheritedMethods( charDataMethods );
    
    ArgumentRules* chartypeArgRules            = new ArgumentRules();
    ArgumentRules* ishomologousArgRules        = new ArgumentRules();
    ArgumentRules* empiricalBaseArgRules       = new ArgumentRules();
    ArgumentRules* invSitesArgRules            = new ArgumentRules();
    ArgumentRules* setCodonPartitionArgRules   = new ArgumentRules();
    ArgumentRules* setCodonPartitionArgRules2  = new ArgumentRules();
    ArgumentRules* setNumStatesPartitionArgRules   = new ArgumentRules();
    setCodonPartitionArgRules->push_back(  new ArgumentRule(""           , Natural::getClassTypeSpec()              , ArgumentRule::BY_VALUE) );
    setCodonPartitionArgRules2->push_back( new ArgumentRule(""           , ModelVector<Natural>::getClassTypeSpec() , ArgumentRule::BY_VALUE) );
    setNumStatesPartitionArgRules->push_back(  new ArgumentRule(""           , Natural::getClassTypeSpec()              , ArgumentRule::BY_VALUE) );
    
    
    methods.addFunction("chartype",            new MemberProcedure(RlString::getClassTypeSpec(),      chartypeArgRules           ) );
    methods.addFunction("setCodonPartition",   new MemberProcedure(RlUtils::Void,        setCodonPartitionArgRules  ) );
    methods.addFunction("setCodonPartition",   new MemberProcedure(RlUtils::Void,        setCodonPartitionArgRules2 ) );
    methods.addFunction("setNumStatesPartition",   new MemberProcedure(RlUtils::Void,        setNumStatesPartitionArgRules  ) );
    methods.addFunction("isHomologous",        new MemberProcedure(RlBoolean::getClassTypeSpec(),     ishomologousArgRules       ) );
    methods.addFunction("getEmpiricalBaseFrequencies", new MemberProcedure(Simplex::getClassTypeSpec(),     empiricalBaseArgRules       ) );
    methods.addFunction("getNumInvariantSites", new MemberProcedure(Natural::getClassTypeSpec(),     invSitesArgRules       ) );

}


AbstractDiscreteCharacterData::AbstractDiscreteCharacterData( RevBayesCore::TypedDagNode<RevBayesCore::AbstractDiscreteCharacterData> *d) :
    ModelObject<RevBayesCore::AbstractDiscreteCharacterData>( d ),
    AbstractCharacterData( NULL )
{
    // set the internal value pointer
    setCharacterDataObject( &this->getDagNode()->getValue() );
    
    // insert the character data specific methods
    MethodTable charDataMethods = getCharacterDataMethods();
    methods.insertInheritedMethods( charDataMethods );
    
    ArgumentRules* chartypeArgRules            = new ArgumentRules();
    ArgumentRules* ishomologousArgRules        = new ArgumentRules();
    ArgumentRules* empiricalBaseArgRules       = new ArgumentRules();
    ArgumentRules* invSitesArgRules            = new ArgumentRules();
    ArgumentRules* setCodonPartitionArgRules   = new ArgumentRules();
    ArgumentRules* setCodonPartitionArgRules2  = new ArgumentRules();
    ArgumentRules* setNumStatesPartitionArgRules   = new ArgumentRules();
    setCodonPartitionArgRules->push_back(  new ArgumentRule(""           , Natural::getClassTypeSpec()              , ArgumentRule::BY_VALUE) );
    setCodonPartitionArgRules2->push_back( new ArgumentRule(""           , ModelVector<Natural>::getClassTypeSpec() , ArgumentRule::BY_VALUE) );
    setNumStatesPartitionArgRules->push_back(  new ArgumentRule(""           , Natural::getClassTypeSpec()              , ArgumentRule::BY_VALUE) );
    
    
    methods.addFunction("chartype",            new MemberProcedure(RlString::getClassTypeSpec(),      chartypeArgRules           ) );
    methods.addFunction("setCodonPartition",   new MemberProcedure(RlUtils::Void,        setCodonPartitionArgRules  ) );
    methods.addFunction("setCodonPartition",   new MemberProcedure(RlUtils::Void,        setCodonPartitionArgRules2 ) );
    methods.addFunction("setNumStatesPartition",   new MemberProcedure(RlUtils::Void,        setNumStatesPartitionArgRules  ) );
    methods.addFunction("isHomologous",        new MemberProcedure(RlBoolean::getClassTypeSpec(),     ishomologousArgRules       ) );
    methods.addFunction("getEmpiricalBaseFrequencies", new MemberProcedure(Simplex::getClassTypeSpec(),     empiricalBaseArgRules       ) );
    methods.addFunction("getNumInvariantSites", new MemberProcedure(Natural::getClassTypeSpec(),     invSitesArgRules       ) );

}



AbstractDiscreteCharacterData* AbstractDiscreteCharacterData::concatenate(const RevObject &d) const
{
    const AbstractDiscreteCharacterData* tmp = dynamic_cast<const AbstractDiscreteCharacterData*>( &d );
    if ( tmp != NULL )
    {
        return concatenate( *tmp );
    }
    else
    {
        throw RbException("Cannot add an object of type '" + d.getType() + "' to a character data object.");
    }
}



AbstractDiscreteCharacterData* AbstractDiscreteCharacterData::concatenate(const AbstractDiscreteCharacterData &d) const
{
    AbstractDiscreteCharacterData* cloneObj = clone();
    cloneObj->getDagNode()->getValue().concatenate( d.getValue() );
    
    // return the copy
    return cloneObj;
}



AbstractDiscreteCharacterData* AbstractDiscreteCharacterData::clone() const
{
    return new AbstractDiscreteCharacterData( *this );
}


/* Map calls to member methods */
RevPtr<RevVariable> AbstractDiscreteCharacterData::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    if ( this->getDagNode() != NULL )
    {
        // set the internal value pointer
        setCharacterDataObject( &this->getDagNode()->getValue() );
    }
    
    RevPtr<RevVariable> retVal = executeCharacterDataMethod(name, args, found);
    
    if ( found == true )
    {
        return retVal;
    }
    else if (name == "setCodonPartition")
    {
        found = true;
        
        const RevObject& argument = args[0].getVariable()->getRevObject();
        RevBayesCore::AbstractDiscreteCharacterData &v = dagNode->getValue();
        size_t nChars = v.getNumberOfCharacters();
        
        // e.g. data.setCodonPartition(sites=v(3))
        if ( argument.isType( Natural::getClassTypeSpec() ) )
        {
            size_t n = size_t( static_cast<const Natural&>( argument ).getValue() );
            for (size_t i = 0; i < nChars; i++)
            {
                
                if (i % 3 == (n-1))
                {
                    v.includeCharacter(i);
                }
                else
                {
                    v.excludeCharacter(i);
                }
                
            }
        }
        
        // e.g. data.setCodonPartition(sites=v(1,2))
        else if ( argument.isType( ModelVector<Natural>::getClassTypeSpec() ) )
        {
            const ModelVector<Natural>& x = static_cast<const ModelVector<Natural>&>( argument );
            if (x.size() == 0)
            {
                return NULL;
            }
            
            for (size_t i = 0; i < nChars; i++)
            {
                v.excludeCharacter(i);
            }
            
            for (size_t i = 0; i < x.size(); i++)
            {
                size_t n = x[i];
                for (size_t j = 0; j < nChars; j++)
                {
                    
                    if (j % 3 == (n-1))
                    {
                        v.includeCharacter(j);
                    }
                    
                }
            }
        }
        return NULL;
    }
    else if (name == "setNumStatesPartition")
    {
        found = true;
        
        const RevObject& argument = args[0].getVariable()->getRevObject();
        RevBayesCore::AbstractDiscreteCharacterData &v = dagNode->getValue();
        size_t nChars = v.getNumberOfCharacters();
        size_t nTaxa = v.getNumberOfTaxa();
        
        // e.g. data.setNumStatesPartition(2)
        size_t n = size_t( static_cast<const Natural&>( argument ).getValue() );
        for (size_t i = 0; i < nChars; i++)
        {
            int max = 0;
            for (size_t j = 0; j < nTaxa; j++)
            {
                const RevBayesCore::AbstractDiscreteTaxonData& td = v.getTaxonData(j);
                if (!td.getCharacter(i).isAmbiguous() && !td.getCharacter(i).isGapState())
                {
                    int k = int(td.getCharacter(i).getStateIndex()) + 1;
                    if (k > max)
                    {
                        max = k;
                    }
                }
            }

            if (max == n)
            {
                v.includeCharacter(i);
            }
            else
            {
                v.excludeCharacter(i);
            }
        }
        return NULL;
    }
    else if (name == "isHomologous")
    {
        found = true;
        
        bool ih = this->dagNode->getValue().isHomologyEstablished();
        
        return new RevVariable( new RlBoolean(ih) );
    }
    else if (name == "getEmpiricalBaseFrequencies")
    {
        found = true;
        
        std::vector<double> ebf = this->dagNode->getValue().getEmpiricalBaseFrequencies();
        
        return new RevVariable( new Simplex(ebf) );
    }
    else if (name == "getNumInvariantSites")
    {
        found = true;
        
        size_t n = this->dagNode->getValue().getNumberOfInvariantSites();
        
        return new RevVariable( new Natural(n) );
    }
    
    return ModelObject<RevBayesCore::AbstractDiscreteCharacterData>::executeMethod( name, args, found );
}


/* Get Rev type of object */
const std::string& AbstractDiscreteCharacterData::getClassType(void)
{
    
    static std::string revType = "AbstractDiscreteCharacterData";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& AbstractDiscreteCharacterData::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& AbstractDiscreteCharacterData::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}


