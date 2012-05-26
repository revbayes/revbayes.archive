#include <map>
#include <vector>
#include <string>
#include "AminoAcidState.h"
#include "CharacterState.h"
#include "CharacterData.h"
#include "ContinuousCharacterState.h"
#include "DagNodeContainer.h"
#include "DnaState.h"
#include "NclReader.h"
#include "Parser.h"
#include "RbFileManager.h"
#include "RbNullObject.h"
#include "RlCharacterData.h"
#include "RnaState.h"
#include "StandardState.h"
#include "VariableSlot.h"
#include "Workspace.h"

#import "AnalysisView.h"
#import "InOutlet.h"
#import "RbData.h"
#import "RevBayes.h"
#import "ToolReadData.h"
#import "WindowControllerReadData.h"



@implementation ToolReadData

@synthesize dataAlignment;
@synthesize dataFormat;
@synthesize dataInterleaved;
@synthesize dataType;
@synthesize dataTypeSimulated;
@synthesize fileName;
@synthesize matrixType;
@synthesize numberOfCharacters;
@synthesize numberOfTaxa;
@synthesize pathName;

- (void)awakeFromNib {

}

- (void)closeControlPanel {

    [NSApp stopModal];
	[controlWindow close];
}

- (void)dealloc {

	[fileName release];
	[pathName release];
	[controlWindow release];

	[super dealloc];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {

    [aCoder encodeObject:fileName          forKey:@"fileName"];
    [aCoder encodeObject:pathName          forKey:@"pathName"];
	[aCoder encodeInt:dataAlignment        forKey:@"dataAlignment"];
	[aCoder encodeInt:dataFormat           forKey:@"dataFormat"];
	[aCoder encodeInt:dataInterleaved      forKey:@"dataInterleaved"];
	[aCoder encodeInt:dataType             forKey:@"dataType"];
	[aCoder encodeInt:dataTypeSimulated    forKey:@"dataTypeSimulated"];
	[aCoder encodeInt:numberOfCharacters   forKey:@"numberOfCharacters"];
	[aCoder encodeInt:numberOfTaxa         forKey:@"numberOfTaxa"];
	[aCoder encodeInt:matrixType           forKey:@"matrixType"];

	[super encodeWithCoder:aCoder];
}

- (id)init {

    self = [self initWithScaleFactor:1.0];
    return self;
}

- (id)initWithScaleFactor:(float)sf {

    if ( (self = [super initWithScaleFactor:sf]) ) 
		{
		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];

		// initialize the inlet/outlet information
		[self addOutletOfColor:[NSColor greenColor]];
		[self addOutletOfColor:[NSColor cyanColor]];
        [self setOutletLocations];

		// initialize the data
		fileName           = [[NSString alloc] initWithString:@""];
		pathName           = [[NSString alloc] initWithString:@""];
		dataAlignment      = 0;
        dataFormat         = 0;
		dataType           = 1;
		dataTypeSimulated  = 1;
		dataInterleaved    = 0;
		numberOfTaxa       = 3;
		numberOfCharacters = 1;
		matrixType         = 0;
		
		// initialize the control window and the data inspector
		controlWindow = [[WindowControllerReadData alloc] initWithTool:self];
		}
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];
		
		// read from file
        fileName           = [aDecoder decodeObjectForKey:@"fileName"];
        pathName           = [aDecoder decodeObjectForKey:@"pathName"];
		dataAlignment      = [aDecoder decodeIntForKey:@"dataAlignment"];
		dataFormat         = [aDecoder decodeIntForKey:@"dataFormat"];
		dataInterleaved    = [aDecoder decodeIntForKey:@"dataInterleaved"];
		dataType           = [aDecoder decodeIntForKey:@"dataType"];
		dataTypeSimulated  = [aDecoder decodeIntForKey:@"dataTypeSimulated"];
		numberOfCharacters = [aDecoder decodeIntForKey:@"numberOfCharacters"];
		numberOfTaxa       = [aDecoder decodeIntForKey:@"numberOfTaxa"];
		matrixType         = [aDecoder decodeIntForKey:@"matrixType"];
        [fileName retain];
        [pathName retain];
        
		// initialize the control window
		controlWindow = [[WindowControllerReadData alloc] initWithTool:self];
		}
	return self;
}

- (void)initializeImage {

    itemImage[0] = [NSImage imageNamed:@"Tool_ReadData25.icns"];
    itemImage[1] = [NSImage imageNamed:@"Tool_ReadData50.icns"];
    itemImage[2] = [NSImage imageNamed:@"Tool_ReadData75.icns"];
    itemImage[3] = [NSImage imageNamed:@"Tool_ReadData100.icns"];
    itemImage[4] = [NSImage imageNamed:@"Tool_ReadData125.icns"];
    itemImage[5] = [NSImage imageNamed:@"Tool_ReadData150.icns"];
    itemImage[6] = [NSImage imageNamed:@"Tool_ReadData200.icns"];
    itemImage[7] = [NSImage imageNamed:@"Tool_ReadData400.icns"];
    
	float s[8] = { 0.25, 0.50, 0.75, 1.0, 1.25, 1.50, 2.0, 4.0 };
	for (int i=0; i<8; i++)
        [itemImage[i] setSize:NSMakeSize(ITEM_IMAGE_SIZE*s[i], ITEM_IMAGE_SIZE*s[i])];
}

