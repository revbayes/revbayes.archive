#ifndef RbReader_H
#define RbReader_H

#include <string>
#include <vector>
#include "ncl.h"
#include "nxsmultiformat.h"

class CharacterMatrix;
class Tree;

class RbReader{ //: public MultiFormatReader {

	friend class NxsBlock;
	
	public:
		static RbReader&                 getInstance(void);
						                 RbReader(void);
		virtual			                ~RbReader(void);
		std::vector<CharacterMatrix*>*   convertFromNcl(std::vector<std::string>& fnv);
		bool                             fileExists(const char *fn) const;
        std::vector<CharacterMatrix*>*   readData(const std::vector<std::string> fn, const std::string fileFormat, const std::string dataType, const bool isInterleaved);
		std::vector<CharacterMatrix*>*   readData(const char* fileName, const std::string fileFormat, const std::string dataType, const bool isInterleaved);
		
		// TAH: stuff for reading trees
		std::vector<Tree*>*   readTree(const std::string fn, const std::string fileFormat);
		std::vector<Tree*>*   readTree(const char* fileName, const std::string fileFormat);
		std::vector<Tree*>*   convertTreeFromNcl();
		Tree*   translateNclSimpleTreeToTree(NxsSimpleTree &nTree);
		
		void				clearContent(){ nexusReader.ClearContent(); }

	private:
		MultiFormatReader				 nexusReader;
        CharacterMatrix*                 createAminoAcidMatrix(NxsCharactersBlock* charblock);
        CharacterMatrix*                 createContinuousMatrix(NxsCharactersBlock* charblock);
		CharacterMatrix*                 createNucleotideMatrix(NxsCharactersBlock* charblock);
        CharacterMatrix*                 createStandardMatrix(NxsCharactersBlock* charblock);
};

#endif

