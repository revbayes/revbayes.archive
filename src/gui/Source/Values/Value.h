#import <Cocoa/Cocoa.h>



@interface Value : NSObject <NSCoding> {

}

- (Value*)clone;
- (id)initWithValue:(Value*)v;
- (NSString*)stringRepresentation;

@end
