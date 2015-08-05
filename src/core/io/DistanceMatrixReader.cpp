#include "DistanceMatrixReader.h"
#include "RbFileManager.h"

#include "RbConstants.h"


using namespace RevBayesCore;


DistanceMatrixReader::DistanceMatrixReader(const std::string &fn, char d, size_t numSkipped) : DelimitedDataReader(fn, d, numSkipped)
{
	
	filename = fn;
	
	
	
	//First, get the size of the matrix
	int siz = atoi( chars[0][0].c_str() );
	matrix = MatrixReal(siz);

	for (size_t i = 1; i < chars.size(); ++i)
	{
		
		std::string name = chars[i][0];
		
		names.push_back( name );
		
		for (size_t j = 1; j < chars[i].size(); ++j)
		{
			matrix[i][j] = atof (chars[i][j].c_str());
		}
		
	}
}


const std::vector<std::string>& DistanceMatrixReader::getNames(void)
{
	return names;
}


const MatrixReal& DistanceMatrixReader::getMatrix(void)
{
	return matrix;
}
