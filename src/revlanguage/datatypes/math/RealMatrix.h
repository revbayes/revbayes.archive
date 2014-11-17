//
//  RealMatrix.h
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-27.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef __revbayes__RealMatrix__
#define __revbayes__RealMatrix__

#include <iostream>



#include <iostream>
#include <vector>

#include "MatrixReal.h"
#include "ModelObject.h"
#include "TypedDagNode.h"


namespace RevLanguage {
    
    class RealMatrix : public ModelObject<RevBayesCore::MatrixReal>  {
        
    public:
        
        RealMatrix(void);                                                                                       //!< Default constructor
        RealMatrix(RevBayesCore::TypedDagNode<RevBayesCore::MatrixReal> *mat);                                  //!< Construct from DAG node
        RealMatrix(const RealMatrix& from);                                                                     //!< Copy constructor
        
        
        // the value type definition
        virtual RealMatrix*             clone(void) const;                                                      //!< Clone object
        static const std::string&       getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&          getClassTypeSpec(void);                                                 //!< Get class type spec
        virtual const TypeSpec&         getTypeSpec(void) const;                                                //!< Get language type of the object
        void                            printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
    };

}

#endif /* defined(__revbayes__RealMatrix__) */
