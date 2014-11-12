//
//  RealSymmetricMatrix.h
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-27.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef __revbayes__RealSymmetricMatrix__
#define __revbayes__RealSymmetricMatrix__

#include <iostream>



#include <iostream>
#include <vector>

#include "ModelObject.h"
#include "MatrixRealSymmetric.h"
#include "TypedDagNode.h"


namespace RevLanguage {
    
    class RealSymmetricMatrix : public ModelObject<RevBayesCore::MatrixRealSymmetric>  {
        
    public:
        
        RealSymmetricMatrix(void);
        RealSymmetricMatrix(RevBayesCore::TypedDagNode<RevBayesCore::MatrixRealSymmetric> *mat);                                                            //!< Construct from DAG node
        
        
        // the value type definition
        virtual RealSymmetricMatrix*    clone(void) const;                                                      //!< Clone object
        virtual RevObject*              convertTo(const TypeSpec& type) const;                                  //!< Convert to type
        static const std::string&       getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&          getClassTypeSpec(void);                                                 //!< Get class type spec
        virtual const TypeSpec&         getTypeSpec(void) const;                                                //!< Get language type of the object
        void                            printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
    };

}

#endif /* defined(__revbayes__RealSymmetricMatrix__) */
