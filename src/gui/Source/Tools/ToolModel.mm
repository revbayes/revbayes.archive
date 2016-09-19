#import "DistributionListGui.h"
#import "InOutlet.h"
#import "Parm.h"
#import "ParmPlate.h"
#import "RbData.h"
#import "RevBayes.h"
#import "ServerComm.h"
#import "Tool.h"
#import "ToolMatrixFilter.h"
#import "ToolModel.h"
#import "ToolReadData.h"
#import "WindowControllerModelBrowser.h"
#import "WindowControllerModel.h"
#import "WindowControllerModelSubmission.h"

#include <iostream>
#include <map>
#include <string>
#include "AbstractModelObject.h"
#include "ConstructorFunction.h"
#include "Environment.h"
#include "FunctionTable.h"
#include "RlContainer.h"
#include "RlMove.h"
#include "RlDistribution.h"
#include "RevObject.h"
#include "Workspace.h"



@implementation ToolModel

@synthesize dataMatrices;
@synthesize distributionList;
@synthesize parms;

- (NSMutableArray*)allocateParms {

    parms = [[NSMutableArray alloc] init];
    return parms;
}

- (void)awakeFromNib {

}

- (int)calculateNumTaxa {

	if (dataMatrices != nil)
		{
		NSEnumerator* enumerator = [dataMatrices objectEnumerator];
		id element;
		int nt = -1;
		BOOL allSameSize = YES;
		while ( (element = [enumerator nextObject]) )
			{
			if (nt < 0)
				nt = ([element numTaxa] - [element numExcludedTaxa]);
			else 
				{
				if ( nt != [element numTaxa] )
					allSameSize = NO;
				}
			}
		if (allSameSize == YES)
			return nt;
		}
	return -1;
}

