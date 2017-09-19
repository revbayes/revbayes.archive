#import "AnalysisDocument.h"
#import "RbItem.h"
#import "RbView.h"

NSString* const ToolDataPboardType = @"ToolDataPboardType";


@implementation RbView

@synthesize scaleFactor;
@synthesize scaleIdx;
@synthesize snapToGrid;

- (BOOL)acceptsFirstResponder {

    [[self window] makeFirstResponder:self];
    return YES;
}

- (void)addTrackingForItem:(RbItem*)itm {

    // construct a string containing the item's address as a key for the dictionary
    NSString* itemKey = [NSString stringWithFormat:@"%p", itm];
    
	// see if the tracking area for the item already exists in the dictionary
	// if it is present, remove it
	NSTrackingArea* oldTa = [trackingAreas objectForKey:itemKey];
	if (oldTa != nil)
		{
		[self removeTrackingArea:oldTa];
		[trackingAreas removeObjectForKey:itemKey];
		}
	
	// get the rect for the item in the bottom-left coordinates
	NSRect itemRect;
	itemRect.origin = [itm itemLocation];
	itemRect.size   = [itm itemSize];
	[self transformToBottomLeftCoordinates:(&itemRect.origin)];

	float f = itemRect.size.width * 0.02;
	if ([itm isPlate] == YES)
		f = 3.0;
	itemRect.origin.x += f;
	itemRect.origin.y += f;
	itemRect.size.width  -= 2.0 * f;
	itemRect.size.height -= 2.0 * f;
	
	// allocate a tracking area
	NSTrackingArea* ta = [[NSTrackingArea alloc] initWithRect:itemRect 
													  options:(NSTrackingMouseEnteredAndExited | NSTrackingActiveInActiveApp)
												        owner:self
													 userInfo:nil];
	[self addTrackingArea:ta];
	
	// add the tracking area to the dictionary
	[trackingAreas setObject:ta forKey:itemKey];
}

- (void)concludeDragOperation:(id <NSDraggingInfo>)sender {

	[self setNeedsDisplay:YES];
}

- (IBAction)copy:(id)sender {

    NSPasteboard* pb = [NSPasteboard generalPasteboard];
    NSArray* pbTypes = [NSArray arrayWithObjects:ToolDataPboardType, nil];
    [pb declareTypes:pbTypes owner:nil];
    [self writeToPasteboard:pb];
    [[[NSDocumentController sharedDocumentController] currentDocument] updateChangeCount:NSChangeDone];
	pasteNum = 1;
}

- (IBAction)cut:(id)sender {

    [self copy:sender];
    [self removeSelectedItems];
	pasteNum = 1;
}

- (void)dealloc {

    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (NSDragOperation)draggingEntered:(id <NSDraggingInfo>)sender {

	if ([sender draggingSource] == self)
		return NSDragOperationNone;
	[self setNeedsDisplay:YES];
	return NSDragOperationCopy;
}

- (void)draggingExited:(id <NSDraggingInfo>)sender {

	[self setNeedsDisplay:YES];
}

- (void)drawRect:(NSRect)dirtyRect {

}

- (NSPoint)findNearestGridIntersectionToPoint:(NSPoint)p {

    NSPoint gp;
	float g = gridSize * scaleFactor;
	
    // x coordinate
	int r = (p.x / g) + 1;
	if ( r * g - p.x >= g * 0.5 )
		gp.x = g * (r-1);
	else 
		gp.x = g * r;
		
	// y coordinate
	r = (p.y / g) + 1;
	if ( r * g - p.y >= g * 0.5 )
		gp.y = g * (r-1);
	else 
		gp.y = g * r;
    
    return gp;
}

- (BOOL)hasItemCollided:(RbItem*)itm {

    NSRect r;
    r.origin = [itm itemLocation];
    r.size   = [itm itemSize];

	NSEnumerator* enumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
        if ( element != itm )
            {
            NSRect itemRect;
            itemRect.origin = [element itemLocation];
            itemRect.size   = [element itemSize];
            if ( CGRectIntersectsRect( NSRectToCGRect(itemRect), NSRectToCGRect(r) ) == YES )
                return YES;
            }
		}

	return NO;
}

