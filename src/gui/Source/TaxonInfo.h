#import <Cocoa/Cocoa.h>



@interface TaxonInfo : NSObject {

	NSString*        taxonName;
	NSString*        taxonData;
}

- (NSString*)taxonName;
- (void)setTaxonName:(NSString*)aName;
- (NSString*)taxonData;
- (void)setTaxonData:(NSString*)aData;

@end
