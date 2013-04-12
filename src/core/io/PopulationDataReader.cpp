//
//  PopulationDataReader.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 12/7/12.
//  Copyright (c) 2012 Michael Landis. All rights reserved.
//

#include "PopulationDataReader.h"
#include "RbFileManager.h"
#include "SnpData.h"

//@Michael: Needed to make Windows compiler happy.
#include <stdlib.h>

using namespace RevBayesCore;

PopulationDataReader::PopulationDataReader()
{
    
    ;//  snps = readSnpData("");
}

SnpData* PopulationDataReader::readSnpData(const std::string& fileName)
{
    
    // Expected file format
    //
    // pop_name\tind_count\tmajor_allele_freqs
    // Martian\t48\t42\t41\47\n
    // Jovian\t90\t46\t80\t88\n
    
    // initialize
    SnpData* sd = new SnpData();
    std::vector<std::string> names;
    std::vector<int> counts;
    std::vector<std::vector<double> > freqs;
    std::vector<double> tmpFreqs;;

    // open file
    std::ifstream readStream;
    RbFileManager* f = new RbFileManager(fileName);
    if (!f->openFile(readStream))
        std::cout << "ERROR: Could not open file " << fileName << "\n";
    
    // read file
    bool firstLine = true;
    int numPopulations = 0;
    std::string readLine = "";
    while (std::getline(readStream,readLine))
    {
        
        // skip header
        if (firstLine)
        {
            firstLine = false;
            continue;
        }
        
        int pos = 0;
        std::string field;
        std::stringstream ss(readLine);
        while (ss >> field)
        {
            if (pos == 0)
                names.push_back(field);
            else if (pos == 1)
                counts.push_back(atoi(field.c_str()));
            else if (pos > 1)
                tmpFreqs.push_back(atof(field.c_str()) / counts[numPopulations]);
            pos++;
        };
        
        // prepare for next iteration
        numPopulations++;
        freqs.push_back(tmpFreqs);
        tmpFreqs.clear();
    }
    

    // set SnpData
    sd->setNumPopulations(numPopulations);
    sd->setNumSnps((int)freqs[0].size());
    sd->setPopulationNames(names);
    sd->setNumIndividuals(counts);
    sd->setSnpFrequencies(freqs);
    
    return sd;
}

SnpData* PopulationDataReader::readSnpData2(const std::string& fileName)
{
    
    // Expected file format
    //
    //  nameA    nameB  nameC
    //  fA1,nA1 fB1,nB1 fC1,nC1
    //  fA2,nA2 fB2,nB2 fC2,nC2
    //  ...
    
    // initialize
    SnpData* sd = new SnpData();
    std::vector<std::string> names;
    std::vector<std::vector<int> > numAlleles;
    std::vector<std::vector<int> > numSamples;
    std::vector<std::vector<double > > freqs;
    std::vector<int> tmpNumAlleles;
    std::vector<int> tmpNumSamples;
    std::vector<double> tmpFreqs;
    std::vector<int> numIndividuals;
    
    // open file
    std::ifstream readStream;
    RbFileManager* f = new RbFileManager(fileName);
    if (!f->openFile(readStream))
        std::cout << "ERROR: Could not open file " << fileName << "\n";
    
    // read file
    bool firstLine = true;
    int popIdx = 0;
    int numPopulations = 0;
    std::string readLine = "";
    while (std::getline(readStream,readLine))
    {
        int ni = 0;
        std::string field;
        std::stringstream ss(readLine);
        
        // read the line
        while (ss >> field)
        {
            if (firstLine)
                names.push_back(field);

            else
            {
                unsigned idx = field.find(",");
                int a = atoi(field.substr(0,idx).c_str());
                int b = atoi(field.substr(idx+1,-1).c_str());
                //std::cout << a << " " << b << "\t" << idx << "\n";
                if (b > numIndividuals[popIdx])
                    numIndividuals[popIdx] = b;
                double f = (double)a / (double)b;
                
                tmpNumAlleles.push_back(a);
                tmpNumSamples.push_back(b);
                tmpFreqs.push_back(f);
                popIdx++;
            }
            
        };
        
        // after reading the line
        if (firstLine)
        {
            numPopulations = (int)names.size();
            numAlleles.resize(numPopulations);
            numSamples.resize(numPopulations);
            freqs.resize(numPopulations);
            numIndividuals = std::vector<int>(numPopulations, 0);
        }
        else
        {
            // prepare for next iteration
            for (size_t i = 0; i < names.size(); i++)
            {
                numAlleles[i].push_back(tmpNumAlleles[i]);
                numSamples[i].push_back(tmpNumSamples[i]);
                freqs[i].push_back(tmpFreqs[i]);
            }
            numAlleles.push_back(tmpNumAlleles);
            numSamples.push_back(tmpNumSamples);
            freqs.push_back(tmpFreqs);
            
            tmpNumAlleles.clear();
            tmpNumSamples.clear();
            tmpFreqs.clear();
            popIdx = 0;
        }
        
        firstLine = false;

    }
    
    // set SnpData
    int numSnps = (int)numSamples[0].size();
    sd->setNumSnps(numSnps);
    sd->setPopulationNames(names);
    sd->setNumPopulations(numPopulations);
    sd->setNumAlleles(numAlleles);
    sd->setNumSamples(numSamples);
    sd->setSnpFrequencies(freqs);
    sd->setNumIndividuals(numIndividuals);
    
    /*
    for (size_t i = 0; i < numPopulations; i++)
    {
        std::cout << names[i] << "(" << numIndividuals[i] << ")\t";
        for (size_t j = 0; j < numSnps; j++)
        {
            std::cout << numAlleles[i][j] << "," << numSamples[i][j] << "\t";
        }
        std::cout << "\n";
    }*/
    
    return sd;
}

