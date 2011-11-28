#include <map>
#include <vector>
#include <string>
#include "Character.h"
#include "CharacterData.h"
#include "DagNodeContainer.h"
#include "NclReader.h"
#include "Parser.h"
#include "RbFileManager.h"
#include "VariableSlot.h"
#include "Workspace.h"

#import "CharacterData.h"
#import "RbData.h"
#import "RbDataCell.h"
#import "RbTaxonData.h"
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
        RbTaxonData* td = [[RbTaxonData alloc] init];
        [td setTaxonName:tn];
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
            [td addObservation:cell];
            [cell release];
            }
        [m addTaxonData:td];
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

- (RbData*)makeNewGuiDataMatrixFromCoreMatrixWithAddress:(CharacterData*)cd {

    std::string fn = cd->getFileName();
    NSString* nsfn = [NSString stringWithCString:(fn.c_str()) encoding:NSUTF8StringEncoding];
    RbData* m = [[RbData alloc] init];
    [m setNumTaxa:(int)(cd->getNumberOfTaxa())];
    [m setNumCharacters:(int)(cd->getNumberOfCharacters())];
    [m setName:nsfn];
    if ( cd->getDataType() == "dna" )
        [m setDataType:DNA];
    else if ( cd->getDataType() == "rna" )
        [m setDataType:RNA];
    else if ( cd->getDataType() == "amino acid" )
        [m setDataType:AA];
    else if ( cd->getDataType() == "standard" )
        [m setDataType:STANDARD];
    else if ( cd->getDataType() == "continuous" )
        [m setDataType:CONTINUOUS];

    for (int i=0; i<cd->getNumberOfTaxa(); i++)
        {        
        RbPtr<const TaxonData> td = cd->getTaxonData(i);
        NSString* taxonName = [NSString stringWithCString:td->getTaxonName().c_str() encoding:NSUTF8StringEncoding];
        [m addTaxonName:taxonName];
        RbTaxonData* rbTaxonData = [[RbTaxonData alloc] init];
        [rbTaxonData setTaxonName:taxonName];
        for (int j=0; j<cd->getNumberOfCharacters(); j++)
            {
            RbPtr<const Character> theChar = td->getCharacter(j);
            RbDataCell* cell = [[RbDataCell alloc] init];
            [cell setDataType:[m dataType]];
            if ( [m dataType] != CONTINUOUS )
                {
                unsigned x = theChar->getUnsignedValue();
                NSNumber* n = [NSNumber numberWithUnsignedInt:x];
                [cell setVal:n];
                [cell setIsDiscrete:YES];
                [cell setNumStates:((int)theChar->getNumberOfStates())];
                if ( theChar->isMissingOrAmbiguous() == true )
                    [cell setIsAmbig:YES];
                }
            else 
                {
                double x = theChar->getRealValue();
                NSNumber* n = [NSNumber numberWithDouble:x];
                [cell setVal:n];
                [cell setIsDiscrete:NO];
                [cell setNumStates:0];
                }
            [cell setRow:i];
            [cell setColumn:j];
            [rbTaxonData addObservation:cell];
            [cell release];
            }
        [m addTaxonData:rbTaxonData];
        }
        
    return m;
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
        [myTool closeControlPanel];
		BOOL isSuccessful = [self readDataFile];
		if (isSuccessful == YES)
			{
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
		[myTool closeControlPanel];
		[myTool removeAllDataMatrices];
        [self addBlankDataMatrix];
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
    NSString* fileToOpen;
    [oPanel setAllowedFileTypes:fileTypes];
    int result = (int)[oPanel runModal];
    if ( result == NSFileHandlingPanelOKButton )
        {
        NSArray* filesToOpen = [oPanel URLs];
        int count = (int)[filesToOpen count];
        for (int i=0; i<count; i++) 
            {
            fileToOpen = [[filesToOpen objectAtIndex:i] path];
            }
        }
            
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
    
    // check the workspace and make certain that we use an unused name for the
    // data variable
    std::string variableName = Workspace::userWorkspace()->generateUniqueVariableName();
		    
    // format a string command to read the data file(s) and send the
    // formatted string to the parser
    const char* cmdAsCStr = [fileToOpen UTF8String];
    std::string cmdAsStlStr = cmdAsCStr;
    std::string line = variableName + " <- read(\"" + cmdAsStlStr + "\");";
    std::cout << "line = \"" << line << "\"" << std::endl;
    int coreResult = Parser::getParser().processCommand(line);
    std::cout << "finished parsing, coreResult = " << coreResult << std::endl;
    if (coreResult != 0)
        {
        [self readDataError:@"Data could not be read"];
        return NO;
        }

    // retrieve the value (character data matrix or matrices) from the workspace
    RbPtr<RbLanguageObject> dv = NULL;
    dv = Workspace::userWorkspace()->getValue(variableName);
    std::cout << "dv = " << dv << std::endl;
    if ( dv == NULL )
        {
        [self readDataError:@"Data could not be read"];
        return NO;
        }
    
    // instantiate data matrices for the gui, by reading the matrices that were 
    // read in by the core
    std::cout << "dv type = " << dv->getType() << std::endl;
    DagNodeContainer* dnc = dynamic_cast<DagNodeContainer*>( (RbObject*)dv );
    CharacterData* cd = dynamic_cast<CharacterData*>( (RbObject*)dv );
    if ( dnc != NULL )
        {
        if (dnc != NULL)
            {
            [myTool removeAllDataMatrices];
            for (int i=0; i<dnc->size(); i++)
                {
                VariableSlot* vs = static_cast<VariableSlot*>( (RbObject*)(dnc->getElement(i)) );
                RbPtr<RbLanguageObject> theDagNode = vs->getDagNode()->getValue();
                CharacterData* cd = static_cast<CharacterData*>( (RbObject*)theDagNode );
                RbData* newMatrix = [self makeNewGuiDataMatrixFromCoreMatrixWithAddress:cd];
                [myTool addMatrix:newMatrix];
                }
            }
        else
            {
            [self readDataError:@"Failure reading in a set of character matrices"];
            return NO;
            }
        }
    else if ( cd != NULL )
        {
        std::cout << "cd = " << cd << std::endl;
        if (cd != NULL)
            {
            [myTool removeAllDataMatrices];
            RbData* newMatrix = [self makeNewGuiDataMatrixFromCoreMatrixWithAddress:cd];
            [myTool addMatrix:newMatrix];
            }
        else
            {
            [self readDataError:@"Failed to read character matrix"];
            return NO;
            }
        }
    else
        {
        [self readDataError:@"Data could not be read"];
        return NO;
        }

	return YES;
}

- (void)readDataError:(NSString*)errStr {

    NSRunAlertPanel(@"Problem Reading Data", errStr, @"OK", nil, nil);
    if ( Workspace::userWorkspace()->existsVariable("guiDataVector") )
        Workspace::userWorkspace()->eraseVariable("guiDataVector");
    [myTool removeAllDataMatrices];
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