- (void)closeControlPanel {

    [NSApp endModalSession:mySession];
	[controlWindow close];
    [self setInletsAndOutlets];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {

	[super encodeWithCoder:aCoder];
    [aCoder encodeObject:parms           forKey:@"parms"];
    [aCoder encodeBool:possibleInlets[0] forKey:@"possibleInlets0"];
    [aCoder encodeBool:possibleInlets[1] forKey:@"possibleInlets1"];
    [aCoder encodeBool:possibleInlets[2] forKey:@"possibleInlets2"];
    [aCoder encodeBool:possibleInlets[3] forKey:@"possibleInlets3"];
}

- (BOOL)execute {

    
    [self startProgressIndicator];
    
    [self stopProgressIndicator];
    return YES;
}

- (void)exportModel {

	// open the model submission window and retrieve information from it
    if (modelSubmission == nil)
		modelSubmission = [[WindowControllerModelSubmission alloc] init];
	[[modelSubmission window] makeKeyAndOrderFront:nil];
    [[modelSubmission window] center];
	[modelSubmission showWindow:self];    

	[modelSubmission cleanWindow];
    [NSApp runModalForWindow:[modelSubmission window]];

	// get the path to the file for saving the model
	NSString* newModelName = @"~/Library/Application Support/RevBayes/User Models/";
	newModelName = [newModelName stringByAppendingString:[modelSubmission modelName]];
	newModelName = [newModelName stringByExpandingTildeInPath];

    // set all of the saveAsModelTemplate flags in the parameters to NO: this supresses encoding of the ToolModel object
    id element;
	NSEnumerator* parmEnumerator = [parms objectEnumerator];
	while ( (element = [parmEnumerator nextObject]) )
        [element setSaveAsModelTemplate:YES];

	// archive the model
	[NSKeyedArchiver archiveRootObject:parms toFile:newModelName];

    // reset the saveAsModelTemplate flags
	parmEnumerator = [parms objectEnumerator];
	while ( (element = [parmEnumerator nextObject]) )
        [element setSaveAsModelTemplate:NO];
	
	// submit the model for use by others
	if ( [modelSubmission submitModelToPublic] == YES )
		{
        ServerComm* server = [[ServerComm alloc] init];
        if ([server connectToServer] == NO)
            {
            NSAlert* alert = [[NSAlert alloc] init];
            [alert setMessageText:@"Connection Failure"];
            [alert setInformativeText:@"The model could not be submitted for use by others because the RevBayes server could not be reached. The file was saved to this computer."];
            [alert runModal];
            //NSRunAlertPanel(@"Connection Failure", @"The model could not be submitted for use by others because the RevBayes server could not be reached. The file was saved to this computer.", @"OK", nil, nil);
            return;
            }

		NSData* d = [NSKeyedArchiver archivedDataWithRootObject:parms];
        NSArray* mi = [NSArray arrayWithObjects:[modelSubmission modelName], [modelSubmission creatorName], [modelSubmission notes], nil];
        [server submitModelWithInfo:mi andData:d];
        
        [server disconnectFromServer];
		}
}

- (void)importModel {

	// get the name and path of the model to read
    if (modelBrowser == nil)
		modelBrowser = [[WindowControllerModelBrowser alloc] initWithModelWindow:controlWindow];
	[modelBrowser readCuratedModels];
	[modelBrowser readUserModels];
    [[modelBrowser window] center];
	[modelBrowser showWindow:self];    
    [NSApp runModalForWindow:[modelBrowser window]];
    
    if ([modelBrowser userDidCancel] == YES)
        {
        return;
        }
	
	NSString* sma = [modelBrowser selectedModel];
	NSString* selectedModelName = @"~/Library/Application Support/RevBayes/";
    if ([modelBrowser userSelectedCuratedModel] == YES)
        selectedModelName = [selectedModelName stringByAppendingString:@"Curated Models/"];
    else 
        selectedModelName = [selectedModelName stringByAppendingString:@"User Models/"];
	selectedModelName = [selectedModelName stringByAppendingString:sma];
	selectedModelName = [selectedModelName stringByExpandingTildeInPath];
	
	// read the parameter information from the model file
	NSData* md = [NSData dataWithContentsOfFile:selectedModelName];
	@try {
		parms = [NSKeyedUnarchiver unarchiveObjectWithData:md];
		[controlWindow setParmsPtr:parms];
		}
	@catch (NSException* e) 
		{
        NSAlert* alert = [[NSAlert alloc] init];
        [alert setMessageText:@"Error"];
        [alert setInformativeText:@"Problem reading the model."];
        [alert runModal];
        //NSRunAlertPanel(@"Error", @"Problem reading the model", @"OK", nil, nil);
        modelBrowser = nil;
        return;
		}
    
    // set the window for all of the parameters
	id element;
	NSEnumerator* parmEnumerator = [parms objectEnumerator];
	while ( (element = [parmEnumerator nextObject]) )
        {
        [element setMyModel:self];
        [element setParmsWindow:[controlWindow window]];
        }
		
	// update all of the parameters
	[self touchAllParameters];
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
        possibleInlets[0] = YES;
        possibleInlets[1] = NO;
        possibleInlets[2] = NO;
        possibleInlets[3] = NO;
		[self addInletOfColor:[NSColor greenColor]];
		[self addOutletOfColor:[NSColor blueColor]];
        [self setInletLocations];
        [self setOutletLocations];
        
        [self initializePallet];

		// allcoate the mutable array holding the parameters
		parms = [[NSMutableArray alloc] init];

		// initialize the control window
		controlWindow = [[WindowControllerModel alloc] initWithTool:self andParms:parms];
        modelBrowser    = nil;
		modelSubmission = nil;

		touchOnRevival = YES;
        
        // allocate a list of possible distributions for parameters
        distributionList = [[DistributionListGui alloc] init];
		}
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];

		// read the mutable array holding the parameters
        parms = [aDecoder decodeObjectForKey:@"parms"];
		[self touchAllParameters];
        possibleInlets[0] = [aDecoder decodeBoolForKey:@"possibleInlets0"];
        possibleInlets[1] = [aDecoder decodeBoolForKey:@"possibleInlets1"];
        possibleInlets[2] = [aDecoder decodeBoolForKey:@"possibleInlets2"];
        possibleInlets[3] = [aDecoder decodeBoolForKey:@"possibleInlets3"];

        [self initializePallet];

		// initialize the control window
		controlWindow = [[WindowControllerModel alloc] initWithTool:self andParms:parms];
        modelBrowser    = nil;
		modelSubmission = nil;

        // allocate a list of possible distributions for parameters
        distributionList = [[DistributionListGui alloc] init];
		}
	return self;
}

- (void)initializeImage {

    itemImage[0] = [NSImage imageNamed:@"Tool_Model25.icns"];
    itemImage[1] = [NSImage imageNamed:@"Tool_Model50.icns"];
    itemImage[2] = [NSImage imageNamed:@"Tool_Model75.icns"];
    itemImage[3] = [NSImage imageNamed:@"Tool_Model100.icns"];
    itemImage[4] = [NSImage imageNamed:@"Tool_Model125.icns"];
    itemImage[5] = [NSImage imageNamed:@"Tool_Model150.icns"];
    itemImage[6] = [NSImage imageNamed:@"Tool_Model200.icns"];
    itemImage[7] = [NSImage imageNamed:@"Tool_Model400.icns"];

	float s[8] = { 0.25, 0.50, 0.75, 1.0, 1.25, 1.50, 2.0, 4.0 };
	for (int i=0; i<8; i++)
        [itemImage[i] setSize:NSMakeSize(ITEM_IMAGE_SIZE*s[i], ITEM_IMAGE_SIZE*s[i])];
}

