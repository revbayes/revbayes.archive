/**
 * @file
 * This file contains the declaration of GeneticCode from Bio++.
 *
 * @brief Declaration of Bpp GeneticCode
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: mardi 8 juillet 2014, à 15h $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: BppGeneticCode.h 2014 Gueguen $
 */

#ifndef BppGeneticCode_H
#define BppGeneticCode_H

#include "Cloneable.h"

#include <Bpp/Seq/GeneticCode/GeneticCode.h>

namespace RevBayesCore {
    
    class BppGeneticCode : public Cloneable {
    private:
      const bpp::GeneticCode* pGeneticCode_;

    public:
      
      /**
       * @brief Takes ownership of a bpp GeneticCode*.
       *
       */
      
      BppGeneticCode(const bpp::GeneticCode* gc);

      BppGeneticCode(const BppGeneticCode& m);
      //!< Copy constructor

      virtual ~BppGeneticCode(void);
      //!< Destructor
        
      // overloaded operators
      BppGeneticCode& operator=(const BppGeneticCode& r);

      BppGeneticCode* clone(void) const
      {
        return new BppGeneticCode(*this);
      }

      const bpp::GeneticCode* getGeneticCode() const
      {
        return pGeneticCode_;
      }
      
        
    };
    
  // Global functions using the class
  std::ostream&                       operator<<(std::ostream& o, const BppGeneticCode& x);                                         //!< Overloaded output operator
 
}

#endif