- (BOOL)hasRectCollided:(NSRect)r {

	// Check to see if this rect collides with the borders or
	// with any other items. The top-left corner is (0,0).
	if (r.origin.x < 0.0 || r.origin.y - r.size.height < 0.0)
		return YES;
	NSEnumerator* enumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
		if ([element isPlate] == YES)
			continue;
			
		NSRect itemRect;
		itemRect.origin = [element itemLocation];
        itemRect.size   = [element itemSize];
		
        CGRect NSRectToCGRect(NSRect nsrect);
        
		if ( CGRectIntersectsRect( NSRectToCGRect(itemRect), NSRectToCGRect(r) ) == YES )
			return YES;
		}
	return NO;
}

- (BOOL)hasRectCollidedWithEdge:(NSRect)r {

	if (r.origin.x < 0.0 || r.origin.y - r.size.height < 0.0)
		return YES;
    return NO;
}

- (id)initWithFrame:(NSRect)frame {

    if ( (self = [super initWithFrame:frame]) ) 
		{
		// allocate memory
		trackingAreas = [[NSMutableDictionary alloc] init];
        selectedItems = [[NSMutableSet alloc] init];
        copiedItems = [[NSMutableArray alloc] init];
		
        // find the document owner of this view
        analysisDocumentPtr = [[NSDocumentController sharedDocumentController] currentDocument];

		// post to the default notification center that this object wants to be notified of view bounds changes
		NSNotificationCenter *defaultCenter = [NSNotificationCenter defaultCenter];		 
		[defaultCenter addObserver:self
						  selector:@selector(updateBackgroundColor:)
							  name:@"UpdatedAnalysisBkgrndColor"
							object:nil];
		[defaultCenter addObserver:self
						  selector:@selector(windowDidResize:)
							  name:@"NSWindowDidResizeNotification"
							object:nil];
		[defaultCenter addObserver:self
						  selector:@selector(windowDidResize:)
							  name:@"NSSplitViewDidResizeSubviewsNotification"
							object:nil];

        // register this view as drag source for an image
        [self registerForDraggedTypes:[NSImage imageTypes]];

		// set default values for object variables
		bottomRightCorner       = NSZeroPoint;
        cursorOffset            = NSZeroSize;
		selection.selectedItem  = nil;
		selection.selectionType = ITEM_SELECTION;
        draggedItems            = NO;
        shiftClicked            = NO;
        scaleFactor             = 1.0;
		scaleIdx                = 3;
        gridSize                = 20.0;
        itemTipTimer            = nil;
        isItemTipTimerActive    = NO;

        [self initializeDrawingInfo];
		}
    return self;
}

