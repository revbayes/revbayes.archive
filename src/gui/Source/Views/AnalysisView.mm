#import "AnalysisDocument.h"
#import "AnalysisView.h"
#import "Connection.h"
#import "ConnectionPair.h"
#import "Inlet.h"
#import "InOutlet.h"
#import "Outlet.h"
#import "RevBayes.h"
#import "ScalingScrollView.h"
#import "Tool.h"
#import "ToolAlign.h"
#import "ToolBootstrap.h"
#import "ToolCombiner.h"
#import "ToolConcatenate.h"
#import "ToolDataEntry.h"
#import "ToolDistanceMatrix.h"
#import "ToolLoop.h"
#import "ToolNumericalMcmcOutput.h"
#import "ToolMatrixFilter.h"
#import "ToolMcmc.h"
#import "ToolMcmcDiagnostic.h"
#import "ToolModel.h"
#import "ToolNeighborJoining.h"
#import "ToolParsimony.h"
#import "ToolReadData.h"
#import "ToolSimulate.h"
#import "ToolTreeMcmcDiagnostic.h"
#import "ToolTreeConsensus.h"
#import "ToolTreeSet.h"
#import "ToolView.h"
#import "WindowControllerPreferences.h"

#define BL      0
#define BR      1
#define TL      2
#define TR      3




@implementation AnalysisView

@synthesize isLocked;
@synthesize cancelAnalysis;

- (NSPoint)centerPointBetweenPoint:(NSPoint)p1 andPoint:(NSPoint)p2 {
    
    float h = fabsf( (float)(p1.x - p2.x) ) * 0.5;
    float v = fabsf( (float)(p1.y - p2.y) ) * 0.5;
    
    NSPoint cp;
    if (p1.x < p2.x)
        cp.x = p1.x + h;
    else
        cp.x = p2.x + h;
    if (p1.y < p2.y)
        cp.y = p1.y + v;
    else
        cp.y = p2.y + v;
    
    return cp;
}

- (NSPoint)centerPointBetweenTool:(Tool*)t1 andTool:(Tool*)t2 {

    NSRect toolRect1;
    toolRect1.origin = [t1 itemLocation];
    toolRect1.size   = [[t1 itemImageWithIndex:scaleIdx] size]; 
    [self transformToBottomLeftCoordinates:(&toolRect1.origin)];

    NSRect toolRect2;
    toolRect2.origin = [t2 itemLocation];
    toolRect2.size   = [[t2 itemImageWithIndex:scaleIdx] size]; 
    [self transformToBottomLeftCoordinates:(&toolRect2.origin)];
    
    float leftX = toolRect1.origin.x;
    if (toolRect2.origin.x < leftX)
        leftX = toolRect2.origin.x;
    float rightX = toolRect1.origin.x + toolRect1.size.width;
    if (toolRect2.origin.x + toolRect2.size.width > rightX)
        rightX = toolRect2.origin.x + toolRect2.size.width;

    float lowerY = toolRect1.origin.y;
    if (toolRect2.origin.y < lowerY)
        lowerY = toolRect2.origin.y;
    float upperY = toolRect1.origin.y + toolRect1.size.height;
    if (toolRect2.origin.y + toolRect2.size.height > upperY)
        upperY = toolRect2.origin.y + toolRect2.size.height;
        
    NSPoint cp = NSMakePoint( leftX + 0.5*(rightX-leftX), lowerY + 0.5*(upperY-lowerY) );
    return cp;
}

- (NSPoint)centerPointBetweenRect:(NSRect)r1 andRect:(NSRect)r2 {

    float leftX = r1.origin.x;
    if (r2.origin.x < leftX)
        leftX = r2.origin.x;
    float rightX = r1.origin.x + r1.size.width;
    if (r2.origin.x + r2.size.width > rightX)
        rightX = r2.origin.x + r2.size.width;

    float lowerY = r1.origin.y;
    if (r2.origin.y < lowerY)
        lowerY = r2.origin.y;
    float upperY = r1.origin.y + r1.size.height;
    if (r2.origin.y + r2.size.height > upperY)
        upperY = r2.origin.y + r2.size.height;
        
    NSPoint cp = NSMakePoint( leftX + 0.5*(rightX-leftX), lowerY + 0.5*(upperY-lowerY) );
    return cp;
}

- (void)dealloc {

    [[NSNotificationCenter defaultCenter] removeObserver:self name:@"scaleFactorChangeNotification" object:nil];
}

- (void)dfsForTool:(Tool*)t forArray:(NSMutableArray*)dpa {

    [t setIsVisited:YES];
    for (int i=0; i<[t numOutlets]; i++)
        {
        Outlet* o = [t outletIndexed:i];
        for (int j=0; j<[o numberOfConnections]; j++)
            {
            Connection* c = [o connectionWithIndex:j];
            Tool* daughterTool = [[c inlet] toolOwner];
            if ( [daughterTool isVisited] == NO )
                [self dfsForTool:daughterTool forArray:dpa];
            }
        }
    [dpa addObject:t];
}

- (float)distanceFromPoint:(NSPoint)a toPoint:(NSPoint)b {

    float dX = a.x - b.x;
    float dY = a.y - b.y;
    return sqrt( dX*dX + dY*dY );
}

- (float)distanceFromPoint:(NSPoint)p1 toPoint:(NSPoint)p2 alongRectangle:(NSRect)r {
    
    if ( fabs(p1.x-p2.x) < 0.0001 )
        {
        // both points are on the same vertical edge of the rectangle
        float d = fabs(p1.y-p2.y);
        return d;
        }
    else if ( fabs(p1.y-p2.y) < 0.0001 )
        {
        // both points are on the same horizontal edge of the rectangle
        float d = fabs(p1.x-p2.x);
        return d;
        }
    else if ( fabs(r.size.width - fabs(p1.x-p2.x)) < 0.0001 )
        {
        // the points are on opposite vertical edges of the rectangle
        float d = r.size.width;
        float d1 = (p1.y - r.origin.y) + (p2.y - r.origin.y);
        float d2 = (r.origin.y + r.size.height - p1.y) + (r.origin.y + r.size.height - p2.y);
        if ( d1 < d2 )
            d += d1;
        else
            d += d2;
        return d;
        }
    else if ( fabs(r.size.height - fabs(p1.y-p2.y)) < 0.0001 )
        {
        // the points are on opposite horizontal edges of the rectangle
        float d = r.size.height;
        float d1 = (p1.x - r.origin.x) + (p2.x - r.origin.x);
        float d2 = (r.origin.x + r.size.width - p1.x) + (r.origin.x + r.size.width - p2.x);
        if ( d1 < d2 )
            d += d1;
        else
            d += d2;
        return d;
        }
    return fabs(p1.x-p2.x) + fabs(p1.y-p2.y);
}

- (float)distanceOfPoint:(NSPoint)a0 fromLineDefinedByPoint:(NSPoint)a1 andPoint:(NSPoint)a2 {

	float temp1 = (a2.x - a1.x) * (a1.y - a0.y) - (a1.x - a0.x) * (a2.y - a1.y);
	float temp2 = (a2.x - a1.x) * (a2.x - a1.x) + (a2.y - a1.y) * (a2.y - a1.y);
	float d     = fabs(temp1) / sqrt(temp2);
	return d;
}

- (BOOL)arePoint:(NSPoint)p1 andPoint:(NSPoint)p2 onSameEdgeOfRect:(NSRect)r {

    if ( fabs(p1.x-p2.x) < 0.0001 )
        return YES;
    if ( fabs(p1.y-p2.y) < 0.0001 )
        return YES;
    return NO;
}

