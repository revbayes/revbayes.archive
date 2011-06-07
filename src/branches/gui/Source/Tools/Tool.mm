#import "AnalysisDocument.h"
#import "AnalysisView.h"
#import "InOutlet.h"
#import "RevBayes.h"
#import "Tool.h"



@implementation Tool

@synthesize flagCount;
@synthesize touchOnRevival;

- (void)addInletOfColor:(NSColor*)c {

	InOutlet* il = [[InOutlet alloc] initWithTool:self];
	[il setAmInlet:YES];
	[il setToolColor:c];
	[inlets addObject:il];
    [il release];
	[self setInletLocations];
}

- (void)addOutletOfColor:(NSColor*)c {

	InOutlet* ol = [[InOutlet alloc] initWithTool:self];
	[ol setAmInlet:NO];
	[ol setToolColor:c];
	[outlets addObject:ol];
    [ol release];
	[self setOutletLocations];
}

- (void)awakeFromNib {

}

- (NSColor*)colorOfInletIndexed:(int)idx {

	return [[inlets objectAtIndex:idx] toolColor];
}

- (NSColor*)colorOfOutletIndexed:(int)idx {

	return [[outlets objectAtIndex:idx] toolColor];
}

- (void)dealloc {

	[inlets release];
	[outlets release];
	[super dealloc];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {
    
	[aCoder encodeObject:inlets       forKey:@"inlets"];
	[aCoder encodeObject:outlets      forKey:@"outlets"];
	[aCoder encodeInt:flagCount       forKey:@"flagCount"];
	[aCoder encodeBool:touchOnRevival forKey:@"touchOnRevival"];
	
    [super encodeWithCoder:aCoder];
}

- (InOutlet*)findInOutletWithColor:(NSColor*)c {

	NSEnumerator* enumerator = [inlets objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
		if ([element toolColor] == c)
			return element;
		}
    
	enumerator = [outlets objectEnumerator];
	while ( (element = [enumerator nextObject]) )
		{
		if ([element toolColor] == c)
			return element;
		}
		
	return nil;
}

- (Tool*)getToolOfInlet:(InOutlet*)inlt {

    InOutlet* partnerOutlet = [inlt partner];
    Tool* t = [partnerOutlet toolOwner];
    return t;
}

- (Tool*)getToolOfInletIndexed:(int)idx {

    return [self getToolOfInlet:[inlets objectAtIndex:idx]];
}

- (id)init {

    self = [self initWithScaleFactor:1.0];
    return self;
}

- (id)initWithScaleFactor:(float)sf {

    if ( (self = [super initWithScaleFactor:sf]) ) 
		{
        inlets           = [[NSMutableArray alloc] init];
        outlets          = [[NSMutableArray alloc] init];
		flagCount        = 0;
		touchOnRevival   = NO;
		}
    return self;
}


- (id)initWithScaleFactor:(float)sf andWindowNibName:(NSString*)wNibName {
    
    if ( (self = [super initWithScaleFactor:sf andWindowNibName:wNibName]) ) 
    {
        inlets           = [[NSMutableArray alloc] init];
        outlets          = [[NSMutableArray alloc] init];
		flagCount        = 0;
		touchOnRevival   = NO;
    }
    return self;
}


- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
		inlets         = [aDecoder decodeObjectForKey:@"inlets"];
		outlets        = [aDecoder decodeObjectForKey:@"outlets"];
		flagCount      = [aDecoder decodeIntForKey:@"flagCount"];
		touchOnRevival = [aDecoder decodeBoolForKey:@"touchOnRevival"];
        [inlets retain];
        [outlets retain];
		}
	return self;
}

- (void)initializeImage {

}

- (InOutlet*)inletIndexed:(int)idx {

	return [inlets objectAtIndex:idx];
}

