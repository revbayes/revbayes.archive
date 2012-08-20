#import "ParmPlate.h"
#import "RevBayes.h"
#import "ToolModel.h"



@implementation ParmPlate

@synthesize indexLetter;
@synthesize indexValue;
@synthesize plateRect;
@synthesize numElements;
@synthesize minPlateSize;
@synthesize indexSource;
@synthesize italicsRange;

- (void)awakeFromNib {
    
	[indexSelector setAutoenablesItems:NO];
	[self setIndexValue:[self numElements]];
}

- (IBAction)cancelAction:(id)sender {
    
	[self closeControlPanel];
    [self setIndexLetter:startingIndexLetter];
    [self setIndexSource:startingIndexSource];
    [self setIndexValue:startingIndexValue];
    [self setNumElements:startingNumElements];
}

- (IBAction)changeIndexValue:(id)sender {
    
	[self refreshWindow];
}

- (IBAction)changeIndexSource:(id)sender {
    
	NSString* selTitle = [indexRangeSelector titleOfSelectedItem];
    
	if ( [selTitle isEqualToString:@"Number"] == YES )
		[self setIndexSource:SRC_FIXED_INDEX];
	else if ( [selTitle isEqualToString:@"Number of Character Matrices"] == YES )
		[self setIndexSource:SRC_NUMBER_MATRICES];
	else if ( [selTitle isEqualToString:@"Number Characters in Matrix"] == YES )
		[self setIndexSource:SRC_NUMBER_CHARS];
	else if ( [selTitle isEqualToString:@"Number of Taxa"] == YES )
		[self setIndexSource:SRC_NUMBER_TAXA];
    
	[self refreshWindow];
}

- (void)dealloc {

    NSLog(@"dealloc ParmPlate");
	[super dealloc];
}

- (void)encodeWithCoder:(NSCoder *)aCoder {

	[super encodeWithCoder:aCoder];
	[aCoder encodeRect:plateRect  forKey:@"plateRect"];
	[aCoder encodeInt:numElements forKey:@"numElements"];
	[aCoder encodeInt:indexSource forKey:@"indexSource"];
	[aCoder encodeInt:indexLetter forKey:@"indexLetter"];
}

- (NSString*)getEndingRangeForPlate {

	italicsRange = NSMakeRange(0, 0);
	NSString* pStr;
	if ( indexSource == SRC_FIXED_INDEX )
		{
		if (numElements == 1)
			pStr = @"1";
		else 
			pStr = [NSString stringWithFormat:@"%d", numElements];
		}
	else if ( indexSource == SRC_NUMBER_MATRICES )
		{
		int numMatrices = [self numAttachedMatrices];
		if (numMatrices == 0)
			pStr = @"M";
		else if (numMatrices == 1)
			pStr = @"1";
		else 
			pStr = [NSString stringWithFormat:@"%d", numMatrices];
		}
	else if ( indexSource == SRC_NUMBER_CHARS )
		{
		int numMatrices = [self numAttachedMatrices];
		if (numMatrices == 0)
			{
			ParmPlate* idOfPlateRangingOverMatrices = [self isOnPlateWithIndexSource:SRC_NUMBER_MATRICES];
			if (idOfPlateRangingOverMatrices == nil)
				pStr = @"N[?]";
			else 
				{
				pStr = [NSString stringWithFormat:@"N[%c]", [idOfPlateRangingOverMatrices indexLetter]];
				italicsRange = NSMakeRange(2, 1);
				}
			}
		else if (numMatrices == 1)
			{
			int numSites = [self numCharactersForMatrix:0];
			ParmPlate* idOfPlateRangingOverMatrices = [self isOnPlateWithIndexSource:SRC_NUMBER_MATRICES];
			if (idOfPlateRangingOverMatrices != nil)
				pStr = [NSString stringWithFormat:@"%d", numSites];
			else 
				pStr = @"?";
			}
		else 
			{
			ParmPlate* idOfPlateRangingOverMatrices = [self isOnPlateWithIndexSource:SRC_NUMBER_MATRICES];
			if (idOfPlateRangingOverMatrices != nil)
				{
				pStr = [NSString stringWithFormat:@"N[%c]", [idOfPlateRangingOverMatrices indexLetter]];
				italicsRange = NSMakeRange(2, 1);
				}
			else 
				{
				int numAssignedPlates = [self numAssignedPlates];
				if (numAssignedPlates == 0)
					pStr = @"N[?]";
				else if (numAssignedPlates == 1)
					pStr = @"N[?]";
				else 
					pStr = @"N[?]";
				}
			}
		}
	else if ( indexSource == SRC_NUMBER_TAXA )
		{
		int numMatrices = [self numAttachedMatrices];
		if (numMatrices == 0)
			{
			ParmPlate* idOfPlateRangingOverMatrices = [self isOnPlateWithIndexSource:SRC_NUMBER_MATRICES];
			if (idOfPlateRangingOverMatrices == nil)
				pStr = @"T[?]";
			else 
				{
				pStr = [NSString stringWithFormat:@"T[%c]", [idOfPlateRangingOverMatrices indexLetter]];
				italicsRange = NSMakeRange(2, 1);
				}
			}
		else if (numMatrices == 1)
			{
			int numTaxa = [self numTaxaForMatrix:0];
			ParmPlate* idOfPlateRangingOverMatrices = [self isOnPlateWithIndexSource:SRC_NUMBER_MATRICES];
			if (idOfPlateRangingOverMatrices != nil)
				pStr = [NSString stringWithFormat:@"%d", numTaxa];
			else 
				pStr = @"?";
			}
		else 
			{
			ParmPlate* idOfPlateRangingOverMatrices = [self isOnPlateWithIndexSource:SRC_NUMBER_MATRICES];
			if (idOfPlateRangingOverMatrices != nil)
				{
				pStr = [NSString stringWithFormat:@"T[%c]", [idOfPlateRangingOverMatrices indexLetter]];
				italicsRange = NSMakeRange(2, 1);
				}
			else 
				{
				int numAssignedPlates = [self numAssignedPlates];
				if (numAssignedPlates == 0)
					pStr = @"T[?]";
				else if (numAssignedPlates == 1)
					pStr = @"T[?]";
				else 
					pStr = @"T[?]";
				}
			}
		}
		
	return pStr;
}