- (void)drawRect:(NSRect)dirtyRect {
    
    // mark tools that are in loops (Note: This could be accomplished elsewhere in this
    // class, perhaps when a tool is moved.)
	NSEnumerator* itemEnumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [itemEnumerator nextObject]) )
        [element removeAllLoops];
	itemEnumerator = [itemsPtr objectEnumerator];
	while ( (element = [itemEnumerator nextObject]) )
        {
        if ( [element isLoop] == YES )
            {
            // get the rectangle for the loop
			NSRect loopRect = [element loopRect];
			loopRect.origin = [element itemLocation];
            loopRect.size.width *= scaleFactor;
            loopRect.size.height *= scaleFactor;
			[self transformToBottomLeftCoordinates:(&loopRect.origin)];
            
            NSEnumerator* itemEnumerator2 = [itemsPtr objectEnumerator];
            id t;
            while ( (t = [itemEnumerator2 nextObject]) )
                {
                if ( [t isLoop] == NO )
                    {
                    // get the rectangle for the tool
                    NSImage* itemImage = [t itemImageWithIndex:scaleIdx];
                    NSRect imageRect;
                    imageRect.origin = NSZeroPoint;
                    imageRect.size = [itemImage size];
                    NSRect toolRect = imageRect;
                    toolRect.origin = [t itemLocation];
                    toolRect.size = NSMakeSize(ITEM_IMAGE_SIZE*scaleFactor, ITEM_IMAGE_SIZE*scaleFactor);
                    [self transformToBottomLeftCoordinates:(&toolRect.origin)];
                    
                    if ( CGRectContainsRect( NSRectToCGRect(loopRect), NSRectToCGRect(toolRect) ) )
                        {
                        [t addToolToLoop:element];
                        [element addToolToLoop:t];
                        }
                    }
                }
            }
        }

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

	// get pointers of images to be drawn over items
    NSImage* magnifyingImageOff = [NSImage imageNamed:@"Unselected_Magnifier.icns"];
    [magnifyingImageOff setSize:NSMakeSize(ITEM_IMAGE_SIZE*scaleFactor*0.2, ITEM_IMAGE_SIZE*scaleFactor*0.2)];
    NSImage* loopImage = [NSImage imageNamed:@"loop512.png"];
    [loopImage setSize:NSMakeSize(ITEM_IMAGE_SIZE*scaleFactor*0.2, ITEM_IMAGE_SIZE*scaleFactor*0.2)];

    // instantiate a shadow object
    NSShadow* shadow = [[NSShadow alloc] init];
    [shadow setShadowOffset:NSMakeSize(3.0, -2.0)];
    [shadow setShadowBlurRadius:10.0];
    [NSGraphicsContext saveGraphicsState];
    [shadow set];
	
    // draw the loops
    float borderWidth = ITEM_IMAGE_SIZE * scaleFactor * 0.01;
	itemEnumerator = [itemsPtr objectEnumerator];
	while ( (element = [itemEnumerator nextObject]) )
        {
        if ( [element isLoop] == YES )
            {
			// draw the rectangle (plate)
            [NSGraphicsContext saveGraphicsState];
            [shadow set];

			NSRect pr = [element loopRect];
			pr.origin = [element itemLocation];
            pr.size.width *= scaleFactor;
            pr.size.height *= scaleFactor;
			[self transformToBottomLeftCoordinates:(&pr.origin)];
            NSColor* c = [NSColor colorWithDeviceCyan:0.66 magenta:0.24 yellow:0.19 black:0.0 alpha:0.7];
			[c set];
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
				NSString* endingRangeStr = [element getEndingRangeForLoop];
				NSRange endingRangeRange = [element italicsRange];
                
				NSString* infoStr = nil;
                if ([element isExecuting] == YES)
                    {
                    infoStr = [NSString stringWithFormat:@"%c = %d", [element indexLetter], [element currentIndex]];
                    }
                else
                    {
                    if ([element indexUpperLimit] == 1)
                        infoStr = [NSString stringWithFormat:@"%c %C (", [element indexLetter], uc];
                    else
                        infoStr = [NSString stringWithFormat:@"%c %C (1,...,", [element indexLetter], uc];
                    infoStr = [infoStr stringByAppendingString:endingRangeStr];
                    infoStr = [infoStr stringByAppendingString:@")"];
                    }
                
				NSMutableAttributedString* attrString = [[NSMutableAttributedString alloc] initWithString:infoStr attributes:attrs];
				[attrString applyFontTraits:NSItalicFontMask range:NSMakeRange(0, 1)];
				if ( endingRangeRange.length != 0 )
					{
					endingRangeRange.location += 11;
					[attrString applyFontTraits:NSItalicFontMask range:endingRangeRange];
					}
				NSRect textSize = [attrString boundingRectWithSize:NSMakeSize(1e10, 1e10) options:NSStringDrawingUsesLineFragmentOrigin];
				float padding = 4.0 * scaleFactor;
				textSize.origin.x = pr.origin.x + (pr.size.width - textSize.size.width - padding);
				textSize.origin.y = pr.origin.y + padding;
				NSPoint p = textSize.origin;
				[attrString drawAtPoint:p];
				[element setMinLoopSize:NSMakeSize(textSize.size.width + 2.0*padding, textSize.size.height + 2.0*padding)];
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
        
	// draw the tool images
    NSPoint mouseLoc = [NSEvent mouseLocation];
	mouseLoc = [self convertPoint:mouseLoc fromView:nil];
	itemEnumerator = [itemsPtr objectEnumerator];
	while ( (element = [itemEnumerator nextObject]) )
		{
        if ( [element isLoop] == YES )
            continue;
            
		// get information on the location and size of the tool's image
		NSImage* itemImage = [element itemImageWithIndex:scaleIdx];
		NSRect imageRect;
		imageRect.origin = NSZeroPoint;
		imageRect.size = [itemImage size];
		NSRect drawingRect = imageRect;
		drawingRect.origin = [element itemLocation];
        drawingRect.size = NSMakeSize(ITEM_IMAGE_SIZE*scaleFactor, ITEM_IMAGE_SIZE*scaleFactor);
		[self transformToBottomLeftCoordinates:(&drawingRect.origin)];            
                
		// draw a focus ring behind the tool, if it is currently selected
        if ([element isSelected] == YES)
            {
            NSRect focusRect;
            focusRect = drawingRect;
            focusRect.origin.x -= 1.0;
            focusRect.origin.y -= 1.0;
            focusRect.size.height += 2.0;
            focusRect.size.width += 2.0;
            [[NSColor keyboardFocusIndicatorColor] set];

            NSBezierPath* selRing = [NSBezierPath bezierPath];
            float rad = focusRect.size.width * 0.1;
            [selRing appendBezierPathWithRoundedRect:focusRect xRadius:rad yRadius:rad];
            [selRing fill];
            }

        // flag the object, with a red square behind the image that gets bigger or smaller depending upon the flag count
		if ([element flagCount] > 0)
			{
			float factor = 1.0;
			if ([element flagCount] % 2 == 0)
				factor = 4.0;
            NSRect flagRect;
            flagRect = drawingRect;
            flagRect.origin.x -= 1.0*factor;
            flagRect.origin.y -= 1.0*factor;
            flagRect.size.height += 2.0*factor;
            flagRect.size.width += 2.0*factor;

            [[NSColor redColor] set];

            NSBezierPath* selRing = [NSBezierPath bezierPath];
            float rad = flagRect.size.width * 0.1;
            [selRing appendBezierPathWithRoundedRect:flagRect xRadius:rad yRadius:rad];
            [selRing fill];
			}
            
        // possibly draw a light green rectangle behind the tool, if it is currently executing
        if ([element isCurrentlyExecuting] == YES)
            {
            NSRect flagRect;
            flagRect = drawingRect;
            flagRect.origin.x -= 4.0;
            flagRect.origin.y -= 4.0;
            flagRect.size.height += 8.0;
            flagRect.size.width += 8.0;

            [[NSColor greenColor] set];

            NSBezierPath* selRing = [NSBezierPath bezierPath];
            float rad = flagRect.size.width * 0.1;
            [selRing appendBezierPathWithRoundedRect:flagRect xRadius:rad yRadius:rad];
            [selRing fill];
            }

		// draw the tool image
		[itemImage drawInRect:drawingRect fromRect:imageRect operation:NSCompositeSourceOver fraction:1.0];
        		
		// draw the magnifying glass in the lower-left corner of the tool
        if ([element hasInspectorInfo] == YES)
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
            
        // draw the loop indicator in the top-left corner of the tool
        if ([element isOnLoop] == YES)
            {
			NSRect loopImageRect;
			loopImageRect.origin = NSZeroPoint;
			loopImageRect.size = [loopImage size];
			NSRect lRect = informationRect[scaleIdx];
			lRect.origin = NSMakePoint(drawingRect.origin.x + lOffset[scaleIdx].x, drawingRect.origin.y + lOffset[scaleIdx].y);
            [loopImage drawInRect:lRect fromRect:loopImageRect operation:NSCompositeSourceOver fraction:1.0];
            }
            
		// draw the information button in the lower-right corner of the tool
        if ([element hasController] == YES)
            {
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
            }
        }

    // draw the connections
	itemEnumerator = [itemsPtr objectEnumerator];
	while ( (element = [itemEnumerator nextObject]) )
		{
        // get the size for a standard tool...we assume all tools are the same size
        NSSize toolSize = [[element itemImageWithIndex:scaleIdx] size]; 
        NSRect r1, r2;
        r1.size = toolSize;
        r2.size = toolSize;
        
        // loop over the outlets for this tool
		for (int i=0; i<[element numOutlets]; i++)
            {
			Outlet* theOutlet = [element outletIndexed:i];
            for (int j=0; j<[theOutlet numberOfConnections]; j++)
                {
                // get the connection
                Connection* c = [theOutlet connectionWithIndex:j];

                // get the rectangles for the two tools
                Tool* t1 = [[c outlet] toolOwner];
                Tool* t2 = [[c inlet] toolOwner];
                r1.origin = [t1 itemLocation];
                r2.origin = [t2 itemLocation];
				[self transformToBottomLeftCoordinates:&r1.origin];
				[self transformToBottomLeftCoordinates:&r2.origin];
                
                // get the center point and bounding rects for both tools
                NSRect br1 = r1;
                NSRect br2 = r2;
                NSPoint center, s1, s2;
                [self getBoundingRectForToolWithRect:&br1 andRect:&br2 andCenterPoint:&center withPosition:[[c outlet] offsetPosition] initializingPoint:&s1 andPosition:[[c inlet] offsetPosition] initializingPoint:&s2];
    
                // draw the path in two parts
                NSBezierPath* path1 = [c path1];
                NSBezierPath* path2 = [c path2];
				[[theOutlet toolColor] set];
                [self getBezierPath:path1 forRect:br1 startPoint:[[c outlet] pointForToolWithRect:r1] secondPoint:s1 andEndPoint:center];
                [self getBezierPath:path2 forRect:br2 startPoint:[[c inlet] pointForToolWithRect:r2] secondPoint:s2 andEndPoint:center];
                
                NSBezierPath* thePathToEnlightenedConnections = [NSBezierPath bezierPath];
                [thePathToEnlightenedConnections setLineJoinStyle:NSRoundLineJoinStyle];
                for (int k=0; k<[path1 elementCount]; k++)
                    {
                    NSPoint myPathPoints[3];
                    [path1 elementAtIndex:k associatedPoints:myPathPoints];
                    if (k == 0)
                        [thePathToEnlightenedConnections moveToPoint:myPathPoints[0]];
                    else
                        [thePathToEnlightenedConnections lineToPoint:myPathPoints[0]];
                    }
                for (int k=(int)[path2 elementCount]-1; k>=0; k--)
                    {
                    NSPoint myPathPoints[3];
                    [path2 elementAtIndex:k associatedPoints:myPathPoints];
                    [thePathToEnlightenedConnections lineToPoint:myPathPoints[0]];
                    }
				if ([c isSelected] == YES)
                    [thePathToEnlightenedConnections setLineWidth:(6.0 * scaleFactor)];
                else
                    [thePathToEnlightenedConnections setLineWidth:(4.0 * scaleFactor)];
                [thePathToEnlightenedConnections stroke];
           
                }
            }
        }

    // draw the inlets and outlets
    itemEnumerator = [itemsPtr objectEnumerator];
	while ( (element = [itemEnumerator nextObject]) )
		{
        // get the rectangle for the tool
        NSRect drawingRect;
        drawingRect.origin = [element itemLocation];
        drawingRect.size = [[element itemImageWithIndex:scaleIdx] size];
        [self transformToBottomLeftCoordinates:(&drawingRect.origin)];
        
        NSPoint p1, p2, p3;
        
        // draw the inlets
		for (int i=0; i<[element numInlets]; i++)
			{
			Inlet* theInlet = [element inletIndexed:i];
            [theInlet pointsForToolWithRect:drawingRect atVertex1:&p1 andVertex2:&p2 andVertex3:&p3];
			NSBezierPath *triangle = [NSBezierPath bezierPath];
			[triangle moveToPoint:p1];
			[triangle lineToPoint:p2];
			[triangle lineToPoint:p3];
			[triangle lineToPoint:p1];
			[[element colorOfInletIndexed:i] set];
			[triangle fill];
            [[NSColor blackColor] set];
            [triangle stroke];
            }
            
        // draw the outlets
		for (int i=0; i<[element numOutlets]; i++)
			{
			Outlet* theOutlet = [element outletIndexed:i];
            [theOutlet pointsForToolWithRect:drawingRect atVertex1:&p1 andVertex2:&p2 andVertex3:&p3];
			NSBezierPath *triangle = [NSBezierPath bezierPath];
			[triangle moveToPoint:p1];
			[triangle lineToPoint:p2];
			[triangle lineToPoint:p3];
			[triangle lineToPoint:p1];
			[[element colorOfOutletIndexed:i] set];
			[triangle fill];
            [[NSColor blackColor] set];
            [triangle stroke];
            if ( [element isKindOfClass:[ToolData class]] == YES )
                {
                int n = 0;
                if ( [theOutlet toolColor] == [NSColor greenColor] )
                    n = (int)[(ToolData*)element numAligned];
                else if ( [theOutlet toolColor] == [NSColor cyanColor] )
                    n = (int)[(ToolData*)element numUnaligned];
                [[[NSColor blackColor] colorWithAlphaComponent:0.4] set];
                NSBezierPath* numPath = [self makePathForNumber:n];
                NSRect numBounds = [numPath bounds];
                NSPoint numPoint = [theOutlet getDrawingPositionForToolWithRect:drawingRect andBoundedRect:numBounds];
                NSAffineTransform* transform = [NSAffineTransform transform];
                [transform translateXBy:numPoint.x yBy:numPoint.y];
                [numPath transformUsingAffineTransform:transform];
                [numPath stroke];
                [numPath fill];
                }
            }
        }
    [NSGraphicsContext restoreGraphicsState];
        
    // draw the tool tip
    itemEnumerator = [itemsPtr objectEnumerator];
	while ( (element = [itemEnumerator nextObject]) )
        {
        if ([element showTip] == YES)
            {
			NSPoint p = [element itemCenter];
			[self transformToBottomLeftCoordinates:(&p)];
            
            NSMutableAttributedString* attrString = [element sendTip];
			NSRect tipRect = [attrString boundingRectWithSize:NSMakeSize(1e10, 1e10) options:(NSStringDrawingUsesLineFragmentOrigin)];
            tipRect.origin = p;
            
            NSRect visible = [self visibleRect];
            int i = 0;
            while ( NSContainsRect(visible, tipRect) == NO && i <= 3 ) 
                {
                if (i == 0)
                    tipRect.origin.x -= tipRect.size.width;
                else if (i == 1)
                    tipRect.origin.y -= tipRect.size.height;
                else if (i == 3)
                    tipRect.origin.x += tipRect.size.width;
                p = tipRect.origin;
                i++;
                } 
            
            [[[NSColor darkGrayColor] colorWithAlphaComponent:0.75] set];
            [NSBezierPath fillRect:tipRect];
			[attrString drawAtPoint:p];
            }
        }
        
	// draw an active drag between inlet/outlet or reposition the inlet/outlet
	if ( selection.selectedItem != nil && (selection.selectionType == INLET_SELECTION || selection.selectionType == OUTLET_SELECTION) )
		{
        if (optionClicked == NO)
            {
            InOutlet* ilol = selection.selectedItem;
                
            Tool* t = [ilol toolOwner];
            NSRect drawingRect;
            drawingRect.origin = [t itemLocation];
            drawingRect.size = [[t itemImageWithIndex:scaleIdx] size];
            [self transformToBottomLeftCoordinates:(&drawingRect.origin)];

            NSPoint p1 = [ilol pointForToolWithRect:drawingRect]; 
            NSPoint p2 = cursorLocation;

            NSPoint i1 = p1;
            NSPoint i2 = p2;
            float x = (p1.x - p2.x) * 0.5;
            if (x < 0.0)
                x = p1.x - x;
            else 
                x = p2.x + x;
            i1.x = x;
            i2.x = x;
            
            NSBezierPath* connection = [NSBezierPath bezierPath];
            [connection moveToPoint:p1];
            [connection lineToPoint:i1];
            [connection lineToPoint:i2];
            [connection lineToPoint:p2];
            [[ilol toolColor] set];
            [connection setLineWidth:(4.0 * scaleFactor)];
            [connection stroke];
            }
        else
            {
            
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

- (NSPoint)findClosestPointOnEdgeOfRect:(NSRect)r fromPoint:(NSPoint)p2 {

    NSPoint p1 = NSMakePoint( NSMidX(r), NSMidY(r) );

    float d = [self distanceFromPoint:p1 toPoint:p2];
    float absBX = fabsf( (float)(p1.x-p2.x) );
    float absBY = fabsf( (float)(p1.y-p2.y) );
    float dV = r.size.height*0.5 * d / absBY;
    float dH = r.size.width*0.5 * d / absBX;
    NSPoint closestPt = NSZeroPoint;
    if (absBY/absBX > r.size.height/r.size.width)
        closestPt = [self newPointDistance:dV betweenPoint:p1 andPoint:p2];
    else
        closestPt = [self newPointDistance:dH betweenPoint:p1 andPoint:p2];
    return closestPt;
}

- (void)getBezierPath:(NSBezierPath*)bezy forRect:(NSRect)r startPoint:(NSPoint)sp secondPoint:(NSPoint)p2 andEndPoint:(NSPoint)ep {

    // empty the path of all points
    [bezy removeAllPoints];
    
    // set up the initial segment of the connection
    [bezy moveToPoint:sp];
    [bezy lineToPoint:p2];
    
    // initialize the current position and determine the direction of travel
    NSPoint p = p2;
    
    // get the distance to the end point from all four corners of the rectangle
    float d[4]; // BL, BR, TL, TR
    if ( fabs(r.origin.y-ep.y) < 0.0000001 )
        {
        // ep is on the bottom edge
        d[BL] = fabs(ep.x - r.origin.x);
        d[BR] = fabs(r.origin.x + r.size.width - ep.x);
        d[TL] = d[BL] + r.size.height;
        d[TR] = d[BR] + r.size.height;
        }
    else if ( fabs(r.origin.y+r.size.height-ep.y) < 0.0000001 )
        {
        // ep is on the top edge
        d[TL] = fabs(ep.x - r.origin.x);
        d[TR] = fabs(r.origin.x + r.size.width - ep.x);
        d[BL] = d[TL] + r.size.height;
        d[BR] = d[TR] + r.size.height;
        }
    else if ( fabs(r.origin.x-ep.x) < 0.0000001 )
        {
        // ep is on the left edge
        d[BL] = fabs(ep.y - r.origin.y);
        d[TL] = fabs(r.origin.y + r.size.height - ep.y);
        d[BR] = d[BL] + r.size.width;
        d[TR] = d[TL] + r.size.width;
        }
    else
        {
        // ep is on the right edge
        d[BR] = fabs(ep.y - r.origin.y);
        d[TR] = fabs(r.origin.y + r.size.height - ep.y);
        d[BL] = d[BR] + r.size.width;
        d[TL] = d[TR] + r.size.width;
        }

    // find the corners of the rectangle
    NSPoint cornerPoints[4];
    cornerPoints[BL] = r.origin;
    cornerPoints[BR] = r.origin;
    cornerPoints[BR].x += r.size.width;
    cornerPoints[TL] = r.origin;
    cornerPoints[TL].y += r.size.height;
    cornerPoints[TR] = r.origin;
    cornerPoints[TR].x += r.size.width;
    cornerPoints[TR].y += r.size.height;
    
    int myCorner = -1;
    if ( [self arePoint:p andPoint:ep onSameEdgeOfRect:r] == NO )
        {
        // move p to the closest corner of the rectangle
        if ( fabs(r.origin.y-p.y) < 0.0000001 )
            {
            // p is on the bottom edge
            myCorner = BL;
            p = cornerPoints[BL];
            if (d[1] < d[0])
                {
                myCorner = BR;
                p = cornerPoints[BR];
                }
            }
        else if ( fabs(r.origin.y+r.size.height-p.y) < 0.0000001 )
            {
            // p is on the top edge
            myCorner = TL;
            p = cornerPoints[TL];
            if (d[3] < d[2])
                {
                myCorner = TR;
                p = cornerPoints[TR];
                }
            }
        else if ( fabs(r.origin.x-p.x) < 0.0000001 )
            {
            // p is on the left edge
            myCorner = BL;
            p = cornerPoints[BL];
            if (d[2] < d[0])
                {
                myCorner = TL;
                p = cornerPoints[TL];
                }
            }
        else
            {
            // p is on the right edge
            myCorner = BR;
            p = cornerPoints[BR];
            if (d[3] < d[1])
                {
                myCorner = TR;
                p = cornerPoints[TR];
                }
            }
        [bezy lineToPoint:p];
        }
   
    while( fabs(p.x-ep.x) > 0.0000001 || fabs(p.y-ep.y) > 0.0000001 )
        {
        if ( [self arePoint:p andPoint:ep onSameEdgeOfRect:r] == YES )
            {
            p = ep;
            }
        else
            {
            // move p to the next closest corner
            if (myCorner == BL)
                {
                if ( d[TL] < d[BR] )
                    {
                    myCorner = TL;
                    p = cornerPoints[TL];
                    }
                else
                    {
                    myCorner = BR;
                    p = cornerPoints[BR];
                    }
                }
            else if (myCorner == BR)
                {
                if ( d[BL] < d[TR] )
                    {
                    myCorner = BL;
                    p = cornerPoints[BL];
                    }
                else
                    {
                    myCorner = TR;
                    p = cornerPoints[TR];
                    }
                }
            else if (myCorner == TL)
                {
                if ( d[BL] < d[TR] )
                    {
                    myCorner = BL;
                    p = cornerPoints[BL];
                    }
                else    
                    {
                    myCorner = TR;
                    p = cornerPoints[TR];
                    }
                }
            else // TR
                {
                if ( d[TL] < d[BR] )
                    {
                    myCorner = TL;
                    p = cornerPoints[TL];
                    }
                else
                    {
                    myCorner = BR;
                    p = cornerPoints[BR];
                    }
                }
            }

        [bezy lineToPoint:p];
        }
}

- (void)getBoundingRectForToolWithRect:(NSRect*)r andDestinationPoint:(NSPoint)cp withPosition:(NSPoint)p initializingPoint:(NSPoint*)s {

    // remember the original rectangles
    NSRect origR = *r;
        
    // expand the rectangle a bit
    float vInc = r->size.height * 0.2;
    float hInc = r->size.width  * 0.2;
    r->origin.x -= hInc;
    r->origin.y -= vInc;
    r->size.width += 2.0 * hInc;
    r->size.height += 2.0 * vInc;
    
    // check that the rectangles haven't gone off the analysis view area
    if ( r->origin.x < 0.0 )
        {
        float excess = -r->origin.x;
        r->origin.x = 0.0;
        r->size.width -= excess;
        }
	NSRect bounds = [self bounds];
    if ( r->origin.y + r->size.height > bounds.size.height )
        {
        float excess = r->origin.y + r->size.height - bounds.size.height;
        r->size.height -= excess;
        }
        
    // expand the rectangles towards the cursor point
    if (r->origin.x > cp.x)
        {
        float excess = r->origin.x - cp.x;
        r->origin.x -= excess;
        r->size.width += excess;
        }
    else if (r->origin.x + r->size.width < cp.x)
        {
        r->size.width += cp.x - (r->origin.x + r->size.width);
        }
    if (r->origin.y > cp.y)
        {
        float excess = r->origin.y - cp.y;
        r->origin.y -= excess;
        r->size.height += excess;
        }
    else if (r->origin.y + r->size.height < cp.y)
        {
        r->size.height += cp.y - (r->origin.y + r->size.height);
        }
        
    // find the positions of the inlets/outlets along the rectangle
    NSPoint a0;
    if (p.x < 0.00001)
        {
        a0.x = r->origin.x;
        a0.y = origR.origin.y + p.y * origR.size.height;
        }
    else if (p.x > 0.99999)
        {
        a0.x = r->origin.x + r->size.width;
        a0.y = origR.origin.y + p.y * origR.size.height;
        }
    else if (p.y < 0.00001)
        {
        a0.x = origR.origin.x + p.x * origR.size.width;
        a0.y = r->origin.y;
        }
    else
        {
        a0.x = origR.origin.x + p.x * origR.size.width;
        a0.y = r->origin.y + r->size.height;
        }
    *s = a0;
}

- (void)getBoundingRectForToolWithRect:(NSRect*)r1 andRect:(NSRect*)r2 andCenterPoint:(NSPoint*)cp withPosition:(NSPoint)p1 initializingPoint:(NSPoint*)s1 andPosition:(NSPoint)p2 initializingPoint:(NSPoint*)s2 {

    // remember the original rectangles
    NSRect origR1 = *r1;
    NSRect origR2 = *r2;
    
    // figure out the center point between both rectangles
    (*cp) = [self centerPointBetweenRect:(*r1) andRect:(*r2)];
    
    // expand both rectangles a bit
    float vInc = r1->size.height * 0.2;
    float hInc = r1->size.width  * 0.2;
    r1->origin.x -= hInc;
    r1->origin.y -= vInc;
    r1->size.width += 2.0 * hInc;
    r1->size.height += 2.0 * vInc;
    r2->origin.x -= hInc;
    r2->origin.y -= vInc;
    r2->size.width += 2.0 * hInc;
    r2->size.height += 2.0 * vInc;
    
    // check that the rectangles haven't gone off the analysis view area
    if ( r1->origin.x < 0.0 )
        {
        float excess = -r1->origin.x;
        r1->origin.x = 0.0;
        r1->size.width -= excess;
        }
    if ( r2->origin.x < 0.0 )
        {
        float excess = -r2->origin.x;
        r2->origin.x = 0.0;
        r2->size.width -= excess;
        }
	NSRect bounds = [self bounds];
    if ( r1->origin.y + r1->size.height > bounds.size.height )
        {
        float excess = r1->origin.y + r1->size.height - bounds.size.height;
        r1->size.height -= excess;
        }
    if ( r2->origin.y + r2->size.height > bounds.size.height )
        {
        float excess = r2->origin.y + r2->size.height - bounds.size.height;
        r2->size.height -= excess;
        }
        
    // expand the rectangles towards the center point
    if (r1->origin.x > cp->x)
        {
        float excess = r1->origin.x - cp->x;
        r1->origin.x -= excess;
        r1->size.width += excess;
        }
    else if (r1->origin.x + r1->size.width < cp->x)
        {
        r1->size.width += cp->x - (r1->origin.x + r1->size.width);
        }
    if (r1->origin.y > cp->y)
        {
        float excess = r1->origin.y - cp->y;
        r1->origin.y -= excess;
        r1->size.height += excess;
        }
    else if (r1->origin.y + r1->size.height < cp->y)
        {
        r1->size.height += cp->y - (r1->origin.y + r1->size.height);
        }

    if (r2->origin.x > cp->x)
        {
        float excess = r2->origin.x - cp->x;
        r2->origin.x -= excess;
        r2->size.width += excess;
        }
    else if (r2->origin.x + r2->size.width < cp->x)
        {
        r2->size.width += cp->x - (r2->origin.x + r2->size.width);
        }
    if (r2->origin.y > cp->y)
        {
        float excess = r2->origin.y - cp->y;
        r2->origin.y -= excess;
        r2->size.height += excess;
        }
    else if (r2->origin.y + r2->size.height < cp->y)
        {
        r2->size.height += cp->y - (r2->origin.y + r2->size.height);
        }
    
    // check that the rectangles do not overlap
    if ( CGRectIntersectsRect( NSRectToCGRect(*r1), NSRectToCGRect(*r2) ) == YES )
        {
        // readjust the rectangles
        CGRect intersectRect = CGRectIntersection(*r1, *r2);
        if (intersectRect.size.width > intersectRect.size.height)
            {
            float excess = 0.5 * intersectRect.size.height;
            if (r1->origin.y > r2->origin.y)
                {
                r1->origin.y += excess;
                r1->size.height -= excess;
                r2->size.height -= excess;
                }
            else 
                {
                r1->size.height -= excess;
                r2->origin.y += excess;
                r2->size.height -= excess;
                }
            }
        else
            {
            float excess = 0.5 * intersectRect.size.width;
            if (r1->origin.x > r2->origin.x)
                {
                r1->origin.x += excess;
                r1->size.width -= excess;
                r2->size.width -= excess;
                }
            else 
                {
                r1->size.width -= excess;
                r2->origin.x += excess;
                r2->size.width -= excess;
                }
            }
        }
    
    // move the center point so that it is as close as possible to both inlet/outlets
    // a: get segment in common between the two rectangles
    NSRect lineR1 = *r1;
    NSRect lineR2 = *r2;
    lineR1.origin.y = lineR2.origin.y = 0.0;
    lineR1.size.height = lineR2.size.height = 10.0;
    CGRect lineInt = CGRectIntersection(lineR1, lineR2);
    NSPoint a = NSZeroPoint;
    NSPoint b = NSZeroPoint;
    BOOL foundLineSeg = NO;
    if ( lineInt.size.width > 0.001 )
        {
        a.x = lineInt.origin.x;
        b.x = a.x + lineInt.size.width;
        if ( r1->origin.y < r2->origin.y )
            a.y = b.y = r2->origin.y;
        else
            a.y = b.y = r1->origin.y;
        foundLineSeg = YES;
        }
    lineR1 = *r1;
    lineR2 = *r2;
    lineR1.origin.x = lineR2.origin.x = 0.0;
    lineR1.size.width = lineR2.size.width = 10.0;
    lineInt = CGRectIntersection(lineR1, lineR2);
    if ( lineInt.size.height > 0.001 )
        {
        a.y = lineInt.origin.y;
        b.y = a.y + lineInt.size.height;
        if ( r1->origin.x < r2->origin.x )
            a.x = b.x = r2->origin.x;
        else
            a.x = b.x = r1->origin.x;
        foundLineSeg = YES;
        }
    if ( foundLineSeg == NO )
        {
        a = *cp;
        b = *cp;
        }

    // find the positions of the inlets/outlets along the rectangle
    NSPoint a0, b0;
    if (p1.x < 0.00001)
        {
        a0.x = r1->origin.x;
        a0.y = origR1.origin.y + p1.y * origR1.size.height;
        }
    else if (p1.x > 0.99999)
        {
        a0.x = r1->origin.x + r1->size.width;
        a0.y = origR1.origin.y + p1.y * origR1.size.height;
        }
    else if (p1.y < 0.00001)
        {
        a0.x = origR1.origin.x + p1.x * origR1.size.width;
        a0.y = r1->origin.y;
        }
    else
        {
        a0.x = origR1.origin.x + p1.x * origR1.size.width;
        a0.y = r1->origin.y + r1->size.height;
        }
    if (p2.x < 0.00001)
        {
        b0.x = r2->origin.x;
        b0.y = origR2.origin.y + p2.y * origR2.size.height;
        }
    else if (p2.x > 0.99999)
        {
        b0.x = r2->origin.x + r2->size.width;
        b0.y = origR2.origin.y + p2.y * origR2.size.height;
        }
    else if (p2.y < 0.00001)
        {
        b0.x = origR2.origin.x + p2.x * origR2.size.width;
        b0.y = r2->origin.y;
        }
    else
        {
        b0.x = origR2.origin.x + p2.x * origR2.size.width;
        b0.y = r2->origin.y + r2->size.height;
        }
    *s1 = a0;
    *s2 = b0;
   
    // calculate the distances for the two rectangles
    float da = [self distanceFromPoint:a toPoint:a0 alongRectangle:*r1] + [self distanceFromPoint:a toPoint:b0 alongRectangle:*r2];  
    float db = [self distanceFromPoint:b toPoint:a0 alongRectangle:*r1] + [self distanceFromPoint:b toPoint:b0 alongRectangle:*r2];
    NSPoint c = NSMakePoint( (a.x+b.x)*0.5, (a.y+b.y)*0.5 );
    float dc = [self distanceFromPoint:c toPoint:a0 alongRectangle:*r1] + [self distanceFromPoint:c toPoint:b0 alongRectangle:*r2];
            
    // move the center point
    if (da < db && da < dc)
        *cp = a;
    else if (db < da && db < dc)
        *cp = b;
    else
        *cp = c;

    // if both s1 and s2 are on the same edge, then we need to take into account the possibility
    // that the center point should be between them
    if ( fabs(a0.x-b0.x) < 0.000001 )
        {
        (*cp).y = (a0.y+b0.y) * 0.5;
        }
    else if ( fabs(a0.y-b0.y) < 0.000001 )
        {
        (*cp).x = (a0.x+b0.x) * 0.5;
        }
}

- (NSArray*)getLoops {

    NSMutableArray* myLoops = [[NSMutableArray alloc] init];
	NSEnumerator* enumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
        if ([element isLoop] == YES)
            [myLoops addObject:element];
        }
    return myLoops;
}

- (BOOL)hasItemCollided:(RbItem*)itm {

    if ( [(Tool*)itm isLoop] == YES )
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
            if ( CGRectIntersectsRect( NSRectToCGRect(itemRect), NSRectToCGRect(r) ) == YES && [element isLoop] == NO )
                return YES;
            }
		}

	return NO;
}

- (void)initializeDepthFirstOrderForTools:(NSMutableArray*)dpa {

    [dpa removeAllObjects];
    
    // find the root tools and mark all tools as having
    // not been visited
    NSMutableArray* rootTools = [NSMutableArray arrayWithCapacity:0];
	NSEnumerator* toolEnumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [toolEnumerator nextObject]) )
		{
        [element setIsVisited:NO];
        if ([element hasParents] == NO)
            [rootTools addObject:element];
        }
        
    // perform the depth-first traversal of the tools
	toolEnumerator = [rootTools objectEnumerator];
	while ( (element = [toolEnumerator nextObject]) )
        {
        [self dfsForTool:element forArray:dpa];
        }
}

