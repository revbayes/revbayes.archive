#import "ParmDraw.h"



@implementation ParmDraw

@synthesize drawingTip;
@synthesize drawingType;
@synthesize subscriptPosition;

- (void)addElementWithCode:(int)c andSize:(float)s atPosition:(NSPoint)p withItalics:(BOOL)it whichIsASuperscript:(BOOL)sc {
    
    [drawingCodes addObject:[NSNumber numberWithInt:c]];
    [drawingSizes addObject:[NSNumber numberWithFloat:s]];
    [drawingPositions addObject:[NSValue valueWithPoint:p]];
    [drawingItalics addObject:[NSNumber numberWithBool:it]];
    [drawingSuperscripts addObject:[NSNumber numberWithBool:sc]];
}

- (int)getCodeForElement:(int)idx {

    return [[drawingCodes objectAtIndex:idx] intValue];
}

- (BOOL)getItalicsForElement:(int)idx {

    return [[drawingItalics objectAtIndex:idx] boolValue];
}

- (BOOL)getIsSuperscriptForElement:(int)idx {

    return [[drawingSuperscripts objectAtIndex:idx] boolValue];
}

- (float)getSizeForElement:(int)idx {

    return [[drawingSizes objectAtIndex:idx] floatValue];
}

- (NSPoint)getPositionForElement:(int)idx withSize:(float)s {

    NSPoint p = [[drawingPositions objectAtIndex:idx] pointValue];
    p.x *= s;
    p.y *= s;
    return p;
}

- (id)init {
    
    if ( (self = [super init]) ) 
        {
        drawingCodes        = [[NSMutableArray alloc] init];
        drawingPositions    = [[NSMutableArray alloc] init];
        drawingSizes        = [[NSMutableArray alloc] init];
        drawingItalics      = [[NSMutableArray alloc] init];
        drawingSuperscripts = [[NSMutableArray alloc] init];
        drawingType         = [[NSString alloc] init];
        drawingTip          = [[NSString alloc] init];
        subscriptPosition   = NSMakeRect(0.0, 0.0, 0.2, 0.2);
        }
    
    return self;
}

- (int)numDrawElements {

    return (int)[drawingSizes count];
}

@end