- (BOOL)isFullyConnected {

	NSEnumerator* enumerator = [inlets objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
        {
        if ([element partner] == nil)
            return NO;
        }
    
	enumerator = [outlets objectEnumerator];
	while ( (element = [enumerator nextObject]) )
        {
        if ([element partner] == nil)
            return NO;
        }

    return YES;
}

- (int)numInlets {

	return (int)[inlets count];
}

- (int)numInletsOfColor:(NSColor*)c {

	NSEnumerator* enumerator = [inlets objectEnumerator];
	id element;
    int n = 0;
	while ( (element = [enumerator nextObject]) )
        {
        if ( [element toolColor] == c )
            n++;
        }
    return n;
}

- (int)numOutlets {

	return (int)[outlets count];
}

- (int)numOutletsOfColor:(NSColor*)c {

	NSEnumerator* enumerator = [inlets objectEnumerator];
	id element;
    int n = 0;
	while ( (element = [enumerator nextObject]) )
        {
        if ( [element toolColor] == c )
            n++;
        }
    return n;
}

- (NSPoint)originForControlWindow:(NSWindow*)win {

    // get the document frame
    NSDocument* doc = [[NSDocumentController sharedDocumentController] currentDocument];
    NSWindow* docWin = [[[doc windowControllers] objectAtIndex:0] window];
    NSRect docRect = [docWin frame];
    
    // and the controller window's frame
    NSRect winRect = [win frame];
    
    // get the center of the document window
    float x = NSMidX(docRect);
    float y = NSMidY(docRect);
    
    NSPoint p;
    p.x = x - winRect.size.width * 0.5;
    p.y = y - winRect.size.height * 0.5;
    
    return p;
}

- (InOutlet*)outletIndexed:(int)idx {

	return [outlets objectAtIndex:idx];
}

- (void)removeAllConnections {

	NSEnumerator* enumerator = [inlets objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
        {
        [[element partner] setPartner:nil];
        [element setPartner:nil];
        }
    
	enumerator = [outlets objectEnumerator];
	while ( (element = [enumerator nextObject]) )
        {
		Tool* t = [[element partner] toolOwner];
        [[element partner] setPartner:nil];
        [element setPartner:nil];
		[t updateForConnectionChange];
        }
}

- (void)removeAllConnectionsToInlets {

	NSEnumerator* enumerator = [inlets objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
        {
        [[element partner] setPartner:nil];
        [element setPartner:nil];
        }
}

- (void)removeAllConnectionsToOutlets {

	NSEnumerator* enumerator = [outlets objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
        {
		Tool* t = [[element partner] toolOwner];
        [[element partner] setPartner:nil];
        [element setPartner:nil];
		[t updateForConnectionChange];
        }
}

- (void)removeAllInletsAndOutlets {

    [self removeAllConnections];
    [inlets removeAllObjects];
    [outlets removeAllObjects];
}

- (void)removeAllInlets {

	[self removeAllConnectionsToInlets];
	[inlets removeAllObjects];
}

- (void)removeAllOutlets {

	[self removeAllConnectionsToOutlets];
	[outlets removeAllObjects];
}

- (void)removeInletOfColor:(NSColor*)c {
    
    //NSLog(@"inlet list = %@", inlets);
	NSEnumerator* enumerator = [inlets objectEnumerator];
	id element;
    id idOfInletToRemove = nil;
	while ( (element = [enumerator nextObject]) )
        {
        if ( [element toolColor] == c )
            {
            [[element partner] setPartner:nil];
            [element setPartner:nil];
            idOfInletToRemove = element;
            break;
            }
        }
    //NSLog(@"id of guy to remove = %@", idOfInletToRemove);
    if (idOfInletToRemove != nil)
        {
        [inlets removeObject:idOfInletToRemove];
        [self setInletLocations];
        }
}

- (NSMutableAttributedString*)sendTip {

    NSString* myTip = [NSString stringWithString:@" This is a tool "];

    NSDictionary *attr = [NSDictionary 
                 dictionaryWithObjects:[NSArray arrayWithObjects:[NSFont fontWithName:@"Lucida Grande Bold" size:14.0], [[NSColor whiteColor] colorWithAlphaComponent:1.0], nil] 
                               forKeys: [NSArray arrayWithObjects:NSFontAttributeName,NSForegroundColorAttributeName, nil]];
    
    NSMutableAttributedString* attrString = [[NSMutableAttributedString alloc] initWithString:myTip attributes:attr];

    return attrString;
}

- (void)setImageWithSize:(NSSize)s {

	[self setItemSize:s];
    [self setInletLocations];
    [self setOutletLocations];
}

- (void)setInletLocationsForItemSized:(NSSize)s {

    float itemWidth = s.width;
    float inletSize = itemWidth * 0.2;

	int n = [self numInlets];
	NSPoint p = NSZeroPoint;
	float space = (itemWidth - n * inletSize) / (n + 1);
	float altitude = sqrt(inletSize*inletSize - (0.5*inletSize)*(0.5*inletSize));
	
	for (int i=0; i<n; i++)
		{
		InOutlet* il = [self inletIndexed:i];
		float y = p.y + i * (space + inletSize);
		NSRect r;
		r.origin = NSMakePoint( p.x - altitude * BURY_FRACTION, y + space );
		r.size.width = altitude;
		r.size.height = inletSize;
		[il setInOutletRect:r];
		}
}

- (void)setInletLocations {

    [self setInletLocationsForItemSized:itemSize];
}

- (void)setOutletLocationsForItemSized:(NSSize)s {

    float itemWidth = s.width;
    float inletSize = itemWidth * 0.2;

	int n = [self numOutlets];
	NSPoint p = NSZeroPoint;
	float space = (itemWidth - n * inletSize) / (n + 1);
	float altitude = sqrt(inletSize*inletSize - (0.5*inletSize)*(0.5*inletSize));
	
	for (int i=0; i<n; i++)
		{
		InOutlet* ol = [self outletIndexed:i];
		float y = p.y + i * (space + inletSize);
		NSRect r;
		r.origin = NSMakePoint( p.x + itemWidth - altitude * (1.0-BURY_FRACTION), y + space );
		r.size.width = altitude;
		r.size.height = inletSize;
		[ol setInOutletRect:r];
		}
}

- (void)setOutletLocations {

    [self setOutletLocationsForItemSized:itemSize];
}

- (void)showControlPanel {

}

- (void)showInspectorPanel {

}

- (void)signalDownstreamTools {

	NSEnumerator* enumerator = [outlets objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
        {
		Tool* t = [[element partner] toolOwner];
		[t updateForConnectionChange];
        }
}

- (void)updateForConnectionChange {

	NSEnumerator* enumerator = [outlets objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
        {
		Tool* t = [[element partner] toolOwner];
		[t updateForConnectionChange];
        }
}

@end