- (id)init {

    self = [self initWithScaleFactor:1.0 andTool:nil];
    return self;
}

- (id)initWithScaleFactor:(float)sf andTool:(ToolModel*)t {

    if ( (self = [super initWithScaleFactor:sf andTool:t]) ) 
		{
		// initialize the tool image
		[self initializeImage];

        // initialize some variables
        parmType         = PARM_PLATE;
		plateRect        = NSMakeRect(0.0, 0.0, 200.0*sf, 200.0*sf);
		isPlate          = YES;
		numElements      = 1;
		indexSource      = SRC_FIXED_INDEX;
        hasInspectorInfo = NO;
        [self setImageWithSize:plateRect.size];
		}
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
		// initialize the parameter image
		[self initializeImage];

		// initialize the rectangle 
        plateRect   = [aDecoder decodeRectForKey:@"plateRect"];
		numElements = [aDecoder decodeIntForKey:@"numElements"];
		indexSource = [aDecoder decodeIntForKey:@"indexSource"];
		indexLetter = [aDecoder decodeIntForKey:@"indexLetter"];
        [self setImageWithSize:itemSize];
        [self setImageWithSize:plateRect.size];
		}
	return self;
}

- (void)initializeImage {

}

- (IBAction)okAction:(id)sender {
    
    [self updateParameterSettings];
	[self closeControlPanel];
}

