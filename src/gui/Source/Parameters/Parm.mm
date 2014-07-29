#import "Parm.h"
#import "ParmPlate.h"
#import "RevBayes.h"
#import "ToolModel.h"



@implementation Parm

@synthesize domain;
@synthesize drawingIndex;
@synthesize parmName;
@synthesize parmType;
@synthesize parmsWindow;
@synthesize myModel;
@synthesize isConstant;
@synthesize nameField;
@synthesize nameLabel;
@synthesize isVector;
@synthesize dimensions;

- (void)addChild:(Parm*)p {

	[children addObject:p];
}

- (void)addParent:(Parm*)p {

	[parents addObject:p];
}

- (void)addToPlate:(ParmPlate*)p {

	if ( [containingPlates containsObject:p] == NO )
		[containingPlates addObject:p];
}

- (ParmPlateTree*)assignedTreePlate {
   
	NSEnumerator* enumerator = [containingPlates objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
		if ( [element parmType] == PARM_TREE_PLATE )
			return element;
		}
	return nil;
}

- (IBAction)cancelAction:(id)sender {

	[self closeControlPanel];
}

- (IBAction)changeName:(id)sender {

    [self setParmName:[nameField stringValue]];
    [self refreshWindow];
}

- (void)closeControlPanel {

    [NSApp stopModal];
	[self close];
}

- (Parm*)getChildIndexed:(int)x {

	return [children objectAtIndex:x];
}

- (Parm*)getParentIndexed:(int)x {

	return [parents objectAtIndex:x];
}

- (Parm*)getParentWithName:(NSString*)str {

	id element;
	NSEnumerator* enumerator = [parents objectEnumerator];
	while ( (element = [enumerator nextObject]) )
        {
        if ( [[element parmName] isEqualToString:str] == YES )
            return element;
        }
    return nil;
}

- (ParmPlate*)getPlateIndexed:(int)x {

	return [containingPlates objectAtIndex:x];
}

- (IBAction)helpAction:(id)sender {

}

- (void)dealloc {

    [[NSNotificationCenter defaultCenter] removeObserver:self];
	/* JPHARC [parents release];
	[children release];
    [parmName release];
	[containingPlates release];*/
	/* JPHARC [super dealloc]; */
}

- (void)encodeWithCoder:(NSCoder *)aCoder {

    [super encodeWithCoder:aCoder];

	[aCoder encodeObject:parents          forKey:@"parents"];
	[aCoder encodeObject:children         forKey:@"children"];
    [aCoder encodeObject:parmName         forKey:@"parmName"];
	[aCoder encodeObject:containingPlates forKey:@"containingPlates"];
    [aCoder encodeInt:parmType            forKey:@"parmType"];
    [aCoder encodeInt:domain              forKey:@"domain"];
    [aCoder encodeInt:drawingIndex        forKey:@"drawingIndex"];
    [aCoder encodeBool:isConstant         forKey:@"isConstant"];
    [aCoder encodeBool:isVector           forKey:@"isVector"];
    [aCoder encodeInt:dimensions          forKey:@"dimensions"];
    if (saveAsModelTemplate == NO)
        [aCoder encodeObject:myModel forKey:@"myModel"]; // only save the model if we are saving the entire analysis
                                                         // otherwise, we fill in the model pointer after importing the
                                                         // model template
}

- (void)initializeImage {

}

- (id)init {

    self = [self initWithScaleFactor:1.0 andTool:nil];
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
		parents          = [aDecoder decodeObjectForKey:@"parents"];
		children         = [aDecoder decodeObjectForKey:@"children"];
        parmName         = [aDecoder decodeObjectForKey:@"parmName"];
		containingPlates = [aDecoder decodeObjectForKey:@"containingPlates"];
        parmType         = [aDecoder decodeIntForKey:@"parmType"];
        domain           = [aDecoder decodeIntForKey:@"domain"];
        drawingIndex     = [aDecoder decodeIntForKey:@"drawingIndex"];
        isConstant       = [aDecoder decodeBoolForKey:@"isConstant"];
        isVector         = [aDecoder decodeBoolForKey:@"isVector"];
        dimensions       = [aDecoder decodeIntForKey:@"dimensions"];
        /* JPHARC [parents retain];
        [children retain];
        [parmName retain];
		[containingPlates retain];*/
        if (saveAsModelTemplate == NO)
            myModel = [aDecoder decodeObjectForKey:@"myModel"];
		}
	return self;
}

