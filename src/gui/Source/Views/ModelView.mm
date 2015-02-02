#import "AJHBezierUtils.h"
#import "AnalysisDocument.h"
#import "ModelView.h"
#import "PaletteView.h"
#import "Parm.h"
#import "ParmConstantInteger.h"
#import "ParmConstantIntegerPos.h"
#import "ParmConstantReal.h"
#import "ParmConstantRealPos.h"
#import "ParmConstantVectorReal.h"
#import "ParmDraw.h"
#import "ParmInteger.h"
#import "ParmIntegerPos.h"
#import "ParmPair.h"
#import "ParmPlate.h"
#import "ParmPlateTree.h"
#import "ParmRateMatrix.h"
#import "ParmReal.h"
#import "ParmRealPos.h"
#import "ParmRestore.h"
#import "ParmSimplex.h"
#import "ParmTree.h"
#import "ParmVectorInteger.h"
#import "ParmVectorIntegerPos.h"
#import "ParmVectorReal.h"
#import "ParmVectorRealPos.h"
#import "RevBayes.h"
#import "ToolModel.h"
#import "WindowControllerPreferences.h"

BOOL lineSegmentsIntersecting(CGPoint a, CGPoint b, CGPoint c, CGPoint d);


@implementation ModelView

@synthesize parmCount;
@synthesize showParmNames;
@synthesize myTool;

- (void)addToSelectedLinkListParent:(Parm*)x andChild:(Parm*)y {

    if ([self isLinkSelectedBetweenParent:x andChild:y] == NO)
        {
        ParmPair* pp = [[ParmPair alloc] init];
        [pp setParentParm:x];
        [pp setChildParm:y];
        [selectedLinks addObject:pp];
        }
}

- (void)awakeFromNib {

}

- (void)checkPlateMembership {

	// get an array of the plates
	NSMutableArray* plateArray = [NSMutableArray arrayWithCapacity:1];
	NSEnumerator* parmEnumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [parmEnumerator nextObject]) )
		{
		if ( [element isPlate] == YES )
			[plateArray addObject:element];
		}
		
	// if there are no plates, make certain the parameters reflect this
	if ( [plateArray count] == 0 )
		{
		parmEnumerator = [itemsPtr objectEnumerator];
		while ( (element = [parmEnumerator nextObject]) )
			[element removeFromAllPlates];
		return;
		}
		
	// check each parameter to see if it is on a plate
	NSEnumerator* plateEnumerator = [plateArray objectEnumerator];
	ParmPlate* plt;
	while ( (plt = [plateEnumerator nextObject]) )
		{
		NSRect plRect = [plt plateRect];
		plRect.origin = [plt itemLocation];
		[self transformToBottomLeftCoordinates:&plRect.origin];
		parmEnumerator = [itemsPtr objectEnumerator];
		while ( (element = [parmEnumerator nextObject]) )
			{
			if ( [element isPlate] == NO )
				{
				// decide if the parameter is currently on the plate
				NSRect prRect;
				prRect.origin = [element itemLocation];
				prRect.size   = [element itemSize];
				[self transformToBottomLeftCoordinates:&prRect.origin];
				BOOL isParmCurrentlyOnPlate = NO;
				if ( CGRectContainsRect( NSRectToCGRect(plRect), NSRectToCGRect(prRect) ) )
					isParmCurrentlyOnPlate = YES;
				BOOL wasParmOnPlate = [element isAssignedToPlate:plt];
				if (wasParmOnPlate == YES && isParmCurrentlyOnPlate == NO)
					[element removeFromPlate:plt];
				else if (wasParmOnPlate == NO && isParmCurrentlyOnPlate == YES)
					[element addToPlate:plt];
				}
			}
		}
		
	// check the plates to see if some plates are completely contained by others
	NSEnumerator* plateEnumerator1 = [plateArray objectEnumerator];
	ParmPlate* plt1;
	while ( (plt1 = [plateEnumerator1 nextObject]) )
		{
		// check if this plate is contained by other plates
		NSRect plRect1 = [plt1 plateRect];
		plRect1.origin = [plt1 itemLocation];
		[self transformToBottomLeftCoordinates:&plRect1.origin];
		NSEnumerator* plateEnumerator2 = [plateArray objectEnumerator];
		ParmPlate* plt2;
		while ( (plt2 = [plateEnumerator2 nextObject]) )
			{
			if (plt1 != plt2)
				{
				NSRect plRect2 = [plt2 plateRect];
				plRect2.origin = [plt2 itemLocation];
				[self transformToBottomLeftCoordinates:&plRect2.origin];
				BOOL isPlate2CurrentlyOnPlate1 = NO;
				if ( CGRectContainsRect( NSRectToCGRect(plRect1), NSRectToCGRect(plRect2) ) )
					isPlate2CurrentlyOnPlate1 = YES;
				BOOL wasPlate2OnPlate1 = [plt2 isAssignedToPlate:plt1];
				if (wasPlate2OnPlate1 == YES && isPlate2CurrentlyOnPlate1 == NO)
					[plt2 removeFromPlate:plt1];
				else if (wasPlate2OnPlate1 == NO && isPlate2CurrentlyOnPlate1 == YES)
					[plt2 addToPlate:plt1];
				}
			}
			
		// remove any plates that were deleted underneath it
		NSMutableArray* platesToRemove = [NSMutableArray arrayWithCapacity:1];
		for (int i=0; i<[plt1 numAssignedPlates]; i++)
			{
			ParmPlate* p = [plt1 getPlateIndexed:i];
			if ( [plateArray containsObject:p] == NO )
				[platesToRemove addObject:p];
			}
		plateEnumerator = [platesToRemove objectEnumerator];
		while ( (plt = [plateEnumerator nextObject]) )
			[plt1 removeFromPlate:plt];
		}
		
	// have each parameter update itself depending now that the plates may have been updated
	[myTool touchAllParameters];
		
#	if 0
	NSLog(@"checkPlateMembership");
	NSLog(@"   plate list:");
	plateEnumerator = [plateArray objectEnumerator];
	while (plt = [plateEnumerator nextObject])
		NSLog(@"   plate = %@", plt);
	parmEnumerator = [itemsPtr objectEnumerator];
	while (element = [parmEnumerator nextObject])
		{
		NSLog(@"parameter %@:", element);
		for (int i=0; i<[element numAssignedPlates]; i++)
			{
			ParmPlate* p = [element getPlateIndexed:i];
			NSLog(@"       contained by plate %@", p);
			}
		}
#	endif
}

- (float)distanceFromPoint:(NSPoint)a toPoint:(NSPoint)b {

    float dX = a.x - b.x;
    float dY = a.y - b.y;
    return sqrt( dX*dX + dY*dY );
}

- (float)distanceOfPoint:(NSPoint)a0 fromLineDefinedByPoint:(NSPoint)a1 andPoint:(NSPoint)a2 {

	float temp1 = (a2.x - a1.x) * (a1.y - a0.y) - (a1.x - a0.x) * (a2.y - a1.y);
	float temp2 = (a2.x - a1.x) * (a2.x - a1.x) + (a2.y - a1.y) * (a2.y - a1.y);
	float d     = fabs(temp1) / sqrt(temp2);
	return d;
}

- (BOOL)doesLineWithEndsA:(CGPoint)a andB:(CGPoint)b intersectRect:(CGRect)r {

    // if either end of the line is within the rectangle, then clearly the line intersects it
    if ( CGRectContainsPoint(r, a) == YES || CGRectContainsPoint(r, b) == YES )
        return YES;

    // check to see if the line intersects any of the rectangle's edges
    CGPoint c, d;
    c = r.origin;
    d.x = c.x + r.size.width;
    d.y = c.y;
    if ( lineSegmentsIntersecting(a, b, c, d) == YES )
        return YES;
        
    d.x = c.x;
    d.y = c.y + r.size.height;
    if ( lineSegmentsIntersecting(a, b, c, d) == YES )
        return YES;
        
    c.x = r.origin.x + r.size.width;
    c.y = r.origin.y + r.size.height;
    d.x = c.x - r.size.width;
    d.y = c.y;
    if ( lineSegmentsIntersecting(a, b, c, d) == YES )
        return YES;

    d.x = c.x;
    d.y = c.y - r.size.height;
    if ( lineSegmentsIntersecting(a, b, c, d) == YES )
        return YES;
    
    return NO;
}

BOOL lineSegmentsIntersecting(CGPoint a, CGPoint b, CGPoint c, CGPoint d) {

    //fail if either line segment is zero-length.
    if ( (a.x == b.x && a.y == b.y) || (c.x == d.x && c.y == d.y) ) 
        return NO;

    //fail if the segments share an end-point.
    if ( (a.x == c.x && a.y == c.y) || (b.x == c.x && b.y == c.y) ||
         (a.x == d.x && a.y == d.y) || (b.x == d.x && b.y == d.y) ) 
        return NO;

    // translate the system so that point A is on the origin.
    b.x -= a.x;
    b.y -= a.y;
    c.x -= a.x;
    c.y -= a.y;
    d.x -= a.x;
    d.y -= a.y;

    //discover the length of segment A-B.
    CGFloat ab = sqrt(b.x*b.x + b.y*b.y);

    //rotate the system so that point B is on the positive X a.xis.
    CGFloat theCos = b.x/ab;
    CGFloat theSin = b.y/ab;
    CGFloat newX = c.x*theCos + c.y*theSin;
    c.y = c.y*theCos - c.x*theSin;
    c.x = newX;
    newX = d.x*theCos + d.y*theSin;
    d.y = d.y*theCos - d.x*theSin;
    d.x = newX;
    
    // fail if segment C-D doesn't cross line A-B.
    if ( (c.y < 0.0 && d.y) < 0.0 || (c.y >= 0.0 && d.y >= 0.0) ) 
        return NO;

    //discover the position of the intersection point along line A-B.
    CGFloat intersection = d.x + (c.x-d.x)*d.y/(d.y-c.y);

    // fail if segment C-D crosses line A-B outside of segment A-B.
    if (intersection < 0.0 || intersection > ab) 
        return NO;

    return YES;
}

