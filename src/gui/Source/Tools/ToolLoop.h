#import <Cocoa/Cocoa.h>
#import "Tool.h"
@class WindowControllerLoop;



@interface ToolLoop : Tool <NSCoding> {

	NSRect                    loopRect;
	NSRange                   italicsRange;
	char                      indexLetter;
	int                       indexUpperLimit;
    NSSize                    minLoopSize;

    WindowControllerLoop*     controlWindow;
}

@property (nonatomic) char    indexLetter;
@property (nonatomic) int     indexValue;
@property (nonatomic) NSRect  loopRect;
@property (nonatomic) int     indexUpperLimit;
@property (nonatomic) NSRange italicsRange;
@property (nonatomic) NSSize  minLoopSize;

- (void)closeControlPanel;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (BOOL)execute;
- (NSString*)getEndingRangeForLoop;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (NSMutableAttributedString*)sendTip;
- (void)showControlPanel;

@end
