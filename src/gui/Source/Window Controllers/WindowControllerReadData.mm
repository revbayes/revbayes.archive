#include <map>
#include <vector>
#include <string>
#include "Character.h"
//#include "CharacterContinuous.h"
//#include "CharacterDiscrete.h"
#include "NclReader.h"
#include "RbFileManager.h"

#import "CharacterData.h"
#import "RbData.h"
#import "RbDataCell.h"
#import "ToolReadData.h"
#import "WindowControllerReadData.h"



@implementation WindowControllerReadData

@synthesize dataAlignment;
@synthesize dataFormat;
@synthesize dataInterleaved;
@synthesize dataType;
@synthesize dataTypeSimulated;
@synthesize numberOfOutlets;
@synthesize numberOfCharacters;
@synthesize numberOfTaxa;
@synthesize numberOfStates;
@synthesize fileName;
@synthesize pathName;

- (void)addBlankDataMatrix {

    // allocate the matrix
    RbData* m = [[RbData alloc] init];
    
    // set the dimensions
    [m setNumTaxa:[self numberOfTaxa]];
    [m setNumCharacters:[self numberOfCharacters]];
    
    // set the name
    [m setName:@"Blank Data Matrix"];
    
    // set the data type
    if ( [[dataTypeButton2 titleOfSelectedItem] isEqualToString:@"DNA"] == YES )
        [m setDataType:DNA];
    else if ( [[dataTypeButton2 titleOfSelectedItem] isEqualToString:@"RNA"] == YES )
        [m setDataType:RNA];
    else if ( [[dataTypeButton2 titleOfSelectedItem] isEqualToString:@"Protein"] == YES )
        [m setDataType:AA];
    else if ( [[dataTypeButton2 titleOfSelectedItem] isEqualToString:@"Standard"] == YES )
        [m setDataType:STANDARD];
    else if ( [[dataTypeButton2 titleOfSelectedItem] isEqualToString:@"Continuous"] == YES )
        [m setDataType:CONTINUOUS];
        
    // set a flag if the data is discrete or continuous
    BOOL isDiscrete = YES;
    if ( [[dataTypeButton2 titleOfSelectedItem] isEqualToString:@"Continuous"] == YES )
        isDiscrete = NO;
        
    // fill in the matrix with missing data entries
    for (int i=0; i<[m numTaxa]; i++)
        {
        NSString* tn = [NSString stringWithFormat:@"Taxon_%d", i+1];
        [m addTaxonName:tn];
        for (int j=0; j<[m numCharacters]; j++)
            {
            RbDataCell* cell = [[RbDataCell alloc] init];
            if ( isDiscrete == YES )
                {
                [cell setIsDiscrete:YES];
                [cell setDataType:[m dataType]];
                if ( [m dataType] == DNA || [m dataType] == RNA )
                    {
                    [cell setNumStates:4];
                    [cell setVal:[NSNumber numberWithUnsignedInt:[self missingForNumStates:4]]];
                    }
                else if ( [m dataType] == AA )
                    {
                    [cell setNumStates:20];
                    [cell setVal:[NSNumber numberWithUnsignedInt:[self missingForNumStates:20]]];
                   }
                else if ( [m dataType] == STANDARD )
                    {
                    [cell setNumStates:2];
                    [cell setVal:[NSNumber numberWithUnsignedInt:[self missingForNumStates:2]]];
                    }
                
                    
                }
            else 
                {
                NSNumber* n = [NSNumber numberWithDouble:0.0];
                [cell setVal:n];
                [cell setIsDiscrete:NO];
                [cell setDataType:CONTINUOUS];
                [cell setNumStates:0];
                }
            [cell setRow:i];
            [cell setColumn:j];
            [m addCell:cell];
            [cell release];
            }
        }
        
    // add the matrix to the tool
    [myTool addMatrix:m];
}

- (void)awakeFromNib {
	
	[matrixTypeTab selectTabViewItemAtIndex:[myTool matrixType]];
	[self setControlsEnabledState];
	[self setControlWindowSize];
}

