#import <Foundation/Foundation.h>



@interface ParmDraw : NSObject {

    NSMutableArray*             drawingCodes;
    NSMutableArray*             drawingPositions;
    NSMutableArray*             drawingSizes;
    NSMutableArray*             drawingItalics;
    NSMutableArray*             drawingSuperscripts;
    NSString*                   drawingType;
    NSString*                   drawingTip;
    NSRect                      subscriptPosition;
}

@property (readwrite,retain) NSString* drawingType;
@property (readwrite,retain) NSString* drawingTip;
@property (readwrite) NSRect subscriptPosition;

- (void)addElementWithCode:(int)c andSize:(float)s atPosition:(NSPoint)p withItalics:(BOOL)it whichIsASuperscript:(BOOL)sc;
- (int)getCodeForElement:(int)idx;
- (BOOL)getItalicsForElement:(int)idx;
- (BOOL)getIsSuperscriptForElement:(int)idx;
- (NSPoint)getPositionForElement:(int)idx withSize:(float)s;
- (float)getSizeForElement:(int)idx;
- (int)numDrawElements;

@end