- (id)initWithFrame:(NSRect)frame {

    if ( (self = [super initWithFrame:frame]) ) 
		{        
        // add self as observer for scale factor change
		NSNotificationCenter *defaultCenter = [NSNotificationCenter defaultCenter];		 
		[defaultCenter addObserver:self
						  selector:@selector(scaleFactorChanged:)
							  name:@"scaleFactorChangeNotification"
						    object:nil];

        // get the background color and grid color from the defaults
        NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
        NSData* colorAsData = [defaults objectForKey:RB_AnalysisBgrndColorKey];
        bkgrndColor = [NSKeyedUnarchiver unarchiveObjectWithData:colorAsData];
        colorAsData = [defaults objectForKey:RB_AnalysisGridColorKey];
        gridColor = [NSKeyedUnarchiver unarchiveObjectWithData:colorAsData];
        
        isLocked = NO;
		}
    return self;
}

- (BOOL)isPoint:(NSPoint)p inLineSegmentDefinedByPoint:(NSPoint)a andPoint:(NSPoint)b {

    float d = 6.0 * scaleFactor;
    // make a rectangle on the line a, b
    NSRect r;
    if (a.x < b.x)
        r.origin.x = a.x;
    else
        r.origin.x = b.x;
    if (a.y < b.y)
        r.origin.y = a.y;
    else
        r.origin.y = b.y;
    r.size.width = fabs(a.x-b.x);
    if (r.size.width < d)
        {
        r.size.width = d;
        r.origin.x -= 0.5 * d;
        }
    r.size.height = fabs(a.y-b.y);
    if (r.size.height < d)
        {
        r.size.height = d;
        r.origin.y -= 0.5 * d;
        }
        
    // decide if the point is on the line (in the rectangle
    if ( NSPointInRect(p, r) == YES )
        return YES;    
    return NO;
}