- (IBAction)cancelAction:(id)sender {
	
	[self setToolValues];
	[myTool closeControlPanel];
}

- (IBAction)changeBlankDataType:(id)sender {

}

- (IBAction)changeDataType:(id)sender {

}

- (IBAction)changeFileFormat:(id)sender {

	[self setControlsEnabledState];
}

- (void)dealloc {

	[fileName release];
	[pathName release];
	[super dealloc];
}

- (IBAction)helpButtonAction:(id)sender {

}

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolReadData*)t {

	if ( (self = [super initWithWindowNibName:@"ControlWindowReadData"]) )
        {
		// allocate objects
		fileName = [[NSString alloc] initWithString:@""];
		pathName = [[NSString alloc] initWithString:@""];

		// initialize the address of the tool associated with this control window
        myTool = t;
		
		// initialize the data
		[self setDataAlignment:[myTool dataAlignment]];
        [self setDataFormat:[myTool dataFormat]];
		[self setDataType:[myTool dataType]];
		[self setDataTypeSimulated:[myTool dataTypeSimulated]];
		[self setDataInterleaved:[myTool dataInterleaved]];
		[self setNumberOfTaxa:[myTool numberOfTaxa]];
		[self setNumberOfCharacters:[myTool numberOfCharacters]];
        [self setNumberOfStates:2];
		[self setNumberOfOutlets:[myTool numberOfOutlets]];
		[self setFileName:[myTool fileName]];
		[self setPathName:[myTool pathName]];
        }
	return self;
}

- (unsigned)missingForNumStates:(int)n {

	unsigned val = 0;
	for (int i=0; i<n; i++)
		{
        unsigned mask = 1 << i ;
        val |= mask;
		}
	return val;
}

- (IBAction)okButtonAction:(id)sender {

    // set the tool state to unresolved
    [myTool setIsResolved:NO];
    
	// remember the state of the control panel
	[self setToolValues];

	// perform the action
	NSString* tabViewLabel = [NSString stringWithString:[[matrixTypeTab selectedTabViewItem] label]];
	if ( [tabViewLabel isEqualToString:@"Data Matrix"] == YES )
        {
		// user selected "OK" for a data matrix (to be read into computer memory)
        
		[myTool removeAllDataMatrices];
		BOOL isSuccessful = [self readDataFile];
		
		if (isSuccessful == YES)
			{
			[myTool closeControlPanel];
			[myTool updateForConnectionChange];
            [myTool setIsResolved:YES];
			}
		else 
			{
			}
		}
	else 
		{
		// user selected "OK" for a blank data matrix
		[myTool removeAllDataMatrices];
        [self addBlankDataMatrix];
		[myTool closeControlPanel];
		[myTool updateForConnectionChange];
        [myTool setIsResolved:YES];
		}
}

