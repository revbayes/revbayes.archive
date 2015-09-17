#include "DistanceMatrixReader.h"
#include "RbFileManager.h"

#include "RbConstants.h"


using namespace RevBayesCore;


DistanceMatrixReader::DistanceMatrixReader(const std::string &fn, char d, size_t numSkipped) : DelimitedDataReader(fn, d, numSkipped)
{
	
	filename = fn;
	
	//First, get the size of the matrix
	int siz = chars.size() -1;//atoi( chars[0][0].c_str() );

	std::cout << "chars[0][0].c_str(): " << chars[0][0].c_str() << std::endl;

	
	std::cout << "SIZ: " << siz << std::endl;
	matrix = MatrixReal( siz );

	std::cout << "matSIZ: " << matrix.size()<< " ; " << matrix.getNumberOfColumns() << std::endl;
	std::cout << "chars.size() : " << chars.size() <<std::endl;
	for (size_t i = 1; i < chars.size(); ++i)
	{
		std::cout << "i: " << i << std::endl;
		std::string name = chars[i][0];
		
		names.push_back( name );
		std::cout << "chars[i].size(): " << chars[i].size() << std::endl;
		for (size_t j = 1; j < chars[i].size(); ++j)
		{
			std::cout << "i: " << i << " ; j: " << j << std::endl;
			std::cout << "i: " << i << " ; j: " << j << " ; " << atof(chars[i][j].c_str() ) <<std::endl;
			std::cout << "mat: i: " << i << " ; j: " << j << " ; " << matrix[i-1][j-1]<<std::endl;
			matrix[i-1][j-1] = atof( chars[i][j].c_str() );
			//std::cout << "i: " << i << " ; j: " << j << " ; " << atof (chars[i-1][j-1].c_str()) <<std::endl;
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