- (void)drawRect:(NSRect)dirtyRect {

    // some general parameters for drawing
    float borderWidth = ITEM_IMAGE_SIZE * scaleFactor * 0.01;
    
    // allocate a shadow object
    NSShadow* shadow = [[NSShadow alloc] init];
    [shadow setShadowOffset:NSMakeSize(3.0, -2.0)];
    [shadow setShadowBlurRadius:10.0];

	// set the background of the analysis window
	NSRect bounds = [self bounds];
	[bkgrndColor set];
	[NSBezierPath fillRect:bounds];

    // set the grid
    if ( showGrid == YES )
        {
        float x = 0.0;
        while (x < bounds.size.width)
            {
            x += gridSize * scaleFactor;
            NSPoint p1 = NSMakePoint(x, 0.0);
            NSPoint p2 = NSMakePoint(x, bounds.size.height);
            NSBezierPath* connection = [NSBezierPath bezierPath];
            [connection moveToPoint:p1];
            [connection lineToPoint:p2];
			[gridColor set];
			[connection setLineWidth:0.2];
            [connection stroke];
            }
            
        float y = bounds.size.height;
        while (y > 0.0)
            {
            y -= gridSize * scaleFactor;
            NSPoint p1 = NSMakePoint(0.0, y);
            NSPoint p2 = NSMakePoint(bounds.size.width, y);
            NSBezierPath* connection = [NSBezierPath bezierPath];
            [connection moveToPoint:p1];
            [connection lineToPoint:p2];
			[gridColor set];
			[connection setLineWidth:0.2];
            [connection stroke];
            }
        }
        
	// draw normal (non-tree) plates
	NSEnumerator* parmEnumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [parmEnumerator nextObject]) )
		{
		if ( [element parmType] == PARM_PLATE )
			{
			// draw the rectangle (plate)
            [NSGraphicsContext saveGraphicsState];
            [shadow set];

			NSRect pr = [element plateRect];
			pr.origin = [element itemLocation];
			[self transformToBottomLeftCoordinates:(&pr.origin)];
			[[NSColor lightGrayColor] set];
            NSBezierPath* pp = [NSBezierPath bezierPath];
            float rad = 6.0 * scaleFactor;
            [pp appendBezierPathWithRoundedRect:pr xRadius:rad yRadius:rad];
            [pp fill];
            
			// and the line around the plate
			NSRect focusRect = pr;
			[[NSColor blackColor] set];
			NSBezierPath* selRing = [NSBezierPath bezierPath];
			[selRing setLineWidth:borderWidth];
			[selRing appendBezierPathWithRoundedRect:focusRect xRadius:rad yRadius:rad];
			[selRing stroke];
            [NSGraphicsContext restoreGraphicsState];
			
			// draw the plate's information in the bottom right corner
			if ( [element isCursorOver] == YES || (draggedItems == YES && element == selection.selectedItem) )
				{
                // show the little "i" button
				NSRect iRect = informationRect[scaleIdx];
				float delta  = pr.size.width - ITEM_IMAGE_SIZE * scaleFactor;
				iRect.origin = NSMakePoint(pr.origin.x + rOffset[scaleIdx].x + delta, pr.origin.y + rOffset[scaleIdx].y);
				NSPoint p    = NSMakePoint(pr.origin.x + iOffset[scaleIdx].x + delta, pr.origin.y + iOffset[scaleIdx].y);
				[[[NSColor blackColor] colorWithAlphaComponent:0.25] set];
				[[NSBezierPath bezierPathWithOvalInRect:iRect] fill];
				[selectedAttributedString[scaleIdx] drawAtPoint:p];
				}
			else 
				{
                // write out the mathematical notation for the iterator
				NSDictionary* attrs;
				if ( [element isCursorOver] == YES || (draggedItems == YES && element == selection.selectedItem) )
					attrs = [NSDictionary dictionaryWithObjects:[NSArray arrayWithObjects:[NSFont fontWithName:@"Times" size:(18.0*scaleFactor)], [[NSColor whiteColor] colorWithAlphaComponent:1.0], nil] forKeys:[NSArray arrayWithObjects:NSFontAttributeName,NSForegroundColorAttributeName, nil]];
				else 
					attrs = [NSDictionary dictionaryWithObjects:[NSArray arrayWithObjects:[NSFont fontWithName:@"Times" size:(18.0*scaleFactor)], [[NSColor blackColor] colorWithAlphaComponent:1.0], nil] forKeys:[NSArray arrayWithObjects:NSFontAttributeName,NSForegroundColorAttributeName, nil]];
				unichar uc = 8712;
				NSString* endingRangeStr = [element getEndingRangeForPlate];
				NSRange endingRangeRange = [element italicsRange];
				NSString* infoStr = [NSString stringWithFormat:@"%c %C (1,...,", [element indexLetter], uc];
				infoStr = [infoStr stringByAppendingString:endingRangeStr];
				infoStr = [infoStr stringByAppendingString:@")"];
				NSMutableAttributedString* attrString = [[NSMutableAttributedString alloc] initWithString:infoStr attributes:attrs];
				[attrString applyFontTraits:NSItalicFontMask range:NSMakeRange(0, 1)];
				if ( endingRangeRange.length != 0 )
					{
					endingRangeRange.location += 11;
					[attrString applyFontTraits:NSItalicFontMask range:endingRangeRange];
					}
<<<<<<< HEAD
				NSRect textSize = [attrString boundingRectWithSize:NSMakeSize(1e10, 1e10) options:NSStringDrawingUsesLineFragmentOrigin];
=======
				NSRect textSize = [attrString boundingRectWithSize:NSMakeSize(1e10, 1e10) options:NSStringDrawingUsesDeviceMetrics];
>>>>>>> 61480579c58f84b39cbc487a4a6a95bbd1fa0697
				float padding = 4.0 * scaleFactor;
				textSize.origin.x = pr.origin.x + (pr.size.width - textSize.size.width - padding);
				textSize.origin.y = pr.origin.y + padding;
				NSPoint p = textSize.origin;
				[attrString drawAtPoint:p];
				[element setMinPlateSize:NSMakeSize(textSize.size.width + 2.0*padding, textSize.size.height + 2.0*padding)];
				}
				
			// draw the selection information, with handles
			if ([element isSelected] == YES)
				{
				[[NSColor darkGrayColor] set];
				NSRect handleRect;
				handleRect.origin = pr.origin;
				handleRect.size = NSMakeSize(15.0*scaleFactor, 15.0*scaleFactor);
				float oneHalfWidth = handleRect.size.width * 0.5;
				handleRect.origin.x += (pr.size.width - oneHalfWidth);
				handleRect.origin.y -= oneHalfWidth;
				[[NSBezierPath bezierPathWithRect:handleRect] fill];
				
				handleRect.origin = pr.origin;
				handleRect.origin.x -= oneHalfWidth;
				handleRect.origin.y += (pr.size.height - oneHalfWidth);
				[[NSBezierPath bezierPathWithRect:handleRect] fill];
				}
			}
		}

    // draw tree plates
	parmEnumerator = [itemsPtr objectEnumerator];
	while ( (element = [parmEnumerator nextObject]) )
		{
		if ( [element parmType] == PARM_TREE_PLATE )
			{
			// draw the rectangle (plate)
            [NSGraphicsContext saveGraphicsState];
            [shadow set];
            
			NSRect pr = [element plateRect];
			pr.origin = [element itemLocation];
			[self transformToBottomLeftCoordinates:(&pr.origin)];
			[[NSColor lightGrayColor] set];
            NSBezierPath* pp = [NSBezierPath bezierPath];
            float rad = 6.0 * scaleFactor;
            [pp appendBezierPathWithRoundedRect:pr xRadius:rad yRadius:rad];
            [pp fill];
            
			// and the line around the plate
			NSRect focusRect = pr;
			[[NSColor blackColor] set];
			NSBezierPath* selRing = [NSBezierPath bezierPath];
			[selRing setLineWidth:borderWidth];
			[selRing appendBezierPathWithRoundedRect:focusRect xRadius:rad yRadius:rad];
			[selRing stroke];
            [NSGraphicsContext restoreGraphicsState];
            
            // draw a lightly-shaded tree and a connection circle on the plate, if it is a tree plate
            BOOL drawAsUnrooted = YES;
            if ( [element isConnectedToTree] == YES )
                {
                if ( [[element getOrderingTree] rootednessType] == ROOTED_TREE )
                    drawAsUnrooted = NO;
                }
            NSRect tRect;
            float w, h;
            if ( pr.size.width / pr.size.height < 1.5 )
                {
                w = pr.size.width * 0.8;
                h = w * 2.0 / 3.0;
                }
            else 
                {
                h = pr.size.height * 0.8;
                w = 1.5 * h;
                }
            tRect.origin = NSMakePoint(pr.origin.x + (pr.size.width-w)*0.5, pr.origin.y + (pr.size.height-h)*0.5);
            tRect.size = NSMakeSize(w, h);
            [[[NSColor blackColor] colorWithAlphaComponent:0.10] set];
            
            if ( drawAsUnrooted == YES )
                {
                NSPoint n[6];
                for (int i=0; i<6; i++)
                    n[i] = tRect.origin;
                float seg = w / 3.0;
                n[1].x += seg; 
                n[1].y += seg;
                n[2].y += h;
                n[3].x += h; 
                n[3].y += seg;
                n[4].x += w; 
                n[4].y += h;
                n[5].x += w;
                NSBezierPath* tPath = [NSBezierPath bezierPath];
                [tPath setLineWidth:(0.06 * tRect.size.width)];
                [tPath setLineCapStyle:NSRoundLineCapStyle];
                [tPath moveToPoint:n[0]];
                [tPath lineToPoint:n[1]];            
                [tPath moveToPoint:n[1]];
                [tPath lineToPoint:n[2]];            
                [tPath moveToPoint:n[1]];
                [tPath lineToPoint:n[3]];            
                [tPath moveToPoint:n[3]];
                [tPath lineToPoint:n[4]];            
                [tPath moveToPoint:n[3]];
                [tPath lineToPoint:n[5]];
                [tPath stroke];
                }
            else
                {
                NSPoint tp[14];
                tp[0] = NSMakePoint(0.0, 1.0);
                tp[1] = NSMakePoint(0.5, 1.0);
                tp[2] = NSMakePoint(1.0, 1.0);
                tp[3] = NSMakePoint(1.5, 1.0);
                tp[4] = NSMakePoint(0.00, 0.4);
                tp[5] = NSMakePoint(0.25, 0.4);
                tp[6] = NSMakePoint(0.50, 0.4);
                tp[7] = NSMakePoint(1.00, 0.6);
                tp[8] = NSMakePoint(1.25, 0.6);
                tp[9] = NSMakePoint(1.50, 0.6);
                tp[10] = NSMakePoint(0.25, 0.15);
                tp[11] = NSMakePoint(0.75, 0.15);
                tp[12] = NSMakePoint(1.25, 0.15);
                tp[13] = NSMakePoint(0.75, 0.00);
                for (int i=0; i<14; i++)
                    {
                    tp[i].x *= (1.0/1.5)*tRect.size.width;
                    tp[i].y *= (1.0/1.5)*tRect.size.width;
                    tp[i].x += tRect.origin.x;
                    tp[i].y += tRect.origin.y;
                    }
                NSBezierPath* treePath = [NSBezierPath bezierPath];
                [treePath setLineWidth:(0.06 * tRect.size.width)];
                [treePath setLineCapStyle:NSRoundLineCapStyle];
                [treePath setLineJoinStyle:NSRoundLineJoinStyle];
                [treePath moveToPoint:tp[0]];
                [treePath lineToPoint:tp[4]];
                [treePath lineToPoint:tp[6]];
                [treePath lineToPoint:tp[1]];
                [treePath moveToPoint:tp[2]];
                [treePath lineToPoint:tp[7]];
                [treePath lineToPoint:tp[9]];
                [treePath lineToPoint:tp[3]];
                [treePath moveToPoint:tp[5]];
                [treePath lineToPoint:tp[10]];
                [treePath lineToPoint:tp[12]];
                [treePath lineToPoint:tp[8]];
                [treePath moveToPoint:tp[11]];
                [treePath lineToPoint:tp[13]];
                [treePath stroke];
                }
            
            // draw the little connection circle
            bool isCursorOverThisTreePlateOnActiveConnection = NO;
            if ( optionClicked == YES && selection.selectedItem != nil && selection.selectionType == ITEM_SELECTION)
                isCursorOverThisTreePlateOnActiveConnection = [self isCursorAtPoint:cursorLocation overTreePlate:element];
            NSPoint closestPt = NSZeroPoint;
            if ( [element isConnectedToTree] == YES )
                {
                ParmTree* ct = [element getOrderingTree];
                closestPt = [self findClosestPointBetweenTree:ct andTreePlate:element];
                }
            else
                {
                closestPt = [self findClosestPointToTreeForPlate:element];
                if ( fabs(closestPt.x) < 0.00001 && fabs(closestPt.y) < 0.00001 )
                    {
                    closestPt = pr.origin;
                    closestPt.y += pr.size.height*0.25;
                    }
                }
            
            NSRect cRect = informationRect[scaleIdx];
            float littleCircleScaleFactor = 0.75;
            cRect.size.width *= littleCircleScaleFactor;
            cRect.size.height *= littleCircleScaleFactor;
            cRect.origin = closestPt;
            cRect.origin.x -= cRect.size.width*0.5;
            cRect.origin.y -= cRect.size.height*0.5;
            if ( [element isConnectedToTree] == YES )
                [[NSColor blackColor] set];
            else
                [[NSColor whiteColor] set];
            [[NSBezierPath bezierPathWithOvalInRect:cRect] fill];
            if ( [element isConnectedToTree] == YES )
                [[NSColor greenColor] set];
            else
                [[NSColor redColor] set];
            if (isCursorOverThisTreePlateOnActiveConnection == YES && [element isConnectedToTree] == NO)
                [NSBezierPath setDefaultLineWidth:4.0*scaleFactor];
            else
                [NSBezierPath setDefaultLineWidth:2.0*scaleFactor];
            [[NSBezierPath bezierPathWithOvalInRect:cRect] stroke];
			
			// draw the plate's information in the bottom right corner
			if ( [element isCursorOver] == YES || (draggedItems == YES && element == selection.selectedItem) )
				{
                // show the little "i" button
				NSRect iRect = informationRect[scaleIdx];
				float delta  = pr.size.width - ITEM_IMAGE_SIZE * scaleFactor;
				iRect.origin = NSMakePoint(pr.origin.x + rOffset[scaleIdx].x + delta, pr.origin.y + rOffset[scaleIdx].y);
				NSPoint p    = NSMakePoint(pr.origin.x + iOffset[scaleIdx].x + delta, pr.origin.y + iOffset[scaleIdx].y);
				[[[NSColor blackColor] colorWithAlphaComponent:0.25] set];
				[[NSBezierPath bezierPathWithOvalInRect:iRect] fill];
				[selectedAttributedString[scaleIdx] drawAtPoint:p];
				}
			else 
				{
                // write out the mathematical notation for the iterator
				NSDictionary* attrs;
				if ( [element isCursorOver] == YES || (draggedItems == YES && element == selection.selectedItem) )
					attrs = [NSDictionary dictionaryWithObjects:[NSArray arrayWithObjects:[NSFont fontWithName:@"Times" size:(18.0*scaleFactor)], [[NSColor whiteColor] colorWithAlphaComponent:1.0], nil] forKeys:[NSArray arrayWithObjects:NSFontAttributeName,NSForegroundColorAttributeName, nil]];
				else 
					attrs = [NSDictionary dictionaryWithObjects:[NSArray arrayWithObjects:[NSFont fontWithName:@"Times" size:(18.0*scaleFactor)], [[NSColor blackColor] colorWithAlphaComponent:1.0], nil] forKeys:[NSArray arrayWithObjects:NSFontAttributeName,NSForegroundColorAttributeName, nil]];
				unichar uc = 8712;
				NSString* endingRangeStr = [element getEndingRangeForPlate];
				NSRange endingRangeRange = [element italicsRange];
				NSString* infoStr = [NSString stringWithFormat:@"%c %C (1,...,", [element indexLetter], uc];
				infoStr = [infoStr stringByAppendingString:endingRangeStr];
				infoStr = [infoStr stringByAppendingString:@")"];
				NSMutableAttributedString* attrString = [[NSMutableAttributedString alloc] initWithString:infoStr attributes:attrs];
				[attrString applyFontTraits:NSItalicFontMask range:NSMakeRange(0, 1)];
				if ( endingRangeRange.length != 0 )
					{
					endingRangeRange.location += 11;
					[attrString applyFontTraits:NSItalicFontMask range:endingRangeRange];
					}
<<<<<<< HEAD
				NSRect textSize = [attrString boundingRectWithSize:NSMakeSize(1e10, 1e10) options:NSStringDrawingUsesLineFragmentOrigin];
=======
				NSRect textSize = [attrString boundingRectWithSize:NSMakeSize(1e10, 1e10) options:NSStringDrawingUsesDeviceMetrics];
>>>>>>> 61480579c58f84b39cbc487a4a6a95bbd1fa0697
				float padding = 4.0 * scaleFactor;
				textSize.origin.x = pr.origin.x + (pr.size.width - textSize.size.width - padding);
				textSize.origin.y = pr.origin.y + padding;
				NSPoint p = textSize.origin;
				[attrString drawAtPoint:p];
				[element setMinPlateSize:NSMakeSize(textSize.size.width + 2.0*padding, textSize.size.height + 2.0*padding)];
				}
            
			// draw the selection information, with handles
			if ([element isSelected] == YES)
				{
				[[NSColor darkGrayColor] set];
				NSRect handleRect;
				handleRect.origin = pr.origin;
				handleRect.size = NSMakeSize(15.0*scaleFactor, 15.0*scaleFactor);
				float oneHalfWidth = handleRect.size.width * 0.5;
				handleRect.origin.x += (pr.size.width - oneHalfWidth);
				handleRect.origin.y -= oneHalfWidth;
				[[NSBezierPath bezierPathWithRect:handleRect] fill];
				
				handleRect.origin = pr.origin;
				handleRect.origin.x -= oneHalfWidth;
				handleRect.origin.y += (pr.size.height - oneHalfWidth);
				[[NSBezierPath bezierPathWithRect:handleRect] fill];
				}
			}
		}

	// draw an active drag between parameters (linking them up)
	if ( optionClicked == YES && selection.selectedItem != nil && selection.selectionType == ITEM_SELECTION)
		{
        // check that the user isn't trying to drag from a plate (which is not allowed)
        if ( [selection.selectedItem isPlate] == NO )
            {
            bool isCursorOverTreePlateOnActiveConnection = [self isCursorOverTreePlate:cursorLocation];
            if ( [selection.selectedItem parmType] != PARM_TREE )
                isCursorOverTreePlateOnActiveConnection = NO;

            [NSGraphicsContext saveGraphicsState];
            [shadow set];
            
            NSPoint startPoint = [selection.selectedItem itemCenter];
            [self transformToBottomLeftCoordinates:(&startPoint)];
            NSBezierPath* connection = [NSBezierPath bezierPath];
            [connection moveToPoint:startPoint];
            [connection lineToPoint:cursorLocation];
            [connection setLineWidth:(2.0*scaleFactor)];
            [[NSColor blackColor] set];
            [connection stroke];
            
            if (isCursorOverTreePlateOnActiveConnection == NO)
                {
                NSBezierPath* arrow = [connection bezierPathWithArrowHeadForEndOfLength:20.0 angle:25.0];
                [arrow closePath];
                [arrow fill];
                [arrow stroke];	
                }
            else
                {
                NSRect tipRect = informationRect[scaleIdx];
                tipRect.size.width = [connection lineWidth]*5.0;
                tipRect.size.height = [connection lineWidth]*5.0;
                tipRect.origin = cursorLocation;
                tipRect.origin.x -= tipRect.size.width*0.5;
                tipRect.origin.y -= tipRect.size.height*0.5;
                [[NSBezierPath bezierPathWithOvalInRect:tipRect] fill];
                }
            
            [NSGraphicsContext restoreGraphicsState];
            }
        }         	

    // draw the connections
    [NSGraphicsContext saveGraphicsState];
    [shadow set];
	parmEnumerator = [itemsPtr objectEnumerator];
	while ( (element = [parmEnumerator nextObject]) )
		{
		NSPoint a = [element itemCenter];
		[self transformToBottomLeftCoordinates:(&a)];
		for (int i=0; i<[element numParents]; i++)
			{
            if ( !([element parmType] == PARM_TREE_PLATE && [[element getParentIndexed:i] parmType] == PARM_TREE) )
                {
                // a normal connection between parameters
                // find the beginning and ending points
                NSPoint b = [[element getParentIndexed:i] itemCenter];
                [self transformToBottomLeftCoordinates:(&b)];
                
                // get the offset
                NSPoint newA = [self newPointDistance:(0.55 * ITEM_IMAGE_SIZE * scaleFactor) betweenPoint:a andPoint:b];
                NSPoint newB = [self newPointDistance:(0.55 * ITEM_IMAGE_SIZE * scaleFactor) betweenPoint:b andPoint:a];

                // draw the line
                NSBezierPath* connection = [NSBezierPath bezierPath];
                [connection moveToPoint:newB];
                [connection lineToPoint:newA];

                // set the line width
                [connection setLineWidth:(2.0*scaleFactor)];
                if ( [self isLinkSelectedBetweenParent:[element getParentIndexed:i] andChild:element] == YES )
                    [connection setLineWidth:(4.0*scaleFactor)];

                // set the color
                [[NSColor blackColor] set];
                
                // stroke the line
                [connection stroke];

                // and add the arrow
                NSBezierPath* arrow = [connection bezierPathWithArrowHeadForEndOfLength:(20.0*scaleFactor) angle:25.0];
                [arrow closePath];
                [arrow fill];
                [arrow stroke];
                }
            else
                {
                // we have a connection between a tree and a tree plate
                NSPoint closestP = [self findClosestPointBetweenTree:(ParmTree*)[element getParentIndexed:i] andTreePlate:element];
                NSPoint b = [[element getParentIndexed:i] itemCenter];
                [self transformToBottomLeftCoordinates:(&b)];
                NSPoint newB = [self newPointDistance:(0.55 * ITEM_IMAGE_SIZE * scaleFactor) betweenPoint:b andPoint:a];

                NSBezierPath* connection = [NSBezierPath bezierPath];
                [connection moveToPoint:newB];
                [connection lineToPoint:closestP];
                [connection setLineWidth:(2.0*scaleFactor)];
                if ( [self isLinkSelectedBetweenParent:[element getParentIndexed:i] andChild:element] == YES )
                    [connection setLineWidth:(4.0*scaleFactor)];
                [[NSColor blackColor] set];
                [connection stroke];
                }
			}
        }
    [NSGraphicsContext restoreGraphicsState];
    
	// get pointers to images to be drawn over items
    NSImage* magnifyingImageOff = [NSImage imageNamed:@"Unselected_Magnifier.icns"];
    [magnifyingImageOff setSize:NSMakeSize(ITEM_IMAGE_SIZE*scaleFactor*0.2, ITEM_IMAGE_SIZE*scaleFactor*0.2)];
	
	// draw the parameter images
	parmEnumerator = [itemsPtr objectEnumerator];
	while ( (element = [parmEnumerator nextObject]) )
		{
		// skip if it is a plate
		if ( [element isPlate] == YES )
			continue;

        // get the rectangle for drawing
		NSRect drawingRect;
		drawingRect.origin = [element itemLocation];
		drawingRect.size = [element itemSize];
		[self transformToBottomLeftCoordinates:(&drawingRect.origin)];
        
		// draw a focus ring behind the tool, if it is currently selected
        if ([element isSelected] == YES)
            {
            NSRect focusRect;
            focusRect = drawingRect;
            focusRect.origin.x -= 5.0;
            focusRect.origin.y -= 5.0;
            focusRect.size.height += 10.0;
            focusRect.size.width += 10.0;
            [[NSColor keyboardFocusIndicatorColor] set];
            [[NSBezierPath bezierPathWithOvalInRect:focusRect] fill];
            }

        // draw the image
        [parameterDrawObj drawParameterIndexed:[element drawingIndex] andWithAddress:element inRect:drawingRect];

		// draw the information button in the lower-right corner of the tool
		NSRect iRect = informationRect[scaleIdx];
		iRect.origin = NSMakePoint(drawingRect.origin.x + rOffset[scaleIdx].x, drawingRect.origin.y + rOffset[scaleIdx].y);
		NSPoint p    = NSMakePoint(drawingRect.origin.x + iOffset[scaleIdx].x, drawingRect.origin.y + iOffset[scaleIdx].y);
        
		if ( [element isCursorOver] == YES || (draggedItems == YES && element == selection.selectedItem) )
			{
			[[[NSColor blackColor] colorWithAlphaComponent:0.25] set];
			[[NSBezierPath bezierPathWithOvalInRect:iRect] fill];
			[selectedAttributedString[scaleIdx] drawAtPoint:p];
			}
		else 
			{
			[unselectedAttributedString[scaleIdx] drawAtPoint:p];
			}

		// draw the magnifying glass in the lower-left corner of the tool
        if ( 0 /*[element hasInspectorInfo] == YES*/ )
            {
            
			NSRect infoImageRect;
			infoImageRect.origin = NSZeroPoint;
			infoImageRect.size = [magnifyingImageOff size];
			NSRect mRect = informationRect[scaleIdx];
			mRect.origin = NSMakePoint(drawingRect.origin.x + mOffset[scaleIdx].x, drawingRect.origin.y + mOffset[scaleIdx].y);
            if ( [element isCursorOver] == YES || (draggedItems == YES && element == selection.selectedItem) )
				{
				[[[NSColor blackColor] colorWithAlphaComponent:0.25] set];
				[[NSBezierPath bezierPathWithOvalInRect:mRect] fill];
                [magnifyingImageOff drawInRect:mRect fromRect:infoImageRect operation:NSCompositeSourceOver fraction:1.0];
				}
            else 
				{
                [magnifyingImageOff drawInRect:mRect fromRect:infoImageRect operation:NSCompositeSourceOver fraction:1.0];
				}
            }
            
        // draw the information on plate membership
		if ( [element numAssignedPlates] > 0 )
			{
            ParmDraw* pd = [parameterDrawObj getParmElementIndexed:[element drawingIndex]];
            NSRect subRect = [pd subscriptPosition];
            if ( [element parmType] == PARM_TREE )
                {
                if ([element rootednessType] == ROOTED_TREE)
                    subRect.origin.y -= 0.10;
                }
            float s = subRect.size.width;
            NSDictionary* attrs = [NSDictionary dictionaryWithObjects:[NSArray arrayWithObjects:[NSFont fontWithName:@"Times" size:(1.0*s*drawingRect.size.width)], [[NSColor blackColor] colorWithAlphaComponent:1.0], nil] forKeys:[NSArray arrayWithObjects:NSFontAttributeName,NSForegroundColorAttributeName, nil]];

			NSMutableString* infoStr = [NSMutableString stringWithCapacity:1];
			for (int i=0; i<[element numAssignedPlates]; i++)
				{
				NSString* charStr = [NSString stringWithFormat:@"%c", [[element getPlateIndexed:i] indexLetter]];
				[infoStr appendString:charStr];
				}
			NSMutableAttributedString* attrString = [[NSMutableAttributedString alloc] initWithString:infoStr attributes:attrs];
			[attrString applyFontTraits:NSItalicFontMask range:NSMakeRange(0, [element numAssignedPlates])];
			NSPoint p = subRect.origin;
            p.x *= drawingRect.size.width;
            p.y *= drawingRect.size.width;
            p.x += drawingRect.origin.x;
            p.y += drawingRect.origin.y;
			[attrString drawAtPoint:p];
			}
        }
        
    // draw the parameter names
	if ( showParmNames == YES )
		{
        NSDictionary* attrs = [NSDictionary dictionaryWithObjects:[NSArray arrayWithObjects:[NSFont fontWithName:@"Chalkboard" size:(18.0*scaleFactor)], [[NSColor blackColor] colorWithAlphaComponent:0.5], nil]
								                          forKeys:[NSArray arrayWithObjects:NSFontAttributeName,NSForegroundColorAttributeName, nil]];
		parmEnumerator = [itemsPtr objectEnumerator];
		while ( (element = [parmEnumerator nextObject]) )
			{
			if ([element isPlate] == YES)
				continue;
				
			NSPoint p = [element itemCenter];
			[self transformToBottomLeftCoordinates:(&p)];
			NSString* pn = [element parmName];
			
			NSAttributedString* attrString = [[NSAttributedString alloc] initWithString:pn attributes:attrs];
<<<<<<< HEAD
			NSRect textSize = [attrString boundingRectWithSize:NSMakeSize(1e10, 1e10) options:NSStringDrawingUsesLineFragmentOrigin];
=======
			NSRect textSize = [attrString boundingRectWithSize:NSMakeSize(1e10, 1e10) options:NSStringDrawingUsesDeviceMetrics];
>>>>>>> 61480579c58f84b39cbc487a4a6a95bbd1fa0697
			p.x -= textSize.size.width * 0.5;
			p.y -= (ITEM_IMAGE_SIZE * scaleFactor * 0.5 + textSize.size.height * 0.75);

			[attrString drawAtPoint:p];
			}
		}                                
    
	// draw a sweep for multiple selection
	if (sweepAction == YES)
		{
		NSRect sweepRect;

		NSPoint p1 = sweepStartLocation;
		NSPoint p2 = sweepCurrentLocation;
		sweepRect.origin = p1;
		if (p2.x < p1.x)
			sweepRect.origin.x = p2.x;
		if (p2.y < p1.y)
			sweepRect.origin.y = p2.y;
		sweepRect.size.width = p1.x - p2.x;
		sweepRect.size.height = p1.y - p2.y;
		if (sweepRect.size.width < 0.0)
			sweepRect.size.width = -sweepRect.size.width;
		if (sweepRect.size.height < 0.0)
			sweepRect.size.height = -sweepRect.size.height;

		NSColor* sweepColor = [NSColor whiteColor];
		sweepColor = [sweepColor colorWithAlphaComponent:0.3];
		[sweepColor set];
		[NSBezierPath fillRect:sweepRect];
		}
}