- (BOOL)readDataFile {

    // make an array containing the valid file types that can be chosen
	NSArray* fileTypes = [NSArray arrayWithObjects: @"nex", @"phy", @"fasta", @"fas", @"in", NSFileTypeForHFSTypeCode( 'TEXT' ), nil];
    
    // get the open panel
    NSOpenPanel* oPanel = [NSOpenPanel openPanel];
    [oPanel setAllowsMultipleSelection:NO];
    [oPanel setCanChooseDirectories:YES];

    // open the panel
#   if 1
    NSString* fileToOpen;
    [oPanel setAllowedFileTypes:fileTypes];
    int result = (int)[oPanel runModal];
    if ( result == NSFileHandlingPanelOKButton )
        {
        NSArray* filesToOpen = [oPanel URLs];
        int count = (int)[filesToOpen count];
        for (int i=0; i<count; i++) 
            {
            NSLog(@"%d -- %@\n", i, [[filesToOpen objectAtIndex:i] path]);
            fileToOpen = [[filesToOpen objectAtIndex:i] path];
            }
        }
    
#   else
    NSString* fileToOpen;
    int result = (int)[oPanel runModalForDirectory:nil file:nil types:fileTypes];
    if ( result == NSOKButton ) 
        {
        NSArray* filesToOpen = [oPanel filenames];
        int count = (int)[filesToOpen count];
        for (int i=0; i<count; i++) 
            {
            NSLog(@"%d -- %@\n", i, [filesToOpen objectAtIndex:i]);
            fileToOpen = [filesToOpen objectAtIndex:i];
            }
        }
#   endif
        
	// check to see if the selection is a file or a directory
    NSFileManager* fileManager = [NSFileManager defaultManager];
	BOOL isDir;
	[fileManager fileExistsAtPath:fileToOpen isDirectory:&isDir];
	
	// set the information
	if (isDir == NO)
        {
		NSString *lastComponent = [fileToOpen lastPathComponent];
		NSString *pathLessFilename = [fileToOpen stringByDeletingLastPathComponent];
		[self setFileName:lastComponent];
		[self setPathName:pathLessFilename];
        }
	else 
        {
		[self setFileName:@""];
		[self setPathName:fileToOpen];
        }
    
	// set the information in the tool
	[myTool setFileName:fileName];
	[myTool setPathName:pathName];
		
    NSLog(@"fileToOpen = %@", fileToOpen);
    NSLog(@"isDir = %d", isDir);
    
	// Get the file format information from the ReadData tool panel. Note that if the file format is NEXUS,
    // that we don't need the format/datatype information because the NCL parser can figure that out.
	std::string localFormat = "", localDataType = "";
	bool localIsInterleaved = true;
	if ( [[dataFormatButton titleOfSelectedItem] isEqualToString:@"NEXUS"] == YES ) 
		localFormat = "nexus";
	else if ( [[dataFormatButton titleOfSelectedItem] isEqualToString:@"PHYLIP"] == YES )
		localFormat = "phylip";
	else if ( [[dataFormatButton titleOfSelectedItem] isEqualToString:@"FASTA"] == YES )
		localFormat = "fasta";
	if ( [[dataTypeButton1 titleOfSelectedItem] isEqualToString:@"DNA"] == YES )
		localDataType = "dna";
	else if ( [[dataTypeButton1 titleOfSelectedItem] isEqualToString:@"RNA"] == YES )
		localDataType = "rna";
	else if ( [[dataTypeButton1 titleOfSelectedItem] isEqualToString:@"Protein"] == YES )
		localDataType = "protein";
	else if ( [[dataTypeButton1 titleOfSelectedItem] isEqualToString:@"Standard"] == YES )
		localDataType = "standard";
	if ( [[interleavedFormatButton selectedCell] tag] == 0 )
		localIsInterleaved = false;
    
    std::cerr << "localFormat = " << localFormat << std::endl;
    std::cerr << "localDataType = " << localDataType << std::endl;

    // ^
    // |
    // Objective-C++
    //
    int strLen = (int)[fileToOpen length];
    char* cStr = new char[strLen+10];
    [fileToOpen getCString:cStr maxLength:(strLen+10)  encoding:NSUTF8StringEncoding];
    std::string fn = cStr;
    delete [] cStr;
    //
    // C++
    // |
    // v

    // Get an instance of RbFileManager. From here on out, we use the RevBayes C++ classes to read the
    // directory contents, etc.
    RbFileManager myFileManager( fn );

    // are we reading a single file or are we reading the contents of a directory?
    bool readingDirectory = myFileManager.isDirectory(fn);

    // set up a vector of strings containing the name or names of the files to be read
    std::vector<std::string> vectorOfFileNames;
    if (readingDirectory == true)
        myFileManager.setStringWithNamesOfFilesInDirectory(vectorOfFileNames);
    else 
        vectorOfFileNames.push_back( myFileManager.getFilePath() + "/" + myFileManager.getFileName() );
    if (readingDirectory == true)
        {
        //std::stringstream o1;
        //o1 << "Found " << vectorOfFileNames.size() << " files in directory";
        //RBOUT(o1.str());
        }
    
std::cerr << "readingDirectory = " << readingDirectory << std::endl;
    // get the global instance of the NCL reader and clear warnings from its warnings buffer
    NclReader& reader = NclReader::getInstance();
    reader.clearWarnings();
            
    // Set up a map with the file name to be read as the key and the file type as the value. Note that we may not
    // read all of the files in the string called "vectorOfFileNames" because some of them may not be in a format
    // that can be read.
    std::map<std::string,std::string> fileMap;
    for (std::vector<std::string>::iterator p = vectorOfFileNames.begin(); p != vectorOfFileNames.end(); p++)
        {
        std::cerr << "Checking file " << (*p) << std::endl;
        bool isInterleaved = false;
        std::string myFileType = "unknown", dType = "unknown";
        if (reader.isNexusFile(*p) == true)
            myFileType = "nexus";
        else if (reader.isPhylipFile(*p, dType, isInterleaved) == true)
            myFileType = "phylip";
        else if (reader.isFastaFile(*p, dType) == true)
            myFileType = "fasta";
            
        if (myFileType != "unknown")
            {
            std::string suffix = "|" + dType;
            if ( myFileType == "phylip" )
                {
                if (isInterleaved == true)
                    suffix += "|interleaved";
                else
                    suffix += "|noninterleaved";
                }
            else if ( myFileType == "fasta" )
                suffix += "|noninterleaved";
            else
                suffix += "|unknown";
            myFileType += suffix;
            fileMap.insert( std::make_pair(*p,myFileType) );
            }
        else
            {
            reader.addWarning("Unknown file type");
            }
        std::cerr << "Finished checking file " << (*p) << std::endl;
        }
#   if 1
    std::cout << "File map (" << fileMap.size() << ")" << std::endl;
    for (std::map<std::string,std::string>::iterator it = fileMap.begin(); it != fileMap.end(); it++)
        std::cout << it->first << " -- " << it->second << std::endl;
#   endif
                
    // read the files in the map containing the file names with the output being a vector of pointers to
    // the character matrices that have been read
    std::vector<RbPtr<CharacterData> > myData = reader.readMatrices( fileMap );
    
    // print summary of results of file reading to the user
    if (readingDirectory == true)
        {

        }
    else
        {

        }

	// we have successfully read the data into computer memory
	// add the matrices to the tool
	for (std::vector<RbPtr<CharacterData> >::iterator p = myData.begin(); p != myData.end(); p++)
		{
		//(*p)->print();
		std::string fn = (*p)->getFileName();
std::cerr << "Adding matrix " << fn << std::endl;
		NSString* nsfn = [NSString stringWithCString:(fn.c_str()) encoding:NSUTF8StringEncoding];
		RbData* m = [[RbData alloc] init];
		[m setNumTaxa:(int)((*p)->getNumberOfTaxa())];
		[m setNumCharacters:(int)((*p)->getNumberOfCharacters())];
		[m setName:nsfn];
        if ( (*p)->getDataType() == "DNA" )
            [m setDataType:DNA];
        else if ( (*p)->getDataType() == "RNA" )
            [m setDataType:RNA];
        else if ( (*p)->getDataType() == "Amino Acid" )
            [m setDataType:AA];
        else if ( (*p)->getDataType() == "Standard" )
            [m setDataType:STANDARD];
        else if ( (*p)->getDataType() == "Continuous" )
            [m setDataType:CONTINUOUS];

		for (int i=0; i<(*p)->getNumberOfTaxa(); i++)
			{
			NSString* taxonName = [NSString stringWithCString:(*p)->getTaxonNameWithIndex(i).c_str() encoding:NSUTF8StringEncoding];
			[m addTaxonName:taxonName];
			for (int j=0; j<(*p)->getNumberOfCharacters(); j++)
				{
                RbPtr<const Character> matrixCell = (*p)->getCharacter(i, j);
				RbDataCell* cell = [[RbDataCell alloc] init];
                bool isDiscrete = true;
                if ( matrixCell->getNumberOfStates() == 0 )
                    isDiscrete = false;
				if ( isDiscrete == true )
					{
					unsigned x = matrixCell->getUnsignedValue();
					NSNumber* n = [NSNumber numberWithUnsignedInt:x];
					[cell setVal:n];
					[cell setIsDiscrete:YES];
                    [cell setDataType:[m dataType]];
					[cell setNumStates:(int)(matrixCell->getNumberOfStates())];
					if ( matrixCell->isMissingOrAmbiguous() == true )
						[cell setIsAmbig:YES];
					}
				else 
					{
					double x = matrixCell->getRealValue();
					NSNumber* n = [NSNumber numberWithDouble:x];
					[cell setVal:n];
					[cell setIsDiscrete:NO];
					[cell setDataType:CONTINUOUS];
					[cell setNumStates:0];
					}
				[cell setRow:i];
				[cell setColumn:j];
				//[cell setVal:n];
				[m addCell:cell];
				[cell release];
				}
			}
			
		//[m print];
		[myTool addMatrix:m];
		
		//(*p)->print();
		}




#   if 0
	
	// read the file(s)
	std::vector<CharacterMatrix*>* myData;
	if (isDir == YES)
		{
		// read the contents of a directory
		
		// make a list of all of the files in the directory
		NSString* dirToOpen = [NSString stringWithString:pathName];
		dirToOpen = [dirToOpen stringByAppendingString:@"/"];
		NSError* error = nil;
		NSArray* tempFiles = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:dirToOpen error:&error];
		if (error != nil)
			return NO;
						
		// initialize the list of files
		std::vector<std::string> fileNames;
		id element;
		NSEnumerator* fileEnum = [tempFiles objectEnumerator];
		while ( (element = [fileEnum nextObject]) )
			{
			if ( [element characterAtIndex:0] != '.' )
				{
				NSString* fn = [NSString stringWithString:dirToOpen];
				fn = [fn stringByAppendingString:element];
				
				BOOL dirQuery;
				[[NSFileManager defaultManager] fileExistsAtPath:fn isDirectory:&dirQuery];
				if ( dirQuery == NO )
					{
					int strLen = (int)[fn length];
					char* cStr = new char[strLen+10];
					[fn getCString:cStr maxLength:(strLen+10)  encoding:NSUTF8StringEncoding];
					fileNames.push_back( cStr );
					delete [] cStr;
					}
				}
			}
		
		// instantiate an NCL reader object
		NclReader& myReader = NclReader::getInstance();
		
		// read the file(s)
		try 
			{
			myData = myReader.readData(fileNames, localFormat, localDataType, localIsInterleaved);
			if (myData == NULL)
				throw NxsException("help");
			}
		catch (NxsException& x) 
			{
			NxsString m = x.msg;
			NSString* errFile = [NSString stringWithString:pathName];
			NSAlert* alert = [NSAlert alertWithMessageText:@"Error Reading Directory Contents" 
			                                 defaultButton:@"OK" 
										   alternateButton:nil 
										       otherButton:nil 
								 informativeTextWithFormat:@"Could not read directory \"%@\". Check the settings in the control window to make certain that the file and data types are set correctly. Also, check that the data is formatted correctly in the files.", errFile];
			[alert beginSheetModalForWindow:[self window] modalDelegate:self didEndSelector:nil contextInfo:NULL];
			return NO;
			}
		myReader.clearContent();
		}
	else 
		{
		// read a single file

		// get the file path/name to the file
		NSString* fileToOpen = [NSString stringWithString:pathName];
		fileToOpen = [fileToOpen stringByAppendingString:@"/"];
		fileToOpen = [fileToOpen stringByAppendingString:fileName];
				
		// open and read the data file using the RbReader object (that inherits from an NCL class)
		int strLen = (int)[fileToOpen length];
		char* inFile = new char[strLen+10];
		[fileToOpen getCString:inFile maxLength:(strLen+10)  encoding:NSUTF8StringEncoding];

		// instantiate an NCL reader object
		NclReader& myReader = NclReader::getInstance();
				
		// read the file
		try 
			{
			myData = myReader.readData(inFile, localFormat, localDataType, localIsInterleaved);
			if (myData == NULL)
				throw NxsException("help");
			}
		catch (NxsException& x)
			{
			NxsString m = x.msg;
			NSString* errFile = [NSString stringWithString:fileName];
			NSAlert* alert = [NSAlert alertWithMessageText:@"Error Reading Data File" 
			                                 defaultButton:@"OK" 
										   alternateButton:nil 
										       otherButton:nil 
								 informativeTextWithFormat:@"Could not read file \"%@\". Check the settings in the control window to make certain that the file and data types are set correctly. Also, check that the data is formatted correctly in the file.", errFile];
			[alert beginSheetModalForWindow:[self window] modalDelegate:self didEndSelector:nil contextInfo:NULL];
			return NO;
			}
			
		// free memory 
		delete [] inFile;
		myReader.clearContent();
		}
		
	// we have successfully read the data into computer memory
	// add the matrix to the tool
	for (std::vector<CharacterMatrix*>::iterator p = myData->begin(); p != myData->end(); p++)
		{
		//(*p)->print();
		std::string fn = (*p)->getName();
		NSString* nsfn = [NSString stringWithCString:(fn.c_str()) encoding:NSUTF8StringEncoding];
		RbData* m = [[RbData alloc] init];
		[m setNumTaxa:((*p)->getNumTaxa())];
		[m setNumCharacters:((*p)->getNumCharacters())];
		[m setName:nsfn];
        if ( (*p)->getDataType() == "dna" )
            [m setDataType:DNA];
        else if ( (*p)->getDataType() == "rna" )
            [m setDataType:RNA];
        else if ( (*p)->getDataType() == "amino acid" )
            [m setDataType:AA];
        else if ( (*p)->getDataType() == "standard" )
            [m setDataType:STANDARD];
        else if ( (*p)->getDataType() == "continuous" )
            [m setDataType:CONTINUOUS];

		for (int i=0; i<(*p)->getNumTaxa(); i++)
			{
			NSString* taxonName = [NSString stringWithCString:(*p)->getTaxonIndexed(i).c_str() encoding:NSUTF8StringEncoding];
			[m addTaxonName:taxonName];
			for (int j=0; j<(*p)->getNumCharacters(); j++)
				{
				RbDataCell* cell = [[RbDataCell alloc] init];
				if ((*p)->getIsDiscrete() == true)
					{
					unsigned x = (*p)->getUnsignedValue(i, j);
					NSNumber* n = [NSNumber numberWithUnsignedInt:x];
					[cell setVal:n];
					[cell setIsDiscrete:YES];
					if ( (*p)->getDataType() == "dna" )
						[cell setDataType:DNA];
					else if ( (*p)->getDataType() == "rna" )
						[cell setDataType:RNA];
					else if ( (*p)->getDataType() == "amino acid" )
						[cell setDataType:AA];
					else if ( (*p)->getDataType() == "standard" )
						[cell setDataType:STANDARD];
					[cell setNumStates:((*p)->getNumStates())];
					if ( (*p)->getIsAmbig(i, j) == true )
						[cell setIsAmbig:YES];
					}
				else 
					{
					double x = (*p)->getDoubleValue(i, j);
					NSNumber* n = [NSNumber numberWithDouble:x];
					[cell setVal:n];
					[cell setIsDiscrete:NO];
					[cell setDataType:CONTINUOUS];
					[cell setNumStates:0];
					}
				[cell setRow:i];
				[cell setColumn:j];
				//[cell setVal:n];
				[m addCell:cell];
				[cell release];
				}
			}
			
		//[m print];
		[myTool addMatrix:m];
		
		//(*p)->print();
		}

