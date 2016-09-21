#import "RbData.h"
#import "RbDataCell.h"
#import "RevBayes.h"
#import "ToolBootstrap.h"
#import "ToolData.h"
#import "WindowControllerBootstrap.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "DistributionUniform.h"



@implementation ToolBootstrap

- (void)bootstrap {

    [self removeAllDataMatrices];
    [self removeDataInspector];

    // find the parent tool
    NSMutableArray* parents = [self getParentTools];
    if ([parents count] != 1)
        return;
   if ( [[parents objectAtIndex:0] isKindOfClass:[ToolData class]] == NO )
        return;
    ToolData* dataTool = (ToolData*)[parents objectAtIndex:0];
    
    // get the data matrices in the parent tool
    NSMutableArray* alignedData = [NSMutableArray arrayWithCapacity:1];
    for (int i=0; i<[dataTool numDataMatrices]; i++)
        {
        if ( [[dataTool dataMatrixIndexed:i] isHomologyEstablished] == YES )
            [alignedData addObject:[dataTool dataMatrixIndexed:i]];
        }
    if ( [alignedData count] == 0 )
        return;
    
    // get the global random number generator
    RevBayesCore::RandomNumberGenerator* rng = RevBayesCore::GLOBAL_RNG;
    
    // make new data matrices in this tool that are bootstrapped from the parent tool
    for (RbData* dOriginal in alignedData)
        {
        RbData* dNew = [[RbData alloc] initWithRbData:dOriginal];
        
        [dNew includeAllCharacters];
        for (int i=0; i<[dOriginal numExcludedCharacters]; i++)
            [dNew removeObervationFromEnd];
        [dNew includeAllTaxa];
        NSArray* excludedTaxa = [dOriginal getExcludedTaxa];
        for (NSString* et in excludedTaxa)
            [dNew removeTaxonNamed:et];
        [self addMatrix:dNew];
        
        int ncOriginal = [dOriginal numCharacters];
        for (int c=0; c<ncOriginal - [dOriginal numExcludedCharacters]; c++)
            {
            int whichChar = 0;
            do
                {
                whichChar = (int)(RevBayesCore::RbStatistics::Uniform::rv(*rng) * ncOriginal);
                } while ([dOriginal isCharacterExcluded:whichChar] == YES);
                
            for (int n=0; n<[dNew numTaxa]; n++)
                {
                RbDataCell* cOriginal = [dOriginal cellWithRow:n andColumn:whichChar];
                RbDataCell* cNew      = [dNew      cellWithRow:n andColumn:c        ];
                [cNew setIsDiscrete:[cOriginal isDiscrete]];
                [cNew setDataType:  [cOriginal dataType  ]];
                [cNew setNumStates: [cOriginal numStates ]];
                [cNew setIsAmbig:   [cOriginal isAmbig   ]];
                [cNew setIsGapState:[cOriginal isGapState]];
                [cNew setVal:       [NSNumber numberWithInt:[[cOriginal val] intValue]]];
                }
            }
        }

    [self makeDataInspector];
    [self updateDisplay];
}

- (BOOL)checkForExecute:(NSMutableDictionary*)errors {

    /* This tool requires that a parent tool have at least one data set. */

    // find the parent tool
    NSMutableArray* parents = [self getParentTools];
    if ([parents count] == 0)
        {
        NSString* obId = [NSString stringWithFormat:@"%p", self];
        [errors setObject:@"Bootstrap Tool does not have a parent" forKey:obId];
        return NO;
        }
    else if ([parents count] > 1)
        {
        NSString* obId = [NSString stringWithFormat:@"%p", self];
        [errors setObject:@"Bootstrap Tool has too many parents" forKey:obId];
        return NO;
        }
    if ( [[parents objectAtIndex:0] isKindOfClass:[ToolData class]] == NO )
        {
        NSString* obId = [NSString stringWithFormat:@"%p", self];
        [errors setObject:@"Bootstrap Tool does not have a data tool as a parent" forKey:obId];
        return NO;
        }
    ToolData* dataTool = (ToolData*)[parents objectAtIndex:0];
    
    // check the data matrices in the parent tool
    if ( [dataTool numAligned] == 0)
        {
        NSString* obId = [NSString stringWithFormat:@"%p", self];
        [errors setObject:@"The parent of the Bootstrap Tool does not have any data" forKey:obId];
        return NO;
        }
    if ( [dataTool numUnaligned] > 0)
        {
        NSString* obId = [NSString stringWithFormat:@"%p", self];
        [errors setObject:@"The parent of the Bootstrap Tool contains unaligned data" forKey:obId];
        return NO;
        }

    return YES;
}

- (BOOL)checkForWarning:(NSMutableDictionary*)warnings {

    return YES;
}

- (void)closeControlPanel {

    [NSApp stopModal];
	[controlWindow close];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {
    
	[super encodeWithCoder:aCoder];
}

- (BOOL)execute {
    
    [self setStatusMessage:@"Bootstrapping data matrix"];
    [self bootstrap];
    [self setStatusMessage:@""];

    return [super execute];
}

- (id)init {

    self = [self initWithScaleFactor:1.0];
    return self;
}

- (id)initWithScaleFactor:(float)sf {

    if ( (self = [super initWithScaleFactor:sf]) ) 
		{
        hasController = NO;
        
		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];
		
		// initialize the inlet/outlet information
		[self addInletOfColor:[NSColor greenColor]];
		[self addOutletOfColor:[NSColor greenColor]];
        [self setInletLocations];
        [self setOutletLocations];
            
		// initialize the control window
		controlWindow = [[WindowControllerBootstrap alloc] initWithTool:self];
		}
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
        // initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];
            
        // initialize the control window
		controlWindow = [[WindowControllerBootstrap alloc] initWithTool:self];
		}
	return self;
}

- (void)initializeImage {

    itemImage[0] = [NSImage imageNamed:@"Tool_Bootstrap25.icns"];
    itemImage[1] = [NSImage imageNamed:@"Tool_Bootstrap50.icns"];
    itemImage[2] = [NSImage imageNamed:@"Tool_Bootstrap75.icns"];
    itemImage[3] = [NSImage imageNamed:@"Tool_Bootstrap100.icns"];
    itemImage[4] = [NSImage imageNamed:@"Tool_Bootstrap125.icns"];
    itemImage[5] = [NSImage imageNamed:@"Tool_Bootstrap150.icns"];
    itemImage[6] = [NSImage imageNamed:@"Tool_Bootstrap200.icns"];
    itemImage[7] = [NSImage imageNamed:@"Tool_Bootstrap400.icns"];

	float s[8] = { 0.25, 0.50, 0.75, 1.0, 1.25, 1.50, 2.0, 4.0 };
	for (int i=0; i<8; i++)
        [itemImage[i] setSize:NSMakeSize(ITEM_IMAGE_SIZE*s[i], ITEM_IMAGE_SIZE*s[i])];
}

- (void)prepareForExecution {

}

- (NSMutableAttributedString*)sendTip {

    NSString* myTip = @" Bootstrap Tool ";
    myTip = [myTip stringByAppendingFormat:@"\n # Matrices: %lu ", [self numDataMatrices]];
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

    return @"Bootstrap";
}

- (void)updateForChangeInParent {

    [self bootstrap];
}

@end
