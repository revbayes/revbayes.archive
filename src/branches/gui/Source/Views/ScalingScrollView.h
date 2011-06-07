#import <Cocoa/Cocoa.h>
 


@interface ScalingScrollView : NSScrollView {

    NSPopUpButton*    _scalePopUpButton;
    float             scaleFactor;
}
 
- (void)scalePopUpAction:(id)sender;
- (void)setScaleFactor:(float)factor adjustPopup:(BOOL)flag;
- (float)scaleFactor;
 
@end
