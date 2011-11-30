#import <Cocoa/Cocoa.h>
@class RbDataCell;



@interface RbTaxonData : NSObject <NSCoding> {

	NSMutableArray*      data;
    int                  dataType;
	NSString*            taxonName;
}

@property (readwrite) int dataType;
@property (readwrite,retain) NSString* taxonName;

- (RbDataCell*)dataCellIndexed:(int)idx;
- (id)initWithRbTaxonData:(RbTaxonData*)d;
- (void)addObservation:(RbDataCell*)dc;
- (int)numCharacters;

@end
