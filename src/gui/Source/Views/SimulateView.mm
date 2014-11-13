#import "GuiTree.h"
#import "Node.h"
#import "SimulateView.h"
#import "WindowControllerSimulate.h"




@implementation SimulateView

- (BOOL)acceptsFirstResponder {

    return YES;
}

- (void)awakeFromNib {

    handleSize = NSMakeSize(15.0, 15.0);
    isBiggestRectInitialized = NO;
    factor = 0.95;
}

- (id)initWithFrame:(NSRect)frame {

    if ( (self = [super initWithFrame:frame]) ) 
        {
        isBiggestRectInitialized = NO;
        factor = 0.95;
        }
    return self;
}

- (void)drawRect:(NSRect)dirtyRect {

    // draw the background
	NSRect bounds = [self bounds];
    [[NSColor clearColor] set];
	[NSBezierPath fillRect:bounds];
    
    // get a pointer to the tree
    GuiTree* t = [myWindowController myTree];
    if (t != nil)
        {        
        // find the maximum size of a name
        if (isBiggestRectInitialized == NO)
            [self findBiggestTextRectForTaxonNames];
            
        // instantiate the dictionary with text attributes for writing the taxon labels
        float fs = [myWindowController fontSize];
        NSDictionary* attrs = [NSDictionary dictionaryWithObjects:[NSArray arrayWithObjects:[NSFont fontWithName:@"Chalkboard" size:fs], [[NSColor blackColor] colorWithAlphaComponent:1.0], nil]
                                                  forKeys:[NSArray arrayWithObjects:NSFontAttributeName,NSForegroundColorAttributeName, nil]];
			        
        // draw the tree
		double xOffset = bounds.size.width  * ((1.0-factor)*0.5);
		double yOffset = bounds.size.height * ((1.0-factor)*0.5);
        double xStart  = xOffset;
        double xEnd    = bounds.size.height * factor - biggestNameRect.size.width;
        double h       = xEnd - xStart;
        
		[NSBezierPath setDefaultLineWidth:4.0];
        [[NSColor blackColor] set];
        
        for (int n=0; n<[t numberOfNodes]; n++)
			{
            Node* p = [t downPassNodeIndexed:n];
            
			NSPoint a, b;
			a.x = xOffset + bounds.size.width * factor * [p x];
			a.y = yOffset + h * [p y];
            [p setViewCoordinates:a];
			b.x = xOffset + bounds.size.width * factor * [p x];
			b.y = yOffset + h * [[p ancestor] y];
            
            if ( [p isBranchSelected] == YES )
                [[NSColor redColor] set];
            else
                [[NSColor blackColor] set];
			[NSBezierPath strokeLineFromPoint:a toPoint:b];
            [[NSColor blackColor] set];
			if ([p isLeaf] == NO)
				{
                Node* q = [p descendantIndexed:([p numberOfDescendants]-1)];
				NSPoint l, r, m;
				m.x = xOffset + bounds.size.width * factor * [p x];
				m.y = a.y;
				l.x = xOffset + bounds.size.width * factor * [[p descendantIndexed:0] x] - 0.5*1.0;
				l.y = a.y;
				r.x = xOffset + bounds.size.width * factor * [q x] + 0.5*1.0;
				r.y = a.y;
				
				[NSBezierPath strokeLineFromPoint:l toPoint:m];
				[NSBezierPath strokeLineFromPoint:r toPoint:m];
				}
            else    
                {
                // draw the taxon name
                NSPoint drawPt = NSZeroPoint;
                drawPt.x = xOffset + bounds.size.width * factor * [p x];
                drawPt.y = yOffset + bounds.size.height * factor * [p y];
                drawPt.x += biggestNameRect.size.height * 0.5;
                drawPt.y -= biggestNameRect.size.width;

                [NSGraphicsContext saveGraphicsState];
                NSString* taxonName = [p name];
                NSAttributedString* attrString = [[NSAttributedString alloc] initWithString:taxonName attributes:attrs];
                NSAffineTransform* xform = [NSAffineTransform transform];

                [xform translateXBy:drawPt.x yBy:drawPt.y];
                [xform rotateByDegrees:90.0];
                [xform translateXBy:-drawPt.x yBy:-drawPt.y];
                [xform concat];
                
                [attrString drawAtPoint:drawPt];

                [NSGraphicsContext restoreGraphicsState];
                }
                
            // draw the editing components
            if ( [myWindowController editingTree] == YES && [t isRoot:p] == NO )
                {
                NSRect nodeHandle;
                nodeHandle.origin = a;
                nodeHandle.origin.x -= handleSize.width * 0.5;
                nodeHandle.origin.y -= handleSize.height * 0.5;
                nodeHandle.size = handleSize;
                if ( [p isNodeSelected] == YES )
                    [[NSColor redColor] set];
                else
                    [[NSColor blackColor] set];
				[[NSBezierPath bezierPathWithRect:nodeHandle] fill];
                }
            [[NSColor blackColor] set];
            
            if (activeBranchDrag == YES)
                {
                [[NSColor grayColor] set];
				[NSBezierPath strokeLineFromPoint:startingPoint toPoint:currentPoint];
                [[NSColor blackColor] set];
                }
			}
		
		[NSBezierPath setDefaultLineWidth:1.0];
        }
}