- (NSPoint)findClosestPointToTreeForPlate:(Parm*)tp {

    NSRect pr = [(ParmPlate*)tp plateRect];
    NSPoint closestPt = NSZeroPoint;
    NSPoint p1 = [tp itemCenter];
    [self transformToBottomLeftCoordinates:&p1];
    float smallestD = 1000000000.0;
    NSEnumerator* parmEnumerator = [itemsPtr objectEnumerator];
    id element;
    while ( (element = [parmEnumerator nextObject]) )
        {
        if ( element != tp && [element parmType] == PARM_TREE )
            {
            if ( [element doesTreeOrderPlate] == YES )
                continue;
            NSPoint p2 = [element itemCenter];
            [self transformToBottomLeftCoordinates:&p2];
            float d = [self distanceFromPoint:p1 toPoint:p2];
            if (d < smallestD)
                {
                smallestD = d;
                float absBX = fabsf(p1.x-p2.x);
                float absBY = fabsf(p1.y-p2.y);
                float dV = pr.size.height*0.5 * d / absBY;
                float dH = pr.size.width*0.5 * d / absBX;
                if (absBY/absBX > pr.size.height/pr.size.width)
                    closestPt = [self newPointDistance:dV betweenPoint:p1 andPoint:p2];
                else
                    closestPt = [self newPointDistance:dH betweenPoint:p1 andPoint:p2];
                }
            }
        }
    return closestPt;
}

