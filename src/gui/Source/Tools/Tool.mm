#import "AnalysisDocument.h"
#import "AnalysisView.h"
#import "Connection.h"
#import "Inlet.h"
#import "InOutlet.h"
#import "Outlet.h"
#import "RevBayes.h"
#import "Tool.h"
#import "YRKSpinningProgressIndicator.h"


@implementation Tool

@synthesize flagCount;
@synthesize touchOnRevival;

- (void)addInletOfColor:(NSColor*)c {

	Inlet* il = [[Inlet alloc] initWithTool:self];
	[il setToolColor:c];
	[inlets addObject:il];
    [il release];
	[self setInletLocations];
}

- (void)addOutletOfColor:(NSColor*)c {

	Outlet* ol = [[Outlet alloc] initWithTool:self];
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
    
	[aCoder encodeObject:inlets            forKey:@"inlets"];
	[aCoder encodeObject:outlets           forKey:@"outlets"];
	[aCoder encodeInt:flagCount            forKey:@"flagCount"];
	[aCoder encodeBool:touchOnRevival      forKey:@"touchOnRevival"];
	
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

- (Tool*)getToolOfInlet:(Inlet*)inlt {

    Connection* c = [inlt connection];
    if ( c == nil )
        return nil;
    Tool* t = [[c outlet] toolOwner];
    return t;
}

- (Tool*)getToolOfInletIndexed:(int)idx {

    Inlet* il = [inlets objectAtIndex:idx];
    Connection* c = [il connection];
    if (c == nil)
        return nil;
    Tool* t = [[c outlet] toolOwner];
    return t;
}

- (id)init {

    self = [self initWithScaleFactor:1.0];
    return self;
}

- (id)initWithScaleFactor:(float)sf {

    if ( (self = [super initWithScaleFactor:sf]) ) 
		{
        progressIndicator = nil;
        inlets            = [[NSMutableArray alloc] init];
        outlets           = [[NSMutableArray alloc] init];
		flagCount         = 0;
		touchOnRevival    = NO;
		}
    return self;
}

- (id)initWithScaleFactor:(float)sf andWindowNibName:(NSString*)wNibName {
    
    if ( (self = [super initWithScaleFactor:sf andWindowNibName:wNibName]) ) 
        {
        progressIndicator = nil;
        inlets            = [[NSMutableArray alloc] init];
        outlets           = [[NSMutableArray alloc] init];
		flagCount         = 0;
		touchOnRevival    = NO;
        }
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
        progressIndicator = nil;
		inlets            = [aDecoder decodeObjectForKey:@"inlets"];
		outlets           = [aDecoder decodeObjectForKey:@"outlets"];
		flagCount         = [aDecoder decodeIntForKey:@"flagCount"];
		touchOnRevival    = [aDecoder decodeBoolForKey:@"touchOnRevival"];
        [inlets retain];
        [outlets retain];
		}
	return self;
}

- (void)initializeImage {

}

