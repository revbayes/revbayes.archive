#import "ModelView.h"
#import "PaletteDrawer.h"
#import "PaletteView.h"
#import "ParmDraw.h"
#import "ParmTree.h"
#import "Variable.h"
#import "WindowControllerModel.h"


@implementation PaletteView

- (BOOL)acceptsFirstResponder {

    [[self window] makeFirstResponder:self];
    return YES;
}

- (void)addTrackingAreas {
    
    NSRect r;
    r.size = NSMakeSize( parmSize, parmSize );

    NSMutableArray* parms = [myController variables];
    for (size_t i=0; i<[parms count]; i++)
        {
        Variable* var = [parms objectAtIndex:i];
        r.origin = [var frame].origin;
        [self transformToBottomLeftCoordinates:&(r.origin)];

        NSTrackingArea* ta = [[NSTrackingArea alloc] initWithRect:r
                                                          options:(NSTrackingMouseEnteredAndExited | NSTrackingActiveInActiveApp)
                                                            owner:self
                                                         userInfo:nil];
        [self addTrackingArea:ta];
        [myTrackingAreas addObject:ta];
        }
}

- (void)awakeFromNib {
    
    // number of rows and columns
    NSMutableArray* parms = [myController variables];
    numVariables = (int)[parms count];
    int nCols = 3;
    int nRows = ((int)[parms count] / 3);
    if (nRows % 3 != 0)
        nRows++;
    
    // get the size of the variables
	NSRect visible = [self visibleRect];
    parmSize = (visible.size.width - (nCols+1) * parmSpacing) / nCols;

    // get the size of the frame
    float w = nCols * parmSize + (nCols+1) * parmSpacing;
    float h = nRows * parmSize + (nRows+1) * parmSpacing;
    [self setFrame:NSMakeRect(0.0, 0.0, w, h)];

    // set the subviews
    size_t r = 0, c = 0;
    for (size_t i=0; i<[parms count]; i++)
        {
        c = i % nCols;
        r = i / nCols;
        NSPoint p = NSMakePoint( parmSpacing + c*(parmSpacing+parmSize), (r+1)*(parmSpacing+parmSize) );
        [self transformToBottomLeftCoordinates:&p];
            
        Variable* var = [parms objectAtIndex:i];
        [var setFrame:NSMakeRect(p.x, p.y, parmSize, parmSize)];
        [self addSubview:var];
        }
    
    [self updateViewFrame];
}

- (id)initWithFrame:(NSRect)frame {
    
    if ( (self = [super initWithFrame:frame]) ) 
        {
        numVariables         = 10;
        numConstants         = 10;
        numPlates            = 2;
        numFunctions         = 1;
        numDisplayColumns    = 3;
        parmSize             = 100.0;
        parmSpacing          = 8.0;
		toolTipTimer         = nil;
		isToolTipTimerActive = NO;
        variableForTip       = nil;
        isOverParameterImage = NO;
        [self initializeUnicode];
        
        parmIdOffsets[0] = 0;
        parmIdOffsets[1] = parmIdOffsets[0] + numVariables;
        parmIdOffsets[2] = parmIdOffsets[1] + numConstants;
        parmIdOffsets[3] = parmIdOffsets[2] + numPlates;

        // register this view as drag source for an image
        [self registerForDraggedTypes:[NSImage imageTypes]];
                
		// allocate the tool tracking rects
		myTrackingAreas = [[NSMutableArray alloc] init];

        // register as an observer of window resize events
        NSNotificationCenter *defaultCenter = [NSNotificationCenter defaultCenter];		 
		[defaultCenter addObserver:self
						  selector:@selector(windowDidResize:)
							  name:@"NSWindowDidResizeNotification"
							object:nil];
}
    
    return self;
}