- (void)initializePallet {

    // get a pointer to the global workspace in the core and then
    // extract the list of variables and moves stored there
    RevLanguage::Workspace& myWorkspace = RevLanguage::Workspace::globalWorkspace();
    std::map<std::string, RevLanguage::RevObject*> list = myWorkspace.getTypeTable();

    // construct the list of variables for the random variable and constants pallets
    for (std::map<std::string, RevLanguage::RevObject*>::iterator it = list.begin(); it != list.end(); it++)
        {
        RevLanguage::AbstractModelObject* varPtr = dynamic_cast<RevLanguage::AbstractModelObject*>(it->second);
        if (varPtr != NULL)
            {
            // it's a variable!
            std::cout << "Variable: " << (it)->first << std::endl;
            std::cout << "Ptr:      " << varPtr << std::endl;
            std::cout << "Type:     " << varPtr->getType() << std::endl;

            if ((it)->first[(it)->first.size()-1] == ']')
                {
                std::cout << "   xxxx:   \"" << &varPtr[0] << "\"" << std::endl;
                }
            std::cout << "   Name:   \"" << varPtr->getGuiVariableName() << "\"" << std::endl;
            std::cout << "   Symbol: \"" << varPtr->getGuiLatexSymbol()  << "\"" << std::endl;

            RevLanguage::Container* containerPtr = dynamic_cast<RevLanguage::Container*>(it->second);
            if (containerPtr != NULL)
                {
                std::cout << "   It's a container!" << std::endl;
                }
            }
        }

    // construct the list of moves
    for (std::map<std::string, RevLanguage::RevObject*>::iterator it = list.begin(); it != list.end(); it++)
        {
        RevLanguage::Move* movePtr = dynamic_cast<RevLanguage::Move*>(it->second);
        if (movePtr != NULL)
            {
            // it's a move!
            std::cout << "Move: " << (it)->first << std::endl;
            }

        }
    
    // construct the list of distributions
    RevLanguage::FunctionTable& funcList = myWorkspace.getFunctionTable();
    for (RevLanguage::FunctionTable::iterator it = funcList.begin(); it != funcList.end(); it++)
        {
        RevLanguage::ConstructorFunction* conFunc = dynamic_cast<RevLanguage::ConstructorFunction*>(it->second);
        if (conFunc != NULL)
            {
            RevLanguage::RevObject* revObj = conFunc->getRevObject();
            RevLanguage::Distribution* distPtr = dynamic_cast<RevLanguage::Distribution*>(revObj);
            if (distPtr != NULL)
                {
                // it's a distribution!
                std::cout << "Distribution: " << (it)->first << std::endl;
                std::cout << "   Name: \"" << distPtr->getGuiDistributionName() << "\"" << std::endl;
                }
            }
       }

}

- (BOOL)isInletActiveWithIndex:(int)idx {

    return possibleInlets[idx];
}

- (BOOL)isPlateIndexAvailableForChar:(char)c {

	id element;
	NSEnumerator* parmEnumerator = [parms objectEnumerator];
	while ( (element = [parmEnumerator nextObject]) )
		{
		if ([element isPlate] == YES)
			{
			char pc = [element indexLetter];
			if ( pc == c )
				return NO;
			}
		}
	return YES;
}

- (int)numAttachedMatrices {

	if (dataMatrices == nil)
		return 0;
	return (int)[dataMatrices count];
}

- (int)numCharactersForMatrix:(int)idx {

	if (dataMatrices == nil)
		return 0;
	RbData* dm = [dataMatrices objectAtIndex:idx];
	if (dm != nil)
		return [dm numCharacters];
	return 0;
}

- (int)numExcludedCharactersForMatrix:(int)idx {

	if (dataMatrices == nil)
		return 0;
	RbData* dm = [dataMatrices objectAtIndex:idx];
	if (dm != nil)
		return [dm numExcludedCharacters];
	return 0;
}

- (int)numExcludedTaxaForMatrix:(int)idx {

	if (dataMatrices == nil)
		return 0;
	RbData* dm = [dataMatrices objectAtIndex:idx];
	if (dm != nil)
		return [dm numExcludedTaxa];
	return 0;
}