- (void)initializeDrawingInfo {

    // initialize information for drawing the information circle in the bottom-right corner of the tools
    float sf[8] = { 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 2.0, 4.0 };
    for (int i=0; i<8; i++)
        {
        NSSize ts = NSMakeSize(ITEM_IMAGE_SIZE * sf[i], ITEM_IMAGE_SIZE * sf[i]);
        NSDictionary* selAattrs   = [NSDictionary dictionaryWithObjects:[NSArray arrayWithObjects:[NSFont fontWithName:@"Times" size:(18.0*sf[i])], [[NSColor whiteColor] colorWithAlphaComponent:1.0], nil] forKeys:[NSArray arrayWithObjects:NSFontAttributeName,NSForegroundColorAttributeName, nil]];
        NSDictionary* unselAattrs = [NSDictionary dictionaryWithObjects:[NSArray arrayWithObjects:[NSFont fontWithName:@"Times" size:(18.0*sf[i])], [[NSColor blackColor] colorWithAlphaComponent:1.0], nil] forKeys:[NSArray arrayWithObjects:NSFontAttributeName,NSForegroundColorAttributeName, nil]];
        //unichar uc = 0x2139;
        //wchar_t uc = 0x1D48A; // this should be the code for the script i
        //wchar_t uc = 0x2139;
        //NSString* infoStr = [NSString stringWithFormat:@"%C",  uc];
        NSString* infoStr = @"\u2139";

        selectedAttributedString[i]   = [[NSAttributedString alloc] initWithString:infoStr attributes:selAattrs];
        unselectedAttributedString[i] = [[NSAttributedString alloc] initWithString:infoStr attributes:unselAattrs];

        NSRect iRect = [selectedAttributedString[i] boundingRectWithSize:NSMakeSize(1e10, 1e10) options:NSStringDrawingUsesLineFragmentOrigin];
        NSPoint tempP = iRect.origin;
        //NSRect glyphRect = [selectedAttributedString[i] boundingRectWithSize:NSMakeSize(1e10, 1e10) options:NSStringDrawingUsesLineFragmentOrigin];
        NSRect glyphRect = [selectedAttributedString[i] boundingRectWithSize:NSMakeSize(1e10, 1e10) options:NSStringDrawingUsesDeviceMetrics];
        iRect.size = glyphRect.size;
        iRect.size.height *= 1.25;
        iRect.size.width = iRect.size.height;
        iRect.origin = NSZeroPoint;
        float padding = 4.0 * sf[i];
        rOffset[i] = NSMakePoint(ts.width - iRect.size.width - padding, padding);
        mOffset[i] = NSMakePoint(padding, padding);
        iOffset[i] = rOffset[i];
        lOffset[i] = NSMakePoint(padding*1.5, padding*1.5);
        iOffset[i].x -= glyphRect.origin.x;
        iOffset[i].y += tempP.y;
        iOffset[i].x += (iRect.size.width  - glyphRect.size.width)  * 0.5 + iRect.size.width * 0.02;
        iOffset[i].y += (iRect.size.height - glyphRect.size.height) * 0.5 - iRect.size.width * 0.32;
        lOffset[i].y += (ts.height - iRect.size.height - 3.0 * padding);
        informationRect[i] = iRect;
        }
}

- (void)itemTipFire {

    fireNum++;
    if (fireNum == 1)
        {
        [itemForTip setShowTip:YES];
        [self setNeedsDisplay:YES];
        }
    else if (fireNum == 6)
        {
        [self stopItemTipTimer];
        [self setNeedsDisplay:YES];
        }
}

- (void)keyDown:(NSEvent*)event {

	// inactivate the timer, if it is still going
	if ([analysisDocumentPtr isRbTimerActive] == YES)
		[analysisDocumentPtr invalidateTimer];
	[self stopItemTipTimer];

    NSString *chars = [event characters];
    unichar character = [chars characterAtIndex: 0];
    if (character == NSDeleteCharacter)
		{
		[self removeSelectedConnections];
        [self removeSelectedItems];
		}
}

- (void)mouseDown:(NSEvent*)event {

}

- (void)mouseEntered:(NSEvent*)event {

	//NSLog(@"mouseEntered");
	// The cursor just entered the tracking area for a item. Find out which item we're in.
	ItemSelector mySelection = [self selectItemOnly:[event locationInWindow]];
	if (mySelection.selectedItem != nil)
		{
		itemArea = mySelection;
		//[itemArea.selectedItem setIsCursorOver:YES];
		[self selectCursorOverForItemsAtPoint:[event locationInWindow]];
        [self startItemTipTimerForItem:itemArea.selectedItem];
		[self setNeedsDisplay:YES];
		}
}

- (void)mouseExited:(NSEvent*)event {
	
	//NSLog(@"mouseExited");
	// The cursor just exited the tracking area for a item. Change the state of
	// the item's "isCursorOverTool" flag to "no".
	if (itemArea.selectedItem != nil)
		{
		//[itemArea.selectedItem setIsCursorOver:NO];
		[self selectCursorOverForItemsAtPoint:[event locationInWindow]];
        [self stopItemTipTimer];
		[self setNeedsDisplay:YES];
		}
}

