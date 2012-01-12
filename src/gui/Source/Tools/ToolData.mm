#import "RbData.h"
#import "RbDataCell.h"
#import "RbTaxonData.h"
#import "ToolData.h"
#import "WindowControllerCharacterMatrix.h"
#include <string>
#include "AminoAcidState.h"
#include "Character.h"
#include "CharacterContinuous.h"
#include "CharacterData.h"
#include "DagNodeContainer.h"
#include "DnaState.h"
#include "Parser.h"
#include "RnaState.h"
#include "StandardState.h"
#include "TaxonData.h"
#include "Workspace.h"



@implementation ToolData

@synthesize dataWorkspaceName;

- (void)addMatrix:(RbData*)m {

	[dataMatrices addObject:m];
	[m release];
    hasInspectorInfo = YES;
}

- (void)awakeFromNib {

}

- (RbData*)dataMatrixIndexed:(int)i {

	if (i > [dataMatrices count])
		return nil;
	return [dataMatrices objectAtIndex:i];
}

- (void)dealloc {

    if (dataInspector != nil)
        [dataInspector release];
    [dataMatrices release];
    [dataWorkspaceName release];
    
	[super dealloc];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {
    
	[aCoder encodeObject:dataMatrices      forKey:@"dataMatrices"];
    [aCoder encodeObject:dataWorkspaceName forKey:@"dataWorkspaceName"];
       
	[super encodeWithCoder:aCoder];
}

- (id)init {

    self = [self initWithScaleFactor:1.0];
    return self;
}

- (id)initWithScaleFactor:(float)sf {

    if ( (self = [super initWithScaleFactor:sf]) ) 
		{
		dataMatrices      = [[NSMutableArray alloc] init];
        dataWorkspaceName = [[NSString alloc] initWithString:@""];
        dataInspector     = nil;
        
        // check to see if there are any data matrices stored in the tool
        if ([dataMatrices count] > 0)
            hasInspectorInfo = YES;
		}
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
        dataMatrices      = [aDecoder decodeObjectForKey:@"dataMatrices"];
        dataWorkspaceName = [aDecoder decodeObjectForKey:@"dataWorkspaceName"];
        dataInspector     = nil;
        [dataMatrices retain];
        [dataWorkspaceName retain];

        // check to see if there are any data matrices stored in the tool
        if ([dataMatrices count] > 0)
            hasInspectorInfo = YES;

        // make certain that the data matrices are instantiated in the core
        [self instantiateDataInCore];
		}
	return self;
}

- (void)initializeImage {

}

- (void)instantiateDataInCore {

    // get a path to a temporary directory
    NSString* myTemporaryDirectory = NSTemporaryDirectory();
    
    const char* cStr = [dataWorkspaceName UTF8String];
    std::string variableName = cStr;

    if ( [dataMatrices count] == 1 )
        {
        // there is only a single matrix that the core needs to
        // read in
        RbData* d = [dataMatrices objectAtIndex:0];
        
        NSMutableString* fn = [NSMutableString stringWithString:myTemporaryDirectory];
        [fn appendString:[d name]];
        [d writeToFile:fn];

        const char* cmdAsCStr = [fn UTF8String];
        std::string cmdAsStlStr = cmdAsCStr;
        std::string line = variableName + " <- read(\"" + cmdAsStlStr + "\")";
        int coreResult = Parser::getParser().processCommand(line);
        if (coreResult != 0)
            NSLog(@"Error: Could not create data in workspace");
        if ( !Workspace::userWorkspace()->existsVariable(variableName) )
            NSLog(@"Error: Could not create data in workspace");
        }
    else if ( [dataMatrices count] > 1 )
        {
        // if there are more than one data matrix in this tool, then we
        // save all of the data matrices to a directory that we create in the
        // temporary directory, and then have the core read them all
        NSMutableString* tempDir = [NSMutableString stringWithString:myTemporaryDirectory];
        [tempDir appendString:@"tempDir"];
        [tempDir appendString:@"/"];
        NSFileManager* fileManager= [NSFileManager defaultManager]; 
        BOOL isDir = NO;
        if ( ![fileManager fileExistsAtPath:tempDir isDirectory:&isDir] )
            if ( ![fileManager createDirectoryAtPath:tempDir withIntermediateDirectories:YES attributes:nil error:NULL] )
                NSLog(@"Error: Create folder failed %@", tempDir);

        for (int i=0; i<[dataMatrices count]; i++)
            {
            RbData* d = [dataMatrices objectAtIndex:i];

            NSMutableString* fn = [NSMutableString stringWithString:tempDir];
            [fn appendString:[d name]];
            [d writeToFile:fn];
            //NSLog(@"file name = %@", fn);
            }
            
        const char* cmdAsCStr = [tempDir UTF8String];
        std::string cmdAsStlStr = cmdAsCStr;
        std::string line = variableName + " <- read(\"" + cmdAsStlStr + "\")";
        int coreResult = Parser::getParser().processCommand(line);
        if (coreResult != 0)
            NSLog(@"Error: Could not create data in workspace");
        if ( !Workspace::userWorkspace()->existsVariable(variableName) )
            NSLog(@"Error: Could not create data in workspace");
        }
    [self removeFilesFromTemporaryDirectory];
    
    [self makeDataInspector];

    if ( Workspace::userWorkspace()->existsVariable(variableName) )
        std::cout << "Successfully created data variable named \"" << variableName << "\" in workspace" << std::endl;
}