- (NSPoint)findClosestPointBetweenTree:(ParmTree*)t andTreePlate:(ParmPlateTree*)tp {

    NSRect  pr = [tp plateRect];
    NSPoint p1 = [tp itemCenter];
    NSPoint p2 = [t itemCenter];
    [self transformToBottomLeftCoordinates:&p1];
    [self transformToBottomLeftCoordinates:&p2];
    float d = [self distanceFromPoint:p1 toPoint:p2];
    NSPoint closestPt = NSZeroPoint;
    float absBX = fabsf(p1.x-p2.x);
    float absBY = fabsf(p1.y-p2.y);
    float dV = pr.size.height*0.5 * d / absBY;
    float dH = pr.size.width*0.5 * d / absBX;
    if (absBY/absBX > pr.size.height/pr.size.width)
        closestPt = [self newPointDistance:dV betweenPoint:p1 andPoint:p2];
    else
        closestPt = [self newPointDistance:dH betweenPoint:p1 andPoint:p2];
    return closestPt;
}

- (char)getNextAvailableIndex {

	BOOL takenIndices[26];
	for (int i=0; i<26; i++)
		takenIndices[i] = NO;
	id element;
	NSEnumerator* parmEnumerator = [itemsPtr objectEnumerator];
	while ( (element = [parmEnumerator nextObject]) )
		{
		if ([element isPlate] == YES)
			{
			char c = [element indexLetter];
			int i = 0;
			while (availableIndices[i] != c)
				i++;
			if (i < 26)
				takenIndices[i] = YES;
			}
		}
	for (int i=0; i<26; i++)
		{
		if (takenIndices[i] == NO)
			return availableIndices[i];
		}
	return '0';
}

- (NSString*)getNextParameterName {

    // look through the (infinite) list of default parameter names and choose an unused one
    // (eventually we should exit the infinite loop)
    int i = 1;
    do 
        {
        NSString* potentialName = [NSString stringWithFormat:@"Parameter %d", i];
        if ( [self isNameTaken:potentialName] == NO )
            return potentialName;
        i++;
        } while (1);
                
    // things are quite weird and we will simply send an empty string (not certain how we can ever get here frankly)
    NSString* defaultName = @"";
	return defaultName;
}

- (BOOL)hasItemCollided:(RbItem*)itm {

    if ( [(Parm*)itm isPlate] == YES )
        return NO;
        
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
            if ( CGRectIntersectsRect( NSRectToCGRect(itemRect), NSRectToCGRect(r) ) == YES && [element isPlate] == NO )
                return YES;
            }
		}

	return NO;
}

- (id)initWithFrame:(NSRect)frame {

    if ( (self = [super initWithFrame:frame]) ) 
		{
        // allocate memory for parameter font attributes
        NSDictionary* attrs = [NSDictionary dictionaryWithObjects:[NSArray arrayWithObjects:[NSFont fontWithName:@"Chalkboard" size:(18.0)], [[NSColor blackColor] colorWithAlphaComponent:0.5], nil]
								                          forKeys:[NSArray arrayWithObjects:NSFontAttributeName,NSForegroundColorAttributeName, nil]];
        NSString* tempName = @"TEMP";
        NSAttributedString* tempText = [[NSAttributedString alloc] initWithString:tempName attributes:attrs];
<<<<<<< HEAD
        parmNameHeight = [tempText boundingRectWithSize:NSMakeSize(1e10, 1e10) options:NSStringDrawingUsesLineFragmentOrigin].size.height;
=======
        parmNameHeight = [tempText boundingRectWithSize:NSMakeSize(1e10, 1e10) options:NSStringDrawingUsesDeviceMetrics].size.height;
>>>>>>> 61480579c58f84b39cbc487a4a6a95bbd1fa0697
        
        // allocate an NSView object for drawing parameters
        parameterDrawObj = [[PaletteView alloc] initWithFrame:[self frame]];
        
        // allocate memory for array holding selected links
        selectedLinks = [[NSMutableArray alloc] init];
		
        // add self as observer for scale factor change
		NSNotificationCenter *defaultCenter = [NSNotificationCenter defaultCenter];		 
		[defaultCenter addObserver:self
						  selector:@selector(scaleFactorChanged:)
							  name:@"mvScaleFactorChangeNotification"
						    object:nil];

        // set default values for a few of the parameters in this class 
		optionClicked = NO;
        parmCount     = 0;
		
		// set the available indices
		[self setIndices];

        // get the background color and grid color from the defaults
        NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
        NSData *colorAsData = [defaults objectForKey:RB_ModelBgrndColorKey];
        bkgrndColor = [NSKeyedUnarchiver unarchiveObjectWithData:colorAsData];
        colorAsData = [defaults objectForKey:RB_ModelGridColorKey];
        gridColor = [NSKeyedUnarchiver unarchiveObjectWithData:colorAsData];
        
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

<<<<<<< HEAD
        NSRect iRect = [selectedAttributedString[i] boundingRectWithSize:NSMakeSize(1e10, 1e10) options:NSStringDrawingUsesLineFragmentOrigin];
=======
        NSRect iRect = [selectedAttributedString[i] boundingRectWithSize:NSMakeSize(1e10, 1e10) options:NSStringDrawingUsesDeviceMetrics];
>>>>>>> 61480579c58f84b39cbc487a4a6a95bbd1fa0697
        NSPoint tempP = iRect.origin;
        NSRect glyphRect = [selectedAttributedString[i] boundingRectWithSize:NSMakeSize(1e10, 1e10) options:NSStringDrawingUsesDeviceMetrics];
        iRect.size = glyphRect.size;
        iRect.size.height *= 1.25;
        iRect.size.width = iRect.size.height;
        iRect.origin = NSZeroPoint;
        float padding = 4.0 * sf[i];
        rOffset[i] = NSMakePoint( (ts.width - iRect.size.width)*0.5, (ts.height*0.33 - iRect.size.height)*0.5 );
        mOffset[i] = NSMakePoint(padding, padding);
        iOffset[i] = rOffset[i];
        iOffset[i].x -= glyphRect.origin.x;
        iOffset[i].y += tempP.y;
        iOffset[i].x += (iRect.size.width  - glyphRect.size.width)  * 0.5 + iRect.size.width * 0.02;
        iOffset[i].y += (iRect.size.height - glyphRect.size.height) * 0.5 + iRect.size.width * 0.04;
        informationRect[i] = iRect;
        }
}