- (IBAction)moveToBack:(NSEvent*)event {

	// find index of selected item
	NSEnumerator* itemEnumerator = [itemsPtr objectEnumerator];
	id element;
	RbItem* itemToMoveBackward = nil;
	BOOL foundFirstItem = NO;
	while ( (element = [itemEnumerator nextObject]) )
		{
		if ([element isPlate] == YES && [element isSelected] == YES)
			{
			itemToMoveBackward = element;
			foundFirstItem = YES;
			break;
			}
		}
	if (foundFirstItem == NO)
		return;
		
	// move it to back
	BOOL movedBack = NO;
	do 
		{
		movedBack = [self movePlateBackward:itemToMoveBackward];
		} while (movedBack == YES);
    
	// update the view
	[self setNeedsDisplay:YES];
}

- (IBAction)moveBackward:(NSEvent*)event {

	// find index of selected item
	NSEnumerator* itemEnumerator = [itemsPtr objectEnumerator];
	id element;
	RbItem* itemToMoveBackward = nil;
	BOOL foundFirstItem = NO;
	while ( (element = [itemEnumerator nextObject]) )
		{
		if ([element isPlate] == YES && [element isSelected] == YES)
			{
			itemToMoveBackward = element;
			foundFirstItem = YES;
			break;
			}
		}
	if (foundFirstItem == NO)
		return;

	// move it backward
	[self movePlateBackward:itemToMoveBackward];

	// update the view
	[self setNeedsDisplay:YES];
}

- (IBAction)moveToFront:(NSEvent*)event {

	// find index of selected item
	NSEnumerator* itemEnumerator = [itemsPtr objectEnumerator];
	id element;
	RbItem* itemToMoveForward = nil;
	BOOL foundFirstItem = NO;
	while ( (element = [itemEnumerator nextObject]) )
		{
		if ([element isPlate] == YES && [element isSelected] == YES)
			{
			itemToMoveForward = element;
			foundFirstItem = YES;
			break;
			}
		}
	if (foundFirstItem == NO)
		return;
		
	// move it to back
	BOOL movedForward = NO;
	do 
		{
		movedForward = [self movePlateForward:itemToMoveForward];
		} while (movedForward == YES);

	// update the view
	[self setNeedsDisplay:YES];
}

- (IBAction)moveForward:(NSEvent*)event {

	// find index of selected item
	NSEnumerator* itemEnumerator = [itemsPtr objectEnumerator];
	id element;
	RbItem* itemToMoveForward = nil;
	BOOL foundFirstItem = NO;
	while ( (element = [itemEnumerator nextObject]) )
		{
		if ([element isPlate] == YES && [element isSelected] == YES)
			{
			itemToMoveForward = element;
			foundFirstItem = YES;
			break;
			}
		}
	if (foundFirstItem == NO)
		return;

	// move it backward
	[self movePlateForward:itemToMoveForward];

	// update the view
	[self setNeedsDisplay:YES];
}

- (BOOL)movePlateBackward:(RbItem*)plt {

	// make certain that we have an object to work with
	if (plt == nil)
		return NO;
		
	// and that it is a plate
	if ([plt isPlate] == NO)
		return NO;
		
	// find index of plate
	int idx1 = (int)[itemsPtr indexOfObject:plt];
		
	// find the plate just below the selected one
	NSEnumerator* rItemEnumerator = [itemsPtr reverseObjectEnumerator];
	id element;
	int idx2 = 0;
	BOOL foundSecondItem = NO;
	while ( (element = [rItemEnumerator nextObject]) )
		{
		if ([element isPlate] == YES)
			{
			int idx = (int)[itemsPtr indexOfObject:element];
			if (idx < idx1)
				{
				idx2 = idx;
				foundSecondItem = YES;
				break;
				}
			}
		}
	if (foundSecondItem == NO)
		return NO;
		
	// exchange the two plates
	[itemsPtr exchangeObjectAtIndex:idx1 withObjectAtIndex:idx2];
	
	return YES;
}

