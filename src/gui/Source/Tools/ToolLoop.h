#import <Cocoa/Cocoa.h>
#import "Tool.h"
@class WindowControllerLoop;



@interface ToolLoop : Tool <NSCoding> {

	NSRect                    loopRect;
	NSRange                   italicsRange;
	char                      indexLetter;
	int                       indexUpperLimit;
    NSSize                    minLoopSize;
    int                       currentIndex;
    BOOL                      isExecuting;

    WindowControllerLoop*     controlWindow;
}

@property (nonatomic) char    indexLetter;
@property (nonatomic) int     indexValue;
@property (nonatomic) NSRect  loopRect;
@property (nonatomic) int     indexUpperLimit;
@property (nonatomic) NSRange italicsRange;
@property (nonatomic) NSSize  minLoopSize;
@property (nonatomic) int     currentIndex;
@property (nonatomic) BOOL    isExecuting;

- (void)chooseIndex;
- (void)closeControlPanel;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (BOOL)execute;
- (NSString*)getEndingRangeForLoop;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (NSMutableAttributedString*)sendTip;
- (void)showControlPanel;
- (NSMutableArray*)unavailableIndices;

@end