- (BOOL)isCursorAtPoint:(NSPoint)p overTreePlate:(ParmPlate*)plt {

    if ( [plt parmType] == PARM_TREE_PLATE )
        {
        NSRect pr = [plt plateRect];
        pr.origin = [plt itemLocation];
        [self transformToBottomLeftCoordinates:&(pr.origin)];
        if ( CGRectContainsPoint( NSRectToCGRect(pr), NSPointToCGPoint(p) ) )
            return YES;
        }
    return NO;
}

- (BOOL)isCursorOverTreePlate:(NSPoint)p {

	NSEnumerator* parmEnumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [parmEnumerator nextObject]) )
        {
        if ( [element parmType] == PARM_TREE_PLATE )
            {
            if ( [self isCursorAtPoint:p overTreePlate:element] == YES )
                return YES;
            }
        }
    return NO;
}

- (Parm*)instantiateParm:(NSString*)pName {

    Parm* p = nil;
    if      ( [pName isEqualToString:@"Real Number Random Variable"                    ] == YES ) { p = [[ParmReal               alloc] initWithScaleFactor:scaleFactor andTool:myTool]; }
    else if ( [pName isEqualToString:@"Positive Real Number Random Variable"           ] == YES ) { p = [[ParmRealPos            alloc] initWithScaleFactor:scaleFactor andTool:myTool]; }
    else if ( [pName isEqualToString:@"Integer Number Random Variable"                 ] == YES ) { p = [[ParmInteger            alloc] initWithScaleFactor:scaleFactor andTool:myTool]; }
    else if ( [pName isEqualToString:@"Natural Number Random Variable"                 ] == YES ) { p = [[ParmIntegerPos         alloc] initWithScaleFactor:scaleFactor andTool:myTool]; }
    else if ( [pName isEqualToString:@"Vector of Real Number Random Variables"         ] == YES ) { p = [[ParmVectorReal         alloc] initWithScaleFactor:scaleFactor andTool:myTool]; }
    else if ( [pName isEqualToString:@"Vector of Positive Real Number Random Variables"] == YES ) { p = [[ParmVectorRealPos      alloc] initWithScaleFactor:scaleFactor andTool:myTool]; }
    else if ( [pName isEqualToString:@"Vector of Integer Number Random Variables"      ] == YES ) { p = [[ParmVectorInteger      alloc] initWithScaleFactor:scaleFactor andTool:myTool]; }
    else if ( [pName isEqualToString:@"Vector of Natural Number Random Variables"      ] == YES ) { p = [[ParmVectorIntegerPos   alloc] initWithScaleFactor:scaleFactor andTool:myTool]; }
    else if ( [pName isEqualToString:@"Simplex Random Variable"                        ] == YES ) { p = [[ParmSimplex            alloc] initWithScaleFactor:scaleFactor andTool:myTool]; }
    else if ( [pName isEqualToString:@"Tree Topology Random Variable"                  ] == YES ) { p = [[ParmTree               alloc] initWithScaleFactor:scaleFactor andTool:myTool]; }
    else if ( [pName isEqualToString:@"Real Number Constant"                           ] == YES ) { p = [[ParmConstantReal       alloc] initWithScaleFactor:scaleFactor andTool:myTool]; }
    else if ( [pName isEqualToString:@"Positive Real Number Constant"                  ] == YES ) { p = [[ParmConstantRealPos    alloc] initWithScaleFactor:scaleFactor andTool:myTool]; }
    else if ( [pName isEqualToString:@"Integer Number Constant"                        ] == YES ) { p = [[ParmConstantInteger    alloc] initWithScaleFactor:scaleFactor andTool:myTool]; }
    else if ( [pName isEqualToString:@"Natural Number Constant"                        ] == YES ) { p = [[ParmConstantIntegerPos alloc] initWithScaleFactor:scaleFactor andTool:myTool]; }
    else if ( [pName isEqualToString:@"Vector of Real Number Constants"                ] == YES ) { p = [[ParmConstantVectorReal alloc] initWithScaleFactor:scaleFactor andTool:myTool]; }
    else if ( [pName isEqualToString:@"Vector of Positive Real Number Constants"       ] == YES ) { }
    else if ( [pName isEqualToString:@"Vector of Integer Number Constants"             ] == YES ) { }
    else if ( [pName isEqualToString:@"Vector of Natural Number Constants"             ] == YES ) { }
    else if ( [pName isEqualToString:@"Simplex Constant"                               ] == YES ) { }
    else if ( [pName isEqualToString:@"Tree Topology Constant"                         ] == YES ) { }
    else if ( [pName isEqualToString:@"Plate"                                          ] == YES ) { p = [[ParmPlate              alloc] initWithScaleFactor:scaleFactor andTool:myTool]; }
    else if ( [pName isEqualToString:@"Tree Plate"                                     ] == YES ) { p = [[ParmPlateTree          alloc] initWithScaleFactor:scaleFactor andTool:myTool]; }
    else if ( [pName isEqualToString:@"Rate Matrix Function"                           ] == YES ) { p = [[ParmRateMatrix         alloc] initWithScaleFactor:scaleFactor andTool:myTool]; }
    else { return nil; }
    return p;
}

