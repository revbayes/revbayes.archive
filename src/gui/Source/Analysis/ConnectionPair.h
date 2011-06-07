#import <Cocoa/Cocoa.h>
@class InOutlet;



@interface ConnectionPair : NSObject {

	InOutlet*                 item1;
	InOutlet*                 item2;
}

@property (readwrite,assign) InOutlet* item1;
@property (readwrite,assign) InOutlet* item2;

@end
