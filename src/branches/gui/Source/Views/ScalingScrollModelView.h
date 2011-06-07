#import <Cocoa/Cocoa.h>
 


@interface ScalingScrollModelView : NSScrollView {

    NSPopUpButton*    _scalePopUpButton;
    float             scaleFactor;
}
 
- (void)scalePopUpAction:(id)sender;
- (void)setScaleFactor:(float)factor adjustPopup:(BOOL)flag;
- (float)scaleFactor;
 
@end
