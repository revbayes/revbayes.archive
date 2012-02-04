#import <Cocoa/Cocoa.h>
#import "Tool.h"
@class WindowControllerLoop;



@interface ToolLoop : Tool <NSCoding> {

	NSRect                   loopRect;
	NSSize                   minLoopSize;
	NSRange                  italicsRange;
    
	char                     indexLetter;
	int                      indexSource;
	int                      indexValue;
	int                      numElements;

    char                     startingIndexLetter;
    int                      startingIndexSource;
	int                      startingIndexValue;
    int                      startingNumElements;

    WindowControllerLoop*    controlWindow;
}

@property (readwrite) char    indexLetter;
@property (readwrite) int     indexValue;
@property (readwrite) NSRect  loopRect;
@property (readwrite) int     numElements;
@property (readwrite) NSSize  minLoopSize;
@property (readwrite) int     indexSource;
@property (readwrite) NSRange italicsRange;

- (void)closeControlPanel;
- (void)encodeWithCoder:(NSCoder *)aCoder;
- (void)execute;
- (NSString*)getEndingRangeForLoop;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (NSMutableAttributedString*)sendTip;
- (void)showControlPanel;

@end
