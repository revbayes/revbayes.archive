#import "CarouselTreeView.h"
#import "GuiTree.h"
#import "Node.h"
#import "WindowControllerTreeViewer.h"




@implementation CarouselTreeView

@synthesize drawAsMonophyleticIngroup;

- (void)drawRect:(NSRect)dirtyRect {

    [super drawRect:dirtyRect];

    // get the bounds of the view
	NSRect bounds = [self bounds];

    // paste the image into the background
    //NSImage* image = [NSImage imageNamed:@"page240x500.png"];
    NSImage* image = [NSImage imageNamed:@"page300x625.png"];
    NSRect imageRect;
    imageRect.origin = NSZeroPoint;
    imageRect.size = [image size];
    [image drawInRect:bounds fromRect:imageRect operation:NSCompositeSourceOver fraction:1.0];
    
    // draw the tree
    if (myTree != nil)
        {
        // tree initialization
        [myTree initializeDownPassSequence];
        [myTree setCoordinates:drawAsMonophyleticIngroup];
        
        // choose font size
        float fs = [self chooseFontSizeForWidth:(0.84 * bounds.size.width) andNumberOfLabels:[myTree numberOfTaxa]];
        
        // choose the line width
        float lineWidth = [self chooseLineWithForNumberOfTaxa:[myTree numberOfTaxa]];
		[NSBezierPath setDefaultLineWidth:lineWidth];
    
        // find the maximum size of a name
        NSDictionary* attrs = [NSDictionary dictionaryWithObjects:[NSArray arrayWithObjects:[NSFont fontWithName:@"Chalkboard" size:fs], [[NSColor blackColor] colorWithAlphaComponent:1.0], nil]
                                                  forKeys:[NSArray arrayWithObjects:NSFontAttributeName,NSForegroundColorAttributeName, nil]];
        NSRect biggestNameRect = NSZeroRect;
        for (int i=0; i<[myTree numberOfNodes]; i++)
            {
            Node* p = [myTree downPassNodeIndexed:i];
            if ([p isLeaf] == YES)
                {
                NSString* taxonName = [p name];
                NSAttributedString* attrString = [[NSAttributedString alloc] initWithString:taxonName attributes:attrs];
                NSRect textSize = [attrString boundingRectWithSize:NSMakeSize(1e10, 1e10) options:NSStringDrawingUsesLineFragmentOrigin];
                if (textSize.size.width > biggestNameRect.size.width)
                    biggestNameRect.size.width = textSize.size.width;
                if (textSize.size.height > biggestNameRect.size.height)
                    biggestNameRect.size.height = textSize.size.height;
                }
            }
        float gap = biggestNameRect.size.height * 0.5;

        // get a rectangle for the printable tree area (240 x 500 -> 201 x 266 or 300 x 625 -> 252 x 331) 0.84 & 0.53
        float printableW = 0.84 * bounds.size.width;
        float printableH = 0.53 * bounds.size.height;
        float bottomBuffer = 25.0;
        float sideMargin = printableW * 0.04;
        NSRect drawableArea = NSMakeRect( (bounds.size.width-printableW)*0.5, (bounds.size.height-printableH)*0.5, printableW, printableH);
        NSRect treeArea = drawableArea;
        treeArea.origin.x += sideMargin;  // was 5.0
        treeArea.origin.y += (5.0 + bottomBuffer);
        treeArea.size.width -= 2.0 * sideMargin;  // was 10.0
        treeArea.size.height -= (10.0 + bottomBuffer + biggestNameRect.size.width + gap);
        double xOffset = treeArea.origin.x - bounds.origin.x;
        double yOffset = treeArea.origin.y - bounds.origin.y;
        NSRect labelArea = treeArea;
        labelArea.origin.y += treeArea.size.height + gap;
        labelArea.size.height = biggestNameRect.size.width;
        

        // draw the tree
        for (int n=0; n<[myTree numberOfNodes]; n++)
			{
            Node* p = [myTree downPassNodeIndexed:n];
            
            // draw vertical lines
			NSPoint a, b;
			a.x = xOffset + treeArea.size.width * [p x];
			a.y = yOffset + treeArea.size.height * [p y];
            if ([myTree isRoot:p] == YES)
                {
                b.x = xOffset + treeArea.size.width * [p x];
                b.y = yOffset;
                }
            else
                {
                b.x = xOffset + treeArea.size.width * [p x];
                b.y = yOffset + treeArea.size.height * [[p ancestor] y];
                if ([myTree isRoot:[p ancestor]] == YES && drawAsMonophyleticIngroup == YES && p == [[p ancestor] descendantIndexed:0])
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
                    if ( !(drawAsMonophyleticIngroup == YES && [[q name] isEqualToString:[myTree outgroupName]] == YES) )
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
                drawPt.x += biggestNameRect.size.height * 0.5;

                [NSGraphicsContext saveGraphicsState];
                NSString* taxonNameInit = [p name];
                NSString* taxonName = [taxonNameInit stringByReplacingOccurrencesOfString:@"_" withString:@" "];
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
            Node* p = [myTree getRoot];
            a.x = xOffset + treeArea.size.width * [[p descendantIndexed:0] x];
			a.y = yOffset;
            b.x = xOffset + treeArea.size.width * [p x];
			b.y = yOffset;
			[NSBezierPath strokeLineFromPoint:a toPoint:b];
            }
        
        
        // set the label for the tree, if there is information
        //[treeInfo setStringValue:[myTree info]];
        }
}

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

- (float)chooseLineWithForNumberOfTaxa:(int)n {

    if (n < 50)
        return 1.0;
    else if (n < 75)
        return 0.8;
    else if (n < 100)
        return 0.6;
    else if (n < 125)
        return 0.4;
    else if (n < 150)
        return 0.2;
    return 0.1;
}

- (id)initWithFrame:(NSRect)frame andTree:(GuiTree*)t andController:(WindowControllerTreeViewer*)wc {

    if ( (self = [super initWithFrame:frame]) ) 
        {
        myTree = t;
        myWindowController = wc;
        [self setDrawAsMonophyleticIngroup:[myWindowController drawMonophyleticWrOutgroup]];
        }
    return self;
}

@end
