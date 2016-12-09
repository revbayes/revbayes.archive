#include "DiscreteTaxonData.h"
#include "PomoCountFileReader.h"
#include "RbFileManager.h"

#include "RbConstants.h"

#include <string>


using namespace RevBayesCore;


PomoCountFileReader::PomoCountFileReader(const std::string &fn, const size_t virtualPopulationSize, char d, size_t ns) : DelimitedDataReader(fn, d, ns), virtualPopulationSize_ ( virtualPopulationSize )
{
	filename = fn;
	HomologousDiscreteCharacterData<PomoState>* matrix_ = new HomologousDiscreteCharacterData<PomoState> ();

	// chars is a matrix containing all the lines of the file fn.
	// First line, with the names of the columns:
	// First line should be like: COUNTSFILE  NPOP 5   NSITES N

	int start = -1;
	// Skip comments.
	do {
			start = start + 1;
	}
	while (chars[start][0] == "#");


	if (chars[start][0] != "COUNTSFILE" || chars[0].size() != 5) {
		throw RbException( "File "+fn+" is not a proper Pomo Counts file: first line is not correct, it should be similar to \nCOUNTSFILE NPOP 5 NSITES N\n.");
	}
	else {
        numberOfPopulations_ = StringUtilities::asIntegerNumber( chars[0][2] );
        numberOfSites_ = StringUtilities::asIntegerNumber( chars[0][4] );
	}
	size_t numberOfFields = 2 + numberOfPopulations_;
	// The second line should look like this:
	//CHROM  POS  Sheep    BlackSheep  RedSheep  Wolf     RedWolf
	if (chars[start+1][0] != "CHROM" || chars[1][1] != "POS" || chars[1].size() != numberOfFields) {
		throw RbException( "File "+fn+" is not a proper Pomo Counts file: second line is not correct, it should be similar to \nCHROM POS Sheep BlackSheep RedSheep Wolf RedWolf\n.");
	}
	else {
		for (size_t i = start+2; i < 2 + numberOfPopulations_; ++i ) {
			names_.push_back(chars[1][i]);
		}
	}

	// Setting the taxon names in the data matrix
	std::map<std::string, DiscreteTaxonData<PomoState> > nameToTaxonData;
	for (size_t i = 0; i < names_.size(); ++i ) {
		DiscreteTaxonData<PomoState> tax (names_[i]);
		nameToTaxonData.insert(std::pair< std::string, DiscreteTaxonData<PomoState> >(names_[i], tax) );
	}


	// Reading the other lines to fill up the taxon data
	//std::vector<std::string> chromosomes (numberOfSites_, "") ;
	//std::vector<size_t> positions (numberOfSites_, 0) ;
	//std::vector<std::string> sites (numberOfSites_, "") ;
	//std::map<std::string, std::vector<std::string> > speciesToSites;
	// for (size_t j = 0; j < numberOfPopulations_; ++j)
	// {
	// 	speciesToSites[names_[j]] = sites;
	// }

	for (size_t i = 2; i < chars.size(); ++i)
	{
		if (chars[i].size() != numberOfFields) {
			throw RbException( "File "+fn+" is not a proper Pomo Counts file: line "+ i + " is not correct, it does not have "+ numberOfFields + " space-separated fields.");
		}

		//chromosomes.push_back(chars[i][0]);
    //positions.push_back( StringUtilities::asIntegerNumber( chars[i][1] ) );

		std::string chromosome = chars[i][0];
		size_t position = StringUtilities::asIntegerNumber( chars[i][1] );

		for (size_t j = 2; j < numberOfPopulations_; ++j)
		{
			PomoState pState (chars[i][j], chromosome, position, virtualPopulationSize_ );
			nameToTaxonData.at(names_[j]).addCharacter( pState);
		}
	}
	// We have finished all lines, we fill up the data matrix
	for (std::map<std::string, DiscreteTaxonData<PomoState> >::iterator tax = nameToTaxonData.begin(); tax != nameToTaxonData.end(); ++tax ) {
	 	matrix_->addTaxonData(tax->second);
	}
	return ;
}

const size_t PomoCountFileReader::getNumberOfPopulations( void ){
	return numberOfPopulations_;
}

const size_t PomoCountFileReader::getNumberOfSites( void ){
	return numberOfSites_;
}

HomologousDiscreteCharacterData<PomoState>* PomoCountFileReader::getMatrix( void ){
	return matrix_;
}

const size_t PomoCountFileReader::getVirtualPopulationSize( void ){
	return virtualPopulationSize_;
}
