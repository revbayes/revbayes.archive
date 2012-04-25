#import "Node.h"
#import "Tree.h"
#import "TreeSetView.h"
#import "WindowControllerTreeSet.h"




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
        NSLog(@"drawing tree %@", t);
        
        // initialize coordinates
        [t setCoordinates];

        // draw the tree
		[NSBezierPath setDefaultLineWidth:1.0];
        [[NSColor blackColor] set];
		double factor = 0.95;
		double xOffset = bounds.size.width * 0.025;
		double yOffset = bounds.size.height * 0.025;

        NSLog(@"[t numberOfNodes] = %d", [t numberOfNodes]);
        for (int n=0; n<[t numberOfNodes]; n++)
			{
            Node* p = [t downPassNodeIndexed:n];
            NSLog(@"p = %@ (%f %f)", p, [p x], [p y]);
            
			/*NSPoint a, b;
			a.x = xOffset + bounds.size.width * factor * [p x];
			a.y = yOffset + bounds.size.height * factor * [p y];
			b.x = xOffset + bounds.size.width * factor * [p x];
			b.y = yOffset + bounds.size.height * factor * [[p ancestor] y];
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
				NSPoint w, z;
				w.x = xOffset + bounds.size.width * factor * [p x];
				z.x = w.x;
				w.y = bounds.size.height;
				z.y = (bounds.size.height - 10.0);
				[NSBezierPath strokeLineFromPoint:w toPoint:z];
				}*/
			}
		


        }
}

@end
