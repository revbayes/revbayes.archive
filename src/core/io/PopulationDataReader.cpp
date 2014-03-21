//
//  PopulationDataReader.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 12/7/12.
//  Copyright (c) 2012 Michael Landis. All rights reserved.
//

#include "MicrosatData.h"
#include "PopulationDataReader.h"
#include "RbFileManager.h"
#include "SnpData.h"

//@Michael: Needed to make Windows compiler happy.
#include <stdlib.h>
#include <iostream>

using namespace RevBayesCore;

PopulationDataReader::PopulationDataReader()
{
    
    ;//  snps = readSnpData("");
}

SnpData* PopulationDataReader::readSnpData2(const std::string& fileName)
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
    sd->setNumChromosomes(counts);
    sd->setSnpFrequencies(freqs);
    
    return sd;
}

SnpData* PopulationDataReader::readSnpData(const std::string& fileName, int thinBy)
{
    
    // Expected file format
    //
    //  nameA    nameB  nameC
    //  fA1,fa1 fB1,fb1 fC1,fc1
    //  fA2,fa2 fB2,fb2 fC2,fc2
    //  ...
    
    // initialize
    SnpData* sd = new SnpData();
    std::vector<std::string> names;
    std::vector<bool> outgroup;
    std::vector<std::vector<int> > numAlleles;
    std::vector<std::vector<int> > numSamples;
    std::vector<std::vector<double > > freqs;
    std::vector<int> tmpNumAlleles;
    std::vector<int> tmpNumSamples;
    std::vector<double> tmpFreqs;
    std::vector<int> numChromosomes;
    
    // open file
    std::ifstream readStream;
    RbFileManager* f = new RbFileManager(fileName);
    if (!f->openFile(readStream))
        std::cout << "ERROR: Could not open file " << fileName << "\n";
    
    // read file
    bool firstLine = true;
    int lineNum = 0;
    int popIdx = 0;
    int numPopulations = 0;
    std::string readLine = "";
    while (std::getline(readStream,readLine))
    {
                
        //int ni = 0;
        std::string field;
        std::stringstream ss(readLine);
        
        //std::cout << lineNum << "\n";
        //std::cout << readLine << "\n";
        
        // read the line
        while (ss >> field)
        {
            if (firstLine)
            {
                // if indicated as an outgroup taxon
                if (*field.rbegin() == '*')
                {
                    outgroup.push_back(true);
                    names.push_back(field.substr(0,field.size()-1));
                }
                // otherwise, it's an ingroup taxon
                else
                {
                    outgroup.push_back(false);
                    names.push_back(field);
                }

            }
            else if (lineNum % thinBy == 0)
            {
                size_t idx = field.find(",");
                int a = atoi(field.substr(0,idx).c_str());
                int b = atoi(field.substr(idx+1,-1).c_str());

                // quick hack, should instead set missing data equal to sample mean in BMAG
                // or... simply drop the col if any elt has a+b==0
                
                /*
                if (a == 0 && b == 0)
                {
                    a = 1;
                    b = 1;
                }
                */
                
                if (a+b == 0) { a=1;b=1; }
                
                //double sum = a + b;
                if (a+b > numChromosomes[popIdx])
                    numChromosomes[popIdx] = a+b;
                //double f = 0.0;
                //if (sum > 0)
                double f = 0.0;
                if (a + b > 0)
                    f = double(a) / double(a + b);
//                double f = (double)a / (double)(a+b);
                

                //std::cout << f << " = " << a << " / " << a + b << "\t" << idx << "\n";
                
                tmpNumAlleles.push_back(a);
                tmpNumSamples.push_back(a+b);
                tmpFreqs.push_back(f);
                popIdx++;
            }
            
        };
        
        // after reading the line
        if (firstLine)
        {
            numPopulations = (int)names.size();
            //std::cout << "numPop " << numPopulations << "\n";
            numAlleles.resize(numPopulations);
            numSamples.resize(numPopulations);
            freqs.resize(numPopulations);
            numChromosomes = std::vector<int>(numPopulations, 0);
        }
        else if (lineNum % thinBy == 0)
        {
            // prepare for next iteration
            
            
            for (size_t i = 0; i < names.size(); i++)
            {
                numAlleles[i].push_back(tmpNumAlleles[i]);
                numSamples[i].push_back(tmpNumSamples[i]);
                freqs[i].push_back(tmpFreqs[i]);
            }
            
            //numAlleles.push_back(tmpNumAlleles);
            //numSamples.push_back(tmpNumSamples);
            //freqs.push_back(tmpFreqs);
                        
            tmpNumAlleles.clear();
            tmpNumSamples.clear();
            tmpFreqs.clear();
            popIdx = 0;
        }
        
        lineNum += 1;
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
    sd->setNumChromosomes(numChromosomes);
    sd->setOutgroup(outgroup);
    
    return sd;
}

#if 0

MicrosatData* PopulationDataReader::readMicrosatData(const std::string& fileName, int thinBy)
{
    
    // Expected file format
    //
    //  nameA    nameB  nameC
    //  fA1,fa1 fB1,fb1 fC1,fc1
    //  fA2,fa2 fB2,fb2 fC2,fc2
    //  ...
    
    // initialize
    MicrosatData* md = new MicrosatData();
    std::vector<std::string> names;
    std::vector<bool> outgroup;
    std::vector<std::vector<int> > numSamples;
    std::vector<std::vector<double > > means;
    std::vector<std::vector<double > > vars;
    std::vector<int> tmpNumAlleles;
    std::vector<int> tmpNumSamples;
    std::vector<double> tmpMeans;
    std::vector<double> tmpVars;
    std::vector<int> numChromosomes;
    
    // open file
    std::ifstream readStream;
    RbFileManager* f = new RbFileManager(fileName);
    if (!f->openFile(readStream))
        std::cout << "ERROR: Could not open file " << fileName << "\n";
    
    // read file
    bool firstLine = true;
    int lineNum = 0;
    int popIdx = 0;
    int numPopulations = 0;
    std::string readLine = "";
    while (std::getline(readStream,readLine))
    {
        
        //int ni = 0;
        std::string field;
        std::stringstream ss(readLine);
        
        //std::cout << lineNum << "\n";
        //std::cout << readLine << "\n";
        
        // read the line
        while (ss >> field)
        {
            if (firstLine)
            {
                // if indicated as an outgroup taxon
                if (*field.rbegin() == '*')
                {
                    outgroup.push_back(true);
                    names.push_back(field.substr(0,field.size()-1));
                }
                // otherwise, it's an ingroup taxon
                else
                {
                    outgroup.push_back(false);
                    names.push_back(field);
                }
                
            }
            else if (lineNum % thinBy == 0)
            {
                size_t idx = field.find(",");
                int a = atoi(field.substr(0,idx).c_str());
                int b = atoi(field.substr(idx+1,-1).c_str());
                
                // quick hack, should instead set missing data equal to sample mean in BMAG
                // or... simply drop the col if any elt has a+b==0
                
                /*
                 if (a == 0 && b == 0)
                 {
                 a = 1;
                 b = 1;
                 }
                 */
                
                if (a+b == 0) { a=1;b=1; }
                
                //double sum = a + b;
                if (a+b > numChromosomes[popIdx])
                    numChromosomes[popIdx] = a+b;
                //double f = 0.0;
                //if (sum > 0)
                double f = 0.0;
                if (a + b > 0)
                    f = double(a) / double(a + b);
                //                double f = (double)a / (double)(a+b);
                
                
                //std::cout << f << " = " << a << " / " << a + b << "\t" << idx << "\n";
                
                tmpNumAlleles.push_back(a);
                tmpNumSamples.push_back(a+b);
                tmpFreqs.push_back(f);
                popIdx++;
            }
            
        };
        
        // after reading the line
        if (firstLine)
        {
            numPopulations = (int)names.size();
            //std::cout << "numPop " << numPopulations << "\n";
            numAlleles.resize(numPopulations);
            numSamples.resize(numPopulations);
            freqs.resize(numPopulations);
            numChromosomes = std::vector<int>(numPopulations, 0);
        }
        else if (lineNum % thinBy == 0)
        {
            // prepare for next iteration
            
            
            for (size_t i = 0; i < names.size(); i++)
            {
                numAlleles[i].push_back(tmpNumAlleles[i]);
                numSamples[i].push_back(tmpNumSamples[i]);
                freqs[i].push_back(tmpFreqs[i]);
            }
            
            //numAlleles.push_back(tmpNumAlleles);
            //numSamples.push_back(tmpNumSamples);
            //freqs.push_back(tmpFreqs);
            
            tmpNumAlleles.clear();
            tmpNumSamples.clear();
            tmpFreqs.clear();
            popIdx = 0;
        }
        
        lineNum += 1;
        firstLine = false;
        
    }
    
    // set SnpData
    int numMicrosats = (int)numSamples[0].size();
    md->setNumMicrosats(numMicrosats);
    md->setPopulationNames(names);
    md->setNumPopulations(numPopulations);
    md->setNumSamples(numSamples);
    md->setNumChromosomes(numChromosomes);
    md->setOutgroup(outgroup);
    
    return md;
}

#endif