- (Inlet*)inletIndexed:(int)idx {

	return [inlets objectAtIndex:idx];
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

- (Outlet*)outletIndexed:(int)idx {

	return [outlets objectAtIndex:idx];
}

- (void)removeAllConnections {

    // keep track of the connections to remove and the tools that are affected by the removal
    NSMutableArray* connectionsToRemove = [NSMutableArray arrayWithCapacity:1];
    NSMutableArray* myToolSet = [NSMutableArray arrayWithCapacity:0];
        
    // add all of the connections to the inlets to the map of connections to remove
	NSEnumerator* enumerator = [inlets objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
        {
        for (int i=0; i<[element numberOfConnections]; i++)
            {
            Connection* c = [element connectionWithIndex:i];
            if ( [connectionsToRemove containsObject:c] == NO )
                [connectionsToRemove addObject:c];
            }
        }
    
    // add all of the connections of the outlets to the map of connections to remove
	enumerator = [outlets objectEnumerator];
	while ( (element = [enumerator nextObject]) )
        {
        for (int i=0; i<[element numberOfConnections]; i++)
            {
            Connection* c = [element connectionWithIndex:i];
            if ( [connectionsToRemove containsObject:c] == NO )
                [connectionsToRemove addObject:c];
            Tool* t = [[c inlet] toolOwner];
            [myToolSet addObject:t];
            }
        }
        
    // now we remove the connections
    enumerator = [connectionsToRemove objectEnumerator];
	while ( (element = [enumerator nextObject]) )
        {
        Outlet* theOutlet = [element outlet];
        [theOutlet removeConnection:element];
        }
        
    // finally, we update the tools downstream from this tool
	enumerator = [myToolSet objectEnumerator];
	while ( (element = [enumerator nextObject]) )
        {
        Tool* t = element;
        [t updateForChangeInState];
        }
}

- (void)removeAllConnectionsToInlets {
            
    // add all of the connections to the inlets to the map of connections to remove
	NSEnumerator* enumerator = [inlets objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
        {
        for (int i=0; i<[element numberOfConnections]; i++)
            {
            Connection* c = [element connectionWithIndex:i];
            Outlet* theOutlet = [c outlet];
            [theOutlet removeConnection:c];
            }
        }
        
    // finally, signal tools downstream from this tool
    [self updateForChangeInState];
}

- (void)removeAllConnectionsToOutlets {
    
    // we also keep track of tools downstream from the tool from which connections are removed
    // so we can signal them to update their state
    NSMutableArray* myToolSet = [NSMutableArray arrayWithCapacity:0];
            
    // add all of the connections of the outlets to the map of connections to remove
	NSEnumerator* enumerator = [outlets objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
        {
        for (int i=0; i<[element numberOfConnections]; i++)
            {
            Connection* c = [element connectionWithIndex:i];
            Outlet* theOutlet = element;
            Tool* t = [[c inlet] toolOwner];
            [myToolSet addObject:t];
            [theOutlet removeConnection:c];
            }
        }
                
    // finally, we update the tools downstream from this tool
	enumerator = [myToolSet objectEnumerator];
	while ( (element = [enumerator nextObject]) )
        {
        Tool* t = element;
        [t updateForChangeInState];
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
    
    NSMutableDictionary* myMap = [NSMutableDictionary dictionaryWithCapacity:0];
            
    // add all of the connections to the inlets to the map of connections to remove
	NSEnumerator* enumerator = [inlets objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
        {
        if ( [element toolColor] == c )
            {
            for (int i=0; i<[element numberOfConnections]; i++)
                {
                Connection* c = [element connectionWithIndex:i];
                Outlet* theOutlet = [c outlet];
                [myMap setObject:theOutlet forKey:c];
                }
            }
        }
            
    // now we remove the connections
    NSEnumerator* keyEnumerator = [myMap keyEnumerator];
    id key;
	while ( (key = [keyEnumerator nextObject]) )
        {
        Connection* c = key;
        Outlet* theOutlet = [key objectForKey:key];
        [theOutlet removeConnection:c];
        }
        
    // finally, signal tools downstream from this tool
    [self updateForChangeInState];
}

- (NSMutableAttributedString*)sendTip {

    NSString* myTip = [NSString stringWithString:@" This is a tool "];

    NSDictionary *attr = [NSDictionary 
                 dictionaryWithObjects:[NSArray arrayWithObjects:[NSFont fontWithName:@"Lucida Grande Bold" size:14.0], [[NSColor whiteColor] colorWithAlphaComponent:1.0], nil] 
                               forKeys: [NSArray arrayWithObjects:NSFontAttributeName,NSForegroundColorAttributeName, nil]];
    
    NSMutableAttributedString* attrString = [[NSMutableAttributedString alloc] initWithString:myTip attributes:attr];

    return attrString;
}

- (void)setAnalysisView:(AnalysisView*)av {

    myAnalysisView = av;
}

- (void)setImageWithSize:(NSSize)s {

	[self setItemSize:s];
}

- (void)setInletLocations {

    int n = [self numInlets];
	for (int i=0; i<n; i++)
		{
		Inlet* il = (Inlet*)[self inletIndexed:i];
        NSPoint p = NSMakePoint( 0.0, (double)(i+1)/(n+1) );
        [il setPosition:p];
        }
}

- (void)setOutletLocations {

    int n = [self numOutlets];
	for (int i=0; i<n; i++)
		{
		Outlet* il = [self outletIndexed:i];
        NSPoint p = NSMakePoint( 1.0, (double)(i+1)/(n+1) );
        [il setPosition:p];
        }
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
        for (int i=0; i<[element numberOfConnections]; i++)
            {
            Connection* c = [element connectionWithIndex:i];
            Tool* t = [[c inlet] toolOwner];
            [t updateForChangeInState];
            }
        }
}

- (void)startProgressIndicator {

    // get the size and position of the frame to hold the progress indicator
    float scaleFactor = [myAnalysisView scaleFactor];
    NSRect tRect;
    tRect.origin = [self itemLocation];
    tRect.size   = NSMakeSize(ITEM_IMAGE_SIZE*scaleFactor, ITEM_IMAGE_SIZE*scaleFactor);
    [myAnalysisView transformToBottomLeftCoordinates:(&tRect.origin)];  
    float margine = tRect.size.width * 0.10;          
    
    // instantiate the progress indicator and set the views correctly
    NSView* superView = myAnalysisView;
    NSRect frame = NSMakeRect(tRect.origin.x + margine, tRect.origin.y + margine, tRect.size.width - 2.0*margine, tRect.size.height - 2.0*margine);
    if (progressIndicator != nil)
        [progressIndicator release];
    progressIndicator = [[YRKSpinningProgressIndicator alloc] initWithFrame:frame];
    [superView addSubview:progressIndicator];
    [progressIndicator release];
    
    // set properties of the progress indicator and start it up
    [progressIndicator setUsesThreadedAnimation:YES];
    [progressIndicator setIndeterminate:YES];
    [progressIndicator setColor:[NSColor blueColor]];
    [progressIndicator startAnimation:self];
}

- (void)stopProgressIndicator {

    [progressIndicator stopAnimation:self];
    [progressIndicator removeFromSuperview];
    progressIndicator = nil;
}

- (void)updateForChangeInState {

    NSLog(@"updateForChangeOfState in %@", self);
	NSEnumerator* enumerator = [outlets objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
        {
        for (int i=0; i<[element numberOfConnections]; i++)
            {
            Connection* c = [element connectionWithIndex:i];
            Tool* t = [[c inlet] toolOwner];
            [t updateForChangeInState];
            NSLog(@"%@ is signaling %@ of a change in state", self, t);
            }
        }
}

@end
