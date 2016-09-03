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
@synthesize isCurrentlyExecuting;
@synthesize isLoop;
@synthesize isVisited;
@synthesize isDirty;
@synthesize workspaceName;

- (void)addInletOfColor:(NSColor*)c {

	Inlet* il = [[Inlet alloc] initWithTool:self];
	[il setToolColor:c];
	[inlets addObject:il];
	[self setInletLocations];
}

- (void)addOutletOfColor:(NSColor*)c {

	Outlet* ol = [[Outlet alloc] initWithTool:self];
	[ol setToolColor:c];
	[outlets addObject:ol];
	[self setOutletLocations];
}

- (BOOL)areAnyParentsDirty {

    for (int i=0; i<[self numInlets]; i++)
        {
        Inlet* theInlet = [self inletIndexed:i];
        Tool* t = [self getParentToolOfInlet:theInlet];
        if ( [t isDirty] == YES)
            return YES;
        }
    return NO;
}

- (void)awakeFromNib {

}

- (BOOL)checkForExecute {

    return YES;
}

- (NSColor*)colorOfInletIndexed:(int)idx {

	return [[inlets objectAtIndex:idx] toolColor];
}

- (NSColor*)colorOfOutletIndexed:(int)idx {

	return [[outlets objectAtIndex:idx] toolColor];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {
    
	[aCoder encodeObject:inlets        forKey:@"inlets"];
	[aCoder encodeObject:outlets       forKey:@"outlets"];
	[aCoder encodeInt:flagCount        forKey:@"flagCount"];
	[aCoder encodeBool:touchOnRevival  forKey:@"touchOnRevival"];
    [aCoder encodeBool:isLoop          forKey:@"isLoop"];
    [aCoder encodeObject:workspaceName forKey:@"workspaceName"];
	
    [super encodeWithCoder:aCoder];
}

- (BOOL)execute {
    
    [self startProgressIndicator];
    [self stopProgressIndicator];
    return YES;
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

- (NSMutableArray*)getChildrenTools {

    NSMutableArray* children = [[NSMutableArray alloc] init];
    for (Outlet* ol in outlets)
        {
        for (int i=0; i<[ol numberOfConnections]; i++)
            {
            Connection* c = [ol connectionWithIndex:i];
            Inlet* il = [c inlet];
            Tool* t = [il toolOwner];
            [children addObject:t];
            }
        }
    return children;
}

- (NSMutableArray*)getParentTools {

    NSMutableArray* parents = [[NSMutableArray alloc] init];
    for (Inlet* il in inlets)
        {
        for (int j=0; j<[il numberOfConnections]; j++)
            {
            Connection* c = [il connectionWithIndex:j];
            Tool* t = [[c outlet] toolOwner];
            [parents addObject:t];
            }
        }
    return parents;
}

- (Tool*)getChildToolOfOutletIndexed:(int)idx {

    return nil;
}

- (Tool*)getParentToolOfInlet:(Inlet*)inlt {

    Connection* c = [inlt connection];
    if ( c == nil )
        return nil;
    Tool* t = [[c outlet] toolOwner];
    return t;
}

- (Tool*)getParentToolOfInletIndexed:(int)idx {

    Inlet* il = [inlets objectAtIndex:idx];
    Connection* c = [il connection];
    if (c == nil)
        return nil;
    Tool* t = [[c outlet] toolOwner];
    return t;
}

- (BOOL)hasParents {

    int numParents = 0;
	NSEnumerator* enumerator = [inlets objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
        {
        numParents += [element numberOfConnections];
        }

    if (numParents > 0)
        return YES;
    return NO;
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
        workspaceName     = @"";
		flagCount         = 0;
		touchOnRevival    = NO;
        isLoop            = NO;
        isDirty           = NO;
		}
    return self;
}

- (id)initWithScaleFactor:(float)sf andWindowNibName:(NSString*)wNibName {
    
    if ( (self = [super initWithScaleFactor:sf andWindowNibName:wNibName]) ) 
        {
        progressIndicator = nil;
        inlets            = [[NSMutableArray alloc] init];
        outlets           = [[NSMutableArray alloc] init];
        workspaceName     = @"";
		flagCount         = 0;
		touchOnRevival    = NO;
        isLoop            = NO;
        isDirty           = NO;
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
        isLoop            = [aDecoder decodeBoolForKey:@"isLoop"];
        workspaceName     = [aDecoder decodeObjectForKey:@"workspaceName"];
        isDirty           = NO;
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

- (BOOL)isSomeParentVisited {

	NSEnumerator* enumerator = [inlets objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
        {
        for (int i=0; i<[element numberOfConnections]; i++)
            {
            Connection* c = [element connectionWithIndex:i];
            Tool* t = [[c outlet] toolOwner];
            if ( [t isVisited] == YES )
                return YES;
            }
        }
    return NO;
}

- (void)lockView {

    [myAnalysisView setIsLocked:YES];
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

- (BOOL)performToolTask {

    [self startProgressIndicator];
    [self stopProgressIndicator];
    return YES;
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

- (void)removeFilesFromTemporaryDirectory {

    NSString* temporaryDirectory = NSTemporaryDirectory();
    NSFileManager* fm = [[NSFileManager alloc] init];
    NSDirectoryEnumerator* en = [fm enumeratorAtPath:temporaryDirectory];    
    NSString* file;
    while ( (file = [en nextObject]) ) 
        {
        NSError* err = nil;
        BOOL res = [fm removeItemAtPath:[temporaryDirectory stringByAppendingPathComponent:file] error:&err];
        if (!res && err) 
            {
            }
        }
}

- (void)removeInletOfColor:(NSColor*)c {
    
#   if 1

    NSMutableArray* myArray = [NSMutableArray arrayWithCapacity:0];

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
                [myArray addObject:c];
                }
            }
        }

    enumerator = [myArray objectEnumerator];
	while ( (element = [enumerator nextObject]) )
        {
        Connection* c = element;
        Outlet* theOutlet = [c outlet];
        [theOutlet removeConnection:c];
        }

#   else

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
#   endif
}

- (BOOL)resolveStateOnWindowOK {

    return YES;
}

- (NSMutableAttributedString*)sendTip {

    NSString* myTip = @" This is a tool ";

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

- (void)setStatusMessage:(NSString*)msg {

    NSDictionary* info = [NSDictionary dictionaryWithObject:msg forKey:@"statusMessage"];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"RbStatusBarMessage" object:self userInfo:info];
}

- (void)showControlPanel {

}

- (void)showInspectorPanel {

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
    progressIndicator = [[YRKSpinningProgressIndicator alloc] initWithFrame:frame];
    [superView addSubview:progressIndicator];
    
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

- (NSString*)toolName {

    return @"Unnamed tool";
}

- (void)unlockView {

    [myAnalysisView setIsLocked:NO];
}

- (void)updateChildrenTools {


    [myAnalysisView updateToolsDownstreamFromTool:self];
}

- (void)updateDisplay {

    [myAnalysisView setNeedsDisplay:YES];
}

- (void)updateForChangeInParent {

}

@end
