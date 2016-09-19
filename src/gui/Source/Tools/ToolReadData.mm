#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "CharacterState.h"
#include "AbstractCharacterData.h"
#include "HomologousDiscreteCharacterData.h"
#include "ModelVector.h"
#include "NclReader.h"
#include "Parser.h"
#include "RbFileManager.h"
#include "RevNullObject.h"
#include "RlAminoAcidState.h"
#include "DnaState.h"
#include "RlDnaState.h"
#include "RlRnaState.h"
#include "RlStandardState.h"
#include "Workspace.h"
#include "WorkspaceVector.h"
#include "RlAbstractCharacterData.h"
#include "RlNonHomologousDiscreteCharacterData.h"
#include "RlContinuousCharacterData.h"

#import "AnalysisView.h"
#import "InOutlet.h"
#import "RbData.h"
#import "RbDataCell.h"
#import "RbTaxonData.h"
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

- (void)addBlankDataMatrix {

    // allocate the matrix
    RbData* m = [[RbData alloc] init];
    
    // set the dimensions
    [m setNumTaxa:[self numberOfTaxa]];
    [m setNumCharacters:[self numberOfCharacters]];
    
    // set the name
    [m setName:@"Blank Data Matrix"];
    
    // set the data type
    if ( [controlWindow isBlankMatrixOfDataType:@"DNA"] == YES )
        [m setDataType:DNA];
    else if ( [controlWindow isBlankMatrixOfDataType:@"RNA"] == YES )
        [m setDataType:RNA];
    else if ( [controlWindow isBlankMatrixOfDataType:@"Protein"] == YES )
        [m setDataType:AA];
    else if ( [controlWindow isBlankMatrixOfDataType:@"Standard"] == YES )
        [m setDataType:STANDARD];
    else if ( [controlWindow isBlankMatrixOfDataType:@"Continuous"] == YES )
        [m setDataType:CONTINUOUS];
        
    // set a flag if the data is discrete or continuous
    BOOL isDiscrete = YES;
    if ( [controlWindow isBlankMatrixOfDataType:@"Continuous"] == YES )
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
            }
        [m addTaxonData:td];
        }
        
    // add the matrix to the tool
    [self removeAllDataMatrices];
    [self addMatrix:m];

    [self makeDataInspector];
    [self setIsResolved:YES];
}

- (void)awakeFromNib {

}

- (void)closeControlPanelWithCancel {

}

