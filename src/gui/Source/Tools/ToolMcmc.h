#import <Cocoa/Cocoa.h>
#import "Tool.h"
@class SettingsMcmc;
@class WindowControllerMcmc;



@interface ToolMcmc : Tool <NSCoding> {

    SettingsMcmc*           settings;
    WindowControllerMcmc*   controlWindow;
}

- (void)closeControlPanel;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (void)execute;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (NSMutableAttributedString*)sendTip;
- (void)setInletsAndOutlets;
- (void)showControlPanel;

@end