- (BOOL)isLinkSelectedBetweenParent:(Parm*)x andChild:(Parm*)y {

	NSEnumerator* enumerator = [selectedLinks objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
        {
        if ( ([element parentParm] == x && [element childParm] == y) || ([element parentParm] == y && [element childParm] == x) )
            return YES;
        }
    return NO;
}

- (BOOL)isNameTaken:(NSString*)nme {

	id element;
	NSEnumerator* parmEnumerator = [itemsPtr objectEnumerator];
	while ( (element = [parmEnumerator nextObject]) )
        {
        if ( [nme isEqualToString:[element parmName]] == YES )
            return YES;
        }
    return NO;
}

- (void)keyDown:(NSEvent*)event {

	[super keyDown:event];
	[self checkPlateMembership];
}

- (void)mouseDown:(NSEvent*)event {
	
	// inactivate the timer, if it is still going
	if ([analysisDocumentPtr isRbTimerActive] == YES)
		[analysisDocumentPtr invalidateTimer];
	[self stopItemTipTimer];
    	
	// see which item might have been selected
	ItemSelector mySelection = [self selectItem:[event locationInWindow]];
	selection = mySelection;
	
    optionClicked = NO;
    shiftClicked  = NO;
    draggedItems  = NO;
	sweepAction   = NO;
	if ([event modifierFlags] & NSControlKeyMask)
		{
        // control click
		if ( mySelection.selectedItem != nil && (mySelection.selectionType == ITEM_SELECTION || mySelection.selectionType == INFO_SELECTION || mySelection.selectionType == INSP_SELECTION) )
            [mySelection.selectedItem showControlPanel];
		}
	else if ([event modifierFlags] & NSAlternateKeyMask)
		{
        // option click
        if (mySelection.selectedItem != nil && mySelection.selectionType == ITEM_SELECTION)
			optionClicked = YES;
		else 
			[self removeAllFocusRings];
        }
	else if ([event modifierFlags] & NSShiftKeyMask)
		{
        // shift click
		shiftClicked = YES;
		if (mySelection.selectedItem != nil)
			{
			if (mySelection.selectionType == ITEM_SELECTION || mySelection.selectionType == INFO_SELECTION || mySelection.selectionType == INSP_SELECTION)
                {
				[mySelection.selectedItem flipSelected];
                }
            else if (mySelection.selectionType == LINK_SELECTION)
                {
                // TO DO: Check if the child/parent pair is currently selected. If it is, then 
                // remove the pair. O/w, add the pair.
                if ( [self isLinkSelectedBetweenParent:mySelection.parent andChild:mySelection.child] == YES )
                    [self removeLinkSelectedBetweenParent:mySelection.parent andChild:mySelection.child];
                else 
                    [self addToSelectedLinkListParent:mySelection.parent andChild:mySelection.child];
                }
			}
		else 
			{
			sweepStartLocation = [self convertPoint:[event locationInWindow] fromView:nil];
			[self removeAllFocusRings];
			}
		}
	else
		{	
        // click, with no modifier keys
		if (mySelection.selectedItem != nil)
			{
			if (mySelection.selectionType == INLET_SELECTION || mySelection.selectionType == OUTLET_SELECTION || 
			    mySelection.selectionType == TL_PLATE_RESIZE || mySelection.selectionType == BR_PLATE_RESIZE)
				[[NSCursor crosshairCursor] push];
			else if (mySelection.selectionType == ITEM_SELECTION)
				[mySelection.selectedItem setIsSelected:YES];
			else if (mySelection.selectionType == INFO_SELECTION)
                [mySelection.selectedItem showControlPanel];
			else if (mySelection.selectionType == INSP_SELECTION)
                [mySelection.selectedItem showInspectorPanel];
			NSPoint curP = [[self window] mouseLocationOutsideOfEventStream];
			[self selectCursorOverForItemsAtPoint:[self convertPoint:curP fromView:nil]];
			}
		else 
			{
			sweepStartLocation = [self convertPoint:[event locationInWindow] fromView:nil];
			[self removeAllFocusRings];
			}
		}
}

- (void)mouseDragged:(NSEvent*)event {
	
    // get the position of the cursor
	NSPoint p = [event locationInWindow];
	if (selection.selectedItem != nil)
		{
		if (selection.selectionType == ITEM_SELECTION && optionClicked == NO)
			{
			// we are moving a parameter
			
            // recalculate the item's coordinates: we are dragging the item
            if (draggedItems == NO)
                {
                // update information that we want to do only once during a drag operation
				// change the cursor
                [[NSCursor openHandCursor] push];
				
				// make certain selected items drag properly together
				if (shiftClicked == NO)
					[self removeAllFocusRings];
				[selection.selectedItem setIsSelected:YES];
                }
			draggedItems = YES;
                            
            // convert the point to the view's coordinates
            NSPoint newP = [self convertPoint:p fromView:nil];
            [self transformToTopLeftCoordinates:(&newP)];
            
            // account for the initial point where the item was selected
            newP.x -= cursorOffset.width;
            newP.y += cursorOffset.height;

			// find the nearest grid intersection to this point, if we are snapping the item to the grid
            if (snapToGrid == YES)
                newP = [self findNearestGridIntersectionToPoint:newP];
            
            // get the item's old location
            NSPoint oldP = [selection.selectedItem itemLocation];
			
			// get the change in the x- and y-values
			float xChange = newP.x - oldP.x;
			float yChange = newP.y - oldP.y;
            
            // set new location for all of the selected items
            NSEnumerator* enumerator = [itemsPtr objectEnumerator];
            id element;
			BOOL isCollided = NO;
            while ( (element = [enumerator nextObject]) )
				{
				if ([element isSelected] == YES)
					{
                    NSPoint np = [element itemLocation];
                    np.x += xChange;
                    np.y += yChange;
                    [element setItemLocation:np];
					if ([self hasItemCollided:element] == YES)
						isCollided = YES;
					}
				}
            
            // reset the bottom right corner
            [self setCorners];
			
			// check for collisions
			if ( topLeftCorner.x < 0.0 || topLeftCorner.y < 0.0 || isCollided == YES )
				{
				enumerator = [itemsPtr objectEnumerator];
				while ( (element = [enumerator nextObject]) )
					{
					if ([element isSelected] == YES)
						{
						NSPoint np = [element itemLocation];
						np.x -= xChange;
						np.y -= yChange;
						[element setItemLocation:np];
						}
					}
				}
            
            // update the scroll bars
            [self updateScrollBars];
			}
		else if ( (selection.selectionType == TL_PLATE_RESIZE || selection.selectionType == BR_PLATE_RESIZE) && optionClicked == NO )
			{
			// we are resizing the plate
						
            // convert the point to the view's coordinates
            NSPoint newP = [self convertPoint:p fromView:nil];
            [self transformToTopLeftCoordinates:(&newP)];
            
            // account for the initial point where the item was selected
            //newP.x -= cursorOffset.width;
            //newP.y += cursorOffset.height;

			// find the nearest grid intersection to this point, if we are snapping the item to the grid
            if (snapToGrid == YES)
                newP = [self findNearestGridIntersectionToPoint:newP];
            			
			// get the rectangle for the plate
			NSRect pRect = [selection.selectedItem plateRect];
			
			// get the other corner
			NSPoint otherP = [selection.selectedItem itemLocation];
			if (selection.selectionType == TL_PLATE_RESIZE)
				otherP.x += pRect.size.width;
			else 
				otherP.y -= pRect.size.height;
				
			// check that the plate isn't too small
			NSSize ms = [selection.selectedItem minPlateSize];
			if (fabs(newP.x-otherP.x) > ms.width && fabs(newP.y-otherP.y) > ms.height)
				{
				// get the new rectangle defined by the two corners
				NSRect newRect;
				if (otherP.x < newP.x)
					newRect.origin.x = otherP.x;
				else 
					newRect.origin.x = newP.x;
				if (otherP.y > newP.y)
					newRect.origin.y = otherP.y;
				else 
					newRect.origin.y = newP.y;
				newRect.size.width  = fabs(otherP.x - newP.x);
				newRect.size.height = fabs(otherP.y - newP.y);
				
				// set the new plate
				[selection.selectedItem setItemLocation:newRect.origin];
				[selection.selectedItem setPlateRect:newRect];
				[selection.selectedItem setImageWithSize:newRect.size];
				}
				
			// reset the scroll bars if we are dragging by the bottom-right corner
 			if (selection.selectionType == BR_PLATE_RESIZE)
				{
				[self setCorners];
				[self updateScrollBars];
				}
			}
		else 
			{
			NSPoint newP = [self convertPoint:p fromView:nil];
			cursorLocation = newP;
			}
		}
	else 
		{
        // we are performing a multiple-selection sweep
        if (sweepAction == NO)
            {
            // get the list of items originally selected
            [selectedItems removeAllObjects];
            NSEnumerator* enumerator = [itemsPtr objectEnumerator];
            id element;
            while ( (element = [enumerator nextObject]) )
                {
                if ([element isSelected] == YES)
                    [selectedItems addObject:element];
                // check for selected connections
                }
            }
		sweepAction = YES;
		sweepCurrentLocation = [self convertPoint:p fromView:nil];
        [self selectItemsInSweepArea];
		}
	[self autoscroll:event];
	[self setNeedsDisplay: YES];	
}

- (void)mouseUp:(NSEvent*)event {

	if (selection.selectedItem != nil)
		{
		if (selection.selectionType == ITEM_SELECTION)
			{
			// we had a mouse up on a selected item
            if (draggedItems == NO && shiftClicked == NO && optionClicked == NO)
                {
                [self removeAllFocusRings];
                [selection.selectedItem setIsSelected:YES];
                }
            else if (optionClicked == YES)
                {
                Parm* parm1 = selection.selectedItem;
                ItemSelector mySelection = [self selectNonPlateItemOnly:[event locationInWindow]];
                ItemSelector myPlateSelection = [self selectPlateItemOnly:[event locationInWindow]];
                Parm* parm2 = mySelection.selectedItem;
                Parm* plate2 = myPlateSelection.selectedItem;
                NSLog(@"parm2 = %@", parm2);
                NSLog(@"plate2 = %@", plate2);
                if (parm2 != nil && parm1 != parm2)
                    {
                    [parm1 addChild:parm2];
                    [parm2 addParent:parm1];
					[parm1 updateChildren];
                    }
                else if (plate2 != nil && [parm1 parmType] == PARM_TREE)
                    {
                    if ( [plate2 parmType] == PARM_TREE_PLATE )
                        {
                        if ( [(ParmPlateTree*)plate2 isConnectedToTree] == NO )
                            {
                            [parm1 addChild:plate2];
                            [plate2 addParent:parm1];
                            [parm1 updateChildren];
                            }
                        }
                    }
                }
            if (draggedItems == YES)
                {
                // we finished moving a item
                
                // update the cursor
                [[NSCursor openHandCursor] pop];

                // update the tracking areas
                NSEnumerator* enumerator = [itemsPtr objectEnumerator];
                id element;
                while ( (element = [enumerator nextObject]) )
					[self addTrackingForItem:element];
				[self mouseEntered:event];
				
				// check plate membership
				[self checkPlateMembership];
                }
			}
		else if (selection.selectionType == LINK_SELECTION)
			{
            if (draggedItems == NO && shiftClicked == NO)
                {
                [self removeAllFocusRings];
                [self addToSelectedLinkListParent:selection.parent andChild:selection.child];
                [self showSelectedLinks];
                }
			}
		else if (selection.selectionType == INFO_SELECTION)
            {
            
            }
		else if (selection.selectionType == INSP_SELECTION)
            {
            
            }
		else if (selection.selectionType == TL_PLATE_RESIZE || selection.selectionType == BR_PLATE_RESIZE)
            {
			// update the cursor
			[[NSCursor crosshairCursor] pop];

			// update the tracking areas
			NSEnumerator* enumerator = [itemsPtr objectEnumerator];
			id element;
			while ( (element = [enumerator nextObject]) )
				[self addTrackingForItem:element];

			// check plate membership
			[self checkPlateMembership];
            }
		else 
			{
			// we attempt to finish establishing a connection b/w inlet/outlets

			}
        [[[NSDocumentController sharedDocumentController] currentDocument] updateChangeCount:NSChangeDone];
		}
    else 
        {
        if (sweepAction == YES)
            {
            // we clean up a sweep action
            [selectedItems removeAllObjects];
            }
        }
		
    // put the selection back to a base state
	selection.selectedItem = nil;
    draggedItems = NO;
	shiftClicked = NO;
	sweepAction  = NO;
    
    // update the display
	[self setNeedsDisplay: YES];
}

- (NSPoint)newPointDistance:(float)d betweenPoint:(NSPoint)p1 andPoint:(NSPoint)p2 {

#   if 0
    float dX = p2.x - p1.x;
    float dY = p2.y - p1.y;
    float absDx = fabs(dX);
    float absDy = fabs(dY);
    
    NSPoint p;
    if (absDx > absDy)
        {
        p.x = p1.x + (dX / absDx) * d;
        p.y = p1.y + (dY / absDy) * (absDy*d/absDx);
        }
    else 
        {
        p.x = p1.x + (dX / absDx) * (absDx*d/absDy);
        p.y = p1.y + (dY / absDy) * d;
        }
    return p;

#   else
    float A = p1.x - p2.x;
    float B = p1.y - p2.y;
    float D = sqrt(A*A+B*B);
    float a = (d / D) * A;
    float b = (d / D) * B;
    
    NSPoint p;
    p.x = p1.x - a;
    p.y = p1.y - b;
    return p;
#   endif
}

- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender {

	if ( [sender draggingSource] != self )
		{
		// get the name of the parm that was selected?
        NSString* parmName = [[sender draggingSource] whichParmSelected];
        int parmIdx        = [[sender draggingSource] indexOfParameterWithName:parmName];
        
        // decide if the parameter is a plate or not
        BOOL isPlate = NO;
        if ( [parmName isEqualToString:@"Plate"] == YES || [parmName isEqualToString:@"Tree Plate"] == YES )
            isPlate = YES;
        
		// get the location of the drop and then convert it to the view's coordinate system
		NSPoint p = [sender draggedImageLocation];
		NSPoint newP = [self convertPoint:p fromView:nil];
		p = newP;
		[self transformToTopLeftCoordinates:(&p)];
		
		// Check that the drop location is OK. We don't drop parameters onto other parameters.
        // However, we do allow plates to be dropped anywhere, as long as they fit in the screen.
        if (isPlate == NO)
            {
            NSRect r;
            r.origin = p;
            r.size   = NSMakeSize(ITEM_IMAGE_SIZE*scaleFactor, ITEM_IMAGE_SIZE*scaleFactor);
            if ( [self hasRectCollided:r] == YES )
                return NO;
            }
        else 
            {
            NSRect r;
            r.origin = p;
            r.size   = NSMakeSize(200.0*scaleFactor, 200.0*scaleFactor);
            if ( [self hasRectCollidedWithEdge:r] == YES )
                return NO;
            }
		
		// allocate the parm, set its location, and add it to the list of parameters
        Parm* newParm = [self instantiateParm:parmName];
        if (newParm == nil)
            return NO;

        // set the name of the parm
		if ([newParm isPlate] == NO)
			[newParm setParmName:[self getNextParameterName]];
            
        // set the index for drawing of the parm
        [newParm setDrawingIndex:parmIdx];

        // set the location for the parm
		[newParm setItemLocation:p];
        
		// add the tool to the tracking area
		[self addTrackingForItem:newParm];
		[newParm setIsCursorOver:YES];
        itemArea.selectedItem = newParm;
        
        // set the window that the parameter lives on
        [newParm setParmsWindow:[self window]];

        // add the parm to the array of parameters
		[itemsPtr addObject:newParm];
		
		// reset the bottom right corner
		[self setCorners];
		
		// update the display
		[self setNeedsDisplay:YES];
		
		// add the index (if a plate)
		if ([newParm isPlate] == YES)
			{
			char c = [self getNextAvailableIndex];
			[(ParmPlate*)newParm setIndexLetter:c];
			}
			
		// check plate membership
		[self checkPlateMembership];
		}
	return YES;
}

- (BOOL)readFromPasteboard:(NSPasteboard*)pb {
		
	if ( [itemsPtr count] == 0 )
		pasteNum = 1;
		
    NSData* d = [pb dataForType:ToolDataPboardType];
    NSMutableArray* pastedTools = [NSKeyedUnarchiver unarchiveObjectWithData:d];
    
	NSEnumerator* parmEnumerator = [pastedTools objectEnumerator];
	id element;
	while ( (element = [parmEnumerator nextObject]) )
        {
        // offset the parameters location a bit
        NSPoint p = [element itemLocation];
		p.x += (5.0) * pasteNum;
		p.y += ([element itemSize].width + 5.0) * pasteNum;
		if (snapToGrid == YES)
			p = [self findNearestGridIntersectionToPoint:p];
        [element setItemLocation:p];

        // set the window for the parameter
        [element setParmsWindow:[self window]];
        
		// add the tool to the tracking area
		[self addTrackingForItem:element];
		[element setIsCursorOver:NO];

        // add the parameter to the array of parameters
		[itemsPtr addObject:element];
		
		// reset the bottom right corner
		[self setCorners];
        [self updateScrollBars];
        }
    
	// update the display
	[self setNeedsDisplay:YES];
    		
    return YES;
}

- (void)removeAllFocusRings {

	NSEnumerator* enumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
        [element setIsSelected:NO];
    [selectedLinks removeAllObjects];
    [self setNeedsDisplay:YES];
}

- (void)removeSelectedConnections {

	NSEnumerator* enumerator = [selectedLinks objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
        {
        [[element parentParm] removeChild:[element childParm]];
        [[element childParm] removeParent:[element parentParm]];
        }
    [selectedLinks removeAllObjects];
}

- (void)removeLinkSelectedBetweenParent:(Parm*)x andChild:(Parm*)y {

	NSEnumerator* enumerator = [selectedLinks objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
        {
        if ( ([element parentParm] == x && [element childParm] == y) || ([element parentParm] == y && [element childParm] == x) )
            {
            [selectedLinks removeObject:element];
            return;
            }
        }
}

- (void)scaleFactorChanged:(NSNotification*)notification {

    if ( myScrollView != [notification object] )
        return;

	// get the old and new scale factors
    float oldFactor = scaleFactor;
    scaleFactor = [[notification object] scaleFactor];
	
	// get the index for the scale, so we can choose the correct parameter image to display
	if (scaleFactor > 0.24 && scaleFactor < 0.26)
		scaleIdx = 0;
	else if (scaleFactor > 0.49 && scaleFactor < 0.51)
		scaleIdx = 1;
	else if (scaleFactor > 0.74 && scaleFactor < 0.76)
		scaleIdx = 2;
	else if (scaleFactor > 0.99 && scaleFactor < 1.01)
		scaleIdx = 3;
	else if (scaleFactor > 1.24 && scaleFactor < 1.26)
		scaleIdx = 4;
	else if (scaleFactor > 1.49 && scaleFactor < 1.51)
		scaleIdx = 5;
	else if (scaleFactor > 1.99 && scaleFactor < 2.01)
		scaleIdx = 6;
	else if (scaleFactor > 3.99 && scaleFactor < 4.01)
		scaleIdx = 7;
    
	// calculate the ratio of the scale factors to get a factor by which each
	// element in the interface will be multiplied
    float r = scaleFactor / oldFactor;
    NSSize s = NSMakeSize(ITEM_IMAGE_SIZE * scaleFactor, ITEM_IMAGE_SIZE * scaleFactor);

	// go through all of the parameters, changing the size of each by the factor and also
	// updating the tracking information for each tool
	NSEnumerator* parmEnumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [parmEnumerator nextObject]) )
		{
		// set the scale factor for the item (used when encoding the parameters)
		[element setCurrentScaleFactor:scaleFactor];
		
		// change the size of the tool's image
        NSPoint p = [element itemLocation];
        p.x *= r;
        p.y *= r;
        [element setItemLocation:p];
		if ([element isPlate] == NO)
			[element setImageWithSize:s];
		else 
			{
			NSSize ps = [element plateRect].size;
			ps.height *= r;
			ps.width  *= r;
			[element setImageWithSize:ps];
			NSRect pr;
			pr.origin = p;
			pr.size = ps;
			[element setPlateRect:pr];
			}
		}
 
    // reset the bottom right corner
    [self setCorners];
	
    // update the scroll bars
    [self updateScrollBars];

	// update the tracking information for the parameters
	[self updateAllTrackingAreas];
        		
    [self setNeedsDisplay:YES];
}

- (IBAction)selectAll:(id)sender {

    [self removeAllFocusRings];
    [self selectAllItems];
    [self selectAllConnections];
    [self setNeedsDisplay:YES];
}

- (void)selectAllConnections {

    [selectedLinks removeAllObjects];
	NSEnumerator* enumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
        for (int i=0; i<[element numParents]; i++)
            {
            Parm* p = [element getParentIndexed:i];
            [self addToSelectedLinkListParent:p andChild:element];
            }
		}
}

