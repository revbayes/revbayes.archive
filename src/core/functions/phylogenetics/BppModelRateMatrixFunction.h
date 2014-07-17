/**
 * @file
 * This file contains the declaration of the GTR rate matrix function class.
 * This class is derived from the function class and is used to
 * compute the rate matrix of a general time reversible (GTR) Markov chain.
 *
 * @brief Declaration of the GTR rate matrix function.
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author Laurent Guéguen jeudi 3 juillet 2014, à 21h 45
 * @license GPL version 3
 * @version 1.0
 * @since 2012-07-06, version 1.0
 * @interface Function
 *
 * $Id$
 */



#ifndef BppModelRateMatrixFunction_H
#define BppModelRateMatrixFunction_H

#include "BppRateMatrix.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <Bpp/Phyl/Model/SubstitutionModel.h>

#include <vector>

namespace RevBayesCore {
    
    class BppModelRateMatrixFunction : public TypedFunction<RateMatrix> {
        
    public:
      BppModelRateMatrixFunction(std::map<std::string, const TypedDagNode<double > *>& sTDN, bpp::SubstitutionModel* pSM);
      BppModelRateMatrixFunction(const BppModelRateMatrixFunction &n);  //!< Copy constructor
      virtual ~BppModelRateMatrixFunction(void);                   //!< Virtual destructor
        
        // public member functions
      BppModelRateMatrixFunction* clone(void) const;               //!< Create an independent clone

      void update(void);

      void addParameter(const DagNode* p);

      void removeParameter(const DagNode* p);
      
    protected:
      void swapParameterInternal(const DagNode *oldP, const DagNode *newP); //!< Implementation of swaping parameters
        
    private:
        
      // members map from names to DagNode and the reverse
      
      std::map<std::string, const TypedDagNode< double >* >  _mN2D;
      std::map<const TypedDagNode< double >*,  std::string>  _mD2N;
    };
    
}

#endif