- (BOOL)isFullyConnected {

    // we check the number of connections for the inlets and outlets
    // if any inlet or outlet has 0 connections, then the tool is not fully
    // connected
	NSEnumerator* enumerator = [inlets objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
        {
        if ([element numberOfConnections] == 0)
            return NO;
        }
    
	enumerator = [outlets objectEnumerator];
	while ( (element = [enumerator nextObject]) )
        {
        if ([element numberOfConnections] == 0)
            {
            if ( [element toolColor] == [NSColor greenColor] )
                {
                if ( [self numAlignedMatrices] > 0 )
                    return NO;
                }
            else if ( [element toolColor] == [NSColor cyanColor] )
                {
                if ( [self numUnalignedMatrices] > 0 )
                    return NO;
                }
            }
        }

    return YES;
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
            
    [self startProgressIndicator];
    
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
	[self setFileName:fileName];
	[self setPathName:pathName];
    
    // check the workspace and make certain that we use an unused name for the
    // data variable
    std::string variableName = Workspace::userWorkspace()->generateUniqueVariableName();
    NSString* nsVariableName = [NSString stringWithCString:variableName.c_str() encoding:NSUTF8StringEncoding];
		    
    // format a string command to read the data file(s) and send the
    // formatted string to the parser
    const char* cmdAsCStr = [fileToOpen UTF8String];
    std::string cmdAsStlStr = cmdAsCStr;
    std::string line = variableName + " <- read(\"" + cmdAsStlStr + "\")";
    int coreResult = Parser::getParser().processCommand(line, Workspace::userWorkspace());
    if (coreResult != 0)
        {
        [self readDataError:@"Data could not be read" forVariableNamed:nsVariableName];
        [self stopProgressIndicator];
        return NO;
        }

    // retrieve the value (character data matrix or matrices) from the workspace
    RbLanguageObject* dv = Workspace::userWorkspace()->getValue(variableName).getSingleValue()->clone();
    if ( dv == NULL )
        {
        [self readDataError:@"Data could not be read" forVariableNamed:nsVariableName];
        [self stopProgressIndicator];
        return NO;
        }
    
    // instantiate data matrices for the gui, by reading the matrices that were 
    // read in by the core
    DagNodeContainer* dnc = dynamic_cast<DagNodeContainer*>( dv );
    RlCharacterData* cd = dynamic_cast<RlCharacterData*>( dv );
    if ( dnc != NULL )
        {
        [self removeAllDataMatrices];
        for (int i=0; i<dnc->size(); i++)
            {
            const VariableSlot* vs = static_cast<const VariableSlot*>( (&dnc->getElement(i)) );
            const RbPtr<const RbLanguageObject>& theDagNode = vs->getDagNode()->getValue().getSingleValue();
            const RlCharacterData *cd = static_cast<const RlCharacterData *>( (const RbLanguageObject *) theDagNode );
            RbData* newMatrix = [self makeNewGuiDataMatrixFromCoreMatrixWithAddress:cd->getValue()];
            [newMatrix setAlignmentMethod:@"Unknown"];
            [self addMatrix:newMatrix];
            }
        }
    else if ( cd != NULL )
        {
        [self removeAllDataMatrices];
        RbData* newMatrix = [self makeNewGuiDataMatrixFromCoreMatrixWithAddress:cd->getValue()];
        [newMatrix setAlignmentMethod:@"Unknown"];
        [self addMatrix:newMatrix];
        }
    else
        {
        [self readDataError:@"Data could not be read" forVariableNamed:nsVariableName];
        [self stopProgressIndicator];
        return NO;
        }
        
    // erase the data in the core
    if ( Workspace::userWorkspace()->existsVariable(variableName) )
        Workspace::userWorkspace()->eraseVariable(variableName);
        
    // set the name of the variable in the tool
    [self setDataWorkspaceName:@""];
    
    // set up the data inspector
    [self makeDataInspector];

    [self stopProgressIndicator];
        
    [self setIsResolved:YES];
    [myAnalysisView updateToolsDownstreamFromTool:self];

	return YES;
}

- (NSMutableAttributedString*)sendTip {

    NSString* myTip = [NSString stringWithString:@" Read Data Tool "];
    if ([self isResolved] == YES)
        myTip = [myTip stringByAppendingFormat:@"\n Status: Resolved \n # Matrices: %d ", [self numDataMatrices]];
    else 
        myTip = [myTip stringByAppendingString:@"\n Status: Unresolved "];
    if ([self isFullyConnected] == YES)
        myTip = [myTip stringByAppendingString:@"\n Fully Connected "];
    else 
        myTip = [myTip stringByAppendingString:@"\n Missing Connections "];

    NSDictionary *attr = [NSDictionary 
                 dictionaryWithObjects:[NSArray arrayWithObjects:[NSFont fontWithName:@"Lucida Grande Bold" size:14.0], [[NSColor whiteColor] colorWithAlphaComponent:1.0], nil] 
                               forKeys: [NSArray arrayWithObjects:NSFontAttributeName,NSForegroundColorAttributeName, nil]];

    NSMutableAttributedString* attrString = [[NSMutableAttributedString alloc] initWithString:myTip attributes:attr];

    return attrString;
}

- (void)showControlPanel {

    NSPoint p = [self originForControlWindow:[controlWindow window]];
    [[controlWindow window] setFrameOrigin:p];
	[controlWindow showWindow:self];
	[[controlWindow window] makeKeyAndOrderFront:nil];
    [NSApp runModalForWindow:[controlWindow window]];
}

- (void)updateForChangeInState {

    NSLog(@"updateForChangeInState in %@", self);
}

@end