- (BOOL)movePlateForward:(RbItem*)plt {

	// make certain that we have an object to work with
	if (plt == nil)
		return NO;
		
	// and that it is a plate
	if ([plt isPlate] == NO)
		return NO;
		
	// find index of plate
	int idx1 = (int)[itemsPtr indexOfObject:plt];
		
	// find the plate just below the selected one
	NSEnumerator* itemEnumerator = [itemsPtr objectEnumerator];
	id element;
	int idx2 = 0;
	BOOL foundSecondItem = NO;
	while ( (element = [itemEnumerator nextObject]) )
		{
		if ([element isPlate] == YES)
			{
			int idx = (int)[itemsPtr indexOfObject:element];
			if (idx > idx1)
				{
				idx2 = idx;
				foundSecondItem = YES;
				break;
				}
			}
		}
	if (foundSecondItem == NO)
		return NO;
		
	// exchange the two plates
	[itemsPtr exchangeObjectAtIndex:idx1 withObjectAtIndex:idx2];
	
	return YES;
}

- (int)numPlates {

	NSEnumerator* itemEnumerator = [itemsPtr objectEnumerator];
	id element;
	int numSelected = 0;
	while ( (element = [itemEnumerator nextObject]) )
		{
		if ([element isPlate] == YES)
			numSelected++;
		}
	return numSelected;
}

- (int)numSelectedItems {

	NSEnumerator* itemEnumerator = [itemsPtr objectEnumerator];
	id element;
	int numSelected = 0;
	while ( (element = [itemEnumerator nextObject]) )
		{
		if ([element isSelected] == YES)
			numSelected++;
		}
	return numSelected;
}

- (int)numSelectedPlates {

	NSEnumerator* itemEnumerator = [itemsPtr objectEnumerator];
	id element;
	int numSelected = 0;
	while ( (element = [itemEnumerator nextObject]) )
		{
		if ([element isSelected] == YES && [element isPlate] == YES)
			numSelected++;
		}
	return numSelected;
}

- (IBAction)paste:(id)sender {

    NSPasteboard* pb = [NSPasteboard generalPasteboard];
    if ( [self readFromPasteboard:pb] == NO )
        NSBeep();
    [[[NSDocumentController sharedDocumentController] currentDocument] updateChangeCount:NSChangeDone];
	pasteNum++;
}

- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender {

    return YES;
}

- (BOOL)prepareForDragOperation:(id <NSDraggingInfo>)sender {

	return YES;
}

- (BOOL)readFromPasteboard:(NSPasteboard*)pb {

    return YES;
}

- (void)removeAllFocusRings {

}

- (void)removeAllTrackingAreas {

	// remove the tracking areas from the view
	NSArray* viewsTrackingAreas = [self trackingAreas];
	NSEnumerator* trackingEnum = [viewsTrackingAreas objectEnumerator];
	id ta;
	while ( (ta = [trackingEnum nextObject]) )
		[self removeTrackingArea:ta];

	// and empty the dictionary linking tools with tracking areas
	[trackingAreas removeAllObjects];
}

- (void)removeSelectedConnections {

}

- (void)removeSelectedItems {

    // first, clean up connections and tracking areas
    NSMutableArray* itemsToRemove = [[NSMutableArray alloc] init];
	NSEnumerator* enumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
        {
        if ([element isSelected] == YES)
            {
            [element removeAllConnections];
            [itemsToRemove addObject:element];
            [self removeTrackingForItem:element];
			
			NSPoint p = [element itemLocation];
			NSSize s = [element itemSize];
			[self transformToBottomLeftCoordinates:&p];
			p.x += s.width * 0.5;
			p.y += s.height * 0.5;

			NSPoint pointInWindowCoordinates = [self convertPoint:p toView:nil]; 
			NSPoint pointInScreenCoords      = [[self window] convertBaseToScreen:pointInWindowCoordinates];

			if ( [element isPlate] == NO )
				NSShowAnimationEffect(NSAnimationEffectPoof, pointInScreenCoords, s, NULL, NULL, NULL);
            }
        }
    
    // now, remove the selected tools
    [itemsPtr removeObjectsInArray:itemsToRemove];

    // and clean up
    /* JPHARC [itemsToRemove release]; */
    [self setCorners];
    [self updateScrollBars];
    [self setNeedsDisplay:YES];
    [[[NSDocumentController sharedDocumentController] currentDocument] updateChangeCount:NSChangeDone];
}

