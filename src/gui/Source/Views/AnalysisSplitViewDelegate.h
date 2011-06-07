#import <Cocoa/Cocoa.h>



@interface AnalysisSplitViewDelegate : NSObject <NSSplitViewDelegate> {

	NSMutableDictionary *lengthsByViewIndex;
	NSMutableDictionary *viewIndicesByPriority;
}

- (void)setMinimumLength:(CGFloat)minLength forViewAtIndex:(NSInteger)viewIndex;
- (void)setPriority:(NSInteger)priorityIndex forViewAtIndex:(NSInteger)viewIndex;

@end
