#ifndef DiscreteCharacterState_H
#define DiscreteCharacterState_H

#include "CharacterState.h"
#include "RbBitSet.h"

#include <ostream>
#include <string>
#include <vector>

namespace RevBayesCore {

    class DiscreteCharacterState : public CharacterState {

    public:
        virtual                                ~DiscreteCharacterState(void) {}

        bool                                    operator==(const CharacterState& x) const;          //!< Equality
        bool                                    operator!=(const CharacterState& x) const;          //!< Inequality
        bool                                    operator<(const CharacterState& x) const;           //!< Less than
        void                                    operator++();                                       //!< Increment
        void                                    operator++(int i);                                  //!< Increment
        void                                    operator+=(int i);                                  //!< Increment
        void                                    operator--();                                       //!< Decrement
        void                                    operator--(int i);                                  //!< Decrement
        void                                    operator-=(int i);                                  //!< Decrement

        virtual DiscreteCharacterState*         clone(void) const = 0;                              //!< Get a copy of this object


        void                                    addState(const std::string &symbol);                //!< Add a character state to the set of character states
        size_t                                  getNumberObservedStates(void) const;                //!< How many states are observed for the character
        size_t                                  getNumberOfStates(void) const;                      //!< Get the number of discrete states for the character
        const RbBitSet&                         getState(void) const;                               //!< Get the state (as the bitset)
        size_t                                  getStateIndex(void) const;                          //!< Get the state index
        virtual std::string                     getStringValue(void) const;                                 //!< Get a representation of the character as a string
        bool                                    isAmbiguous(void) const;                            //!< Is the character missing or ambiguous
        bool                                    isStateSet(size_t index) const;                     //!< Is the discrete observation
        void                                    setToFirstState(void);                              //!< Set this character state to the first (lowest) possible state
        void                                    setStateByIndex(size_t index);                      //!< Set the discrete observation
        virtual void                            setState(const std::string &symbol);                //!< Set the discrete observation
        virtual void                            setState(const RbBitSet &s);                        //!< Set the discrete observation

//        virtual unsigned long                   getState(void) const = 0;                           //!< Get the discrete observation

        // Discrete character observation functions
        virtual std::string                     getStateLabels(void) const = 0;                     //!< Get valid state labels
//      virtual void                            setState(size_t pos, bool val) = 0;                 //!< Set the discrete observation
        virtual const std::vector<double>       getWeights() const ;                                //!< Get the weights of the state
        bool                                    isWeighted() const ;                                //!< Is the state weighted?
        void                                    setWeighted(bool wd) ;                              //!< Set whether the state is weighted


    protected:
                                                DiscreteCharacterState(size_t n);                   //!< Constructor


        size_t                                  index_single_state;
        size_t                                  num_observed_states;
        RbBitSet                                state;
        bool                                    weighted;

    };

}

#endif