- (void)keyDown:(NSEvent*)event {
    
	// inactivate the timer, if it is still going
	if ([analysisDocumentPtr isRbTimerActive] == YES)
		[analysisDocumentPtr invalidateTimer];
	[self stopItemTipTimer];

    // maybe we're deleting stuff
    NSString *chars = [event characters];
    unichar character = [chars characterAtIndex:0];
    if (character == NSDeleteCharacter)
		{
        // we are deleting stuff!
        
        // make a list of tools that will be deleted
        NSMutableArray* toolsToRemove = [NSMutableArray arrayWithCapacity:0];
        NSEnumerator* toolEnumerator = [itemsPtr objectEnumerator];
        id element;
        while ( (element = [toolEnumerator nextObject]) )
            {
            if ( [element isSelected] == YES )
                [toolsToRemove addObject:element];
            }
            
        // initialize the depth-first traversal order for the graph of tools
        NSMutableArray* depthFirstOrder = [NSMutableArray arrayWithCapacity:0];
        [self initializeDepthFirstOrderForTools:depthFirstOrder];
        
        // mark mark all tools as having not been visited
        toolEnumerator = [itemsPtr objectEnumerator];
        while ( (element = [toolEnumerator nextObject]) )
            [element setIsVisited:NO];
            
        // mark the tools in the array of tools to remove as having been visited
        toolEnumerator = [toolsToRemove objectEnumerator];
        while ( (element = [toolEnumerator nextObject]) )
            [element setIsVisited:YES];
            
        // we may also be removing connections, in which case we also have to
        // mark tools downstream from removed connections as having been visited
        toolEnumerator = [itemsPtr objectEnumerator];
        while ( (element = [toolEnumerator nextObject]) )
            {
            for (int i=0; i<[element numOutlets]; i++)
                {
                Outlet* o = [element outletIndexed:i];
                for (int j=0; j<[o numberOfConnections]; j++)
                    {
                    Connection* c = [o connectionWithIndex:j];
                    if ( [c isSelected] == YES )
                        {
                        Tool* t = [[c inlet] toolOwner];
                        [t setIsVisited:YES];
                        }
                    }
                }
            }
        
        // mark tools downstream from removed tools/connections as visited
        toolEnumerator = [depthFirstOrder reverseObjectEnumerator];
        while ( (element = [toolEnumerator nextObject]) )
            {
            if ([element isSomeParentVisited] == YES)
                [element setIsVisited:YES];
            }
            
        // unmark tools that were in the toolArray
        toolEnumerator = [toolsToRemove objectEnumerator];
        while ( (element = [toolEnumerator nextObject]) )
            [element setIsVisited:NO];

        // remove the connections and tools
		[self removeSelectedConnections];
        [self removeSelectedItems];
        
        // and remove the tools from the depth-first array
        [depthFirstOrder removeObjectsInArray:toolsToRemove];
        
        // update tools in need of update
        toolEnumerator = [depthFirstOrder reverseObjectEnumerator];
        while ( (element = [toolEnumerator nextObject]) )
            {
            if ( [element isVisited] == YES )
                [element updateForChangeInParent];
            }
                
        }
}

