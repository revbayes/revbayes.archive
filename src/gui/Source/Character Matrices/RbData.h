#import <Cocoa/Cocoa.h>
@class RbDataCell;



@interface RbData : NSObject <NSCoding> {

	NSMutableArray*      data;
    int                  dataType;
	NSString*            name;
	int                  numCharacters;
	int                  numTaxa;
	NSMutableArray*      taxonNames;
	NSMutableSet*        excludedTaxa;
	NSMutableSet*        excludedCharacters;
}

@property (readwrite) int dataType;
@property (readwrite,retain) NSString* name;
@property (readwrite) int numCharacters;
@property (readwrite) int numTaxa;

- (void)addCell:(RbDataCell*)cell;
- (void)addTaxonName:(NSString*)n;
- (RbDataCell*)cellWithRow:(int)r andColumn:(int)c;
- (RbDataCell*)dataCellIndexed:(int)idx;
- (int)dataSize;
- (void)excludeTaxonIndexed:(int)idx;
- (void)excludeCharacterIndexed:(int)idx;
- (void)includeAllCharacters;
- (void)includeAllTaxa;
- (int)indexOfTaxonNamed:(NSString*)nme;
- (id)initWithRbData:(RbData*)d;
- (BOOL)isCharacterExcluded:(int)idx;
- (BOOL)isTaxonExcluded:(int)idx;
- (BOOL)isCharacterMissAmbig:(int)idx;
- (int)numExcludedCharacters;
- (int)numExcludedTaxa;
- (void)print;
- (void)restoreTaxonIndexed:(int)idx;
- (void)restoreCharacterIndexed:(int)idx;
- (NSString*)taxonWithIndex:(int)i;

@end