- (void)closeControlPanelWithOK {

    [NSApp stopModal];
	[controlWindow close];

    // set the tool state to unresolved
    [self setIsResolved:NO];
    
    BOOL isSuccessful = [self performToolTask];
    if (isSuccessful == YES)
        {
        }
    else 
        {
        NSLog(@"Unsuccessful reading of data");
        // should catch this error
        }
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
		fileName           = @"";
		pathName           = @"";
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

- (id)initWithCoder:(NSCoder*)aDecoder {

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

- (unsigned)missingForNumStates:(int)n {

	unsigned val = 0;
	for (int i=0; i<n; i++)
		{
        unsigned mask = 1 << i ;
        val |= mask;
		}
	return val;
}

- (BOOL)performToolTask {

    if ([controlWindow makeBlankMatrix] == NO)
        {
        BOOL isSuccessful = [self readDataFile];
        if ( isSuccessful == YES )
            [myAnalysisView updateToolsDownstreamFromTool:self];
        return isSuccessful;
        }
    else
        {
        // make a blank matrix
        [self addBlankDataMatrix];
        [myAnalysisView updateToolsDownstreamFromTool:self];
        return YES;
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
    NSString* fileToOpen = @"";
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
    else
        {
        return NO;
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
    std::string variableName = RevLanguage::Workspace::userWorkspace().generateUniqueVariableName();
    NSString* nsVariableName = [NSString stringWithCString:variableName.c_str() encoding:NSUTF8StringEncoding];
		    
    // format a string command to read the data file(s) and send the
    // formatted string to the parser
    const char* cmdAsCStr = [fileToOpen UTF8String];
    std::string cmdAsStlStr = cmdAsCStr;
    std::string line = variableName + " = readCharacterData(\"" + cmdAsStlStr + "\",alwaysReturnAsVector=TRUE)";

    int coreResult = RevLanguage::Parser::getParser().processCommand(line, &RevLanguage::Workspace::userWorkspace());
    if (coreResult != 0)
        {
        [self readDataError:@"Data could not be read" forVariableNamed:nsVariableName];
        [self stopProgressIndicator];
        return NO;
        }
    
    // retrieve the value (character data matrix or matrices) from the workspace
    const RevLanguage::RevObject& dv = RevLanguage::Workspace::userWorkspace().getRevObject(variableName);
    if ( dv == RevLanguage::RevNullObject::getInstance() )
        {
        [self readDataError:@"Data could not be read" forVariableNamed:nsVariableName];
        [self stopProgressIndicator];
        return NO;
        }
    
    // instantiate data matrices for the gui, by reading the matrices that were
    // read in by the core and stored in the WorkspaceVector
    const WorkspaceVector<RevLanguage::AbstractCharacterData> *dnc = dynamic_cast<const WorkspaceVector<RevLanguage::AbstractCharacterData> *>( &dv );
    if (dnc != NULL)
        {
        [self removeAllDataMatrices];
        for (int i=0; i<dnc->size(); i++)
            {
            RbData* newMatrix = NULL;
            const RevBayesCore::AbstractCharacterData* cd = &((*dnc)[i].getValue());
            
            if (cd->isHomologyEstablished() == true)
                {
                // homology (alignment) has been established
                if (cd->getDataType() == "RNA")
                    {
                    std::string type = "RNA";
                    newMatrix = [self makeNewGuiDataMatrixFromCoreMatrixWithAddress:(*cd) andDataType:type];
                    }
                else if (cd->getDataType() == "DNA")
                    {
                    std::string type = "DNA";
                    newMatrix = [self makeNewGuiDataMatrixFromCoreMatrixWithAddress:(*cd) andDataType:type];
                    }
                else if (cd->getDataType() == "Protein")
                    {
                    std::string type = "Protein";
                    newMatrix = [self makeNewGuiDataMatrixFromCoreMatrixWithAddress:(*cd) andDataType:type];
                    }
                else if (cd->getDataType() == "Standard")
                    {
                    std::string type = "Standard";
                    newMatrix = [self makeNewGuiDataMatrixFromCoreMatrixWithAddress:(*cd) andDataType:type];
                    }
                else if (cd->getDataType() == "Continuous")
                    {
                    std::string type = "Continuous";
                    newMatrix = [self makeNewGuiDataMatrixFromCoreMatrixWithAddress:(*cd) andDataType:type];
                    }
                else
                    {
                    [self readDataError:@"Unrecognized data type" forVariableNamed:nsVariableName];
                    [self stopProgressIndicator];
                    return NO;
                    }

                if (newMatrix == NULL)
                    {
                    [self stopProgressIndicator];
                    [self readDataError:@"Data could not be read" forVariableNamed:nsVariableName];
                    return NO;
                    }
                
                if ([controlWindow isDataFormatAutomaticallyDetermined] == NO)
                    {
                    if ([controlWindow dataAlignment] == 1)
                        [newMatrix setIsHomologyEstablished:NO];
                    }
                [newMatrix setAlignmentMethod:@"Unknown"];
                [self addMatrix:newMatrix];
                }
            else
                {
                // homology (alignment) has not been established
                if (cd->getDataType() == "RNA")
                    {
                    std::string type = "RNA";
                    newMatrix = [self makeNewGuiDataMatrixFromCoreMatrixWithAddress:(*cd) andDataType:type];
                    }
                else if (cd->getDataType() == "DNA")
                    {
                    std::string type = "DNA";
                    newMatrix = [self makeNewGuiDataMatrixFromCoreMatrixWithAddress:(*cd) andDataType:type];
                    }
                else if (cd->getDataType() == "Protein")
                    {
                    std::string type = "Protein";
                    newMatrix = [self makeNewGuiDataMatrixFromCoreMatrixWithAddress:(*cd) andDataType:type];
                    }
                else if (cd->getDataType() == "Standard")
                    {
                    std::string type = "Standard";
                    newMatrix = [self makeNewGuiDataMatrixFromCoreMatrixWithAddress:(*cd) andDataType:type];
                    }
                else if (cd->getDataType() == "Continuous")
                    {
                    std::string type = "Continuous";
                    newMatrix = [self makeNewGuiDataMatrixFromCoreMatrixWithAddress:(*cd) andDataType:type];
                    {
                    [self readDataError:@"Homology must be established for standard  data" forVariableNamed:nsVariableName];
                    [self stopProgressIndicator];
                    return NO;
                    }
                else if (cd->getDataType() == "Continuous")
                    {
                    [self readDataError:@"Homology must be established for continuous data" forVariableNamed:nsVariableName];
                    [self stopProgressIndicator];
                    return NO;
                    }
                else
                    {
                    [self readDataError:@"Unrecognized data type" forVariableNamed:nsVariableName];
                    [self stopProgressIndicator];
                    return NO;
                    }

                if (newMatrix == NULL)
                    {
                    [self stopProgressIndicator];
                    [self readDataError:@"Data could not be read" forVariableNamed:nsVariableName];
                    return NO;
                    }
                
                if ([controlWindow isDataFormatAutomaticallyDetermined] == NO)
                    {
                    if ([controlWindow dataAlignment] == 1)
                        [newMatrix setIsHomologyEstablished:NO];
                    }
                [newMatrix setAlignmentMethod:@"Unknown"];
                [self addMatrix:newMatrix];
                }
            else
                {
                // homology (alignment) has not been established
                if (cd->getDataType() == "RNA")
                    {
                    
                    }
                else if (cd->getDataType() == "DNA")
                    {
                    }
                else if (cd->getDataType() == "Protein")
                    {
                    
                    }
                else if (cd->getDataType() == "Standard")
                    {
                    
                    }
                else if (cd->getDataType() == "Continuous")
                    {
                    
                    }
                else
                    {
                    [self readDataError:@"Unrecognized data type" forVariableNamed:nsVariableName];
                    [self stopProgressIndicator];
                    return NO;
                    }

                if (newMatrix == NULL)
                    {
                    [self stopProgressIndicator];
                    [self readDataError:@"Data could not be read" forVariableNamed:nsVariableName];
                    return NO;
                    }
                
                if ([controlWindow isDataFormatAutomaticallyDetermined] == NO)
                    {
                    if ([controlWindow dataAlignment] == 1)
                        [newMatrix setIsHomologyEstablished:NO];
                    }
                [newMatrix setAlignmentMethod:@"Unknown"];
                [self addMatrix:newMatrix];
                }
            }
        }
    else
        {
        [self stopProgressIndicator];
        [self readDataError:@"Data could not be read" forVariableNamed:nsVariableName];
        return NO;
        }
    
    // erase the data in the core
    if ( RevLanguage::Workspace::userWorkspace().existsVariable(variableName) )
        RevLanguage::Workspace::userWorkspace().eraseVariable(variableName);
        
    // set the name of the variable in the tool
    [self setDataWorkspaceName:@""];
    
    // set up the data inspector
    [self makeDataInspector];

    [self stopProgressIndicator];
    [self setIsResolved:YES];

	return YES;
}

- (NSMutableAttributedString*)sendTip {

    NSString* myTip = @" Read Data Tool ";
    if ([self isResolved] == YES)
        myTip = [myTip stringByAppendingFormat:@"\n Status: Resolved \n # Matrices: %lu ", [self numDataMatrices]];
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

- (NSString*)toolName {

    return @"Read Data";
}

- (void)updateForChangeInUpstreamState {

}

@end
