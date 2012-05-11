///**
// * @file
// * This file contains the implementation of Dist_ctmm, which holds
// * the parameters and functions related to a distribution on
// * discrete states from a continuous-time discrete-state Markov
// * model.
// *
// * @brief Implementation of Dist_ctmm
// *
// * (c) Copyright 2009- under GPL version 3
// * @date Last modified: $Date$
// * @author The RevBayes Development Core Team
// * @license GPL version 3
// * @version 1.0
// * @since 2009-08-27, version 1.0
// *
// * $Id$
// */
//
//#include "CharacterStateDiscrete.h"
//#include "Dist_ctmm.h"
//#include "RandomNumberFactory.h"
//#include "RandomNumberGenerator.h"
//#include "RateMatrix.h"
//#include "RbException.h"
//#include "RbUtil.h"
//#include "Simplex.h"
//#include "TransitionProbabilityMatrix.h"
//
//#include <cmath>
//#include <sstream>
//#include <vector>
//
//
///** Default constructor for parser use */
//Dist_ctmm::Dist_ctmm( void ) : DistributionDiscrete(  ) {
//}
//
//
//Dist_ctmm::Dist_ctmm(const Dist_ctmm& d) : DistributionDiscrete(d), 
//                                           randomVariable( NULL ),     
//                                           rateMatrix( d.rateMatrix ),
//                                           time( d.time ),
//                                           initialState( d.initialState ) {
//    
//    if (d.randomVariable != NULL) {
//        randomVariable = d.randomVariable->clone();
//    }
//}
//
//
///** Destructor. We need to free the random variable. */
//Dist_ctmm::~Dist_ctmm( void ) {
//    delete randomVariable;
//}
//
//
//Dist_ctmm& Dist_ctmm::operator=(const Dist_ctmm &d) {
//    
//    if (this != &d) {
//        DistributionDiscrete::operator=(d);
//        
//        randomVariable = d.randomVariable->clone();
//    }
//    
//    return *this;
//}
//
///** Clone this object */
//Dist_ctmm* Dist_ctmm::clone( void ) const {
//
//    return new Dist_ctmm( *this );
//}
//
//
///** Get class name of object */
//const std::string& Dist_ctmm::getClassName(void) { 
//    
//    static std::string rbClassName = "Continuous time Markov model";
//    
//	return rbClassName; 
//}
//
///** Get class type spec describing type of object */
//const TypeSpec& Dist_ctmm::getClassTypeSpec(void) { 
//    
//    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( DistributionDiscrete::getClassTypeSpec() ) );
//    
//	return rbClass; 
//}
//
///** Get type spec */
//const TypeSpec& Dist_ctmm::getTypeSpec( void ) const {
//    
//    static TypeSpec typeSpec = getClassTypeSpec();
//    
//    return typeSpec;
//}
//
//
//
///** Get member variable rules */
//const MemberRules& Dist_ctmm::getMemberRules( void ) const {
//
//    static MemberRules memberRules = MemberRules();
//    static bool        rulesSet = false;
//
//    if ( !rulesSet ) {
//        memberRules.push_back( new ValueRule( "Q", RateMatrix::getClassTypeSpec()             ) );
//        memberRules.push_back( new ValueRule( "v", RealPos::getClassTypeSpec()                ) );
//        memberRules.push_back( new ValueRule( "a", CharacterStateDiscrete::getClassTypeSpec() ) );
//
//        rulesSet = true;
//    }
//
//    return memberRules;
//}
//
//
///** Get the number of states in the distribution */
//size_t Dist_ctmm::getNumberOfStates( void ) const {
//
//    size_t numStates  = static_cast<const CharacterStateDiscrete&>( initialState->getValue() ).getNumberOfStates();
//    
//    return numStates;
//}
//
//
///** Get the probability mass vector */
//const Simplex& Dist_ctmm::getProbabilityMassVector( void ) {
//
//    // get the information from the arguments for reading the file
//    const RateMatrix&                q = static_cast<const RateMatrix&>( rateMatrix->getValue() );
//    const RealPos&                   t = static_cast<const RealPos&>( time->getValue() );
////    const CharacterStateDiscrete*    c = static_cast<const CharacterStateDiscrete*>(    members[2].getValue() );
//    
//    // initialize the number of states
//    const size_t nStates = q.getNumberOfStates();
//    
//    // check that the number of states isn't 1
//    if ( nStates < 2 )
//    {
//        std::stringstream o;
//        o << "Too few states for the rate matrix";
//        throw( RbException(o.str()) );
//    }
//    
//    // construct a rate matrix of the correct dimensions
//    TransitionProbabilityMatrix m = TransitionProbabilityMatrix(nStates);
//    
//    // calculate the transition probabilities    
//    q.calculateTransitionProbabilities( t.getValue(), m );
//    
//    //
//    return probMassVector; // \TODO Return vector
//}
//
//
///** Get the state vector for this distribution */
//const std::vector<RbLanguageObject*>& Dist_ctmm::getStateVector( void ) const {
//    return stateVector;
//}
//
///** Get random variable type */
//const TypeSpec& Dist_ctmm::getVariableType( void ) const {
//
//    static TypeSpec varTypeSpec = CharacterStateDiscrete::getClassTypeSpec();
//    
//    return varTypeSpec;
//}
//
//
///**
// * This function calculates the natural log of the
// * probability of a random character state variable.
// *
// * @brief Natural log of probability of character state
// *
// * @param value Observed character state
// * @return      Natural log of the probability
// */
//double Dist_ctmm::lnPdf( const RbLanguageObject& value ) const {
//
//    // Get the parameters
//    const RateMatrix&             Q      = static_cast<const RateMatrix&            >( rateMatrix->getValue() );
//    double                        t      = static_cast<const RealPos&               >( time->getValue() ).getValue();
//    const CharacterStateDiscrete& start  = static_cast<const CharacterStateDiscrete&>( initialState->getValue() );
//    const CharacterStateDiscrete& stop   = static_cast<const CharacterStateDiscrete&>( value );
//    
//    // calculate the transition probability matrix
//    
//    // initialize the number of states
//    size_t nStates = Q.getNumberOfStates();
//    
//    // construct a rate matrix of the correct dimensions
//    TransitionProbabilityMatrix m = TransitionProbabilityMatrix(nStates);
//    
//    // calculate the transition probabilities    
//    Q.calculateTransitionProbabilities( t, m );
//    
//    double lnprob = 0.0;
//    const std::vector<bool>& startState = start.getStateVector();
//    const std::vector<bool>& stopState  = stop.getStateVector();
//    size_t indexStart=0;
//    size_t indexEnd=0;
//    for (std::vector<bool>::const_iterator itStart=startState.begin() ; itStart!=startState.end(); itStart++, indexStart++) {
//        // test whether the state is set
//        if (*itStart) {
//            for (std::vector<bool>::const_iterator itStop=stopState.begin(); itStop!=stopState.end(); itStop++, indexEnd++) {
//                // test whether the state is set
//                if (*itStop) {
//                    lnprob += log(m[indexStart][indexEnd]);
//                }
//            }
//        }
//    }
//    
//    return lnprob;
//}
//
//
///**
// * This function calculates the probability
// * of a random character state variable.
// *
// * @brief Probability of character state
// *
// * @param value Observed character state
// * @return      Probability
// */
//double Dist_ctmm::pdf( const RbLanguageObject& value ) const {
//
//    // Get the parameters
//    const RateMatrix&             Q      = static_cast<const RateMatrix&            >( rateMatrix->getValue() );
//    double                        t      = static_cast<const RealPos&               >( time->getValue() ).getValue();
//    const CharacterStateDiscrete& start  = static_cast<const CharacterStateDiscrete&>( initialState->getValue() );
//    const CharacterStateDiscrete& stop   = static_cast<const CharacterStateDiscrete&>( value );
//
//    // calculate the transition probability matrix
//    
//    // initialize the number of states
//    size_t nStates = Q.getNumberOfStates();
//    
//    // construct a rate matrix of the correct dimensions
//    TransitionProbabilityMatrix m = TransitionProbabilityMatrix(nStates);
//    
//    // calculate the transition probabilities    
//    Q.calculateTransitionProbabilities( t, m );
//    
//    double prob = 1.0;
//    const std::vector<bool>& startState = start.getStateVector();
//    const std::vector<bool>& stopState  = stop.getStateVector();
//    size_t indexStart=0;
//    size_t indexEnd=0;
//    for (std::vector<bool>::const_iterator itStart=startState.begin() ; itStart!=startState.end(); itStart++, indexStart++) {
//        // test whether the state is set
//        if (*itStart) {
//            for (std::vector<bool>::const_iterator itStop=stopState.begin(); itStop!=stopState.end(); itStop++, indexEnd++) {
//                // test whether the state is set
//                if (*itStop) {
//                    prob *= (m[indexStart][indexEnd]);
//                }
//            }
//        }
//    }
//
//    return prob;
//}
//
//
///**
// * This function generates a character state from the distribution
// *
// * @brief Random draw from distribution
// *
// * @return      Randomly drawn character state
// */
//void Dist_ctmm::rvSingleValue( std::vector<size_t> &offset ) {
//    
////    RandomNumberGenerator* rng = GLOBAL_RNG;
////    randomVariable.value[offset[2]] = RbStatistics::Normal::rv(mean.value[offset[0]], sd.value[offset[1]], *rng);
//    
//}
//
//
///** We catch here the setting of the member variables to store our parameters. */
//void Dist_ctmm::setInternalParameters(const std::vector<RbValue<void *> > &p) {
//    
//    rateMatrix.value        = static_cast<RateMatrix*>( p[0].value );
//    rateMatrix.lengths      = p[0].lengths;
//    
//    time.value              = static_cast<double*>( p[1].value );
//    time.lengths            = p[1].lengths;
//    
//    initialState.value      = static_cast<char*>( p[2].value );
//    initialState.lengths    = p[2].lengths;
//    
//    randomVariable.value    = static_cast<char*>( p[3].value );
//    randomVariable.lengths  = p[3].lengths;
//    
//}
//
