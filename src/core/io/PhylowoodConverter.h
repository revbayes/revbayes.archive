//
//  PhylowoodConverter.h
//  revbayes-proj
//
//  Created by Michael Landis on 3/2/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#ifndef __revbayes_proj__PhylowoodConverter__
#define __revbayes_proj__PhylowoodConverter__

namespace RevBayesCore {
    
    class DelimitedDataReader;
    class RbFileManager;
    class TimeAtlas;
    class TimeTree;
    class PhylowoodConverter {
        
    public:
        PhylowoodConverter(const std::string &sfn, const std::string &tfn, const std::string &gfn, const std::string &pfn, double b, const std::string& ct);
        ~PhylowoodConverter(void);
        void                                        convert(void);
        
    private:
        std::string                                 buildCharacterHistoryString(TopologyNode* n, unsigned end);
        std::string                                 buildExtendedNewick(TopologyNode* n);
        std::string                                 buildPhylowoodString(void);
        void                                        makeMarginalAreaProbs(void);
        void                                        formatError(RbFileManager& fm, std::string& errorStr);
        void                                        makeBits(void);
        void                                        test(void);

        TimeTree* tree;
        TimeAtlas* atlas;
        DelimitedDataReader* dat;
        
        std::vector<std::vector<unsigned> > bits;
        std::vector<std::vector<double> > marginalStartProbs;
        std::vector<std::vector<double> > marginalEndProbs;
        
        std::string stateFilename;
        std::string treeFilename;
        std::string geoFilename;
        std::string phwFilename;
        std::string chartype;
    
        double burn;
        size_t numNodes;
        size_t numStates;
        size_t numCharacters;
        size_t numEpochs;
    };
};



#endif /* defined(__revbayes_proj__PhylowoodConverter__) */
