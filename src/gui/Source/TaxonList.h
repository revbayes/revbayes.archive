#import <Cocoa/Cocoa.h>



@interface TaxonList : NSObject {

	NSString*        taxonName;
}

- (NSString*)taxonName;
- (void)setTaxonName:(NSString*)aName;

@end
