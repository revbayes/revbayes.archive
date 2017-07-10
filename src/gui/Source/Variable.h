#import <Cocoa/Cocoa.h>



@interface Variable : NSView <NSCoding> {

    size_t              dimensions;
    NSString*           name;
    NSString*           interfaceName;
    NSString*           interfaceSymbol;
    float               parmSize;
}

@property (nonatomic,strong) NSString* name;
@property (nonatomic)        size_t    dimensions;
@property (nonatomic,strong) NSString* interfaceName;
@property (nonatomic,strong) NSString* interfaceSymbol;

- (void)print;

@end
