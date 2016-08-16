//
//  RlRateGenerator.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 3/17/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#include "ArgumentRule.h"
#include "ModelVector.h"
#include "Natural.h"
#include "Real.h"
#include "RealPos.h"
#include "RlBoolean.h"
#include "RlRateGenerator.h"
#include "RlRateMatrix.h"

using namespace RevLanguage;

RateGenerator::RateGenerator(void) : ModelObject<RevBayesCore::RateGenerator>()
{
    
    // add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), "The index of the row.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "[]", ModelVector<RealPos>::getClassTypeSpec(), squareBracketArgRules) );
    
    
    // add method for call "x[]" as a function
    ArgumentRules* sizeArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "size", Natural::getClassTypeSpec(), sizeArgRules) );
    
}


RateGenerator::RateGenerator( const RevBayesCore::RateGenerator &v) : ModelObject<RevBayesCore::RateGenerator>( v.clone() )
{
    
    // add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), "The index of the row.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "[]", ModelVector<RealPos>::getClassTypeSpec(), squareBracketArgRules) );
    
    
    // add method for call "x[]" as a function
    ArgumentRules* sizeArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "size", Natural::getClassTypeSpec(), sizeArgRules) );
    
}


RateGenerator::RateGenerator( RevBayesCore::RateGenerator *v) : ModelObject<RevBayesCore::RateGenerator>( v )
{
    
    // add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), "The index of the row.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "[]", ModelVector<RealPos>::getClassTypeSpec(), squareBracketArgRules) );
    
    
    // add method for call "x[]" as a function
    ArgumentRules* sizeArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "size", Natural::getClassTypeSpec(), sizeArgRules) );
    
}


RateGenerator::RateGenerator( RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator> *m) : ModelObject<RevBayesCore::RateGenerator>( m )
{
    
    // add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), "The index of the row.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "[]", ModelVector<RealPos>::getClassTypeSpec(), squareBracketArgRules) );
    
    
    // add method for call "x[]" as a function
    ArgumentRules* sizeArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "size", Natural::getClassTypeSpec(), sizeArgRules) );
    
}


RateGenerator* RateGenerator::clone() const
{
    return new RateGenerator( *this );
}


/* Map calls to member methods */
RevPtr<RevVariable> RateGenerator::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    if (name == "[]")
    {
        found = true;
        
        RevBayesCore::RateMatrix* rm = dynamic_cast<RevBayesCore::RateMatrix*>( &this->dagNode->getValue() );

        // index operations are supported if the RateGenerator can be safely cast as a RateMatrix
        if (rm != NULL) {
            // get the member with give index
            const Natural& index = static_cast<const Natural&>( args[0].getVariable()->getRevObject() );
            
            if ( this->dagNode->getValue().getNumberOfStates() < (size_t)(index.getValue()) ) {
                throw RbException("Index out of bounds in []");
            }

            RevBayesCore::RbVector<double> elementVector;
            for (size_t i=0; i < this->dagNode->getValue().size(); ++i) {
                double v = rm->getRate( (size_t)(index.getValue())-1, i, 0.0, 1.0);
//                std::cout << v << " ";
                elementVector.push_back( v );
            }
//            std::cout << "\n";
            
            // MJL: The returned vector is not dynamically updated. Perhaps "[]" should be a MemberFunction, not MemberProcedure?
            return new RevVariable( new ModelVector<Real>( elementVector ) );
            
        }
        else {
            int n = 0; //(int)this->dagNode->getValue().getNumberOfStates();
            return new RevVariable( new Natural(n) );
            throw RbException("Currently deprecated. Blame Michael (or Sebastian)!");
        }
    }
    else if (name == "size")
    {
        found = true;
        
        int n = 0; //(int)this->dagNode->getValue().getNumberOfStates();
        return new RevVariable( new Natural(n) );
    }
    
    return ModelObject<RevBayesCore::RateGenerator>::executeMethod( name, args, found );
}


/* Get Rev type of object */
const std::string& RateGenerator::getClassType(void) {
    
    static std::string revType = "RateGenerator";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& RateGenerator::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& RateGenerator::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}
