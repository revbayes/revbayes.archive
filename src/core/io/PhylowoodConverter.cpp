//
//  PhylowoodConverter.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 3/2/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#include "DelimitedDataReader.h"
#include "NclReader.h"
#include "NewickTreeReader.h"
#include "PhylowoodConverter.h"
#include "RbFileManager.h"
#include "TimeTree.h"
#include "TimeAtlas.h"
#include "TimeAtlasDataReader.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace RevBayesCore;

RevBayesCore::PhylowoodConverter::PhylowoodConverter(const std::string &sfn, const std::string &tfn, const std::string &gfn, const std::string &pfn, double b) :
    stateFilename(sfn),
    treeFilename(tfn),
    geoFilename(gfn),
    phwFilename(pfn),
    burn(b)
{
    
    convert();
}

PhylowoodConverter::~PhylowoodConverter(void)
{
    delete atlas;
    delete dat;
}

void PhylowoodConverter::convert(void) {
    
    // get atlas info
    TimeAtlasDataReader* tadr = new TimeAtlasDataReader(geoFilename);
    atlas = new TimeAtlas(tadr);
    numStates = pow(2, atlas->getNumAreas());
    numCharacters = atlas->getNumAreas();
    numEpochs = atlas->getNumEpochs();
    
    // get tree info
    NclReader reader = NclReader();
    std::vector<TimeTree> trees;
    std::vector<TimeTree*> tmp = reader.readTimeTrees( treeFilename );
    if (tmp.size() > 0)
        tree = tmp[0];
    numNodes = tree->getNumberOfNodes();
    
    // get state info
    dat = new DelimitedDataReader(stateFilename);
    makeBits();
    makeMarginalAreaProbs();
    
    std::string s = buildPhylowoodString();

    // open the stream to the file
    std::fstream outStream;
    outStream.open( phwFilename.c_str(), std::fstream::out );
    outStream << s;
    outStream.close();

}


/** Format the error exception string for problems specifying the file/path name */
void PhylowoodConverter::formatError(RevBayesCore::RbFileManager& fm, std::string& errorStr) {
    
    bool fileNameProvided    = fm.isFileNamePresent();
    bool isFileNameGood      = fm.testFile();
    bool isDirectoryNameGood = fm.testDirectory();
    
    if ( fileNameProvided == false && isDirectoryNameGood == false )
        errorStr += "Could not read contents of directory \"" + fm.getFilePath() + "\" because the directory does not exist";
    else if (fileNameProvided == true && (isFileNameGood == false || isDirectoryNameGood == false)) {
        errorStr += "Could not read file named \"" + fm.getFileName() + "\" in directory named \"" + fm.getFilePath() + "\" ";
        if (isFileNameGood == false && isDirectoryNameGood == true)
            errorStr += "because the file does not exist";
        else if (isFileNameGood == true && isDirectoryNameGood == false)
            errorStr += "because the directory does not exist";
        else
            errorStr += "because neither the directory nor the file exist";
    }
}