- (int)numTaxaForMatrix:(int)idx {

	if (dataMatrices == nil)
		return 0;
	RbData* dm = [dataMatrices objectAtIndex:idx];
	if (dm != nil)
		return [dm numTaxa];
	return 0;
}

- (NSArray*)numTaxaForMatrices {

	int nm = 0;
	if (dataMatrices == nil)
		nm = 0;
	else 
		nm = (int)[dataMatrices count];
		
	NSArray* retArray;
	if (nm == 0)
		{
		retArray = [NSArray arrayWithObjects:nil];
		}
	else 
		{
		NSMutableArray* txa = [NSMutableArray arrayWithCapacity:1];
		for (int i=0; i<[dataMatrices count]; i++)
			{
			RbData* dm = [dataMatrices objectAtIndex:i];
			int n = [dm numTaxa];
			[txa addObject:[NSNumber numberWithInt:n]];
			}
		retArray = [NSArray arrayWithArray:txa];
		}
	return retArray;
}

- (NSMutableAttributedString*)sendTip {

    NSString* myTip = @" Model Tool ";
    if ([self isResolved] == YES)
        myTip = [myTip stringByAppendingString:@"\n Status: Resolved "];
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

- (void)setInletsAndOutlets {

    
}

- (void)setPossibleInletTo:(BOOL)tf forPossibility:(int)idx {
    
    possibleInlets[idx] = tf;
}

- (void)showControlPanel {

    NSPoint p = [self originForControlWindow:[controlWindow window]];
    [[controlWindow window] setFrameOrigin:p];
	[controlWindow setSourceInformation];
	[controlWindow showWindow:self];    
	[[controlWindow window] makeKeyAndOrderFront:nil];
    mySession = [NSApp beginModalSessionForWindow:[controlWindow window]];
	[NSApp runModalSession:mySession];	
}

- (void)touchAllParameters {

	id element;
	NSEnumerator* parmEnumerator = [parms objectEnumerator];
	while ( (element = [parmEnumerator nextObject]) )
        [element updateChildren];
}

- (NSString*)toolName {

    return @"Model Specification";
}

- (void)updateForChangeInUpstreamState {

    [self startProgressIndicator];

    // set the tool state to unresolved
    [self setIsResolved:NO];
    
	// attempt to get a pointer to the parent tool
    Tool* t = nil;
    for (int i=0; i<[self numInlets]; i++)
        {
        Tool* myParentTool = [self getParentToolOfInletIndexed:i];
        if ( [myParentTool isKindOfClass:[ToolData class]] == YES )
            {
            t = myParentTool;
            break;
            }
        }
		
	// update the state of this tool depending upon the state/presence of the parent tool
	if (t == nil)
		{
		// we don't have a parent tool
		dataMatrices = nil;
		}
	else 
		{
		// there is a parent tool 
        dataMatrices = [(ToolData*)t dataMatrices];
		}

	// make certain that all of the parameters update their state
	// to reflect the new connectivity of the tool
	[self touchAllParameters];
        
    [self stopProgressIndicator];
}

- (void)updateInlets {
    
    int numAlignedSequenceInlets = [self numInletsOfColor:[NSColor greenColor]];
    if (possibleInlets[0] == YES && numAlignedSequenceInlets == 0)
        [self addInletOfColor:[NSColor greenColor]];
    else if (possibleInlets[0] == NO && numAlignedSequenceInlets == 1)
        [self removeInletOfColor:[NSColor greenColor]];
    
    int numUnalignedSequenceInlets = [self numInletsOfColor:[NSColor cyanColor]];
    if (possibleInlets[1] == YES && numUnalignedSequenceInlets == 0)
        [self addInletOfColor:[NSColor cyanColor]];
    else if (possibleInlets[1] == NO && numUnalignedSequenceInlets == 1)
        [self removeInletOfColor:[NSColor cyanColor]];

    int numTreeInlets = [self numInletsOfColor:[NSColor redColor]];
    if (possibleInlets[2] == YES && numTreeInlets == 0)
        [self addInletOfColor:[NSColor redColor]];
    else if (possibleInlets[2] == NO && numTreeInlets == 1)
        [self removeInletOfColor:[NSColor redColor]];

    int numNumberInlets = [self numInletsOfColor:[NSColor orangeColor]];
    if (possibleInlets[3] == YES && numNumberInlets == 0)
        [self addInletOfColor:[NSColor orangeColor]];
    else if (possibleInlets[3] == NO && numNumberInlets == 1)
        [self removeInletOfColor:[NSColor orangeColor]];

}

@end