- (NSBezierPath*)makePathForNumber:(int)n {

    NSString* numStr = [NSString stringWithFormat:@"%d", n];
    NSFont* numFont = [NSFont systemFontOfSize:(12.0*scaleFactor)];

    NSTextView* textview = [[NSTextView alloc] init];
    [textview setString:numStr];
    [textview setFont:numFont];

    NSLayoutManager* layoutManager = [textview layoutManager];
    NSRange range = [layoutManager glyphRangeForCharacterRange:NSMakeRange(0, [numStr length]) actualCharacterRange:NULL];
    NSGlyph glyphs[5];
    [layoutManager getGlyphs:glyphs range:range];

    NSBezierPath* path = [NSBezierPath bezierPath];
    [path moveToPoint:NSMakePoint(0.0, 0.0)];
    [path appendBezierPathWithGlyphs:glyphs count:range.length inFont:numFont];
    
    return path;
}

- (void)mouseDown:(NSEvent*)event {

    if (isLocked == YES)
        return;
	
	// inactivate the timer, if it is still going
	if ([analysisDocumentPtr isRbTimerActive] == YES)
		[analysisDocumentPtr invalidateTimer];
	[self stopItemTipTimer];
    	
	// see which item might have been selected
	ItemSelector mySelection = [self selectItem:[event locationInWindow]];
	selection = mySelection;
	
    shiftClicked  = NO;
    draggedItems  = NO;
	sweepAction   = NO;
    optionClicked = NO;
	if ([event modifierFlags] & NSControlKeyMask)
		{
        // control click
		if ( mySelection.selectedItem != nil && (mySelection.selectionType == ITEM_SELECTION || mySelection.selectionType == INFO_SELECTION || mySelection.selectionType == INSP_SELECTION) )
            [mySelection.selectedItem showControlPanel];
		}
	else if ([event modifierFlags] & NSAlternateKeyMask)
		{
        // option click (do nothing as option click is currently undefined for tools)
        if ( mySelection.selectedItem != nil && (mySelection.selectionType == INLET_SELECTION || mySelection.selectionType == OUTLET_SELECTION) )
            optionClicked = YES;
		}
	else if ([event modifierFlags] & NSShiftKeyMask)
		{
        // shift click
		shiftClicked = YES;
		if (mySelection.selectedItem != nil)
			{
			if (mySelection.selectionType == ITEM_SELECTION || mySelection.selectionType == LINK_SELECTION || mySelection.selectionType == INFO_SELECTION || mySelection.selectionType == INSP_SELECTION)
				[mySelection.selectedItem flipSelected];
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

    if (isLocked == YES)
        return;
	
    // get the position of the cursor
	NSPoint p = [event locationInWindow];
	if (selection.selectedItem != nil)
		{
		if (selection.selectionType == ITEM_SELECTION)
			{
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
			NSRect pRect = [selection.selectedItem loopRect];
			
			// get the other corner
			NSPoint otherP = [selection.selectedItem itemLocation];
			if (selection.selectionType == TL_PLATE_RESIZE)
				otherP.x += pRect.size.width;
			else 
				otherP.y -= pRect.size.height;
				
			// check that the plate isn't too small
			NSSize ms = [selection.selectedItem minLoopSize];
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
				[selection.selectedItem setLoopRect:newRect];
                //[selection.selectedItem setItemSize:newRect.size];
				[selection.selectedItem setImageWithSize:newRect.size];
				}
				
			// reset the scroll bars if we are dragging by the bottom-right corner
 			if (selection.selectionType == BR_PLATE_RESIZE)
				{
				[self setCorners];
				[self updateScrollBars];
				}
			}
        else if ( (selection.selectionType == INLET_SELECTION || selection.selectionType == OUTLET_SELECTION) && optionClicked == YES )
            {
            // moving the position on an inlet/outlet
            InOutlet* myInOutlet = selection.selectedItem;
            Tool* t = [myInOutlet toolOwner];
            
            NSRect toolRect;
            toolRect.origin = [t itemLocation];
            toolRect.size = [[t itemImageWithIndex:scaleIdx] size];
            [self transformToBottomLeftCoordinates:(&toolRect.origin)];
            
            NSPoint newPosition = [self findClosestPointOnEdgeOfRect:toolRect fromPoint:[self convertPoint:p fromView:nil] ];
            float x = (newPosition.x - toolRect.origin.x) / toolRect.size.width;
            if (x < 0.0000001)
                x = 0.0;
            else if (x > 0.999999)
                x = 1.0;
            float y = (newPosition.y - toolRect.origin.y) / toolRect.size.height;
            if (y < 0.0000001)
                y = 0.0;
            else if (y > 0.999999)
                y = 1.0;

            if ( x < 0.0000001 || x > 0.999999 )
                {
                // vertical edge
                if (y < 0.11)
                    y = 0.11;
                else if (y > 0.78)
                    y = 0.78;
                }
            else if ( y < 0.0000001 || y > 0.999999 )
                {
                // horizontal edge
                if (x < 0.11)
                    x = 0.11;
                else if (x > 0.78)
                    x = 0.78;
                }
            newPosition = NSMakePoint(x, y);
        
            [myInOutlet setPosition:newPosition];
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
                for (int i=0; i<[element numOutlets]; i++)
                    {
                    Outlet* theOutlet = [element outletIndexed:i];
                    for (int i=0; i<[theOutlet numberOfConnections]; i++)
                        {
                        Connection* c = [theOutlet connectionWithIndex:i];
                        if ( [c isSelected] == YES )
                            [selectedItems addObject:c];
                        }
                    }
                }
            }
		sweepAction = YES;
		sweepCurrentLocation = [self convertPoint:p fromView:nil];
        [self selectItemsInSweepArea];
		}
	[self autoscroll:event];
	[self setNeedsDisplay: YES];	
}