- (void)drawParameterIndexed:(int)idx andWithAddress:(Parm*)pPtr inRect:(NSRect)r {
    
    // NOTE!!! : r is assumed to be square

    // allocate a shadow object
    NSShadow* shadow = [[NSShadow alloc] init];
    [shadow setShadowOffset:NSMakeSize(3.0, -2.0)];
    [shadow setShadowBlurRadius:10.0];

    float borderWidth = r.size.width * 0.01;
    
    // get a pointer to the draw element
    ParmDraw* pd = [drawElements objectAtIndex:idx];

    if ( [[pd drawingType] isEqualToString:@"Plate"] == YES || [[pd drawingType] isEqualToString:@"Tree Plate"] == YES )
        {
        // draw a plate 
        
        // draw the rectangular background
        [[NSColor lightGrayColor] set];
        NSBezierPath* bp = [NSBezierPath bezierPath];
        float rad = 6.0 * (r.size.width/ITEM_IMAGE_SIZE);
        [bp appendBezierPathWithRoundedRect:r xRadius:rad yRadius:rad];
        [bp fill];
        
        // draw the black line around the circle
        [[NSColor blackColor] set];
        [NSBezierPath setDefaultLineWidth:(0.02 * r.size.width)];
        [bp stroke];
        
        if ( [[pd drawingType] isEqualToString:@"Tree Plate"] == YES )
            {
            float w = r.size.width * 0.8;
            float h = w * 2.0 / 3.0;
            NSRect tRect = NSMakeRect(r.origin.x+r.size.width*0.1, r.origin.y+(r.size.height-h)*0.5, w, h);
			[[[NSColor blackColor] colorWithAlphaComponent:0.25] set];
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
            [tPath setLineWidth:(0.06 * r.size.width)];
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
        }
    else 
        {
        // draw a circular parameter
        
        // draw the circle background
        [NSGraphicsContext saveGraphicsState];
        if (pPtr != nil)
            [shadow set];
        [[NSColor lightGrayColor] set];
        [[NSBezierPath bezierPathWithOvalInRect:r] fill];
        [NSGraphicsContext restoreGraphicsState];
            
        // draw stripes indicating that the parameter is a constant
        BOOL drawStripes = NO;
        if (pPtr == nil)
            {
            if ([self parmId] == 1)
                drawStripes = YES;
            }
        else
            {
            if ([pPtr isConstant] == YES)
                drawStripes = YES;
            }
        if ( drawStripes == YES )
            {
            // draw light diagonal lines for constant parameters
            int nlines = 15;
            float inc = r.size.width / nlines;
            float rad = r.size.width * 0.5;
            float h = -sqrtf(2.0) * rad;
            while (h < sqrtf(2.0)*rad)
                {
                h += inc;
                float insideBit = 4.0 * h * h - 8.0 * (h*h - rad*rad);
                if (insideBit < 0.0)
                    break;
                float sx = sqrtf( insideBit );
                float x1 = -(2.0 * h + sx) / 4.0;
                float x2 = -(2.0 * h - sx) / 4.0;
                float y1, y2;
                if (h < -rad)
                    {
                    y1 = -sqrtf( rad*rad - x1*x1 );
                    y2 = -sqrtf( rad*rad - x2*x2 );
                    }
                else if (h > rad)
                    {
                    y1 = sqrtf( rad*rad - x1*x1 );
                    y2 = sqrtf( rad*rad - x2*x2 );
                    }
                else
                    {
                    y1 = -sqrtf( rad*rad - x1*x1 );
                    y2 = sqrtf( rad*rad - x2*x2 );
                    }
                
                NSPoint c;
                c.x = NSMidX(r);
                c.y = NSMidY(r);
                NSPoint a = NSMakePoint(x1+c.x, y1+c.y);
                NSPoint b = NSMakePoint(x2+c.x, y2+c.y);
                NSBezierPath* hl = [NSBezierPath bezierPath];
                [hl moveToPoint:a];
                [hl lineToPoint:b];
                [[NSColor blackColor] set];
                [hl setLineWidth:0.25];
                [hl stroke];
                }
            }
        
        // draw the black line around the circle
        [[NSColor blackColor] set];
        [NSBezierPath setDefaultLineWidth:borderWidth];
        [[NSBezierPath bezierPathWithOvalInRect:r] stroke];
        
        // draw the separator line
        NSPoint lp1 = NSMakePoint(r.origin.x + 0.1*r.size.width, r.origin.y + 0.33*r.size.width);
        NSPoint lp2 = NSMakePoint(r.origin.x + 0.9*r.size.width, r.origin.y + 0.33*r.size.width);
        NSBezierPath* separatorLine = [NSBezierPath bezierPath];
        [separatorLine setLineCapStyle:NSRoundLineCapStyle];
        [separatorLine moveToPoint:lp1];
        [separatorLine lineToPoint:lp2];
        [separatorLine setLineWidth:borderWidth];
        [[NSColor blackColor] set];
        [separatorLine stroke];
        
        // draw the text on the parameter
        if ( [[pd drawingType] isEqualToString:@"Tree Topology Random Variable"] == YES || [[pd drawingType] isEqualToString:@"Tree Topology Constant"] )
            {
            // draw a tree
            BOOL isRooted = NO;
            if ( pPtr != nil )
                {
                if ( [(ParmTree*)pPtr rootednessType] == ROOTED_TREE )
                    isRooted = YES;
                }
            
            if (isRooted == NO)
                {
                // draw an unrooted tree
                NSPoint a = NSMakePoint(0.0, 0.0);
                NSPoint b = NSMakePoint(0.5, 0.5);
                NSPoint c = NSMakePoint(0.0, 1.0);
                NSPoint d = NSMakePoint(1.0, 0.5);
                NSPoint e = NSMakePoint(1.5, 1.0);
                NSPoint f = NSMakePoint(1.5, 0.0);
                
                a.x *= 0.35 * r.size.width;
                b.x *= 0.35 * r.size.width;
                c.x *= 0.35 * r.size.width;
                d.x *= 0.35 * r.size.width;
                e.x *= 0.35 * r.size.width;
                f.x *= 0.35 * r.size.width;
                a.y *= 0.35 * r.size.width;
                b.y *= 0.35 * r.size.width;
                c.y *= 0.35 * r.size.width;
                d.y *= 0.35 * r.size.width;
                e.y *= 0.35 * r.size.width;
                f.y *= 0.35 * r.size.width;
                
                float w = f.x - a.x;
                float xOffset = (r.size.width - w) * 0.5;
                
                a.x += r.origin.x + xOffset;
                b.x += r.origin.x + xOffset;
                c.x += r.origin.x + xOffset;
                d.x += r.origin.x + xOffset;
                e.x += r.origin.x + xOffset;
                f.x += r.origin.x + xOffset;
                a.y += r.origin.y + 0.40 * r.size.width;
                b.y += r.origin.y + 0.40 * r.size.width;
                c.y += r.origin.y + 0.40 * r.size.width;
                d.y += r.origin.y + 0.40 * r.size.width;
                e.y += r.origin.y + 0.40 * r.size.width;
                f.y += r.origin.y + 0.40 * r.size.width;
                
                NSBezierPath* treePath = [NSBezierPath bezierPath];
                [treePath setLineWidth:(0.02 * r.size.width)];
                [treePath setLineCapStyle:NSRoundLineCapStyle];
                [treePath moveToPoint:a];
                [treePath lineToPoint:b];
                [treePath moveToPoint:b];
                [treePath lineToPoint:c];
                [treePath moveToPoint:b];
                [treePath lineToPoint:d];
                [treePath moveToPoint:d];
                [treePath lineToPoint:e];
                [treePath moveToPoint:d];
                [treePath lineToPoint:f];
                [treePath stroke];
                }
            else
                {
                // draw a rooted tree
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
                    tp[i].x *= 0.35 * r.size.width;
                    tp[i].y *= 0.35 * r.size.width;
                    float w = 1.5 * 0.35 * r.size.width;
                    float xOffset = (r.size.width - w) * 0.5;
                    tp[i].x += r.origin.x + xOffset;
                    tp[i].y += r.origin.y + 0.4 * r.size.width;
                    }
                NSBezierPath* treePath = [NSBezierPath bezierPath];
                [treePath setLineWidth:(0.02 * r.size.width)];
                [treePath setLineCapStyle:NSRoundLineCapStyle];
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
            }
        else
            {
            float lftShiftAmount = 0.0;
            for (int n=0; n<[pd numDrawElements]; n++)
                {
                if ( [pd getIsSuperscriptForElement:n] == YES && pPtr != nil )
                    {
                    int numDigitsInSuperscript = (int)(log((double)[pPtr dimensions]) / log(10.0)) + 1;
                    lftShiftAmount += numDigitsInSuperscript * 0.03 * r.size.width;
                    break;
                    }
                }
            
            for (int n=0; n<[pd numDrawElements]; n++)
                {
                unichar c = [pd getCodeForElement:n];
                float   s = [pd getSizeForElement:n];
                NSPoint p = [pd getPositionForElement:n withSize:r.size.width];
                NSString* parmStr;
                if (pPtr != nil && [pd getIsSuperscriptForElement:n] == YES)
                    parmStr = [NSString stringWithFormat:@"%d", [pPtr dimensions]];
                else
                    parmStr = [NSString stringWithFormat:@"%C", c];
                NSDictionary* attrs = [NSDictionary dictionaryWithObjects:[NSArray arrayWithObjects:[NSFont fontWithName:@"Times" size:(1.0*s*r.size.width)], [[NSColor blackColor] colorWithAlphaComponent:1.0], nil] forKeys:[NSArray arrayWithObjects:NSFontAttributeName,NSForegroundColorAttributeName, nil]];
                NSMutableAttributedString* attrString = [[NSMutableAttributedString alloc] initWithString:parmStr attributes:attrs];
                if ( [pd getItalicsForElement:n] == YES && !(pPtr != nil && [pd getIsSuperscriptForElement:n] == YES) )
                    [attrString applyFontTraits:NSItalicFontMask range:NSMakeRange(0,1)];
                p.x += r.origin.x - lftShiftAmount;
                p.y += r.origin.y;
                [attrString drawAtPoint:p];
                }
            }
                                    
        }
}

- (void)drawRect:(NSRect)dirtyRect {
    
    
	// set the background of the analysis window
	NSRect bounds = [self bounds];
	[[NSColor whiteColor] set];
	[NSBezierPath fillRect:bounds];
    
    return;
    
#   if 0
    
    // draw the parameters
    int parmId = [self parmId];
    int nParms = [self numParms];
    NSRect r;
    r.size = NSMakeSize( parmSize, parmSize );
    for (int i=1, k=0; i<=[self numRows]; i++)
        {
        for (int j=0; j<[self numColumns]; j++)
            {
            // find the lower left corner of the parameter and transform it to the view's coordinate system
            r.origin = NSMakePoint( j*(parmSize+parmSpacing)+parmSpacing, i*(parmSize+parmSpacing) );
            [self transformToBottomLeftCoordinates:&(r.origin)];
            
            // draw the parameter
            [self drawParameterIndexed:(parmIdOffsets[parmId]+k) andWithAddress:nil inRect:r];
            
            k++;
            if (k == nParms)
                break;
            }
        if (k == nParms)
            break;
        }
    
    // draw the tool tip
	if (isToolTipTimerActive == YES && variableForTip != nil)
		{
        ParmDraw* pd = [drawElements objectAtIndex:(parmIdOffsets[parmId] + toolForToolTip)];

        int r = toolForToolTip / numDisplayColumns;
        int c = toolForToolTip % numDisplayColumns;
		NSPoint p;
		p.x = (c  ) * (parmSize+parmSpacing) + parmSpacing;
		p.y = (r+1) * (parmSize+parmSpacing) - parmSpacing;

        [self transformToBottomLeftCoordinates:&p];
        
		NSString* myTip = [NSString stringWithString:[pd drawingTip]];
		NSDictionary *attr = [NSDictionary 
                              dictionaryWithObjects:[NSArray arrayWithObjects:[NSFont fontWithName:@"Lucida Grande Bold" size:14.0], [[NSColor whiteColor] colorWithAlphaComponent:1.0], nil] 
                              forKeys: [NSArray arrayWithObjects:NSFontAttributeName,NSForegroundColorAttributeName, nil]];
		NSMutableAttributedString* attrString = [[NSMutableAttributedString alloc] initWithString:myTip attributes:attr];
		NSRect tipRect = [attrString boundingRectWithSize:NSMakeSize(1e10, 1e10) options:(NSStringDrawingUsesLineFragmentOrigin)];
		tipRect.origin = p;
        if (tipRect.origin.x + tipRect.size.width > bounds.size.width)
            {
            tipRect.origin.x = bounds.size.width - tipRect.size.width - parmSpacing;
            p = tipRect.origin;
            }
        
		[[[NSColor darkGrayColor] colorWithAlphaComponent:0.75] set];
		[NSBezierPath fillRect:tipRect];
		[attrString drawAtPoint:p];
		}
#   endif
}

- (ParmDraw*)getParmElementIndexed:(int)idx {

    return [drawElements objectAtIndex:idx];
}

- (int)indexOfParameterWithName:(NSString*)pName {

	NSEnumerator* enumerator = [drawElements objectEnumerator];
	id element;
    int i = 0;
	while ( (element = [enumerator nextObject]) )
        {
        if ( [[element drawingType] isEqualToString:pName] == YES )
            return i;
        i++;
        }
    return -1;
}

- (void)initializeUnicode {
    
    // initialize the random variables **************************
    
    drawElements = [[NSMutableArray alloc] init];
    
    // real number    
    ParmDraw* pd = [[ParmDraw alloc] init];
    [pd setDrawingType:@"Real Number Random Variable"];
    [pd setDrawingTip:@"Real number random variable"];
    [pd setSubscriptPosition:NSMakeRect(0.70, 0.35, 0.20, 0.20)];
    [pd addElementWithCode:0x0052 andSize:0.50 atPosition:NSMakePoint(0.35, 0.30) withItalics:NO whichIsASuperscript:NO];
    [drawElements addObject:pd];

    // positive real number
    pd = [[ParmDraw alloc] init];
    [pd setDrawingType:@"Positive Real Number Random Variable"];
    NSString* msg = [NSString stringWithFormat:@"Positive real number random\nvariable (x %C 0)", 0x2265];
    [pd setDrawingTip:msg];
    [pd setSubscriptPosition:NSMakeRect(0.70, 0.35, 0.20, 0.20)];
    [pd addElementWithCode:0x0052 andSize:0.50 atPosition:NSMakePoint(0.35, 0.30) withItalics:NO whichIsASuperscript:NO];
    [pd addElementWithCode:0x002B andSize:0.25 atPosition:NSMakePoint(0.22, 0.45) withItalics:NO whichIsASuperscript:NO];
    [drawElements addObject:pd];

    // integer
    pd = [[ParmDraw alloc] init];
    [pd setDrawingType:@"Integer Number Random Variable"];
    [pd setDrawingTip:@"Integer random variable"];
    [pd setSubscriptPosition:NSMakeRect(0.67, 0.35, 0.20, 0.20)];
    [pd addElementWithCode:0x005A andSize:0.5 atPosition:NSMakePoint(0.35, 0.30) withItalics:NO whichIsASuperscript:NO];
    [drawElements addObject:pd];

    // natural numbers (0, 1, 2, ...)
    pd = [[ParmDraw alloc] init];
    [pd setDrawingType:@"Natural Number Random Variable"];
    [pd setDrawingTip:@"Natural number random var-\niable (0, 1, 2, ...)"];
    [pd setSubscriptPosition:NSMakeRect(0.65, 0.35, 0.20, 0.20)];
    [pd addElementWithCode:0x004E andSize:0.5 atPosition:NSMakePoint(0.32, 0.30) withItalics:NO whichIsASuperscript:NO];
    [drawElements addObject:pd];

    // vector of real numbers
    pd = [[ParmDraw alloc] init];
    [pd setDrawingType:@"Vector of Real Number Random Variables"];
    [pd setDrawingTip:@"A vector of real numbers"];
    [pd setSubscriptPosition:NSMakeRect(0.70, 0.35, 0.20, 0.20)];
    [pd addElementWithCode:0x0052 andSize:0.50 atPosition:NSMakePoint(0.35, 0.30) withItalics:NO  whichIsASuperscript:NO];
    [pd addElementWithCode:0x006E andSize:0.20 atPosition:NSMakePoint(0.64, 0.65) withItalics:YES whichIsASuperscript:YES];
    [drawElements addObject:pd];

    // vector of positive real numbers
    pd = [[ParmDraw alloc] init];
    [pd setDrawingType:@"Vector of Positive Real Number Random Variables"];
    [pd setDrawingTip:@"A vector of positive\nreal numbers"];
    [pd setSubscriptPosition:NSMakeRect(0.70, 0.35, 0.20, 0.20)];
    [pd addElementWithCode:0x0052 andSize:0.50 atPosition:NSMakePoint(0.35, 0.30) withItalics:NO  whichIsASuperscript:NO];
    [pd addElementWithCode:0x002B andSize:0.25 atPosition:NSMakePoint(0.22, 0.45) withItalics:NO  whichIsASuperscript:NO];
    [pd addElementWithCode:0x006E andSize:0.20 atPosition:NSMakePoint(0.64, 0.65) withItalics:YES whichIsASuperscript:YES];
    [drawElements addObject:pd];
    
    // vector of integers
    pd = [[ParmDraw alloc] init];
    [pd setDrawingType:@"Vector of Integer Number Random Variables"];
    [pd setDrawingTip:@"A vector of integers"];
    [pd setSubscriptPosition:NSMakeRect(0.70, 0.35, 0.20, 0.20)];
    [pd addElementWithCode:0x005A andSize:0.50 atPosition:NSMakePoint(0.35, 0.30) withItalics:NO  whichIsASuperscript:NO];
    [pd addElementWithCode:0x006E andSize:0.20 atPosition:NSMakePoint(0.66, 0.65) withItalics:YES whichIsASuperscript:YES];
    [drawElements addObject:pd];
    
    // vector of natural numbers (0, 1, 2, ...)
    pd = [[ParmDraw alloc] init];
    [pd setDrawingType:@"Vector of Natural Number Random Variables"];
    [pd setDrawingTip:@"A vector of natural numbers"];
    [pd setSubscriptPosition:NSMakeRect(0.70, 0.35, 0.20, 0.20)];
    [pd addElementWithCode:0x004E andSize:0.50 atPosition:NSMakePoint(0.32, 0.30) withItalics:NO  whichIsASuperscript:NO];
    [pd addElementWithCode:0x006E andSize:0.20 atPosition:NSMakePoint(0.68, 0.65) withItalics:YES whichIsASuperscript:YES];
    [drawElements addObject:pd];

    // simplex
    pd = [[ParmDraw alloc] init];
    [pd setDrawingType:@"Simplex Random Variable"];
    [pd setDrawingTip:@"A simplex (a vector of\npositive real numbers\nconstrained to sum to one)"];
    [pd setSubscriptPosition:NSMakeRect(0.70, 0.35, 0.20, 0.20)];
    [pd addElementWithCode:0x0053 andSize:0.50 atPosition:NSMakePoint(0.35, 0.30) withItalics:NO  whichIsASuperscript:NO];
    [pd addElementWithCode:0x006E andSize:0.20 atPosition:NSMakePoint(0.61, 0.65) withItalics:YES whichIsASuperscript:YES];
    [drawElements addObject:pd];

    // tree
    pd = [[ParmDraw alloc] init];
    [pd setDrawingType:@"Tree Topology Random Variable"];
    [pd setDrawingTip:@"A tree topology"];
    [pd setSubscriptPosition:NSMakeRect(0.70, 0.45, 0.20, 0.20)];
    [pd addElementWithCode:0x03C4 andSize:0.7 atPosition:NSMakePoint(0.40, 0.30) withItalics:NO whichIsASuperscript:NO];
    [drawElements addObject:pd];

    // initialize the constant variables **************************

    // real number    
    pd = [[ParmDraw alloc] init];
    [pd setDrawingType:@"Real Number Constant"];
    [pd setDrawingTip:@"Real number constant"];
    [pd setSubscriptPosition:NSMakeRect(0.70, 0.35, 0.20, 0.20)];
    [pd addElementWithCode:0x0052 andSize:0.50 atPosition:NSMakePoint(0.35, 0.30) withItalics:NO whichIsASuperscript:NO];
    [drawElements addObject:pd];
    
    // positive real number
    pd = [[ParmDraw alloc] init];
    [pd setDrawingType:@"Positive Real Number Constant"];
    msg = [NSString stringWithFormat:@"Positive real number\nconstant (x %C 0)", 0x2265];
    [pd setDrawingTip:msg];
    [pd setSubscriptPosition:NSMakeRect(0.70, 0.35, 0.20, 0.20)];
    [pd addElementWithCode:0x0052 andSize:0.50 atPosition:NSMakePoint(0.35, 0.30) withItalics:NO whichIsASuperscript:NO];
    [pd addElementWithCode:0x002B andSize:0.25 atPosition:NSMakePoint(0.22, 0.45) withItalics:NO whichIsASuperscript:NO];
    [drawElements addObject:pd];
    
    // integer
    pd = [[ParmDraw alloc] init];
    [pd setDrawingType:@"Integer Number Constant"];
    [pd setDrawingTip:@"Integer constant"];
    [pd setSubscriptPosition:NSMakeRect(0.70, 0.35, 0.20, 0.20)];
    [pd addElementWithCode:0x005A andSize:0.5 atPosition:NSMakePoint(0.35, 0.30) withItalics:NO whichIsASuperscript:NO];
    [drawElements addObject:pd];
    
    // natural numbers (0, 1, 2, ...)
    pd = [[ParmDraw alloc] init];
    [pd setDrawingType:@"Natural Number Constant"];
    [pd setDrawingTip:@"Natural number constant\n(0, 1, 2, ...)"];
    [pd setSubscriptPosition:NSMakeRect(0.70, 0.35, 0.20, 0.20)];
    [pd addElementWithCode:0x004E andSize:0.5 atPosition:NSMakePoint(0.32, 0.30) withItalics:NO whichIsASuperscript:NO];
    [drawElements addObject:pd];
    
    // vector of real numbers
    pd = [[ParmDraw alloc] init];
    [pd setDrawingType:@"Vector of Real Number Constants"];
    [pd setDrawingTip:@"A vector of real\nnumber constants"];
    [pd setSubscriptPosition:NSMakeRect(0.70, 0.35, 0.20, 0.20)];
    [pd addElementWithCode:0x0052 andSize:0.50 atPosition:NSMakePoint(0.35, 0.30) withItalics:NO  whichIsASuperscript:NO];
    [pd addElementWithCode:0x006E andSize:0.20 atPosition:NSMakePoint(0.64, 0.65) withItalics:YES whichIsASuperscript:YES];
    [drawElements addObject:pd];

    // vector of positive real numbers
    pd = [[ParmDraw alloc] init];
    [pd setDrawingType:@"Vector of Positive Real Number Constants"];
    [pd setDrawingTip:@"A vector of positive\nreal number constants"];
    [pd setSubscriptPosition:NSMakeRect(0.70, 0.35, 0.20, 0.20)];
    [pd addElementWithCode:0x0052 andSize:0.50 atPosition:NSMakePoint(0.35, 0.30) withItalics:NO  whichIsASuperscript:NO];
    [pd addElementWithCode:0x002B andSize:0.25 atPosition:NSMakePoint(0.22, 0.45) withItalics:NO  whichIsASuperscript:NO];
    [pd addElementWithCode:0x006E andSize:0.20 atPosition:NSMakePoint(0.64, 0.65) withItalics:YES whichIsASuperscript:YES];
    [drawElements addObject:pd];
    
    // vector of integers
    pd = [[ParmDraw alloc] init];
    [pd setDrawingType:@"Vector of Integer Number Constants"];
    [pd setDrawingTip:@"A vector of integer constants"];
    [pd setSubscriptPosition:NSMakeRect(0.70, 0.35, 0.20, 0.20)];
    [pd addElementWithCode:0x005A andSize:0.50 atPosition:NSMakePoint(0.35, 0.30) withItalics:NO  whichIsASuperscript:NO];
    [pd addElementWithCode:0x006E andSize:0.20 atPosition:NSMakePoint(0.66, 0.65) withItalics:YES whichIsASuperscript:YES];
    [drawElements addObject:pd];
    
    // vector of natural numbers (0, 1, 2, ...)
    pd = [[ParmDraw alloc] init];
    [pd setDrawingType:@"Vector of Natural Number Constants"];
    [pd setDrawingTip:@"A vector of natural\nnumber constants"];
    [pd setSubscriptPosition:NSMakeRect(0.70, 0.35, 0.20, 0.20)];
    [pd addElementWithCode:0x004E andSize:0.50 atPosition:NSMakePoint(0.32, 0.30) withItalics:NO  whichIsASuperscript:NO];
    [pd addElementWithCode:0x006E andSize:0.20 atPosition:NSMakePoint(0.68, 0.65) withItalics:YES whichIsASuperscript:YES];
    [drawElements addObject:pd];
    
    // simplex
    pd = [[ParmDraw alloc] init];
    [pd setDrawingType:@"Simplex Constant"];
    [pd setDrawingTip:@"A simplex constant (a\nvector of positive real\nnumbers constrained to\nsum to one)"];
    [pd setSubscriptPosition:NSMakeRect(0.70, 0.35, 0.20, 0.20)];
    [pd addElementWithCode:0x0053 andSize:0.50 atPosition:NSMakePoint(0.35, 0.30) withItalics:NO  whichIsASuperscript:NO];
    [pd addElementWithCode:0x006E andSize:0.20 atPosition:NSMakePoint(0.61, 0.65) withItalics:YES whichIsASuperscript:YES];
    [drawElements addObject:pd];
    
    // tree
    pd = [[ParmDraw alloc] init];
    [pd setDrawingType:@"Tree Topology Constant"];
    [pd setDrawingTip:@"A tree topology constant"];
    [pd setSubscriptPosition:NSMakeRect(0.70, 0.35, 0.20, 0.20)];
    [pd addElementWithCode:0x03C4 andSize:0.7 atPosition:NSMakePoint(0.40, 0.30) withItalics:NO whichIsASuperscript:NO];
    [drawElements addObject:pd];

    // initialize the plates **************************
    
    // normal plate
    pd = [[ParmDraw alloc] init];
    [pd setDrawingType:@"Plate"];
    [pd setDrawingTip:@"A plate object denoting\nreplication"];
    [pd addElementWithCode:0x03C4 andSize:0.7 atPosition:NSMakePoint(0.40, 0.30) withItalics:NO whichIsASuperscript:NO];
    [drawElements addObject:pd];

    // tree plate
    pd = [[ParmDraw alloc] init];
    [pd setDrawingType:@"Tree Plate"];
    [pd setDrawingTip:@"A tree plate object denoting\na set of taxon bipartitions"];
    [pd addElementWithCode:0x03C4 andSize:0.7 atPosition:NSMakePoint(0.40, 0.30) withItalics:NO whichIsASuperscript:NO];
    [drawElements addObject:pd];

    // initialize the functions **************************

    // rate matrix
    pd = [[ParmDraw alloc] init];
    [pd setDrawingType:@"Rate Matrix Function"];
    [pd setDrawingTip:@"The rate matrix for a\ncontinuous-time Markov model"];
    [pd setSubscriptPosition:NSMakeRect(0.70, 0.35, 0.20, 0.20)];
    [pd addElementWithCode:0x0051 andSize:0.5 atPosition:NSMakePoint(0.33, 0.33) withItalics:NO whichIsASuperscript:NO];
    [drawElements addObject:pd];

}

- (int)numColumns {

    return numDisplayColumns;
}

- (int)numParms {
    
    int nParms = 0;
    if ( [[drawerPtr titleOfSelectedPaletteSet] isEqualToString:@"Variables"] == YES )
        nParms = numVariables;
    else if ( [[drawerPtr titleOfSelectedPaletteSet] isEqualToString:@"Constants"] == YES )
        nParms = numConstants;
    else if ( [[drawerPtr titleOfSelectedPaletteSet] isEqualToString:@"Plates"] == YES )
        nParms = numPlates;
    else if ( [[drawerPtr titleOfSelectedPaletteSet] isEqualToString:@"Functions"] == YES )
        nParms = numFunctions;
    return nParms;
}

- (int)numRows {
    
    int nParms = [self numParms];
    int nRows = (nParms / numDisplayColumns);
    if ( nParms % numDisplayColumns != 0 )
        nRows++;
    return nRows;
}

- (void)mouseDown:(NSEvent*)event {
    
    //mouseDownEvent = event;
}

- (void)mouseDragged:(NSEvent*)event {
    
    NSPoint down = [mouseDownEvent locationInWindow];
    NSPoint drag = [event locationInWindow];
    float distance = hypot(down.x-drag.x, down.y-drag.y);
    if ( distance < 3.0 )
        return;
    
    // kill the timer
    [self stopToolTipTimer];
    
    if (isOverParameterImage == NO)
        return;

    // get the location of the mouseDown event
    NSPoint p = [self convertPoint:down fromView:nil];

#   if 0
    // TEMP
    // create the image that will be dragged
    NSSize s = NSMakeSize(parmSize, parmSize);
    NSImage* anImage = [[NSImage alloc] initWithSize:s];
    [anImage lockFocus];
    [self drawParameterIndexed:(parmIdOffsets[[self parmId]] + potentialToolForToolTip) andWithAddress:nil inRect:NSMakeRect(parmSize*0.025, parmSize*0.025, parmSize*0.95, parmSize*0.95)];
    [anImage unlockFocus];
    
    // drag from the center of the image
    p.x -= s.width/2.0;
    p.y -= s.height/2.0;
    
    // get the pasteboard
    NSPasteboard* pb = [NSPasteboard pasteboardWithName:NSDragPboard];
    
    // put the image on the pasteboard
    [pb declareTypes:[NSArray arrayWithObject: NSTIFFPboardType] owner:self];
    [pb addTypes:[NSArray arrayWithObject:NSPDFPboardType] owner:self];
    
    // start the drag
	[self dragImage:anImage 
	             at:p 
			 offset:NSMakeSize(0.0, 0.0) 
			  event:mouseDownEvent 
		 pasteboard:pb 
		     source:self 
		  slideBack:NO];
#   endif
}

- (void)mouseEntered:(NSEvent*)event {
    
    NSPoint p = [event locationInWindow];
    p = [self convertPoint:p fromView:nil];
    [self transformToTopLeftCoordinates:&p];
    
    potentialVariable = nil;
    for (Variable* var in [myController variables])
        {
        NSRect r = [var frame];
        if ( NSPointInRect( p, r ) == YES )
            {
            potentialVariable = var;
            break;
            }
        }

	[self startToolTipTimer];
    isOverParameterImage = YES;
}

- (void)mouseExited:(NSEvent*)event {
	
    isOverParameterImage = NO;
	[self stopToolTipTimer];
}

- (int)parmId {
    
    int parmId = 0;
    if ( [[drawerPtr titleOfSelectedPaletteSet] isEqualToString:@"Variables"] == YES )
        parmId = 0;
    else if ( [[drawerPtr titleOfSelectedPaletteSet] isEqualToString:@"Constants"] == YES )
        parmId = 1;
    else if ( [[drawerPtr titleOfSelectedPaletteSet] isEqualToString:@"Plates"] == YES )
        parmId = 2;
    else if ( [[drawerPtr titleOfSelectedPaletteSet] isEqualToString:@"Functions"] == YES )
        parmId = 3;
    return parmId;
}

- (void)removeAllTrackingAreas {
    
	// remove the tracking areas from the view
	NSArray* viewsTrackingAreas = [self trackingAreas];
	NSEnumerator* trackingEnum = [viewsTrackingAreas objectEnumerator];
	id ta;
	while ( (ta = [trackingEnum nextObject]) )
		[self removeTrackingArea:ta];
    
	// and empty the dictionary linking tools with tracking areas
	[myTrackingAreas removeAllObjects];
}

- (PaletteView*)sendMyPointer {

    return self;
}

- (void)setViewBounds {
    
    // get the drawing height
    int nRows = [self numRows];
    float h = nRows * (parmSize+parmSpacing) + parmSpacing;
    
    // get the size of the visible rectangle
    NSRect vRect = [self visibleRect];
    
    // get the frame
    NSRect fRect = [self frame];
    
    if ( h > vRect.size.height )
        fRect.size.height = h;
    else
        fRect.size.height = vRect.size.height;
    [self setFrame:fRect];
    
    [self setNeedsDisplay:YES];
}

- (void)startToolTipTimer {
    
    [self stopToolTipTimer];
    
    toolTipTimer = [NSTimer scheduledTimerWithTimeInterval:1.0 target:self selector:@selector(toolTipFire) userInfo:nil repeats:YES];
    isToolTipTimerActive = YES;
    fireNum = 0;
}

- (void)stopToolTipTimer {
    
    if (isToolTipTimerActive == YES)
        {
        [toolTipTimer invalidate];
        isToolTipTimerActive = NO;
        toolTipTimer = nil;
        variableForTip = nil;
        }
	[self setNeedsDisplay:YES];
}

- (void)toolTipFire {
    
    fireNum++;
    if (fireNum == 1)
        {
		variableForTip = potentialVariable;
        if (variableForTip != nil)
            NSLog(@"tip for tool %@", [variableForTip name]);
        [self setNeedsDisplay:YES];
        }
    else if (fireNum == 6)
        {
        [self stopToolTipTimer];
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

- (void)updateViewFrame {

    NSMutableArray* parms = [myController variables];
    numVariables = (int)[parms count];
    int nCols = 3;
    int nRows = ((int)[parms count] / 3);
    if (nRows % 3 != 0)
        nRows++;
    float w = nCols * parmSize + (nCols+1) * parmSpacing;
    float h = nRows * parmSize + (nRows+1) * parmSpacing;
	NSRect visible = [self visibleRect];
    NSRect newFrame = visible;
	if ( visible.size.width < w )
		newFrame.size.width = w;
	if ( visible.size.height < h )
		newFrame.size.height = h;
	[self setFrame:newFrame];

    
    // update the tracking areas
    [self removeAllTrackingAreas];
    [self addTrackingAreas];
    [self setNeedsDisplay:YES];
}

- (BOOL)validateMenuItem:(NSMenuItem*)item {
	
	SEL act = [item action];
	if ( act == @selector(showParameterPalette:) )
        {
        return YES;
        }

    return YES;
}

- (NSString*)whichParmSelected {

    if (variableForTip != nil)
        return [variableForTip name];
    return @"";
}

- (void)windowDidResize:(NSNotification*)notification {

    if ( [[[notification object] className] isEqualToString:@"NSDrawerWindow"] )
        [self updateViewFrame];
}

@end
