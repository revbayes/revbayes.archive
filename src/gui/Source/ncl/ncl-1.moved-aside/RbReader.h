#ifndef RbReader_H
#define RbReader_H

#include "ncl.h"


class RbReader : public NxsReader {

	public:
		ifstream inf;
		ofstream outf;

		RbReader(char *infname, char *outfname) : NxsReader()
			{
			inf.open(infname, ios::binary);
			//outf.open(outfname);
			}

		~RbReader(void)
			{
			inf.close();
			//outf.close();
			}

	void ExecuteStarting(void) {}
	void ExecuteStopping(void) {}

	bool EnteringBlock(NxsString blockName)
		{
		cout << "Reading \"" << blockName << "\" block..." << endl;
		outf << "Reading \"" << blockName << "\" block..." << endl;

		// Returning true means it is ok to delete any data associated with 
		// blocks of this type read in previously
		//
		return true;	
		}

	void SkippingBlock(NxsString blockName)
		{
		cout << "Skipping unknown block (" << blockName << ")..." << endl;
		outf << "Skipping unknown block (" << blockName << ")..." << endl;
		}

	void SkippingDisabledBlock(NxsString blockName) {}

	void OutputComment(const NxsString &msg)
		{
		outf << msg;
		}

	void NexusError(NxsString msg, file_pos pos, unsigned line, unsigned col)
		{
		cerr << endl;
		cerr << "Error found at line " << line;
		cerr << ", column " << col;
		cerr << " (file position " << pos << "):" << endl;
		cerr << msg << endl;

		outf << endl;
		outf << "Error found at line " << line;
		outf << ", column " << col;
		outf << " (file position " << pos << "):" << endl;
		outf << msg << endl;

		exit(0);
		}
	};

class RbToken : public NxsToken {

	public:

		RbToken(istream &is, ostream &os) : out(cout), NxsToken(is) {}

		void OutputComment(const NxsString &msg)
			{
			cout << msg << endl;
			out << msg << endl;
			}

	private:
		ostream &out;
};
	
#endif
