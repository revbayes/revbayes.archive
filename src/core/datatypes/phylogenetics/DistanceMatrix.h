//
//  DistanceMatrix.h
//
//  Created by Bastien Boussau on 4/8/15.
//  Copyright (c) 2015 Bastien Boussau. All rights reserved.
//

#ifndef __DistanceMatrix__
#define __DistanceMatrix__

#include "Cloneable.h"
#include "DistanceMatrixReader.h"
#include <string>

namespace RevBayesCore {
    class DistanceMatrix : public Cloneable
    {
        
    public:
		DistanceMatrix( );
        DistanceMatrix(DistanceMatrixReader* tadr);
        DistanceMatrix(const DistanceMatrix& a);
		DistanceMatrix(const MatrixReal& a, std::vector<std::string>& nam);
        DistanceMatrix&                                 operator=(const DistanceMatrix& a);
        virtual DistanceMatrix*                         clone(void) const;
        
		std::vector<std::string>                        getNames(void) const;
        MatrixReal      								getMatrix(void) const;
		unsigned 										getSize(void) const;
        std::string                                     getFilename(void) const;
        std::string                                     getDatatype(void) const;  
		RbVector<double>&                       		operator[](size_t index);
		const RbVector<double>&                 		operator[](size_t index) const;
		const double& 									getElement( size_t i, size_t j ) const;
		size_t 											size(void) const;
    protected:
        MatrixReal								        matrix;
		std::vector<std::string>                        names;
        
    private:
        unsigned                                        numTips;
        std::string                                     filename;
        
    };
    
    std::ostream&                                       operator<<(std::ostream& o, const DistanceMatrix& x);
}


#endif /* defined(__DistanceMatrix__) */
