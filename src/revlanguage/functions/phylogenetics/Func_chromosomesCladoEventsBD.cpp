//
//  Func_chromosomesCladoEventsBD.cpp
//
//  Created by Will Freyman on 6/22/16.
//

#include "ChromosomesCladogenicBirthDeathFunction.h"
#include "CladogeneticSpeciationRateMatrix.h"
#include "Func_chromosomesCladoEventsBD.h"
#include "RealPos.h"
#include "ModelVector.h"

using namespace RevLanguage;

/** default constructor */
Func_chromosomesCladoEventsBD::Func_chromosomesCladoEventsBD( void ) : TypedFunction<CladogeneticSpeciationRateMatrix>( ) {
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_chromosomesCladoEventsBD* Func_chromosomesCladoEventsBD::clone( void ) const {
    
    return new Func_chromosomesCladoEventsBD( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::CladogeneticSpeciationRateMatrix >* Func_chromosomesCladoEventsBD::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* sr = static_cast<const ModelVector<RealPos> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    
    int ns = static_cast<const Natural &>( this->args[1].getVariable()->getRevObject() ).getValue();
    
    RevBayesCore::ChromosomesCladogenicBirthDeathFunction* f = new RevBayesCore::ChromosomesCladogenicBirthDeathFunction( sr, ns );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_chromosomesCladoEventsBD::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
        
        argumentRules.push_back( new ArgumentRule( "speciation_rates", ModelVector<RealPos>::getClassTypeSpec() , "The speciation rates for different cladogenetic event types.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "max_chromosomes", Natural::getClassTypeSpec(), "The maximum number of chromosomes.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        
        rules_set = true;
    }
    
    return argumentRules;
}


const std::string& Func_chromosomesCladoEventsBD::getClassType(void)
{
    
    static std::string rev_type = "Func_chromosomesCladoEventsBD";
    
    return rev_type;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_chromosomesCladoEventsBD::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_chromosomesCladoEventsBD::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnChromosomesCladoEventsBD";
    
    return f_name;
}


const TypeSpec& Func_chromosomesCladoEventsBD::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}
