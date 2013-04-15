/**
 * @file
 * This file contains the declaration of NewickConvert, which is our simple class to
 * convert trees written in Newick format into our own tree format.
 *
 * @brief Declaration of NclReader
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-07-12 11:08:07 +0200 (Thu, 12 Jul 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id: NclReader.h 1674 2012-07-12 09:08:07Z hoehna $
 */

#ifndef FastaWriter_H
#define FastaWriter_H

#include "AbstractCharacterData.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

namespace RevBayesCore {
    
    class FastaWriter {
        
    public:
        FastaWriter();
        void writeData(const std::string& fn, const AbstractCharacterData &d);
        
        
    };
    
}


#endif /* defined(__revbayes_mlandis__PopulationDataReader__) */
