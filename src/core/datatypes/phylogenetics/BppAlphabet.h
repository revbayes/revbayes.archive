/**
 * @file
 * This file contains the declaration of Alphabet from Bio++.
 *
 * @brief Declaration of Bpp Alphabet
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: mardi 8 juillet 2014, à 15h $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: BppAlphabet.h 2014 Gueguen $
 */

#ifndef BppAlphabet_H
#define BppAlphabet_H

#include "Cloneable.h"

#include <Bpp/Seq/Alphabet/Alphabet.h>

namespace RevBayesCore {
    
    class BppAlphabet : public Cloneable {
    private:
      const bpp::Alphabet* pAlphabet_;

    public:
      
      /**
       * @brief Takes ownership of a bpp Alphabet*.
       *
       */
      
      BppAlphabet(const bpp::Alphabet* alphabet);

      BppAlphabet(const BppAlphabet& m);
      //!< Copy constructor

      virtual ~BppAlphabet(void);
      //!< Destructor
        
      // overloaded operators
      BppAlphabet& operator=(const BppAlphabet& r);

      BppAlphabet* clone(void) const
      {
        return new BppAlphabet(*this);
      }

      const bpp::Alphabet* getAlphabet() const
      {
        return pAlphabet_;
      }
      
        
    };
    
  // Global functions using the class
  std::ostream&                       operator<<(std::ostream& o, const BppAlphabet& x);                                         //!< Overloaded output operator
 
}

#endif


