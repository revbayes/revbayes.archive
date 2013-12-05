//
//  DelimitedDataReader.h
//  rb_mlandis
//
//  Created by Michael Landis on 12/2/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__DelimitedDataReader__
#define __rb_mlandis__DelimitedDataReader__

#include <vector>
#include <string>

namespace RevBayesCore {
    
    class DelimitedDataReader {
        
    public:
        
        DelimitedDataReader(std::string fn, char d='\t');
        void readData(void);
        std::vector<std::vector<std::string> > getChars(void);
        
    protected:
        std::string filename;
        char delimiter;
        std::vector<std::vector<std::string> > chars;
        
    };
    
}
#endif /* defined(__rb_mlandis__DelimitedDataReader__) */
