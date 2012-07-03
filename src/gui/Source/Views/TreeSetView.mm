#import "Node.h"
#import "GuiTree.h"
#import "TreeSetView.h"
#import "WindowControllerTreeViewer.h"




@implementation TreeSetView

- (id)initWithFrame:(NSRect)frame {

    if ( (self = [super initWithFrame:frame]) ) 
        {
        }
    return self;
}

- (void)drawRect:(NSRect)dirtyRect {

    // draw the background
	NSRect bounds = [self bounds];
    [[NSColor whiteColor] set];
	[NSBezierPath fillRect:bounds];
    
    // get a pointer to the current tree
    Tree* t = [myWindowController activeTree];

    // draw the tree
    if (t != nil)
        {
        [t initializeDownPassSequence];
        
        float fs = [myWindowController fontSize];

        // find the maximum size of a name
        NSDictionary* attrs = [NSDictionary dictionaryWithObjects:[NSArray arrayWithObjects:[NSFont fontWithName:@"Chalkboard" size:fs], [[NSColor blackColor] colorWithAlphaComponent:1.0], nil]
                                                  forKeys:[NSArray arrayWithObjects:NSFontAttributeName,NSForegroundColorAttributeName, nil]];
        NSRect biggestNameRect = NSZeroRect;
        for (int i=0; i<[t numberOfNodes]; i++)
            {
            Node* p = [t downPassNodeIndexed:i];
            if ([p isLeaf] == YES)
                {
                NSString* taxonName = [p name];
                NSAttributedString* attrString = [[NSAttributedString alloc] initWithString:taxonName attributes:attrs];
                NSRect textSize = [attrString boundingRectWithSize:NSMakeSize(1e10, 1e10) options:nil];
                [attrString release];
                if (textSize.size.width > biggestNameRect.size.width)
                    biggestNameRect.size.width = textSize.size.width;
                if (textSize.size.height > biggestNameRect.size.height)
                    biggestNameRect.size.height = textSize.size.height;
                }
            }
			        
        // initialize coordinates
        [t setCoordinates];

        // draw the tree
        //float oldDefaultLineWidth = [NSBezierPath defaultLineWidth];
		[NSBezierPath setDefaultLineWidth:2.0];
        [[NSColor blackColor] set];
		double factor = 0.95;
		double xOffset = bounds.size.width * 0.025;
		double yOffset = bounds.size.height * 0.025;
        double xStart = xOffset;
        double xEnd   = bounds.size.height * factor - biggestNameRect.size.width;
        double h      = xEnd - xStart;

        NSLog(@"[t numberOfNodes] = %d", [t numberOfNodes]);
        for (int n=0; n<[t numberOfNodes]; n++)
			{
            Node* p = [t downPassNodeIndexed:n];
            NSLog(@"p = %@ (%f %f)", p, [p x], [p y]);
            
			NSPoint a, b;
			a.x = xOffset + bounds.size.width * factor * [p x];
			a.y = yOffset + h * [p y];
			b.x = xOffset + bounds.size.width * factor * [p x];
			b.y = yOffset + h * [[p ancestor] y];
			[NSBezierPath strokeLineFromPoint:a toPoint:b];
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
                [attrString release];

                [NSGraphicsContext restoreGraphicsState];
                }
			}
		
		[NSBezierPath setDefaultLineWidth:1.0];

        }
}

@end