std::string PhylowoodConverter::buildPhylowoodString(void)
{
    std::stringstream nhxStrm;
    
    // begin nexus file
    nhxStrm << "#NEXUS" << "\n\n";
    
    // phylowood settings block
    nhxStrm << "Begin phylowood;\n";
    nhxStrm << "\tdrawtype pie\n";
    nhxStrm << "\tmodeltype biogeography\n";
    nhxStrm << "\tareatype discrete\n";
    nhxStrm << "\tmaptype clean\n";
    nhxStrm << "\tpieslicestyle even\n";
    nhxStrm << "\tpiefillstyle outwards\n";
    nhxStrm << "\ttimestart -" << tree->getRoot().getAge() << "\n";
    nhxStrm << "\tmarkerradius " << 200 << "\n";
    nhxStrm << "\tminareaval " << 0.1 << "\n";
    nhxStrm << "End;\n\n";
    
    // bayarea-fig block
    nhxStrm << "Begin bayarea-fig;\n";
    nhxStrm << "\tmapheight\t100\n";
    nhxStrm << "\tmapwidth\t150\n";
    nhxStrm << "\tcanvasheight\t2000\n";
    nhxStrm << "\tcanvaswidth\t1000\n";
    nhxStrm << "\tminareaval\t0.15\n";
    nhxStrm << "\tareacolors black\n";
    nhxStrm << "\tareatypes";
    for (unsigned i = 0; i < numCharacters; i++)
        nhxStrm << " 1";
    nhxStrm << "\n";
    nhxStrm << "\tareanames Default\n";
    nhxStrm << "End;\n\n";
    
    // taxa block
    nhxStrm << "Begin taxa;\n";
    nhxStrm << "\tDimensions ntax=" << tree->getNumberOfTips() << ";\n";
    nhxStrm << "\tTaxlabels\n";
    for (unsigned i = 0; i < tree->getNumberOfNodes(); i++)
    {
        TopologyNode* p = &tree->getNode(i);
        if (p->isTip())
        {
            nhxStrm << "\t\t" << p->getName() << "\n";
        }
    }
    nhxStrm << "\t\t;\n";
    nhxStrm << "End;\n\n";
    
    // geo block
    nhxStrm << "Begin geo;\n";
    nhxStrm << "\tDimensions ngeo=" << numCharacters << ";\n";
    nhxStrm << "\tCoords\n";
    
    for (unsigned i = 0; i < numCharacters; i++)
    {
        double lat = atlas->getAreas()[numEpochs-1][i]->getLatitude();
        double lon = atlas->getAreas()[numEpochs-1][i]->getLongitude();
        nhxStrm << "\t\t" << i << "\t" << lat << "\t" << lon;
        if (i != 0)
            nhxStrm << ",";
        nhxStrm << "\n";

    }
    nhxStrm << "\t\t;\n";
    nhxStrm << "End;\n\n";
    
    // tree block
    nhxStrm << "Begin trees;\n";
    nhxStrm << "\tTranslate\n";
    for (unsigned i = 0; i < tree->getNumberOfNodes(); i++)
    {
        TopologyNode* p = &tree->getNode(i);
        if (p->isTip())
        {
            nhxStrm << "\t\t" << p->getIndex() << "\t" << p->getName();
            if (i < (tree->getNumberOfNodes() - 1))
                nhxStrm << ",";
            nhxStrm << "\n";
        }
    }
    nhxStrm << "\t\t;\n";
    
    // write tree string
    std::string treeStr = "";
    treeStr = buildExtendedNewick(&tree->getRoot());
//    std::cout << treeStr << "\n";
//    std::cout << "nhxStr\n" << treeStr << "\n";
    nhxStrm << "tree TREE1 = " << treeStr << ";\n";
    nhxStrm << "End;\n";
    
//    std::cout << "[";
//    for (size_t i = 0; i < numCharacters; i++)
//    {
//        if (i != 0)
//            std::cout << ",";
//        std::cout << (double)childCharacterCounts[0][i] / numSamples;
//    }
//    std::cout << "]\n";
    
    return nhxStrm.str();
}


