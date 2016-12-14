/**
 * @file
 * This file contains the declaration of PomoState, which is
 * the class for the DNA data types plus two-state polymorphic states in RevBayes.
 * Instead of the usual coding by bitfields, we choose coding by ints.
 *
 * @brief Declaration of PomoState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-05-24 09:58:04 +0200 (Thu, 24 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: PomoState.h 1568 2012-05-24 07:58:04Z hoehna $
 */

#ifndef PomoState_H
#define PomoState_H

#include "DiscreteCharacterState.h"
#include <ostream>
#include <set>

namespace RevBayesCore {

    class PomoState : public DiscreteCharacterState {

    public:
        PomoState(void);                                                                    //!< Default constructor
        PomoState(size_t vps);                                                              //!< Constructor with virtual population size
        PomoState(const std::string &s);                                                    //!< Constructor with an observation
        PomoState(const std::string &s, const std::string chromosome,
          const size_t position, const size_t virtualPopulationSize );                      //!< Constructor that sets all fields
        PomoState*                       clone(void) const;                                 //!< Get a copy of this object

        // Discrete character observation functions
        std::string                     getDataType(void) const;                            //!< Get the datatype as a common string.
        const std::string&              getStateLabels(void) const;                         //!< Get valid state labels
        std::string                     getStringValue(void) const;                         //!< Get a representation of the character as a string
        void                            setVirtualPopulationSize(size_t populationSize);    //!< Set the virtual population size for the state space
        void                            setState(const std::string &symbol);                //!< Compute the internal state value for this character.
        void                            setChromosome(std::string chromosome);              //!< Set the chromosome for the state
        void                            setPosition(size_t position);                       //!< Set the position for the state
        const std::string               getChromosome( void );                              //!< Get the chromosome for the state
        const size_t                    getPosition( void );                                //!< Get the position for the state

        const std::vector<double>       getWeights( void ) const;                            //!< Get the weight of the state


    private:
        std::string                     chromosome_;                                        //!< The chromosome on which the state lies
        size_t                          position_;                                          //!< The position of the state in the chromosome
        size_t                          virtualPopulationSize_;                             //!< The virtual population size of the Pomo model (by default, 10)
        std::vector<double>             weights_;                                           //!< Weights are used when the "average" option is used
        bool                            pomoRandomSampling_;                                //!< Do we perform random sampling? Otherwise we would do average.
    };

}

#endif
