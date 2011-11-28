#import <Cocoa/Cocoa.h>
@class RbDataCell;



@interface RbTaxonData : NSObject <NSCoding> {

	NSMutableArray*      data;
    int                  dataType;
	NSString*            taxonName;
	int                  numCharacters;
}

@property (readwrite) int dataType;
@property (readwrite,retain) NSString* taxonName;
@property (readwrite) int numCharacters;

- (RbDataCell*)dataCellIndexed:(int)idx;
- (id)initWithRbTaxonData:(RbTaxonData*)d;
- (void)addObservation:(RbDataCell*)dc;

@end