- (void)findBiggestTextRectForTaxonNames {

    biggestNameRect = NSZeroRect;
    GuiTree* t = [myWindowController myTree];
    if (t != nil)
        {
        float fs = [myWindowController fontSize];
        NSDictionary* attrs = [NSDictionary dictionaryWithObjects:[NSArray arrayWithObjects:[NSFont fontWithName:@"Chalkboard" size:fs], [[NSColor blackColor] colorWithAlphaComponent:1.0], nil]
                                                  forKeys:[NSArray arrayWithObjects:NSFontAttributeName,NSForegroundColorAttributeName, nil]];
        for (int i=0; i<[t numberOfNodes]; i++)
            {
            Node* p = [t downPassNodeIndexed:i];
            if ([p isLeaf] == YES)
                {
                NSString* taxonName = [p name];
                NSAttributedString* attrString = [[NSAttributedString alloc] initWithString:taxonName attributes:attrs];
                NSRect textSize = [attrString boundingRectWithSize:NSMakeSize(1e10, 1e10) options:NSStringDrawingUsesDeviceMetrics];
                if (textSize.size.width > biggestNameRect.size.width)
                    biggestNameRect.size.width = textSize.size.width;
                if (textSize.size.height > biggestNameRect.size.height)
                    biggestNameRect.size.height = textSize.size.height;
                }
            }
        isBiggestRectInitialized = YES;
        }
}

- (void)keyDown:(NSEvent*)event {
    
    // maybe we're deleting a clade
    NSString* chars = [event characters];
    unichar character = [chars characterAtIndex:0];
    if (character == NSDeleteCharacter)
		{
        if ( [myWindowController editingTree] == NO )
            return;
        if ( selectedNode == nil)
            return;
        if ( [myWindowController myTree] == nil )
            return;
        if ( [selectedNode isNodeSelected] == YES )
            {
            GuiTree* t = [myWindowController myTree];
            [t removeSubtreeAboveNode:selectedNode];
            }
        [self setNeedsDisplay:YES];
        }
}

- (void)mouseDown:(NSEvent*)event {
	    	
    if ( [myWindowController editingTree] == NO )
        return;
    
	// where did the user click?
    NSPoint p = [self convertPoint:[event locationInWindow] fromView:nil];
    startingPoint = p;
    activeBranchDrag = NO;
    
    // is the click somewhere on the tree?
    GuiTree* t = [myWindowController myTree];
    selectedNode = nil;
    if (t != nil)
        {
        [t deselectAllNodes];
        for (int n=0; n<[t numberOfNodes]; n++)
			{
            Node* nde = [t downPassNodeIndexed:n];
            
            if ( [t isRoot:nde] == YES )
                continue;
            
            // is the click on a node (for changing branch length)?
            NSRect nodeHandle;
            nodeHandle.origin = [nde viewCoordinates];
            nodeHandle.origin.x -= handleSize.width * 0.5;
            nodeHandle.origin.y -= handleSize.height * 0.5;
            nodeHandle.size = handleSize;
			if ( CGRectContainsPoint( NSRectToCGRect(nodeHandle), NSPointToCGPoint(p) ) )
                {
                [nde setIsNodeSelected:YES];
                selectedNode = nde;
                
                if ([selectedNode numberOfDescendants] == 0)   
                    {
                    upperLimitY = [selectedNode y];
                    }
                else 
                    {
                    double maxY = 1.0;
                    for (int i=0; i<[selectedNode numberOfDescendants]; i++)
                        {
                        Node* d = [selectedNode descendantIndexed:i];
                        if ( [d y] < maxY )
                            maxY = [d y];
                        }
                    upperLimitY = maxY;
                    }
                if ([selectedNode ancestor] == nil)
                    lowerLimitY = 0.0;
                else
                    lowerLimitY = [[selectedNode ancestor] y];
                
                break;
                }
                
            // is the click near the branch for this node?
            if ([nde ancestor] != nil)
                {
                NSPoint a = [nde viewCoordinates];
                NSPoint b = [[nde ancestor] viewCoordinates];
                if ( p.y < a.y && p.y > b.y )
                    {
                    if ( fabs(p.x - a.x) < 5.0 )
                        {
                        selectedNode = nde;
                        [nde setIsBranchSelected:YES];
                        break;
                        }
                    }
                }
            
            }
        }

    [self setNeedsDisplay:YES];
}