- (void)refreshWindow {

	float panelHeight = 500.0;
    
	// set the index value
	char possIndices[26] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
	ToolModel* tm = [self myModel];
	char currentIndex = [self indexLetter];
	[indexSelector removeAllItems];
	for (int i=0; i<26; i++)
		{
		NSString* idxStr = [NSString stringWithFormat:@"%c", possIndices[i]];
		[indexSelector addItemWithTitle:idxStr];
		if ( [tm isPlateIndexAvailableForChar:possIndices[i]] == NO && currentIndex != possIndices[i] )
			[[indexSelector itemWithTitle:idxStr] setEnabled:NO];
		if ( currentIndex == possIndices[i] )
			[indexSelector selectItemWithTitle:idxStr];
		}
    
	// set the index range selector
	[indexRangeSelector removeAllItems];
	[indexRangeSelector addItemWithTitle:@"Number"];
	[indexRangeSelector addItemWithTitle:@"Number of Character Matrices"];
	[indexRangeSelector addItemWithTitle:@"Number Characters in Matrix"];
	[indexRangeSelector addItemWithTitle:@"Number of Taxa"];
	if ( indexSource == SRC_FIXED_INDEX )
		[indexRangeSelector selectItemWithTitle:@"Number"];
	else if ( indexSource == SRC_NUMBER_MATRICES )
		[indexRangeSelector selectItemWithTitle:@"Number of Character Matrices"];
	else if ( indexSource == SRC_NUMBER_CHARS )
		[indexRangeSelector selectItemWithTitle:@"Number Characters in Matrix"];
	else if ( indexSource == SRC_NUMBER_TAXA )
		[indexRangeSelector selectItemWithTitle:@"Number of Taxa"];
    
	// set the value range information
	if ( indexSource == SRC_FIXED_INDEX )
		{
		[valueLabelField setHidden:NO];
		[indexRangeField setHidden:NO];
		panelHeight = 500.0;
		}
	else	
		{
		[valueLabelField setHidden:YES];
		[indexRangeField setHidden:YES];
		panelHeight = 470.0;
		}
    
	// set the message
	if ( indexSource == SRC_FIXED_INDEX )
		{
		if (indexValue == 1)
			[msgLabelField setStringValue:@"This plate iterates over a range of one. Consider changing the range for this plate, or simply deleting the plate."];
		else 
			{
			NSString* msg = [NSString stringWithFormat:@"This plate iterates over a range of %d.", indexValue];
			[msgLabelField setStringValue:msg];
			}
		}
	else if ( indexSource == SRC_NUMBER_MATRICES )
		{
		int numMatrices = [self numAttachedMatrices];
		if (numMatrices == 0)
			[msgLabelField setStringValue:@"This plate will range over the number of character matrices attached to the Model Tool. Currently, the Model Tool does not have data matrices attached to it."];
		else if (numMatrices == 1)
			[msgLabelField setStringValue:@"This plate will range over the number of character matrices attached to the Model Tool. Currently, the Model Tool contains one data matrix. Consider deleting this plate because it is not sensible to have a plate with a range of one."];
		else 
			{
			NSString* msg = [NSString stringWithFormat:@"This plate will range over the number of character matrices attached to the Model Tool. Currently, the Model Tool contains %d data matrices.", numMatrices];
			[msgLabelField setStringValue:msg];
			}
		}
	else if ( indexSource == SRC_NUMBER_CHARS )
		{
		int numMatrices = [self numAttachedMatrices];
		if (numMatrices == 0)
			{
			ParmPlate* idOfPlateRangingOverMatrices = [self isOnPlateWithIndexSource:SRC_NUMBER_MATRICES];
			if (idOfPlateRangingOverMatrices == nil)
				[msgLabelField setStringValue:@"This plate will range over the number of characters in a character matrix attached to the Model Tool. Currently, the Model Tool does not have a data matrix attached to it."];
			else 
				{
				NSString* msg = [NSString stringWithFormat:@"This plate will range over the number of characters in the %c-th character matrix attached to the Model Tool. Currently, the Model Tool does not have data matrices attached to it.", [idOfPlateRangingOverMatrices indexLetter]];
				[msgLabelField setStringValue:msg];
				}
			}
		else if (numMatrices == 1)
			{
			int numSites = [self numCharactersForMatrix:0];
			int numExcludedSites = [self numExcludedCharactersForMatrix:0];
			ParmPlate* idOfPlateRangingOverMatrices = [self isOnPlateWithIndexSource:SRC_NUMBER_MATRICES];
            NSString* msg;
			if (idOfPlateRangingOverMatrices == nil)
				msg = [NSString stringWithFormat:@"This plate will range over the number of characters in a character matrix attached to the Model Tool. Currently, this plate ranges over %d characters.", numSites-numExcludedSites];
			else 
				msg = [NSString stringWithFormat:@"This plate will range over the number of characters in a character matrix attached to the Model Tool. However, the plate is contained by another plate that also ranges over character matrices. This is inconsistant, because there is only one character matrix attached to the Model Tool."];
			[msgLabelField setStringValue:msg];
			}
		else 
			{
			ParmPlate* idOfPlateRangingOverMatrices = [self isOnPlateWithIndexSource:SRC_NUMBER_MATRICES];
			if (idOfPlateRangingOverMatrices != nil)
				{
				NSString* msg = [NSString stringWithFormat:@"This plate will range over the number of characters in the %c-th character matrix attached to the Model Tool.", [idOfPlateRangingOverMatrices indexLetter]];
				[msgLabelField setStringValue:msg];
				}
			else 
				{
				int numAssignedPlates = [self numAssignedPlates];
				NSString* msg;
				if (numAssignedPlates == 0)
					msg = @"This plate will range over the number of characters in one of several character matrices attached to the Model Tool. Currently, this plate is not contained by any plates that could be configured to range over character matrices";
				else if (numAssignedPlates == 1)
					msg = @"This plate will range over the number of characters in one of several character matrices attached to the Model Tool. Currently, this plate is not contained by a plate that ranges over character matrices. However, there is one plate containing this one that could be configured to range over character matrices";
				else 
					msg = [NSString stringWithFormat:@"This plate will range over the number of characters in one of several character matrices attached to the Model Tool. Currently, this plate is not contained by a plate that ranges over character matrices. However, there are %d plates containing this one that could be configured to range over character matrices", numAssignedPlates];
				[msgLabelField setStringValue:msg];
				}
			}
		}
	else if ( indexSource == SRC_NUMBER_TAXA )
		{
		int numMatrices = [self numAttachedMatrices];
		if (numMatrices == 0)
			{
			ParmPlate* idOfPlateRangingOverMatrices = [self isOnPlateWithIndexSource:SRC_NUMBER_MATRICES];
			if (idOfPlateRangingOverMatrices == nil)
				[msgLabelField setStringValue:@"This plate will range over the number of taxa in a character matrix attached to the Model Tool. Currently, the Model Tool does not have a data matrix attached to it."];
			else 
				{
				NSString* msg = [NSString stringWithFormat:@"This plate will range over the number of taxa in the %c-th character matrix attached to the Model Tool. Currently, the Model Tool does not have data matrices attached to it.", [idOfPlateRangingOverMatrices indexLetter]];
				[msgLabelField setStringValue:msg];
				}
			}
		else if (numMatrices == 1)
			{
			int numTaxa = [self numTaxaForMatrix:0];
			int numExcludedTaxa = [self numExcludedTaxaForMatrix:0];
			ParmPlate* idOfPlateRangingOverMatrices = [self isOnPlateWithIndexSource:SRC_NUMBER_MATRICES];
            NSString* msg;
			if (idOfPlateRangingOverMatrices != nil)
				msg = [NSString stringWithFormat:@"This plate will range over the number of taxa in a character matrix attached to the Model Tool. Currently, this plate ranges over %d taxa.", numTaxa-numExcludedTaxa];
			else 
				msg = [NSString stringWithFormat:@"This plate will range over the number of taxa in a character matrix attached to the Model Tool. However, the plate is contained by another plate that also ranges over character matrices. This is inconsistant, because there is only one character matrix attached to the Model Tool."];
			[msgLabelField setStringValue:msg];
			}
		else 
			{
			ParmPlate* idOfPlateRangingOverMatrices = [self isOnPlateWithIndexSource:SRC_NUMBER_MATRICES];
			if (idOfPlateRangingOverMatrices != nil)
				{
				NSString* msg = [NSString stringWithFormat:@"This plate will range over the number of taxa in the %c-th character matrix attached to the Model Tool.", [idOfPlateRangingOverMatrices indexLetter]];
				[msgLabelField setStringValue:msg];
				}
			else 
				{
				int numAssignedPlates = [self numAssignedPlates];
				NSString* msg;
				if (numAssignedPlates == 0)
					msg = @"This plate will range over the number of taxa in one of several character matrices attached to the Model Tool. Currently, this plate is not contained by any plates that could be configured to range over character matrices";
				else if (numAssignedPlates == 1)
					msg = @"This plate will range over the number of taxa in one of several character matrices attached to the Model Tool. Currently, this plate is not contained by a plate that ranges over character matrices. However, there is one plate containing this one that could be configured to range over character matrices";
				else 
					msg = [NSString stringWithFormat:@"This plate will range over the number of taxa in one of several character matrices attached to the Model Tool. Currently, this plate is not contained by a plate that ranges over character matrices. However, there are %d plates containing this one that could be configured to range over character matrices", numAssignedPlates];
				[msgLabelField setStringValue:msg];
				}
			}
		}
	
	// resize panel
	[self resizeWindowTo:panelHeight];
}

- (void)rememberSettings {
    
    startingIndexLetter = indexLetter;
    startingIndexSource = indexSource;
	startingIndexValue  = indexValue;
    startingNumElements = numElements;
}

- (void)showControlPanel {
    
    NSPoint p = [self originForControlWindow:[self window]];
    [[self window] setFrameOrigin:p];
    [self rememberSettings];
    [self refreshWindow];
	[self showWindow:self];
	[[self window] makeKeyAndOrderFront:nil];
    [NSApp runModalForWindow:[self window]];
}

- (void)updateParameterSettings {
    
    NSLog(@"in updateParameterSettings");
    
	// set the index for the plate
	char possIndices[26] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
	int idx = (int)[indexSelector indexOfSelectedItem];
	[self setIndexLetter:possIndices[idx]];
	
	// set the source for the index
	[self setIndexSource:indexSource];
	[self setNumElements:indexValue];
	
    // refresh the display
    [self refreshWindow];
    
	//[super updateParameterSettings];
}

- (NSString*)xibName {
    
	NSString* xn = @"ControlWindowPlate";
	return xn;
}

@end