- (void)selectAllItems {

	NSEnumerator* enumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
        [element setIsSelected:YES];
		}
    [self setNeedsDisplay:YES];
}

- (ItemSelector)selectItem:(NSPoint)forPoint {

	// set to base state
	ItemSelector mySelection;
	mySelection.selectedItem = nil;
	mySelection.selectionType = ITEM_SELECTION;
	
	// transform point to coordinate system of view parameters
	NSPoint p = [self convertPoint:forPoint fromView:nil];
	
	// enumerate all of the parameters, to see if any of them have been selected
	NSEnumerator* enumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
		if ([element isPlate] == NO)
			{
			// selecting for a normal parameter
			// initialize information on the parameter
			NSRect toolRect, infoRect, inspRect;
			toolRect.origin = [element itemLocation];
			toolRect.size = [element itemSize];
			[self transformToBottomLeftCoordinates:(&toolRect.origin)];
			
            infoRect = inspRect = informationRect[scaleIdx];
            infoRect.origin = NSMakePoint(toolRect.origin.x + rOffset[scaleIdx].x, toolRect.origin.y + rOffset[scaleIdx].y);
				
			// 1, check if the point is the information button (opening the control window)
			if ( CGRectContainsPoint( NSRectToCGRect(infoRect), NSPointToCGPoint(p) ) )
				{
				mySelection.selectedItem = element;
				mySelection.selectionType = INFO_SELECTION;
				return mySelection;
				}

			// 2, check if the point is the inspector button 
			if ([element hasInspectorInfo] == YES)
				{
				if ( CGRectContainsPoint( NSRectToCGRect(inspRect), NSPointToCGPoint(p) ) )
					{
					mySelection.selectedItem = element;
					mySelection.selectionType = INSP_SELECTION;
					return mySelection;
					}
				}
							
			// 3, check if the point is in the parameter image
			if ( CGRectContainsPoint( NSRectToCGRect(toolRect), NSPointToCGPoint(p) ) )
				{
				cursorOffset.width  = p.x - toolRect.origin.x;
				cursorOffset.height = p.y - toolRect.origin.y;
				mySelection.selectedItem = element;
				mySelection.selectionType = ITEM_SELECTION;
				return mySelection;
				}
			}
		}
				
    // check if the point is any of the connections between objects
    enumerator = [itemsPtr objectEnumerator];
    while ( (element = [enumerator nextObject]) )
        {
        // get the center of the parameter, and transform the coordinates to the view's system (bottom-left)
        NSPoint pc = [element itemCenter];
        [self transformToBottomLeftCoordinates:(&pc)];
        
        NSPoint a = [element itemCenter];
        [self transformToBottomLeftCoordinates:(&a)];
        for (int i=0; i<[element numParents]; i++)
            {
            NSPoint b = [[element getParentIndexed:i] itemCenter];
            [self transformToBottomLeftCoordinates:(&b)];

            if ( [self distanceOfPoint:p fromLineDefinedByPoint:a andPoint:b] < 3.0 )
                {
                mySelection.selectionType = LINK_SELECTION;
                mySelection.selectedItem = element;
                mySelection.child = element;
                mySelection.parent = [element getParentIndexed:i];
                return mySelection;
                }
            }
        }            

	// enumerate all of the plates, to see if any of them have been selected ... we assume tree plates are on top
    // first enumerate tree plates
    NSEnumerator* rEnumerator = [itemsPtr reverseObjectEnumerator];
    while ( (element = [rEnumerator nextObject]) )
		{
		if ([element parmType] == PARM_TREE_PLATE) 
			{
			// selecting for a plate
			NSRect pRect;
			pRect.origin = [element itemLocation];
			pRect.size   = [element itemSize];
			[self transformToBottomLeftCoordinates:(&pRect.origin)];
            
			NSRect brRect;
			brRect.origin = pRect.origin;
			brRect.size = NSMakeSize(15.0*scaleFactor, 15.0*scaleFactor);
			float oneHalfWidth = brRect.size.width * 0.5;
			brRect.origin.x += (pRect.size.width - oneHalfWidth);
			brRect.origin.y -= oneHalfWidth;
            
			NSRect tlRect;
			tlRect.origin = pRect.origin;
			tlRect.origin.x -= oneHalfWidth;
			tlRect.origin.y += (pRect.size.height - oneHalfWidth);
			tlRect.size = brRect.size;
			
			// 1, check if the point is in the top left handle
			if ( CGRectContainsPoint( NSRectToCGRect(tlRect), NSPointToCGPoint(p) ) && [element isSelected] == YES )
				{
				mySelection.selectedItem  = element;
				mySelection.selectionType = TL_PLATE_RESIZE;
				cursorOffset = NSMakeSize(0.0, 0.0);
				p.x = pRect.origin.x;
				p.y = pRect.origin.y + pRect.size.height;
				return mySelection;
				}
            
			// 2, check if the point is in the bottom right handle
			if ( CGRectContainsPoint( NSRectToCGRect(brRect), NSPointToCGPoint(p) ) && [element isSelected] == YES )
				{
				mySelection.selectedItem  = element;
				mySelection.selectionType = BR_PLATE_RESIZE;
				cursorOffset = NSMakeSize(0.0, 0.0);
				p.x = pRect.origin.x + pRect.size.width;
				p.y = pRect.origin.y;
				return mySelection;
				}
            
			// 3, check if we are selecting the information button
			NSRect iRect = informationRect[scaleIdx];
			float delta  = pRect.size.width - ITEM_IMAGE_SIZE * scaleFactor;
			iRect.origin = NSMakePoint(pRect.origin.x + rOffset[scaleIdx].x + delta, pRect.origin.y + rOffset[scaleIdx].y);
			if ( CGRectContainsPoint( NSRectToCGRect(iRect), NSPointToCGPoint(p) ) )
				{
				mySelection.selectedItem = element;
				mySelection.selectionType = INFO_SELECTION;
				return mySelection;
				}
            
			// 4, check if the point is in the plate itself
			if ( CGRectContainsPoint( NSRectToCGRect(pRect), NSPointToCGPoint(p) ) )
				{
				cursorOffset.width  = p.x - pRect.origin.x;
				cursorOffset.height = p.y - pRect.origin.y;
				mySelection.selectedItem = element;
				mySelection.selectionType = ITEM_SELECTION;
				return mySelection;
				}
			
			}
		}
    
    // then enumerate normal plates
    rEnumerator = [itemsPtr reverseObjectEnumerator];
    while ( (element = [rEnumerator nextObject]) )
		{
		if ([element parmType] == PARM_PLATE) 
			{
			// selecting for a plate
			NSRect pRect;
			pRect.origin = [element itemLocation];
			pRect.size   = [element itemSize];
			[self transformToBottomLeftCoordinates:(&pRect.origin)];

			NSRect brRect;
			brRect.origin = pRect.origin;
			brRect.size = NSMakeSize(15.0*scaleFactor, 15.0*scaleFactor);
			float oneHalfWidth = brRect.size.width * 0.5;
			brRect.origin.x += (pRect.size.width - oneHalfWidth);
			brRect.origin.y -= oneHalfWidth;

			NSRect tlRect;
			tlRect.origin = pRect.origin;
			tlRect.origin.x -= oneHalfWidth;
			tlRect.origin.y += (pRect.size.height - oneHalfWidth);
			tlRect.size = brRect.size;
			
			// 1, check if the point is in the top left handle
			if ( CGRectContainsPoint( NSRectToCGRect(tlRect), NSPointToCGPoint(p) ) && [element isSelected] == YES )
				{
				mySelection.selectedItem  = element;
				mySelection.selectionType = TL_PLATE_RESIZE;
				cursorOffset = NSMakeSize(0.0, 0.0);
				p.x = pRect.origin.x;
				p.y = pRect.origin.y + pRect.size.height;
				return mySelection;
				}
				
			// 2, check if the point is in the bottom right handle
			if ( CGRectContainsPoint( NSRectToCGRect(brRect), NSPointToCGPoint(p) ) && [element isSelected] == YES )
				{
				mySelection.selectedItem  = element;
				mySelection.selectionType = BR_PLATE_RESIZE;
				cursorOffset = NSMakeSize(0.0, 0.0);
				p.x = pRect.origin.x + pRect.size.width;
				p.y = pRect.origin.y;
				return mySelection;
				}
				
			// 3, check if we are selecting the information button
			NSRect iRect = informationRect[scaleIdx];
			float delta  = pRect.size.width - ITEM_IMAGE_SIZE * scaleFactor;
			iRect.origin = NSMakePoint(pRect.origin.x + rOffset[scaleIdx].x + delta, pRect.origin.y + rOffset[scaleIdx].y);
			if ( CGRectContainsPoint( NSRectToCGRect(iRect), NSPointToCGPoint(p) ) )
				{
				mySelection.selectedItem = element;
				mySelection.selectionType = INFO_SELECTION;
				return mySelection;
				}
				
			// 4, check if the point is in the plate itself
			if ( CGRectContainsPoint( NSRectToCGRect(pRect), NSPointToCGPoint(p) ) )
				{
				cursorOffset.width  = p.x - pRect.origin.x;
				cursorOffset.height = p.y - pRect.origin.y;
				mySelection.selectedItem = element;
				mySelection.selectionType = ITEM_SELECTION;
				return mySelection;
				}
			
			}
		}
		
	return mySelection;
}