- (void)mouseDragged:(NSEvent*)event {

    if ( [myWindowController editingTree] == NO )
        return;

    if ( selectedNode == nil)
        return;
            
    // get the position of the cursor
    NSPoint p = [self convertPoint:[event locationInWindow] fromView:nil];
    
    if ( [selectedNode isNodeSelected] == YES )
        {
        // a node was selected
        NSRect bounds = [self bounds];
        double xOffset = bounds.size.width  * ((1.0-factor)*0.5);
        double yOffset = bounds.size.height * ((1.0-factor)*0.5);
        double xStart  = xOffset;
        double xEnd    = bounds.size.height * factor - biggestNameRect.size.width;
        double h       = xEnd - xStart;
        double minY    = yOffset + h * lowerLimitY;
        double maxY    = yOffset + h * upperLimitY;
        double newY    = ((p.y - yOffset)/h);
        if ( p.y < maxY && p.y > minY)
            [selectedNode setY:newY];
        }
    else 
        {
        // a branch was selected
        activeBranchDrag = YES;
        currentPoint = p;

        GuiTree* t = [myWindowController myTree];
        if (t != nil)
            {
            [t deselectAllNodes];
            for (int n=0; n<[t numberOfNodes]; n++)
                {
                Node* nde = [t downPassNodeIndexed:n];
                NSPoint a = [nde viewCoordinates];
                NSPoint b = [[nde ancestor] viewCoordinates];
                if ( p.y < a.y && p.y > b.y )
                    {
                    if ( fabs(p.x - a.x) < 5.0 )
                        [nde setIsBranchSelected:YES];
                    }
                }
            [selectedNode setIsBranchSelected:YES];
            }
        }
    
    [self setNeedsDisplay:YES];
}

- (void)mouseUp:(NSEvent*)event {

    if ( [myWindowController editingTree] == NO )
        return;

    if ( selectedNode == nil)
        return;

    // get the position of the cursor
    NSPoint p = [self convertPoint:[event locationInWindow] fromView:nil];

    if ( [selectedNode isBranchSelected] == YES )
        {
        
        float maxY = [selectedNode viewCoordinates].y;

        GuiTree* t = [myWindowController myTree];
        Node* newAttachmentNode = nil;
        if (t != nil)
            {
            for (int n=0; n<[t numberOfNodes]; n++)
                {
                Node* nde = [t downPassNodeIndexed:n];
                NSPoint a = [nde viewCoordinates];
                NSPoint b = [[nde ancestor] viewCoordinates];
                if ( p.y < a.y && p.y > b.y )
                    {
                    if ( fabs(p.x - a.x) < 5.0 && p.y < maxY )
                        {
                        newAttachmentNode = nde;
                        [nde setIsBranchSelected:YES];
                        break;
                        }
                    }
                }
            NSPoint selectedNodeCoordinates = [selectedNode viewCoordinates];
            if ( newAttachmentNode != nil && selectedNode != newAttachmentNode && p.y < selectedNodeCoordinates.y )
                {
                float newY1 = ([newAttachmentNode viewCoordinates].y - p.y) / ([newAttachmentNode viewCoordinates].y - [[newAttachmentNode ancestor] viewCoordinates].y);
                float newY = [newAttachmentNode y] - newY1 * ([newAttachmentNode y] - [[newAttachmentNode ancestor] y]);
    
                [t moveFromBranch:selectedNode toBranch:newAttachmentNode forTreeYCoordinates:newY];
                }
            [selectedNode setIsBranchSelected:NO];
            [newAttachmentNode setIsBranchSelected:NO];
            }
            
        [t deselectAllNodes];
        }
    activeBranchDrag = NO;
        
    [self setNeedsDisplay:YES];
}

@end