- (id)initWithScaleFactor:(float)sf andTool:(ToolModel*)t {

    if ( (self = [super initWithScaleFactor:sf]) ) 
		{
        parents          = [[NSMutableArray alloc] init];
        children         = [[NSMutableArray alloc] init];
        parmName         = @"";
		containingPlates = [[NSMutableArray alloc] init];
        isSelected       = NO;
        isConstant       = NO;
        isVector         = NO;
        dimensions       = 1;
		myModel          = t;
        drawingIndex     = 0;
        hasInspectorInfo = YES;
		}
    return self;
}

- (BOOL)isAssignedToPlate:(ParmPlate*)p {

	if ( [containingPlates containsObject:p] == YES )
		return YES;
	return NO;
}

- (ParmPlate*)isOnPlateWithIndexSource:(int)src {

	NSEnumerator* enumerator = [containingPlates objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
		if ( [element indexSource] == src )
			return element;
		}
	return nil;
}

- (int)numAttachedMatrices {

	return [myModel numAttachedMatrices];
}

- (int)numCharactersForMatrix:(int)idx {

	return [myModel numCharactersForMatrix:idx];
}

- (int)numExcludedCharactersForMatrix:(int)idx {

	return [myModel numExcludedCharactersForMatrix:idx];
}

- (int)numExcludedTaxaForMatrix:(int)idx {

	return [myModel numExcludedTaxaForMatrix:idx];
}

- (int)numTaxaForMatrix:(int)idx {

	return [myModel numTaxaForMatrix:idx];
}

- (int)numAssignedPlates {

	return (int)[containingPlates count];
}

- (int)numChildren {

	return (int)[children count];
}

- (int)numParents {

	return (int)[parents count];
}


- (IBAction)okAction:(id)sender {

	[self closeControlPanel];
}

- (NSPoint)originForControlWindow:(NSWindow*)win {

    // get the tool model frame
    NSRect toolRect = [parmsWindow frame];
    
    // and the controller window's frame
    NSRect winRect = [win frame];
    
    // get the center of the document window
    float x = NSMidX(toolRect);
    float y = NSMidY(toolRect);
    
    NSPoint p;
    p.x = x - winRect.size.width * 0.5;
    p.y = y - winRect.size.height * 0.5;
    return p;
}

- (void)refreshWindow {

}

- (void)removeAllConnections {

	id element;
	NSEnumerator* enumerator = [parents objectEnumerator];
	while ( (element = [enumerator nextObject]) )
		[element removeChild:self];
	[parents removeAllObjects];
	
	enumerator = [children objectEnumerator];
	while ( (element = [enumerator nextObject]) )
		[element removeParent:self];
	[children removeAllObjects];
}

- (void)removeChild:(Parm*)c {

	[children removeObject:c];
}

- (void)removeFromAllPlates {

	[containingPlates removeAllObjects];
}

- (void)removeFromPlate:(ParmPlate*)p {

	if ( [containingPlates containsObject:p] == YES )
		[containingPlates removeObject:p];
}

- (void)removeParent:(Parm*)p {

	[parents removeObject:p];
}

- (void)resizeWindowTo:(float)h {

	NSRect oldFrame = [[self window] frame];
    NSRect newFrame = oldFrame;
    newFrame.size.height = h;
    newFrame.origin.y += (oldFrame.size.height - newFrame.size.height);
    [[self window] setFrame:newFrame display:YES animate:YES];
}

- (void)setImageWithSize:(NSSize)s {

	[self setItemSize:s];
}

- (void)showControlPanel {

    NSPoint p = [self originForControlWindow:[self window]];
    [[self window] setFrameOrigin:p];
    [self refreshWindow];
	[self showWindow:self];
	[[self window] makeKeyAndOrderFront:nil];
    [NSApp runModalForWindow:[self window]];
}

- (void)updateChildren {

	NSLog(@"Updating parameter: %@", self);
	NSEnumerator* enumerator = [children objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		[element updateChildren];
}

@end