#   endif

	return YES;
}

- (void)setControlWindowSize {

	NSRect oldFrame = [[self window] frame];
    NSRect newFrame = oldFrame;

	NSString* tabViewLabel = [NSString stringWithString:[[matrixTypeTab selectedTabViewItem] label]];
	if ( [tabViewLabel isEqualToString:@"Data Matrix"] == YES )
		{
        newFrame.size.height = 280.0;
        newFrame.origin.y += (oldFrame.size.height - newFrame.size.height);
        [[self window] setFrame:newFrame display:YES animate:YES];
		}
	else if ( [tabViewLabel isEqualToString:@"Blank Matrix"] == YES )
		{
        newFrame.size.height = 253.0;
        newFrame.origin.y += (oldFrame.size.height - newFrame.size.height);
        [[self window] setFrame:newFrame display:YES animate:YES];
		}
}

- (void)setControlsEnabledState {

    [dataAlignmentButton setEnabled:NO];
    [dataAlignmentField setTextColor:[NSColor grayColor]];
	
	NSString* tabViewLabel = [NSString stringWithString:[[matrixTypeTab selectedTabViewItem] label]];
	if ( [tabViewLabel isEqualToString:@"Data Matrix"] == YES )
		{
		if ( [[dataFormatButton titleOfSelectedItem] isEqualToString:@"NEXUS"] == YES)
			{
			[dataTypeButton1 setEnabled:NO];
            [interleavedFormatButton setEnabled:NO];
			[dataTypeField setTextColor:[NSColor grayColor]];
			[interleavedFormatField setTextColor:[NSColor grayColor]];
			}
		else if ( [[dataFormatButton titleOfSelectedItem] isEqualToString:@"FASTA"] == YES)
			{
			[dataTypeButton1 setEnabled:YES];
            [interleavedFormatButton setEnabled:NO];
			[dataTypeField setTextColor:[NSColor blackColor]];
			[interleavedFormatField setTextColor:[NSColor grayColor]];
			}
		else if ( [[dataFormatButton titleOfSelectedItem] isEqualToString:@"Unknown"] == YES)
			{
			[dataTypeButton1 setEnabled:NO];
            [interleavedFormatButton setEnabled:NO];
			[dataTypeField setTextColor:[NSColor grayColor]];
			[interleavedFormatField setTextColor:[NSColor grayColor]];
			}
		else 
			{
			[dataTypeButton1 setEnabled:YES];
			[interleavedFormatButton setEnabled:YES];
			[dataTypeField setTextColor:[NSColor blackColor]];
			[interleavedFormatField setTextColor:[NSColor blackColor]];
			}
		}
}