- (void)removeTrackingForItem:(RbItem*)itm {

    // construct a string containing the tool's address as a key for the dictionary
    NSString* itemKey = [NSString stringWithFormat:@"%p", itm];
    
	// see if the tracking area for the tool already exists in the dictionary
	// if it is present, remove it
	NSTrackingArea* oldTa = [trackingAreas objectForKey:itemKey];
	if (oldTa != nil)
		{
		[self removeTrackingArea:oldTa];
		[trackingAreas removeObjectForKey:itemKey];
		}
}

- (void)scaleFactorChanged:(NSNotification*)notification {

}

- (IBAction)selectAll:(id)sender {

	NSEnumerator* enumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
        [element setIsSelected:YES];
    [self setNeedsDisplay:YES];
}

- (void)selectAllItems {

}

- (void)selectCursorOverForItemsAtPoint:(NSPoint)p {

	// transform point to coordinate system of view objects (such as tools)
	NSPoint tp = [self convertPoint:p fromView:nil];

	// enumerate all of the tools, to see if any of them have been selected
	NSEnumerator* enumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
		// set cursor over to no
		[element setIsCursorOver:NO];
		
		// initialize information on the tool
		NSRect itemRect;
		itemRect.origin = [element itemLocation];
		itemRect.size   = [element itemSize];
		[self transformToBottomLeftCoordinates:(&itemRect.origin)];

		float f = itemRect.size.width * 0.02;
		if ([element isPlate] == YES)
			f = 3.0;
		itemRect.origin.x += f;
		itemRect.origin.y += f;
		itemRect.size.width  -= 2.0 * f;
		itemRect.size.height -= 2.0 * f;
            			
		// check if the point is in the tool image
		if ( CGRectContainsPoint( NSRectToCGRect(itemRect), NSPointToCGPoint(tp) ) )
			[element setIsCursorOver:YES];
		}
}

- (ItemSelector)selectItem:(NSPoint)forPoint {

    ItemSelector mySelection;
    return mySelection;
}

- (void)selectItemsInSweepArea {

}

- (ItemSelector)selectItemOnly:(NSPoint)forPoint {

	// set to base state
	ItemSelector mySelection;
	mySelection.selectedItem = nil;
	mySelection.selectionType = ITEM_SELECTION;
	
	// transform point to coordinate system of view objects (such as tools)
	NSPoint p = [self convertPoint:forPoint fromView:nil];
	
	// enumerate all of the tools, to see if any of them have been selected
	NSEnumerator* enumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
		// initialize information on the tool
		NSRect itemRect;
		itemRect.origin = [element itemLocation];
		itemRect.size = [element itemSize];
		[self transformToBottomLeftCoordinates:(&itemRect.origin)];
            			
		// check if the point is in the tool image
		if ( CGRectContainsPoint( NSRectToCGRect(itemRect), NSPointToCGPoint(p) ) )
            {
            cursorOffset.width  = p.x - itemRect.origin.x;
            cursorOffset.height = p.y - itemRect.origin.y;
			mySelection.selectedItem = element;
			mySelection.selectionType = ITEM_SELECTION;
			return mySelection;
            }
		}
		
	return mySelection;
}

- (ItemSelector)selectNonPlateItemOnly:(NSPoint)forPoint {

	// set to base state
	ItemSelector mySelection;
	mySelection.selectedItem = nil;
	mySelection.selectionType = ITEM_SELECTION;
	
	// transform point to coordinate system of view objects (such as tools)
	NSPoint p = [self convertPoint:forPoint fromView:nil];
	
	// enumerate all of the tools, to see if any of them have been selected
	NSEnumerator* enumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
		if ([element isPlate] == NO)
			{
			// initialize information on the tool
			NSRect itemRect;
			itemRect.origin = [element itemLocation];
			itemRect.size = [element itemSize];
			[self transformToBottomLeftCoordinates:(&itemRect.origin)];
							
			// check if the point is in the tool image
			if ( CGRectContainsPoint( NSRectToCGRect(itemRect), NSPointToCGPoint(p) ) )
				{
				cursorOffset.width  = p.x - itemRect.origin.x;
				cursorOffset.height = p.y - itemRect.origin.y;
				mySelection.selectedItem = element;
				mySelection.selectionType = ITEM_SELECTION;
				return mySelection;
				}
			}
		}
		
	return mySelection;
}