- (void)instantiateDataInspector {

    NSLog(@"instantiating data inspector");
    if (dataInspector != nil)
        [dataInspector release];
    dataInspector = [[WindowControllerCharacterMatrix alloc] initWithTool:self];
    [dataInspector window];
    NSLog(@"finished instantiating data inspector");
}

- (void)makeDataInspector {

    //if ( [dataMatrices count] > 0 )
    //    [NSThread detachNewThreadSelector:@selector(instantiateDataInspector) toTarget:self withObject:nil];
    [self instantiateDataInspector];
}

- (RbData*)makeNewGuiDataMatrixFromCoreMatrixWithAddress:(CharacterData*)cd {

    std::string fn = cd->getFileName();
    
    NSString* nsfn = [NSString stringWithCString:(fn.c_str()) encoding:NSUTF8StringEncoding];
    RbData* m = [[RbData alloc] init];
    [m setNumTaxa:(int)(cd->getNumberOfTaxa())];
    if ( cd->getIsHomologyEstablished() == true )
        [m setIsHomologyEstablished:YES];
    else
        [m setIsHomologyEstablished:NO];
    [m setNumCharacters:(int)(cd->getNumberOfCharacters())];
    [m setName:nsfn];
    if ( cd->getDataType() == DnaState_name )
        [m setDataType:DNA];
    else if ( cd->getDataType() == RnaState_name )
        [m setDataType:RNA];
    else if ( cd->getDataType() == AminoAcidState_name )
        [m setDataType:AA];
    else if ( cd->getDataType() == StandardState_name )
        [m setDataType:STANDARD];
    else if ( cd->getDataType() == CharacterContinuous_name )
        [m setDataType:CONTINUOUS];

    for (int i=0; i<cd->getNumberOfTaxa(); i++)
        {        
        RbPtr<const TaxonData> td = cd->getTaxonData(i);
        NSString* taxonName = [NSString stringWithCString:td->getTaxonName().c_str() encoding:NSUTF8StringEncoding];
        [m addTaxonName:taxonName];
        RbTaxonData* rbTaxonData = [[RbTaxonData alloc] init];
        [rbTaxonData setTaxonName:taxonName];
        for (int j=0; j<cd->getNumberOfCharacters(i); j++)
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

- (NSString*)nameOfMatrixIndexed:(int)idx {

    return [[dataMatrices objectAtIndex:idx] name];
}

- (int)numDataMatrices {

	return (int)[dataMatrices count];
}

- (void)removeAllDataMatrices {

	[dataMatrices removeAllObjects];
    hasInspectorInfo = NO;
    [self removeDataInspector];
}

- (void)removeDataInspector {

    if ( dataInspector != nil )
        [dataInspector release];
    dataInspector = nil;
}

- (void)removeFilesFromTemporaryDirectory {

    NSString* temporaryDirectory = NSTemporaryDirectory();
    NSFileManager* fm = [[[NSFileManager alloc] init] autorelease];
    NSDirectoryEnumerator* en = [fm enumeratorAtPath:temporaryDirectory];    
    NSString* file;
    while ( file = [en nextObject] ) 
        {
        NSError* err = nil;
        BOOL res = [fm removeItemAtPath:[temporaryDirectory stringByAppendingPathComponent:file] error:&err];
        if (!res && err) 
            {
            }
        }
}

- (void)showInspectorPanel {

    if (dataInspector == nil)
        NSLog(@"No idea why we don't have a data inspector");
    [[dataInspector window] center];
    [dataInspector showWindow:self];
}

@end
