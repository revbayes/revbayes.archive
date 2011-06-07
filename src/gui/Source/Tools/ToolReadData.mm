#import "InOutlet.h"
#import "RbData.h"
#import "RevBayes.h"
#import "ToolReadData.h"
#import "WindowControllerCharacterMatrix.h"
#import "WindowControllerReadData.h"



@implementation ToolReadData

@synthesize dataAlignment;
@synthesize dataFormat;
@synthesize dataInterleaved;
@synthesize dataType;
@synthesize dataTypeSimulated;
@synthesize fileName;
@synthesize matrixType;
@synthesize numberOfOutlets;
@synthesize numberOfCharacters;
@synthesize numberOfTaxa;
@synthesize pathName;
@synthesize dataMatrices;

- (void)addMatrix:(RbData*)m {

	[dataMatrices addObject:m];
	[m release];
    hasInspectorInfo = YES;
}

- (void)awakeFromNib {

}

- (void)closeControlPanel {

    [NSApp stopModal];
	[controlWindow close];
    [self setInletsAndOutlets];
}

- (RbData*)dataMatrixIndexed:(int)i {

	if (i > [dataMatrices count])
		return nil;
	return [dataMatrices objectAtIndex:i];
}

- (void)dealloc {

	[fileName release];
	[pathName release];
	[controlWindow release];
	[dataMatrices release];
    if (dataInspector != nil)
        [dataInspector release];
	[super dealloc];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {

    [aCoder encodeObject:fileName        forKey:@"fileName"];
    [aCoder encodeObject:pathName        forKey:@"pathName"];
	[aCoder encodeInt:dataAlignment      forKey:@"dataAlignment"];
	[aCoder encodeInt:dataFormat         forKey:@"dataFormat"];
	[aCoder encodeInt:dataInterleaved    forKey:@"dataInterleaved"];
	[aCoder encodeInt:dataType           forKey:@"dataType"];
	[aCoder encodeInt:dataTypeSimulated  forKey:@"dataTypeSimulated"];
	[aCoder encodeInt:numberOfOutlets    forKey:@"numberOfOutlets"];
	[aCoder encodeInt:numberOfCharacters forKey:@"numberOfCharacters"];
	[aCoder encodeInt:numberOfTaxa       forKey:@"numberOfTaxa"];
	[aCoder encodeInt:matrixType         forKey:@"matrixType"];
	[aCoder encodeObject:dataMatrices    forKey:@"dataMatrices"];

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
		numberOfOutlets    = 1;
		matrixType         = 0;
		
		// initialize the array holding the data matrices
		dataMatrices = [[NSMutableArray alloc] init];

		// initialize the control window and the data inspector
		controlWindow = [[WindowControllerReadData alloc] initWithTool:self];
        dataInspector = nil;
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
		numberOfOutlets    = [aDecoder decodeIntForKey:@"numberOfOutlets"];
		numberOfCharacters = [aDecoder decodeIntForKey:@"numberOfCharacters"];
		numberOfTaxa       = [aDecoder decodeIntForKey:@"numberOfTaxa"];
		matrixType         = [aDecoder decodeIntForKey:@"matrixType"];
		dataMatrices       = [aDecoder decodeObjectForKey:@"dataMatrices"];
        [fileName retain];
        [pathName retain];
		[dataMatrices retain];
        
        // check to see if there are any data matrices stored in the tool
        if ([dataMatrices count] > 0)
            hasInspectorInfo = YES;

		// initialize the control window
		controlWindow = [[WindowControllerReadData alloc] initWithTool:self];
        dataInspector = nil;
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

- (NSString*)nameOfMatrixIndexed:(int)idx {

    return [[dataMatrices objectAtIndex:idx] name];
}

- (int)numDataMatrices {

	return (int)[dataMatrices count];
}

- (void)removeAllDataMatrices {

	[dataMatrices removeAllObjects];
    hasInspectorInfo = NO;
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

- (void)setInletsAndOutlets {

	if ( [controlWindow numberOfOutlets] != [self numOutlets] )
		{
		[self removeAllInletsAndOutlets];
		for (int i=0; i<[controlWindow numberOfOutlets]; i++)
			{
			if ([controlWindow dataAlignment] == 0)
				[self addOutletOfColor:[NSColor greenColor]];
			else 
				[self addOutletOfColor:[NSColor cyanColor]];
			}
		}
    if ( [controlWindow dataAlignment] == 0 )
        {
        if ( [[self outletIndexed:0] toolColor] != [NSColor greenColor] )
            {
            [self removeAllInletsAndOutlets];
			for (int i=0; i<[controlWindow numberOfOutlets]; i++)
				[self addOutletOfColor:[NSColor greenColor]];
            }
        }
    else
        {
        if ( [[self outletIndexed:0] toolColor] != [NSColor cyanColor] )
            {
            [self removeAllInletsAndOutlets];
			for (int i=0; i<[controlWindow numberOfOutlets]; i++)
				[self addOutletOfColor:[NSColor cyanColor]];
            }
        }
}

- (void)showControlPanel {

    NSPoint p = [self originForControlWindow:[controlWindow window]];
    [[controlWindow window] setFrameOrigin:p];
	[controlWindow showWindow:self];
	[[controlWindow window] makeKeyAndOrderFront:nil];
    [NSApp runModalForWindow:[controlWindow window]];
}

- (void)showInspectorPanel {

    if (dataInspector != nil)
        [dataInspector release];
    dataInspector = [[WindowControllerCharacterMatrix alloc] initWithTool:self];
    [[dataInspector window] center];
    [dataInspector showWindow:self];
}

@end