- (void)setAnalysisDocumentPtr:(AnalysisDocument*)dp {

	analysisDocumentPtr = dp;
}

- (void)setCorners {

    bottomRightCorner = NSZeroPoint;
	topLeftCorner     = NSZeroPoint;
	topLeftCorner.x   = 1000000000.0;
	topLeftCorner.y   = 1000000000.0;

	NSEnumerator* enumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
		NSRect drawingRect;
		drawingRect.origin = [element itemLocation];
		drawingRect.size   = [element itemSize];
		
		if ( drawingRect.origin.x + drawingRect.size.width > bottomRightCorner.x )
			bottomRightCorner.x = drawingRect.origin.x + drawingRect.size.width;
		if ( drawingRect.origin.y > bottomRightCorner.y )
			bottomRightCorner.y = drawingRect.origin.y;
			
		if ( drawingRect.origin.x < topLeftCorner.x )
			topLeftCorner.x = drawingRect.origin.x;
		if ( drawingRect.origin.y - drawingRect.size.height < topLeftCorner.y )
			topLeftCorner.y = drawingRect.origin.y - drawingRect.size.height;
		}
}

- (void)setItems:(NSMutableArray*)itm {

    // set the instance variable pointer of this class to point to the
    // mutable array of tools, stored in the AnalysisDocument class
    itemsPtr = itm;

    // update the tracking area
	[self updateAllTrackingAreas];
        
    // update the display
	[self setNeedsDisplay:YES];
}

- (void)setShowGrid:(BOOL)x {

    showGrid = x;
    [self setNeedsDisplay:YES];
}

- (BOOL)showGrid {

    return showGrid;
}

- (IBAction)showGridAction:(id)sender {

    if ([self showGrid] == YES)
        [self setShowGrid:NO];
    else 
        [self setShowGrid:YES];
}

- (IBAction)showParameterPalette:(id)sender {

    [[NSNotificationCenter defaultCenter] postNotificationName:@"ShowParameterPalette" object:self];
    NSLog(@"showParameterPalette from RbView");
}

- (void)showTrackingAreas {

	NSLog(@"Tracking areas:");
	NSArray* viewsTrackingAreas = [self trackingAreas];
	NSEnumerator* trackingEnum = [viewsTrackingAreas objectEnumerator];
	id ta;
	while ( (ta = [trackingEnum nextObject]) )
		NSLog(@"tracking area = %@", ta);

	// and empty the dictionary linking tools with tracking areas
	NSLog(@"Tracking area dictionary:");
	NSEnumerator* trackingKeys = [trackingAreas keyEnumerator];
	id tKey;
	while ( (tKey = [trackingKeys nextObject]) )
		{
		NSTrackingArea* tObj = [trackingAreas objectForKey:tKey];
		NSLog(@"key/value pair = %@ / %@", tKey, tObj);
		}
}

- (IBAction)snapToGridAction:(id)sender {

    if ([self snapToGrid] == YES)
        [self setSnapToGrid:NO];
    else 
        [self setSnapToGrid:YES];
}

- (void)startItemTipTimerForItem:(RbItem*)itm {

    [analysisDocumentPtr invalidateTimer];
    [self stopItemTipTimer];
    
    itemTipTimer = [NSTimer scheduledTimerWithTimeInterval:1.0 target:self selector:@selector(itemTipFire) userInfo:nil repeats:YES];
    isItemTipTimerActive = YES;
    fireNum = 0;
    itemForTip = itm;
}

- (void)stopItemTipTimer {
 
    if (isItemTipTimerActive == YES)
        {
        [itemTipTimer invalidate];
        [itemForTip setShowTip:NO];
        isItemTipTimerActive = NO;
        itemTipTimer = nil;
        itemForTip = nil;
        }
}