void PhylowoodConverter::makeMarginalAreaProbs(void) {

    // read data
    std::vector<std::vector<std::string> > stateChars = dat->getChars();
    size_t numSamples = stateChars.size() - 1;

    // get state counts/node
    marginalStartProbs.resize(numNodes);
    marginalEndProbs.resize(numNodes);
    for (size_t i = 0; i < numNodes; i++)
    {
        marginalStartProbs[i].resize(numCharacters, 0.0);
        marginalEndProbs[i].resize(numCharacters, 0.0);
    }

    // get data
    int skip = 1;
    std::vector<std::vector<unsigned> > idx;
    for (size_t i = 0; i < stateChars.size(); i++)
    {
        for (size_t j = 0; j < stateChars[i].size(); j++)
        {
            
            if (j < skip)
            {
                idx.push_back(std::vector<unsigned>(2,-1));
                continue;
            }
            
            if (i == 0)
            {
                std::vector<unsigned> tmp(2);
                const std::string& s = stateChars[0][j];
                std::string tok;
                std::stringstream ss(s.c_str());

                // branch end
                std::getline(ss, tok, '_');
                tmp[0] = (tok == "start") ? 0 : 1;
                
                // node
                std::getline(ss, tok, '_');
                tmp[1] = std::atoi(tok.c_str());
                
                idx.push_back(tmp);
            }
            else
            {
                // get node, state, and bits for i,j cell in tab-delimited file
                size_t nodeIdx = idx[j][1];
                unsigned stateIdx = std::atoi(stateChars[i][j].c_str());
                const std::vector<unsigned>& b = bits[stateIdx];
                
                // start
                if (idx[j][0] == 0) {
                    for (size_t k = 0; k < b.size(); k++)
                    {
                        marginalStartProbs[nodeIdx][k] += b[k];
                    }
                }
                // end
                else if (idx[j][0] == 1) {
                    for (size_t k = 0; k < b.size(); k++)
                    {
                        marginalEndProbs[nodeIdx][k] += b[k];
                    }
                }
            }
        }
    }
    
    // normalize
    for (size_t i = 0; i < marginalEndProbs.size(); i++)
    {
        for (size_t j = 0; j < marginalEndProbs[i].size(); j++)
        {
            marginalStartProbs[i][j] = (double)(marginalStartProbs[i][j]) / numSamples;
            marginalEndProbs[i][j]   = (double)(marginalEndProbs[i][j])   / numSamples;
        }
    }
}

void PhylowoodConverter::makeBits(void)
{
    size_t numStates = (int)(pow(2,numCharacters));
    bits = std::vector<std::vector<unsigned> >(numStates, std::vector<unsigned>(numCharacters, 0));
    for (size_t i = 1; i < numStates; i++)
    {
        size_t n = i;
        for (size_t j = 0; j < numCharacters; j++)
        {
            bits[i][j] = n % 2;
            n /= 2;
            if (n == 0)
                break;
        }
    }
}

std::string PhylowoodConverter::buildCharacterHistoryString(TopologyNode* n, unsigned end)
{
    std::stringstream ss;
    
    const size_t& nodeIdx = n->getIndex();
    const std::vector<double>& characters = ( end == 0 ? marginalStartProbs[nodeIdx] : marginalEndProbs[nodeIdx] );
    for (size_t i = 0; i < characters.size(); i++)
    {
        if (i != 0)
            ss << ",";
        ss << characters[i];
    }
    
    return ss.str();
}


/* Build newick string */
std::string PhylowoodConverter::buildExtendedNewick( TopologyNode* n ) {
    // create the newick string
    std::stringstream o;
    
    // extended data is only found on admixture nodes
    std::string additionalInfo = "";
    
    // loop over admixture nodes per branch
    std::stringstream characterStream;
    
    double br = 1.0;
    
    characterStream << "[";
    
    // character history
    characterStream << "&ch={" << buildCharacterHistoryString(n, 1) << "}";
    characterStream << ",&pa={" << buildCharacterHistoryString(n, 0) << "}";
    
    // ... whatever else
    characterStream << "]";
    
    additionalInfo = characterStream.str();
    
    // test whether this is a internal or external node
    if (n->isTip()) {
        // this is a tip so we just return the name of the node
        o << n->getIndex() << additionalInfo << ":" << n->getBranchLength();
    }
    
    else {
        o << "(";
        for (size_t i=0; i<(n->getNumberOfChildren()-1); i++) {
            o << buildExtendedNewick( &n->getChild(i) ) << ",";
        }
        o << buildExtendedNewick( &n->getChild(n->getNumberOfChildren()-1) ) << ")" << additionalInfo << ":" << n->getBranchLength() * br;
    }
    
    return o.str();
}