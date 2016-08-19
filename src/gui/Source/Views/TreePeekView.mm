#import "GuiTree.h"
#import "Node.h"
#import "TreePeekView.h"
#import "WindowControllerTreePeek.h"



@implementation TreePeekView

- (float)chooseFontSizeForWidth:(float)w andNumberOfLabels:(int)n {

    // choose font size
    for (int i=100; i>=0; i--)
        {
        float s = (i+1) * 0.2;
        NSDictionary* attrs = [NSDictionary dictionaryWithObjects:[NSArray arrayWithObjects:[NSFont fontWithName:@"Chalkboard" size:s], [[NSColor blackColor] colorWithAlphaComponent:1.0], nil]
                                                  forKeys:[NSArray arrayWithObjects:NSFontAttributeName,NSForegroundColorAttributeName, nil]];
        NSString* str = @"THIS IS A TEST";
        NSAttributedString* attrString = [[NSAttributedString alloc] initWithString:str attributes:attrs];
        NSRect textSize = [attrString boundingRectWithSize:NSMakeSize(1e10, 1e10) options:NSStringDrawingUsesLineFragmentOrigin];
        if ( textSize.size.height * n < w )
            return s;
        }
    return 1.0;
}

- (void)drawRect:(NSRect)dirtyRect {

    [super drawRect:dirtyRect];

    // get the bounds and fill in the color
	NSRect bounds = [self bounds];
    [[NSColor whiteColor] set];
	[NSBezierPath fillRect:bounds];

    [[NSColor blackColor] set];
    
    GuiTree* t = [myController currentTree];
    if (t != nil)
        {
        
        BOOL drawAsMonophyleticIngroup = [myController drawAsMonophyleticIngroup];

        // tree initialization
        [t initializeDownPassSequence];
        [t setCoordinates:drawAsMonophyleticIngroup];
        
        // choose font size
        float fs = [self chooseFontSizeForWidth:bounds.size.width andNumberOfLabels:[t numberOfTaxa]];
        
        // choose the line width
        //float lineWidth = [self chooseLineWithForNumberOfTaxa:[myTree numberOfTaxa]];
        float lineWidth = 1.0;
		[NSBezierPath setDefaultLineWidth:lineWidth];
    
        // find the maximum size of a name
        float lengthOfLongestName = [t maxNameWidthForFont:[NSFont fontWithName:@"Chalkboard" size:fs]];
        float heightOfHighestName = [t maxNameHeightForFont:[NSFont fontWithName:@"Chalkboard" size:fs]];

        // get a rectangle for the printable tree area
        float gap = heightOfHighestName * 0.5;
        float printableW = bounds.size.width;
        float printableH = bounds.size.height;
        float bottomBuffer = 0.0;
        float sideMargin = printableW * 0.04;
        NSRect drawableArea = NSMakeRect( (bounds.size.width-printableW)*0.5, (bounds.size.height-printableH)*0.5, printableW, printableH);
        NSRect treeArea = drawableArea;
        treeArea.origin.x += sideMargin;
        treeArea.origin.y += (5.0 + bottomBuffer);
        treeArea.size.width -= 2.0 * sideMargin;
        treeArea.size.height -= (10.0 + bottomBuffer + lengthOfLongestName + gap);
        double xOffset = treeArea.origin.x - bounds.origin.x;
        double yOffset = treeArea.origin.y - bounds.origin.y;
        NSRect labelArea = treeArea;
        labelArea.origin.y += treeArea.size.height + gap;
        labelArea.size.height = lengthOfLongestName;        

        // draw the tree
        for (int n=0; n<[t numberOfNodes]; n++)
			{
            Node* p = [t downPassNodeIndexed:n];
            
            // draw vertical lines
			NSPoint a, b;
			a.x = xOffset + treeArea.size.width * [p x];
			a.y = yOffset + treeArea.size.height * [p y];
            if ([t isRoot:p] == YES)
                {
                b.x = xOffset + treeArea.size.width * [p x];
                b.y = yOffset;
                }
            else
                {
                b.x = xOffset + treeArea.size.width * [p x];
                b.y = yOffset + treeArea.size.height * [[p ancestor] y];
                if ([t isRoot:[p ancestor]] == YES && drawAsMonophyleticIngroup == YES && p == [[p ancestor] descendantIndexed:0])
                    b.y = yOffset;
                }
			[NSBezierPath strokeLineFromPoint:a toPoint:b];
            
            // draw horizontal lines or taxon label
			if ([p isLeaf] == NO)
				{
                NSPoint l, r;
                l = a;
                r = a;
                l.x = xOffset + treeArea.size.width;
                r.x = 0.0;
                for (size_t i=0; i<[p numberOfDescendants]; i++)
                    {
                    Node* q = [p descendantIndexed:i];
                    if ( !(drawAsMonophyleticIngroup == YES && [q index] == [t outgroupIdx]) )
                        {
                        double qX = (xOffset + treeArea.size.width * [q x]);
                        if ( qX < l.x )
                            l.x = qX;
                        if ( qX > r.x )
                            r.x = qX;
                        }
                    }
				[NSBezierPath strokeLineFromPoint:l toPoint:r];
				}
            else    
                {
                // draw the taxon name
                NSPoint drawPt = NSZeroPoint;
                drawPt.x = xOffset + labelArea.size.width * [p x];
                drawPt.y = labelArea.origin.y;
                drawPt.x += heightOfHighestName * 0.5;

                [NSGraphicsContext saveGraphicsState];
                NSString* taxonName = [p name];
                NSDictionary* attrs = [NSDictionary dictionaryWithObjects:[NSArray arrayWithObjects:[NSFont fontWithName:@"Chalkboard" size:fs], nil]
                                                                  forKeys:[NSArray arrayWithObjects:NSFontAttributeName, nil]];
                NSAttributedString* attrString = [[NSAttributedString alloc] initWithString:taxonName attributes:attrs];
                NSAffineTransform* xform = [NSAffineTransform transform];

                [xform translateXBy:drawPt.x yBy:drawPt.y];
                [xform rotateByDegrees:90.0];
                [xform translateXBy:-drawPt.x yBy:-drawPt.y];
                [xform concat];
                
                [attrString drawAtPoint:drawPt];

                [NSGraphicsContext restoreGraphicsState];
                }
			}
		if (drawAsMonophyleticIngroup == YES)
            {
			NSPoint a, b;
            Node* p = [t getRoot];
            a.x = xOffset + treeArea.size.width * [[p descendantIndexed:0] x];
			a.y = yOffset;
            b.x = xOffset + treeArea.size.width * [p x];
			b.y = yOffset;
			[NSBezierPath strokeLineFromPoint:a toPoint:b];
            }
        }
}

@end