- (void)setIndices {

	availableIndices[ 0] = 'i';
	availableIndices[ 1] = 'j';
	availableIndices[ 2] = 'k';
	availableIndices[ 3] = 'l';
	availableIndices[ 4] = 'm';
	availableIndices[ 5] = 'n';
	availableIndices[ 6] = 'u';
	availableIndices[ 7] = 'v';
	availableIndices[ 8] = 'w';
	availableIndices[ 9] = 'x';
	availableIndices[10] = 'y';
	availableIndices[11] = 'z';
	availableIndices[12] = 'a';
	availableIndices[13] = 'b';
	availableIndices[14] = 'c';
	availableIndices[15] = 'd';
	availableIndices[16] = 'e';
	availableIndices[17] = 'f';
	availableIndices[18] = 'g';
	availableIndices[19] = 'h';
	availableIndices[20] = 'o';
	availableIndices[21] = 'p';
	availableIndices[22] = 'q';
	availableIndices[23] = 'r';
	availableIndices[24] = 's';
	availableIndices[25] = 't';
	availableIndices[26] = '\0';
}

- (void)setItems:(NSMutableArray*)itm {

    // set the instance variable pointer of this class to point to the
    // mutable array of tools, stored in the AnalysisDocument class
    itemsPtr = itm;

    // update the tracking area
	[self updateAllTrackingAreas];

    // update the window information for any parameter that happen 
    // live on the window (this is different from the base class implementation)
	NSEnumerator* parmEnumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [parmEnumerator nextObject]) )
        [element setParmsWindow:[self window]];
        
    // update the display
	[self setNeedsDisplay:YES];
}

- (void)selectItemsInSweepArea {

    // remove all of the selected connections
    [selectedLinks removeAllObjects];
    
    // set up the rectangle for the sweep area
    NSRect sweepRect;
    NSPoint p1 = sweepStartLocation;
    NSPoint p2 = sweepCurrentLocation;
    sweepRect.origin = p1;
    if (p2.x < p1.x)
        sweepRect.origin.x = p2.x;
    if (p2.y < p1.y)
        sweepRect.origin.y = p2.y;
    sweepRect.size.width = p1.x - p2.x;
    sweepRect.size.height = p1.y - p2.y;
    if (sweepRect.size.width < 0.0)
        sweepRect.size.width = -sweepRect.size.width;
    if (sweepRect.size.height < 0.0)
        sweepRect.size.height = -sweepRect.size.height;
        
    // enumerate the parameters, deciding if any of them are in the sweep rectangle
	NSEnumerator* enumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
        // get a rectangle for the parameter
		NSRect parmRect;
		parmRect.origin = [element itemLocation];
		parmRect.size   = [element itemSize];
		[self transformToBottomLeftCoordinates:(&parmRect.origin)];
        
        // decide if the tool's rectangle is inside the sweep rectangle
		if ( CGRectIntersectsRect( NSRectToCGRect(sweepRect), NSRectToCGRect(parmRect) ) )
            {
            if ( [selectedItems containsObject:element] == NO )
                [element setIsSelected:YES];
            }
        else 
            {
            if ( [element isSelected] == YES && [selectedItems containsObject:element] == NO )
                [element setIsSelected:NO];
            }
            
        // decide if any of the tool's connections are in the sweep rectangle
        NSPoint ap = [element itemCenter];
        [self transformToBottomLeftCoordinates:&ap];
        CGPoint a = NSPointToCGPoint(ap);
        for (int i=0; i<[element numParents]; i++)
            {
            Parm* p = [element getParentIndexed:i];
            NSPoint bp = [p itemCenter];
            [self transformToBottomLeftCoordinates:&bp];
            CGPoint b = NSPointToCGPoint(bp);
            
            if ( [self doesLineWithEndsA:a andB:b intersectRect:NSRectToCGRect(sweepRect)] == YES )
                [self addToSelectedLinkListParent:p andChild:element];
            }
        
        }
        
    // update the view to reflect the possible selections
    [self setNeedsDisplay:YES];
}

- (ItemSelector)selectPlateItemOnly:(NSPoint)forPoint {

	// set to base state
	ItemSelector mySelection;
	mySelection.selectedItem = nil;
	mySelection.selectionType = ITEM_SELECTION;
	
	// transform point to coordinate system of view objects (such as tools)
	NSPoint p = [self convertPoint:forPoint fromView:nil];
	
	// enumerate all of the items, to see if any of them have been selected 
    
    // we always check tree plates first
	NSEnumerator* enumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
		if ([element parmType] == PARM_TREE_PLATE)
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
    
    //then we check normal plates
	enumerator = [itemsPtr objectEnumerator];
	while ( (element = [enumerator nextObject]) )
		{
		if ([element parmType] == PARM_PLATE)
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
    if (showParmNames == YES)
        bottomRightCorner.y += 0.75 * parmNameHeight * scaleFactor;
}

- (void)showSelectedLinks {

	NSEnumerator* enumerator = [selectedLinks objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
        NSLog(@"selected link between parameters %@ and %@", [element parentParm], [element childParm]);
}

- (void)showParameters:(NSMutableArray*)a {

    NSLog(@"List of parameters and their connections:");
	NSEnumerator* enumerator = [a objectEnumerator];
	id element;
    int i = 0;
	while ( (element = [enumerator nextObject]) )
        {
        NSLog(@"%d Parameter: %@ (%@)", ++i, element, [element parmName]);
        for (int i=0; i<[element numParents]; i++)
            NSLog(@"   Parent: %@", [element getParentIndexed:i]);
        for (int i=0; i<[element numChildren]; i++)
            NSLog(@"   Child:  %@", [element getChildIndexed:i]);
        }
}

- (void)updateBackgroundColor:(NSNotification*)notification {

    bkgrndColor = [[notification object] analysisBkgrndColor];
    gridColor   = [[notification object] analysisGridColor];
    [self setNeedsDisplay:YES];
}

- (void)writeToPasteboard:(NSPasteboard*)pb {
    
	// remove connections between selected and unselected tools so that
	// the archiving of the copied/cut parameters goes OK
	NSMutableArray* itemPairs = [NSMutableArray arrayWithCapacity:1];
	NSEnumerator* itemEnumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [itemEnumerator nextObject]) )
		{
        NSMutableArray* removedParents = [NSMutableArray arrayWithCapacity:0];
		for (int i=0; i<[element numParents]; i++)
			{
            Parm* parParm = [element getParentIndexed:i];
            if ( [element isSelected] == YES && [parParm isSelected] == YES && [self isLinkSelectedBetweenParent:parParm andChild:element] == NO )
                {
                ParmPair* pp = [[ParmPair alloc] init];
                [pp setParentParm:parParm];
                [pp setChildParm:element];
                [itemPairs addObject:pp];
                [removedParents addObject:parParm];
                }
            else if ( ([element isSelected] == YES && [parParm isSelected] == NO) || ([element isSelected] == NO && [parParm isSelected] == YES) )
                {
                ParmPair* pp = [[ParmPair alloc] init];
                [pp setParentParm:parParm];
                [pp setChildParm:element];
                [itemPairs addObject:pp];
                [removedParents addObject:parParm];
                }
            }
        NSEnumerator* removedParentsEnumerator = [removedParents objectEnumerator];
        id rp;
        while ( (rp = [removedParentsEnumerator nextObject]) )
            {
            // remove the links
            [rp removeChild:element];
            [element removeParent:rp];
            }
		}
		
    // get a list of the selected items to archive
    [copiedItems removeAllObjects];
	itemEnumerator = [itemsPtr objectEnumerator];
	while ( (element = [itemEnumerator nextObject]) )
		{
        if ([element isSelected] == YES)
            [copiedItems addObject:element];
        }
        
    // We also need to remove any references to parameters that were not
    // in the list of parameters to copy. If we don't do this, then the
    // archiving will not occur properly. We remember the connections
    // using the ParmRestore class so we can restore the original graph
    // to its pre-copy state.
    NSMutableArray* itemParms = [NSMutableArray arrayWithCapacity:1];
	itemEnumerator = [copiedItems objectEnumerator];
	while ( (element = [itemEnumerator nextObject]) )
		{
        ParmRestore* parmRest = [[ParmRestore alloc] init];
        [parmRest setParameter:element];
        [parmRest setParameter1:nil];
        [parmRest setParameter2:nil];
        [parmRest setParameter3:nil];
        [parmRest setParameter4:nil];
        if ( [copiedItems containsObject:[element parameter1]] == NO )
            {
            [parmRest setParameter1:[element parameter1]];
            [element setParameter1:nil];
            }
        if ( [copiedItems containsObject:[element parameter2]] == NO )
            {
            [parmRest setParameter2:[element parameter1]];
            [element setParameter2:nil];
            }
        if ( [copiedItems containsObject:[element parameter3]] == NO )
            {
            [parmRest setParameter3:[element parameter1]];
            [element setParameter3:nil];
            }
        if ( [copiedItems containsObject:[element parameter4]] == NO )
            {
            [parmRest setParameter4:[element parameter1]];
            [element setParameter4:nil];
            }
        [itemParms addObject:parmRest];
        }
        
    // archive the data to the pasteboard
    NSData* d = [NSKeyedArchiver archivedDataWithRootObject:copiedItems];
    [pb setData:d forType:ToolDataPboardType];
	
	// re-establish any removed connections
	NSEnumerator* connectionEnumerator = [itemPairs objectEnumerator];
	while ( (element = [connectionEnumerator nextObject]) )
		{
        Parm* p = [element parentParm];
        Parm* c = [element childParm];
        [p addChild:c];
        [c addParent:p];
		}
        
    // restore the parameter settings in the original graph
    /*NSEnumerator* yaEnumerator = [itemParms objectEnumerator];
    id ppRestore;
    while ( (ppRestore = [yaEnumerator nextObject]) )
        {
        if ( [ppRestore parameter1] != nil )
            [[ppRestore parameter] setParameter1:[ppRestore parameter1]];
        if ( [ppRestore parameter2] != nil )
            [[ppRestore parameter] setParameter2:[ppRestore parameter2]];
        if ( [ppRestore parameter3] != nil )
            [[ppRestore parameter] setParameter3:[ppRestore parameter3]];
        if ( [ppRestore parameter4] != nil )
            [[ppRestore parameter] setParameter4:[ppRestore parameter4]];
        }*/
}

@end