- (void)transformToBottomLeftCoordinates:(NSPoint*)forPoint {

	NSRect bounds = [self bounds];
    float origX = (*forPoint).x;
    float origY = (*forPoint).y;
    
    float newX = origX;
    float newY = bounds.size.height - origY;
    
    (*forPoint).x = newX;
    (*forPoint).y = newY;
}

- (void)transformToTopLeftCoordinates:(NSPoint*)forPoint {

	NSRect bounds = [self bounds];
    float origX = (*forPoint).x;
    float origY = (*forPoint).y;
    
    float newX = origX;
    float newY = bounds.size.height - origY;
    
    (*forPoint).x = newX;
    (*forPoint).y = newY;
}

- (void)updateAllTrackingAreas {

	[self removeAllTrackingAreas];
	
	NSEnumerator* itemEnumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [itemEnumerator nextObject]) )
        [self addTrackingForItem:element];
}

- (void)updateBackgroundColor:(NSNotification*)notification {

}

- (void)updateScrollBars {

    // get information on the bounds and used area
	NSRect visible = [self visibleRect];
    float brX      = bottomRightCorner.x;
    float brY      = bottomRightCorner.y;

    // update the size of the frame
	NSRect newFrame = visible;
	if ( visible.size.width < brX )
		newFrame.size.width = brX;
	if ( visible.size.height < brY )
		newFrame.size.height = brY;
	[self setFrame:newFrame];
}

- (BOOL)validateMenuItem:(NSMenuItem*)item {
    
	int numSelected       = [self numSelectedItems];
	int numSelectedPlates = [self numSelectedPlates];
	int numPlates         = [self numPlates];
	SEL act               = [item action];
	
	if ( act == @selector(cut:) )
        {
        if ( numSelected > 0 )
            return YES;
        return NO;
        }
	else if ( act == @selector(copy:) )
        {
        if ( numSelected > 0 )
            return YES;
        return NO;
        }
	else if ( act == @selector(paste:) )
        {
        NSArray* typeArray = [NSArray arrayWithObjects:ToolDataPboardType, nil]; 
        NSString* type = [[NSPasteboard generalPasteboard] availableTypeFromArray:typeArray]; 
        if (!type)
            return NO; 
        return YES;
        }
	else if ( act == @selector(selectAll:) )
        {
        if ([itemsPtr count] > 0)
            return YES;
        return NO;
        }
    else if ( act == @selector(snapToGridAction:) )
		{
		if ( [self snapToGrid] == YES )
			[item setTitle:@"Do Not Snap to Grid"];
		else 
			[item setTitle:@"Snap to Grid"];
		return YES;
		}
    else if ( act == @selector(showGridAction:) )
		{
		if ( [self showGrid] == YES )
			[item setTitle:@"Hide Grid"];
		else 
			[item setTitle:@"Show Grid"];
		return YES;
		}

    else if ( act == @selector(moveToBack:) )
		{
		if (numPlates > 1 && numSelectedPlates == 1)
			return YES;
		return NO;
		}
    else if ( act == @selector(moveBackward:) )
		{
		if (numPlates > 1 && numSelectedPlates == 1)
			return YES;
		return NO;
		}
    else if ( act == @selector(moveToFront:) )
		{
		if (numPlates > 1 && numSelectedPlates == 1)
			return YES;
		return NO;
		}
    else if ( act == @selector(moveForward:) )
		{
		if (numPlates > 1 && numSelectedPlates == 1)
			return YES;
		return NO;
		}
	else if ( act == @selector(showParameterPalette:) )
        {
        return YES;
        }

    return YES;
}

- (void)windowDidResize:(NSNotification*)notification {

	[self setCorners];
    [self updateScrollBars];

	// update the tracking information for the tools
	NSEnumerator* itemEnumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [itemEnumerator nextObject]) )
		[self addTrackingForItem:element];
}

- (void)writeToPasteboard:(NSPasteboard*)pb {

}

@end