- (void)setToolValues {

	[myTool setDataAlignment:[self dataAlignment]];
	[myTool setDataFormat:[self dataFormat]];
	[myTool setDataType:[self dataType]];
	[myTool setDataTypeSimulated:[self dataTypeSimulated]];
	[myTool setDataInterleaved:[self dataInterleaved]];
	[myTool setNumberOfTaxa:[self numberOfTaxa]];
	[myTool setNumberOfCharacters:[self numberOfCharacters]];
	[myTool setNumberOfOutlets:[self numberOfOutlets]];
	[myTool setFileName:[self fileName]];
	[myTool setPathName:[self pathName]];
	[myTool setMatrixType:(int)[matrixTypeTab indexOfTabViewItem:[matrixTypeTab selectedTabViewItem]]];
}

- (void)tabView:(NSTabView*)tabView didSelectTabViewItem:(NSTabViewItem*)tabViewItem {

	NSString* tabViewLabel = [NSString stringWithString:[tabViewItem label]];
	if ( [tabViewLabel isEqualToString:@"Data Matrix"] == YES )
        {
        [okButton setTitle:(@"Import")];
        }
	else if ( [tabViewLabel isEqualToString:@"Blank Matrix"] == YES )
        {
        [okButton setTitle:(@"OK")];
        }
	[self setControlWindowSize];
}

- (BOOL)validateMenuItem:(NSMenuItem*)item {
    
	if ( [[item title] isEqualToString:@"Standard"] == YES )
		{
		if ( [[dataFormatButton titleOfSelectedItem] isEqualToString:@"FASTA"] == YES )
			return NO;
		}
	return YES;
}

- (void)windowDidLoad {

}

@end
