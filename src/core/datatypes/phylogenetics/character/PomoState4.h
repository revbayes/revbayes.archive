/**
 * @file
 * This file contains the declaration of PomoState4, which is
 * the class for the DNA data types plus two-state polymorphic states in RevBayes.
 * Instead of the usual coding by bitfields, we choose coding by ints.
 *
 * @brief Declaration of PomoState4
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-05-24 09:58:04 +0200 (Thu, 24 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: PomoState4.h 1568 2012-05-24 07:58:04Z hoehna $
 */

#ifndef PomoState4_H
#define PomoState4_H

#include "DiscreteCharacterState.h"
#include "DistributionBinomial.h"
#include <ostream>
#include <set>

namespace RevBayesCore {

    class PomoState4 : public DiscreteCharacterState {

    public:
        PomoState4(void);                                                                    //!< Default constructor
        PomoState4(size_t vps);                                                              //!< Constructor with virtual population size
        PomoState4(const std::string &s);                                                    //!< Constructor with an observation
        PomoState4(const std::string &s, const std::string chromosome,
                  const size_t position, const size_t virtualPopulationSize,
                  std::vector<double> weights  );                                                   //!< Constructor that sets all fields
        PomoState4(const std::string &s, const std::string chromosome,
                  const size_t position, const size_t virtualPopulationSize);
        PomoState4(const PomoState4& t);                                                //!< Copy constructor

        PomoState4*                       clone(void) const;                                 //!< Get a copy of this object

        // Discrete character observation functions
        void                            addState(const std::string &symbol);                //!< Add a character state to the set of character states
        RbBitSet                        getState(void) const;                               //!< Get the state (as the bitset)
        void                            setToFirstState(void);                              //!< Set this character state to the first (lowest) possible state
        void                            setState(const std::string &symbol);                //!< Compute the internal state value for this character.
        void                            setStateByIndex(size_t index);                      //!< Set the discrete observation

        std::string                     getDataType(void) const;                            //!< Get the datatype as a common string.
        std::string                     getStateLabels(void) const;                         //!< Get valid state labels
        std::string                     getStringValue(void) const;                         //!< Get a representation of the character as a string
        void                            setVirtualPopulationSize(size_t populationSize);    //!< Set the virtual population size for the state space
        void                            setChromosome(std::string chromosome);              //!< Set the chromosome for the state
        void                            setPosition(size_t position);                       //!< Set the position for the state
        const std::string               getChromosome( void );                              //!< Get the chromosome for the state
        const size_t                    getPosition( void );                                //!< Get the position for the state

        const std::vector<double>       getWeights( void ) const;                            //!< Get the weight of the state
        // bool                            isWeighted( void ) const;
        // void                            setWeighted( bool tf );
        bool                            isGapState(void) const;                             //!< Get whether this is a gapped character state
        bool                            isMissingState(void) const;                         //!< Get whether this is a missing character state
        void                            setGapState(bool tf);                               //!< set whether this is a gapped character
        void                            setMissingState(bool tf);                           //!< set whether this is a missing character
        void                            populateWeightedStatesForMonoallelicState(int id1, int sum); //!< Sets the weights of all the states compatible with a monoallelic state


    private:

        bool                            is_gap;
        bool                            is_missing;
        size_t                          index_single_state;
        size_t                          num_observed_states;
        RbBitSet                        state;

        std::string                     chromosome_;                                        //!< The chromosome on which the state lies
        size_t                          position_;                                          //!< The position of the state in the chromosome
        size_t                          virtualPopulationSize_;                             //!< The virtual population size of the Pomo model (by default, 10)
        //std::vector<double>             weights_;                                           //!< Weights are used when the "average" option is used
        //bool                                    weighted_;
        std::string stringValue_;                           //!< The string description of the state.
    };

}

#endif
