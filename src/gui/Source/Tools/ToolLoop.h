#import <Cocoa/Cocoa.h>
#import "Tool.h"
@class WindowControllerLoop;



@interface ToolLoop : Tool <NSCoding> {

	NSRect                    loopRect;
	NSSize                    minLoopSize;
	NSRange                   italicsRange;
    
	char                      indexLetter;
	int                       indexSource;
	int                       indexValue;
	int                       numElements;

    char                      startingIndexLetter;
    int                       startingIndexSource;
	int                       startingIndexValue;
    int                       startingNumElements;

    WindowControllerLoop*     controlWindow;
}

@property (nonatomic) char    indexLetter;
@property (nonatomic) int     indexValue;
@property (nonatomic) NSRect  loopRect;
@property (nonatomic) int     numElements;
@property (nonatomic) NSSize  minLoopSize;
@property (nonatomic) int     indexSource;
@property (nonatomic) NSRange italicsRange;

- (void)closeControlPanel;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (BOOL)execute;
- (NSString*)getEndingRangeForLoop;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (NSMutableAttributedString*)sendTip;
- (void)showControlPanel;

@end
