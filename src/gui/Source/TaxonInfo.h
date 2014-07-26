#import <Cocoa/Cocoa.h>



@interface TaxonInfo : NSObject {

	NSString*        taxonName;
	NSString*        taxonData;
}

@property (nonatomic,strong) NSString* taxonData;
@property (nonatomic,strong) NSString* taxonName;

@end
