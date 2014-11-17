///**
// * @file
// * This file contains the declaration of ContinuousCharacterState, which is
// * the class for the continuous data types in RevBayes.
// * 
// * Currently, continuous characters can neither be gaps nor be missing or ambiguous.
// *
// * @brief Declaration of ContinuousCharacterState
// *
// * (c) Copyright 2009-
// * @date Last modified: $Date: 2012-05-24 09:58:04 +0200 (Thu, 24 May 2012) $
// * @author The RevBayes Development Core Team
// * @license GPL version 3
// *
// * $Id: ContinuousCharacterState.h 1568 2012-05-24 07:58:04Z hoehna $
// */
//
//#ifndef ContinuousCharacterState_H
//#define ContinuousCharacterState_H
//
//#include "CharacterState.h"
//#include <ostream>
//#include <vector>
//
//namespace RevBayesCore {
//
//    class ContinuousCharacterState : public CharacterState {
//    
//    public:
//                                        ContinuousCharacterState(void);                                 //!< Default constructor
//                                        ContinuousCharacterState(const ContinuousCharacterState& s);    //!< Copy constructor
//                                        ContinuousCharacterState(const double x);                       //!< Constructor with mean value
//                                        ContinuousCharacterState(const double x, const double v);       //!< Constructor with mean and variance values
//
//        bool                            operator==(const CharacterState& x) const;                      //!< Equality
//        bool                            operator!=(const CharacterState& x) const;                      //!< Inequality
//        bool                            operator<(const CharacterState& d) const;                       //!< Less than
// 
//        ContinuousCharacterState*       clone(void) const;                                              //!< Get a copy of this object
//
//        // character observation functions
//        std::string                     getDatatype(void) const;                                        //!< Get the datatype as a common string.
//        double                          getMean(void) const;                                            //!< Get the continuous observation
//        size_t                          getNumberOfStates(void) const;                                  //!< Get the number states for a character (return 0 in base class)
//        std::string                     getStringValue(void) const;                                     //!< Get a representation of the character as a string
//        double                          getVariance(void) const;                                        //!< Get the continuous observation
//        bool                            isAmbiguous(void) const;                                        //!< Is the character missing or ambiguous
//        bool                            isGapState(void) const;                                         //!< Get whether this is a gapped character state
//        void                            setGapState(bool tf);                                           //!< Set whether this is a gapped character
//        void                            setMean(double x);                                              //!< Set the continuous observation
//        void                            setVariance(const double x);                                    //!< Set the continuous observation
//    
//    private:
//        double                          mean;                                                           //!< Mean value (or just the value)
//        double                          variance;                                                       //!< Variance (most likely 0)
//    
//    };
//    
//}
//
//#endif
//