- (NSPoint)newPointDistance:(float)d betweenPoint:(NSPoint)p1 andPoint:(NSPoint)p2 {

    float A = p1.x - p2.x;
    float B = p1.y - p2.y;
    float D = sqrt(A*A+B*B);
    float a = (d / D) * A;
    float b = (d / D) * B;
    
    NSPoint p;
    p.x = p1.x - a;
    p.y = p1.y - b;
    return p;
}

- (void)markAllToolsAsClean {

    NSEnumerator* toolEnumerator = [itemsPtr objectEnumerator];
    id element;
    while ( (element = [toolEnumerator nextObject]) )
        [element setIsDirty:NO];
}

- (void)mouseUp:(NSEvent*)event {

    if (isLocked == YES)
        return;

	if (selection.selectedItem != nil)
		{
		if (selection.selectionType == ITEM_SELECTION)
			{
			// we had a mouse up on a selected item
            if (draggedItems == NO && shiftClicked == NO)
                {
                [self removeAllFocusRings];
                [selection.selectedItem setIsSelected:YES];
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
                    {
					[self addTrackingForItem:element];
                    }
				[self mouseEntered:event];
                }
			}
		else if (selection.selectionType == LINK_SELECTION)
			{
            if (draggedItems == NO && shiftClicked == NO)
                {
                [self removeAllFocusRings];
                [selection.selectedItem setIsSelected:YES];
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
            // Currently, we check the membership of tools on loops at the beginning of drawRect in this class
            }
		else 
			{
			// we attempt to finish establishing a connection b/w inlet/outlets
			InOutlet* start = selection.selectedItem;
				
			NSPoint p = [event locationInWindow];
			ItemSelector mouseUpSelection = [self selectItem:p];

			if (mouseUpSelection.selectedItem != nil && mouseUpSelection.selectedItem != [start toolOwner] && mouseUpSelection.selectedItem != start)
				{
				// get the other end of the connection
				InOutlet* end = nil;
				if (mouseUpSelection.selectionType == INLET_SELECTION || mouseUpSelection.selectionType == OUTLET_SELECTION)
					end = mouseUpSelection.selectedItem;
				else if (mouseUpSelection.selectionType == ITEM_SELECTION)
					end = [mouseUpSelection.selectedItem findInOutletWithColor:[start toolColor]];
					
				// are both start and end inlets or are both outlets?
				BOOL sameInOutType = NO;
				if ( !([start amInlet] != [end amInlet]) )
					sameInOutType = YES;
					
				// are they the same tool type?
				BOOL sameClassType = NO;
				if (end != nil)
					{
					Tool* t1 = [start toolOwner];
					Tool* t2 = [end toolOwner];
					NSString* className1 = NSStringFromClass([t1 class]); 
					NSString* className2 = NSStringFromClass([t2 class]); 
					if ( [className1 isEqualToString:className2] == YES )
						sameClassType = YES;
					}
				
				// are they the same color?
				BOOL sameColor = YES;
				if (end != nil)
					{
                    if ( [start toolColor] != [end toolColor] )
                        sameColor = NO;
                    }
                    
                // are there multiple connections between the same two tools?
                BOOL multipleConnectionsToSameTool = NO;
				if (end != nil)
					{
                    NSMutableArray* childTools = [[start toolOwner] getChildrenTools];
                    [childTools addObject:[end toolOwner]];
                    for (int i=0; i<[childTools count]; i++)
                        {
                        Tool* t1 = [childTools objectAtIndex:i];
                        int nSame = 0;
                        for (int j=0; j<[childTools count]; j++)
                            {
                            if ( t1 == [childTools objectAtIndex:j] )
                                nSame++;
                            }
                        if (nSame > 1)
                            {
                            multipleConnectionsToSameTool = YES;
                            break;
                            }
                        }
                    }
					
				// make the connection
				if ( end != nil && start != end && [start amInlet] != [end amInlet] && [start toolColor] == [end toolColor] && [start toolOwner] != [end toolOwner] && sameClassType == NO && multipleConnectionsToSameTool == NO )
					{
                    // first, find the outlet and inlet for the two tools
                    Outlet* theOutlet = nil;
                    Inlet* theInlet = nil;
                    if ( [start amOutlet] == YES && [end amInlet] == YES )
                        {
                        theOutlet = (Outlet*)start;
                        theInlet = (Inlet*)end;
                        }
                    else if ( [start amInlet] == YES && [end amOutlet] == YES )
                        {
                        theOutlet = (Outlet*)end;
                        theInlet = (Inlet*)start;
                        }
                    else
                        {
                        NSLog(@"Problem finding inlet and outlet");
                        }
                        
                    // find out if the inlet already has a connection, in which case we remove that connection before establishing this one
                    if ( [theInlet hasConnection] == YES )
                        {
                        Connection* c = [theInlet connection];
                        [[c outlet] removeConnection:c];
                        }
                        
                    // we have the outlet manage the connection
                    [theOutlet addConnectionWithInlet:theInlet];
					}
					
				// inform the user of certain errors
				if (sameInOutType == YES)
					{
                    NSAlert* alert = [[NSAlert alloc] init];
                    [alert setMessageText:@"Warning: Incorrect Tool Connection"];
                    [alert setInformativeText:@"You must connect outlets to inlets."];
                    [alert beginSheetModalForWindow:[self window] completionHandler:nil];
                    }
				else if (sameColor == NO)
					{
                    NSAlert* alert = [[NSAlert alloc] init];
                    [alert setMessageText:@"Warning: Incorrect Tool Connection"];
                    [alert setInformativeText:@"You must connect outlets with inlets of the same color."];
                    [alert beginSheetModalForWindow:[self window] completionHandler:nil];
					}
				else if (sameClassType == YES)
					{
                    NSAlert* alert = [[NSAlert alloc] init];
                    [alert setMessageText:@"Warning: Incorrect Tool Connection"];
                    [alert setInformativeText:@"You cannot connect tools of the same type."];
                    [alert beginSheetModalForWindow:[self window] completionHandler:nil];
					}
                else if (multipleConnectionsToSameTool == YES)
                    {
                    NSAlert* alert = [[NSAlert alloc] init];
                    [alert setMessageText:@"Warning: Multiple Connections Between Tools"];
                    [alert setInformativeText:@"You cannot connect two tools multiple times."];
                    [alert beginSheetModalForWindow:[self window] completionHandler:nil];
                    }
				}
			[[NSCursor crosshairCursor] pop];
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

- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender {

	if ( [sender draggingSource] != self )
		{
		// which tool was selected?
		int toolIdx = [[sender draggingSource] whichToolSelected];
        BOOL isLoop = NO;
        if (toolIdx == TOOL_LOOP)
            isLoop = YES;
		
		// get the location of the drop and then convert it to the view's coordinate system
		NSPoint p = [sender draggedImageLocation];
		NSPoint newP = [self convertPoint:p fromView:nil];
		p = newP;
		[self transformToTopLeftCoordinates:(&p)];

		// find the nearest grid intersection to this point, if we are snapping the tool to the grid
		if (snapToGrid == YES)
			p = [self findNearestGridIntersectionToPoint:p];
		
		// Check that the drop location is OK. We don't drop parameters onto other parameters.
        // However, we do allow plates to be dropped anywhere, as long as they fit in the screen.
        if (isLoop == NO)
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
		
		// allocate the tool, set its location, and add it to the list of tools
        Tool* newTool;
        if (toolIdx == TOOL_LOOP)
            newTool = [[ToolLoop alloc] initWithScaleFactor:scaleFactor];
        else if (toolIdx == TOOL_READDATA)
            newTool = [[ToolReadData alloc] initWithScaleFactor:scaleFactor];
        else if (toolIdx == TOOL_DATAENTRY)
            newTool = [[ToolDataEntry alloc] initWithScaleFactor:scaleFactor];
        else if (toolIdx == TOOL_ALIGN)
            newTool = [[ToolAlign alloc] initWithScaleFactor:scaleFactor];
        else if (toolIdx == TOOL_COMBINER)
            newTool = [[ToolCombiner alloc] initWithScaleFactor:scaleFactor];
        else if (toolIdx == TOOL_CONCATENATE)
            newTool = [[ToolConcatenate alloc] initWithScaleFactor:scaleFactor];
        else if (toolIdx == TOOL_MATRIXFILTER)
            newTool = [[ToolMatrixFilter alloc] initWithScaleFactor:scaleFactor];
        else if (toolIdx == TOOL_BOOTSTRAP)
            newTool = [[ToolBootstrap alloc] initWithScaleFactor:scaleFactor];
        else if (toolIdx == TOOL_MODEL)
            newTool = [[ToolModel alloc] initWithScaleFactor:scaleFactor];
        else if (toolIdx == TOOL_MCMC)
            newTool = [[ToolMcmc alloc] initWithScaleFactor:scaleFactor];
        else if (toolIdx == TOOL_SIMULATE)
            newTool = [[ToolSimulate alloc] initWithScaleFactor:scaleFactor];
        else if (toolIdx == TOOL_DISTANCEMATRIX)
            newTool = [[ToolDistanceMatrix alloc] initWithScaleFactor:scaleFactor];
        else if (toolIdx == TOOL_NEIGHBORJOINING)
            newTool = [[ToolNeighborJoining alloc] initWithScaleFactor:scaleFactor];
        else if (toolIdx == TOOL_PARSIMONY)
            newTool = [[ToolParsimony alloc] initWithScaleFactor:scaleFactor];
        else if (toolIdx == TOOL_TREECONTAINER)
            newTool = [[ToolTreeSet alloc] initWithScaleFactor:scaleFactor];
        else if (toolIdx == TOOL_NUMBERSET)
            newTool = [[ToolNumericalMcmcOutput alloc] initWithScaleFactor:scaleFactor];
        else if (toolIdx == TOOL_TREEDIAGNOSIS)
            newTool = [[ToolTreeMcmcDiagnostic alloc] initWithScaleFactor:scaleFactor];
        else if (toolIdx == TOOL_NUMBERDIAGNOSIS)
            newTool = [[ToolMcmcDiagnostic alloc] initWithScaleFactor:scaleFactor];
        else if (toolIdx == TOOL_TREECONSENSUS)
            newTool = [[ToolTreeConsensus alloc] initWithScaleFactor:scaleFactor];

        // set the location for the tool
		[newTool setItemLocation:p];
		
		// add the tool to the tracking area
		[self addTrackingForItem:newTool];
		[newTool setIsCursorOver:YES];
        itemArea.selectedItem = newTool;

        // add the tool to the array of tools
		[itemsPtr addObject:newTool];
        
        // set the view for the tool
        [newTool setAnalysisView:self];
		
		// reset the bottom right corner
		[self setCorners];
        		
        // set an update flag to the document
        [[[NSDocumentController sharedDocumentController] currentDocument] updateChangeCount:NSChangeDone];

		// update the display
		[self setNeedsDisplay:YES];
        
        // set index for loop (not the best place to do this)
        if ([newTool isLoop] == YES)
            [(ToolLoop*)newTool chooseIndex];
		}
	return YES;
}

- (BOOL)readFromPasteboard:(NSPasteboard*)pb {

	if ( [itemsPtr count] == 0 )
		pasteNum = 1;
		
    NSData* d = [pb dataForType:ToolDataPboardType];
    NSMutableArray* pastedTools = [NSKeyedUnarchiver unarchiveObjectWithData:d];

	NSEnumerator* toolEnumerator = [pastedTools objectEnumerator];
	id element;
	while ( (element = [toolEnumerator nextObject]) )
        {
        // offset the tools location a bit
        NSPoint p = [element itemLocation];
		p.x += (5.0) * pasteNum;
		p.y += ([element itemSize].width + 5.0) * pasteNum;
		if (snapToGrid == YES)
			p = [self findNearestGridIntersectionToPoint:p];
        [element setItemLocation:p];
        
		// add the tool to the tracking area
		[self addTrackingForItem:element];
		[element setIsCursorOver:NO];

        // add the tool to the array of tools
		[itemsPtr addObject:element];
        //[element release];
		
		// reset the bottom right corner
		[self setCorners];
        [self updateScrollBars];
        }

	// update the tool's state
    NSMutableArray* depthFirstOrder = [NSMutableArray arrayWithCapacity:0];
    [self initializeDepthFirstOrderForTools:depthFirstOrder];
    toolEnumerator = [depthFirstOrder reverseObjectEnumerator];
    while ( (element = [toolEnumerator nextObject]) )
        {
        if ([pastedTools containsObject:element] == YES)
            [element updateForChangeInParent];
		}
        
	// update the display
	[self setNeedsDisplay:YES];
		
    return YES;
}

- (void)removeAllFocusRings {

	NSEnumerator* enumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
        [element setIsSelected:NO];
        for (int i=0; i<[element numOutlets]; i++)
            {
            Outlet* ol = [element outletIndexed:i];
            for (int j=0; j<[ol numberOfConnections]; j++)
                {
                [[ol connectionWithIndex:j] setIsSelected:NO];
                }
            }
		}
    [self setNeedsDisplay:YES];
}

- (void)removeSelectedConnections {

    // collect the list of connections to remove
    NSMutableArray* connectionsToRemove = [NSMutableArray arrayWithCapacity:1];
	NSEnumerator* enumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
        {
		// Loop through all of the outlets for this tool, looking for
		// outlets that are selected. If the outlet has been selected,
		// then remove the pointers between this outlet and its inlet. Note
		// that only outlets are selected.
		for (int i=0; i<[element numOutlets]; i++)
            {
            Outlet* theOutlet = [element outletIndexed:i];
            for (int j=0; j<[theOutlet numberOfConnections]; j++)
                {
                Connection* c = [theOutlet connectionWithIndex:j];
                if ( [c isSelected] == YES )
                    {
                    if ( [connectionsToRemove containsObject:c] == NO )
                        [connectionsToRemove addObject:c];
                    }
                }
			}
        }
    
    if ([connectionsToRemove count] > 0)
        {
        NSAlert* alert = [[NSAlert alloc] init];
        if ([connectionsToRemove count] == 1)
            {
            [alert setMessageText:@"Warning: Removing a connection between tools"];
            [alert setInformativeText:@"Removing a connection can lead to loss of information in downstream tools"];
            }
        else
            {
            [alert setMessageText:@"Warning: Removing connections between tools"];
            [alert setInformativeText:@"Removing connections can lead to loss of information in downstream tools"];
            }
        [alert addButtonWithTitle:@"Continue"];
        [alert addButtonWithTitle:@"Cancel"];
        [alert beginSheetModalForWindow:[self window] completionHandler:^(NSModalResponse returnCode) {
            
            if (returnCode == NSAlertFirstButtonReturn)
                {
                // remove the connections
                for (Connection* theConnection in connectionsToRemove)
                    {
                    Outlet* ol = [theConnection outlet];
                    [ol removeConnection:theConnection];
                    }
                    
                // reset the view and inform the document of the changes
                [self setNeedsDisplay:YES];
                [[[NSDocumentController sharedDocumentController] currentDocument] updateChangeCount:NSChangeDone];
                }
            else if (returnCode == NSAlertSecondButtonReturn)
                {
                return;
                }
            
            }];
        }
        
}

- (void)scaleFactorChanged:(NSNotification*)notification {

    if ( myScrollView != [notification object] )
        return;

	// get the old and new scale factors
    float oldFactor = scaleFactor;
    scaleFactor = [[notification object] scaleFactor];
	
	// get the index for the scale, so we can choose the correct tool image to display
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

	// go through all of the tools, changing the size of each by the factor and also
	// updating the tracking information for each tool
    [analysisDocumentPtr scaleToolsByScale:scaleFactor andFactor:r];
 
    // reset the bottom right corner
    [self setCorners];
	
    // update the scroll bars
    [self updateScrollBars];

	// update the tracking information for the tools
	[self updateAllTrackingAreas];
        		
    [self setNeedsDisplay:YES];
}

- (IBAction)selectAll:(id)sender {

    AnalysisDocument* doc = [[NSDocumentController sharedDocumentController] currentDocument];
    [doc selectAllTools];
}

- (void)selectAllItems {

	NSEnumerator* enumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
        [element setIsSelected:YES];
		for (int i=0; i<[element numOutlets]; i++)
			{
            Outlet* theOutlet = [element outletIndexed:i];
            for (int j=0; j<[theOutlet numberOfConnections]; j++)
                {
                Connection* c = [theOutlet connectionWithIndex:j];
                [c setIsSelected:YES];
                }
			}
		}
    [self setNeedsDisplay:YES];
}

- (ItemSelector)selectItem:(NSPoint)forPoint {

	// set to base state
	ItemSelector mySelection;
	mySelection.selectedItem = nil;
	mySelection.selectionType = ITEM_SELECTION;
	
	// transform point to coordinate system of view objects (such as tools)
	NSPoint p = [self convertPoint:forPoint fromView:nil];
	
	// enumerate all of the tools, to see if any of them have been selected
    // we first check for everything but connections, which we leave for last
	NSEnumerator* enumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
        if ( [element isLoop] == YES )
            continue;
            
		// initialize information on the tool
		NSRect toolRect, infoRect, inspRect;
		toolRect.origin = [element itemLocation];
		toolRect.size = [element itemSize];
		[self transformToBottomLeftCoordinates:(&toolRect.origin)];
        
        infoRect.size = inspRect.size = NSMakeSize(ITEM_IMAGE_SIZE*scaleFactor*0.2, ITEM_IMAGE_SIZE*scaleFactor*0.2);
        infoRect.origin = inspRect.origin = toolRect.origin;
        infoRect.origin.x += toolRect.size.width - infoRect.size.width - toolRect.size.width * 0.05;
        infoRect.origin.y += toolRect.size.height * 0.05;
        inspRect.origin.x += toolRect.size.width * 0.05;
        inspRect.origin.y += toolRect.size.height * 0.05;

		// 1, check to see if the point is in any of the inlets 
		for (int i=0; i<[element numInlets]; i++)
			{
			Inlet* il = [element inletIndexed:i];
            NSRect r = [il rectForToolWithRect:toolRect];
            if ( NSPointInRect(p, r) == YES )
				{
				mySelection.selectedItem = il;
				mySelection.selectionType = INLET_SELECTION;
				return mySelection;
				}
			}
			
		// 2, check to see if the point is in any of the outlets
		for (int i=0; i<[element numOutlets]; i++)
			{
			Outlet* ol = [element outletIndexed:i];
            NSRect r = [ol rectForToolWithRect:toolRect];
            if ( NSPointInRect(p, r) == YES )
				{
				mySelection.selectedItem = ol;
				mySelection.selectionType = OUTLET_SELECTION;
				return mySelection;
				}
			}

        // 3, check if the point is the information button (opening the control window)
        if ( CGRectContainsPoint( NSRectToCGRect(infoRect), NSPointToCGPoint(p) ) )
            {
            if ([element hasController] == YES)
                {
                mySelection.selectedItem = element;
                mySelection.selectionType = INFO_SELECTION;
                return mySelection;
                }
            }

        // 4, check if the point is the inspector button 
        if ([element hasInspectorInfo] == YES)
            {
            if ( CGRectContainsPoint( NSRectToCGRect(inspRect), NSPointToCGPoint(p) ) )
                {
                mySelection.selectedItem = element;
                mySelection.selectionType = INSP_SELECTION;
                return mySelection;
                }
            }
            			
		// 5, check if the point is in the tool image
		if ( CGRectContainsPoint( NSRectToCGRect(toolRect), NSPointToCGPoint(p) ) )
            {
            cursorOffset.width  = p.x - toolRect.origin.x;
            cursorOffset.height = p.y - toolRect.origin.y;
			mySelection.selectedItem = element;
			mySelection.selectionType = ITEM_SELECTION;
			return mySelection;
            }
		}
        
    // now, check for clicks on connections
	enumerator = [itemsPtr objectEnumerator];
	while ( (element = [enumerator nextObject]) )
		{            
        // 6, check if the point is any of the connections between objects
        for (int i=0; i<[element numOutlets]; i++)
            {
            Outlet* theOutlet = [element outletIndexed:i];
            for (int j=0; j<[theOutlet numberOfConnections]; j++)
                {
                Connection* c = [theOutlet connectionWithIndex:j];
                NSBezierPath* path1 = [c path1];
                if ( [path1 elementCount] > 0 )
                    {
                    NSPoint myPoints[3];
                    [path1 elementAtIndex:0 associatedPoints:myPoints];
                    NSPoint ptA = myPoints[0];
                    for (int k=1; k<[path1 elementCount]; k++)
                        {
                        [path1 elementAtIndex:k associatedPoints:myPoints];
                        NSPoint ptB = myPoints[0];
                        if ( [self isPoint:p inLineSegmentDefinedByPoint:ptA andPoint:ptB] == YES )
                            {
                            mySelection.selectedItem = c;
                            mySelection.selectionType = LINK_SELECTION;
                            return mySelection;
                            }
                        }
                    }
                NSBezierPath* path2 = [c path2];
                if ( [path2 elementCount] > 0 )
                    {
                    NSPoint myPoints[3];
                    [path2 elementAtIndex:0 associatedPoints:myPoints];
                    NSPoint ptA = myPoints[0];
                    for (int k=1; k<[path2 elementCount]; k++)
                        {
                        [path2 elementAtIndex:k associatedPoints:myPoints];
                        NSPoint ptB = myPoints[0];
                        if ( [self isPoint:p inLineSegmentDefinedByPoint:ptA andPoint:ptB] == YES )
                            {
                            mySelection.selectedItem = c;
                            mySelection.selectionType = LINK_SELECTION;
                            return mySelection;
                            }
                        }
                    }
                    
                }
            }
		}

    // check to see if we have selected a loop tool
	enumerator = [itemsPtr reverseObjectEnumerator];
	while ( (element = [enumerator nextObject]) )
		{
        if ( [element isLoop] == YES )
            {
			// selecting for a plate
			NSRect pRect;
			pRect.origin = [element itemLocation];
			pRect.size   = [element itemSize];
            //pRect.size.width *= scaleFactor;
            //pRect.size.height *= scaleFactor;
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

- (void)selectItemsInSweepArea {

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
        
    // enumerate the tools, deciding if any of them are in the sweep rectangle
	NSEnumerator* enumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
        // get a rectangle for the tool
		NSRect toolRect;
		toolRect.origin = [element itemLocation];
		toolRect.size   = [element itemSize];
		[self transformToBottomLeftCoordinates:(&toolRect.origin)];
        
        // decide if the tool's rectangle is inside the sweep rectangle
		if ( CGRectIntersectsRect( NSRectToCGRect(sweepRect), NSRectToCGRect(toolRect) ) )
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
		for (int i=0; i<[element numOutlets]; i++)
            {
            Outlet* theOutlet = [element outletIndexed:i];
            for (int j=0; j<[theOutlet numberOfConnections]; j++)
                {
                Connection* c = [theOutlet connectionWithIndex:j];
                BOOL isConnectionInSweepArea = NO;
                NSBezierPath* path1 = [c path1];
                if ( [path1 elementCount] > 0 )
                    {
                    NSPoint myPoints[3];
                    [path1 elementAtIndex:0 associatedPoints:myPoints];
                    NSPoint ptA = myPoints[0];
                    for (int k=1; k<[path1 elementCount]; k++)
                        {
                        [path1 elementAtIndex:k associatedPoints:myPoints];
                        NSPoint ptB = myPoints[0];
                        NSRect r = NSZeroRect;
                        if (ptA.x < ptB.x)
                            r.origin.x = ptA.x;
                        else
                            r.origin.x = ptB.x;
                        if (ptA.y < ptB.y)
                            r.origin.y = ptA.y;
                        else
                            r.origin.y = ptB.y;
                        r.size.width = fabs(ptA.x-ptB.x);
                        r.size.height = fabs(ptA.y-ptB.y);
                        if (r.size.width < 4.0)
                            r.size.width = 4.0;
                        if (r.size.height < 4.0)
                            r.size.height = 4.0;
                        if ( CGRectIntersectsRect(NSRectToCGRect(sweepRect), NSRectToCGRect(r)) )
                            isConnectionInSweepArea = YES;
                        }
                    }
                NSBezierPath* path2 = [c path2];
                if ( [path2 elementCount] > 0 )
                    {
                    NSPoint myPoints[3];
                    [path2 elementAtIndex:0 associatedPoints:myPoints];
                    NSPoint ptA = myPoints[0];
                    for (int k=1; k<[path1 elementCount]; k++)
                        {
                        [path2 elementAtIndex:k associatedPoints:myPoints];
                        NSPoint ptB = myPoints[0];
                        NSRect r = NSZeroRect;
                        if (ptA.x < ptB.x)
                            r.origin.x = ptA.x;
                        else
                            r.origin.x = ptB.x;
                        if (ptA.y < ptB.y)
                            r.origin.y = ptA.y;
                        else
                            r.origin.y = ptB.y;
                        r.size.width = fabs(ptA.x-ptB.x);
                        r.size.height = fabs(ptA.y-ptB.y);
                        if (r.size.width < 4.0)
                            r.size.width = 4.0;
                        if (r.size.height < 4.0)
                            r.size.height = 4.0;
                        if ( CGRectIntersectsRect(NSRectToCGRect(sweepRect), NSRectToCGRect(r)) )
                            isConnectionInSweepArea = YES;
                        }
                    }
                 
                if ( isConnectionInSweepArea == YES )
                    {
                    if ( [selectedItems containsObject:c] == NO )
                        [c setIsSelected:YES];
                    }
                else
                    {
                    if ( [c isSelected] == YES && [selectedItems containsObject:c] == NO )
                        [c setIsSelected:NO];
                    }
                    
                }
            }
        }
        
    // update the view to reflect the possible selections
    [self setNeedsDisplay:YES];
}

- (NSMutableArray*)unavailableIndices {

    NSMutableArray* usedIndices = [[NSMutableArray alloc] init];
	NSEnumerator* enumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
        if ( [element isLoop] == YES )
            {
            NSString* c = [NSString stringWithFormat:@"%c", [element indexLetter]];
            [usedIndices addObject:c];
            }
        }
    return usedIndices;
}

- (void)unselectAllItems {

	NSEnumerator* enumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
        [element setIsSelected:NO];
		for (int i=0; i<[element numOutlets]; i++)
			{
            Outlet* theOutlet = [element outletIndexed:i];
            for (int j=0; j<[theOutlet numberOfConnections]; j++)
                {
                Connection* c = [theOutlet connectionWithIndex:j];
                [c setIsSelected:NO];
                }
			}
		}
    [self setNeedsDisplay:YES];
}

- (void)updateBackgroundColor:(NSNotification*)notification {

    bkgrndColor = [[notification object] analysisBkgrndColor];
    gridColor   = [[notification object] analysisGridColor];
    [self setNeedsDisplay:YES];
}

/* When a tool changes its state, e.g. after reading data, it may invalidate the
   states of its children tool(s) which were instantiated assuming that this tool
   had a different state. We rely on the AnalysisView to perform the updating of
   tools downstream of the tool whose state has changed. You would think that it
   would be easier for each tool to signal its children that they their state 
   needs to be updated. However, if you do this, tools downstream from the tool
   whose state has changed will be updated multiple times. The analysis view is
   the logical place to manage updating of tools because the tools are owned by
   the AnalysisView. Note that this function calls updateForChangeInParent for each
   tool. */
- (void)updateToolsDownstreamFromTool:(Tool*)t {

    // initialize the depth-first traversal order for the graph of tools
    NSMutableArray* depthFirstOrder = [NSMutableArray arrayWithCapacity:0];
    [self initializeDepthFirstOrderForTools:depthFirstOrder];
    
    // mark all tools as having not been visited and as clean
    NSEnumerator* toolEnumerator = [itemsPtr objectEnumerator];
    id element;
    while ( (element = [toolEnumerator nextObject]) )
        [element setIsVisited:NO];
        
    // mark the tool of interest as having been visited
    [t setIsVisited:YES];

    // mark tools downstream from removed tools/connections as visited
    toolEnumerator = [depthFirstOrder reverseObjectEnumerator];
    while ( (element = [toolEnumerator nextObject]) )
        {
        if ([element isSomeParentVisited] == YES)
            [element setIsVisited:YES];
        }
        
    // unmark the tool of interest
    [t setIsVisited:NO];

    // update tools in need of update
    toolEnumerator = [depthFirstOrder reverseObjectEnumerator];
    while ( (element = [toolEnumerator nextObject]) )
        {
        if ( [element isVisited] == YES )
            [element updateForChangeInParent];
        }
}

- (void)writeToPasteboard:(NSPasteboard*)pb {

	// remove connections between selected and unselected tools so that
	// the archiving of the copied/cut tools goes OK
	NSMutableArray* itemPairs = [NSMutableArray arrayWithCapacity:1];
    NSMutableArray* connectionsToRemove = [NSMutableArray arrayWithCapacity:1];
	NSEnumerator* itemEnumerator = [itemsPtr objectEnumerator];
	id element;
	while ( (element = [itemEnumerator nextObject]) )
		{
		for (int i=0; i<[element numOutlets]; i++)
			{
			Outlet* theOutlet = [element outletIndexed:i];
            for (int j=0; j<[theOutlet numberOfConnections]; j++)
                {
                Connection* c = [theOutlet connectionWithIndex:j];
                Tool* t1 = [theOutlet toolOwner];
                Tool* t2 = [[c inlet] toolOwner];
                if ( [t1 isSelected] == YES && [t2 isSelected] == YES && [c isSelected] == NO && [connectionsToRemove containsObject:c] == NO )
                    {
                    ConnectionPair* p = [[ConnectionPair alloc] init];
                    [p setOutlet:theOutlet];
                    [p setInlet:[c inlet]];
                    [itemPairs addObject:p];
                    [connectionsToRemove addObject:c];
                    }
                else if ( ([t1 isSelected] == YES && [t2 isSelected] == NO) || ([t1 isSelected] == NO && [t2 isSelected] == YES)  )
                    {
                    if ( [connectionsToRemove containsObject:c] == NO )
                        {
                        ConnectionPair* p = [[ConnectionPair alloc] init];
                        [p setOutlet:theOutlet];
                        [p setInlet:[c inlet]];
                        [itemPairs addObject:p];
                        [connectionsToRemove addObject:c];
                        }
                    }
                }
			}
		}
        
    // remove the connections (temporarily)
	NSEnumerator* connEnumerator = [connectionsToRemove objectEnumerator];
	while ( (element = [connEnumerator nextObject]) )
        {
        Outlet* theOutlet = [element outlet];
        [theOutlet removeConnection:element];
        }
		
    // get a list of the selected items to archive
    [copiedItems removeAllObjects];
	itemEnumerator = [itemsPtr objectEnumerator];
	while ( (element = [itemEnumerator nextObject]) )
		{
        if ([element isSelected] == YES)
            [copiedItems addObject:element];
        }
        
    // archive the data to the pasteboard
    NSData* d = [NSKeyedArchiver archivedDataWithRootObject:copiedItems];
    [pb setData:d forType:ToolDataPboardType];
	
	// re-establish any removed connections
	NSEnumerator* connectionEnumerator = [itemPairs objectEnumerator];
	while ( (element = [connectionEnumerator nextObject]) )
		{
        Outlet* theOutlet = [element outlet];
        Inlet* theInlet = [element inlet];
        //[element setOutlet:theOutlet];
        //[element setInlet:theInlet];
        [theOutlet addConnectionWithInlet:theInlet];
		}
}

@end
