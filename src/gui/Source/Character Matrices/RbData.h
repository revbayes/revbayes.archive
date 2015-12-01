#import <Cocoa/Cocoa.h>
@class RbDataCell;
@class RbTaxonData;



@interface RbData : NSObject <NSCoding> {

	NSMutableArray*                    data;
    int                                dataType;
    BOOL                               isHomologyEstablished;
    NSString*                          alignmentMethod;
	NSString*                          name;
	NSString*                          stateLabels;
	int                                numCharacters;
	int                                numTaxa;
	NSMutableArray*                    taxonNames;
	NSMutableSet*                      excludedTaxa;
	NSMutableSet*                      excludedCharacters;
    RbData*                            copiedFrom;
}

@property (nonatomic)        int       dataType;
@property (nonatomic)        BOOL      isHomologyEstablished;
@property (nonatomic,strong) NSString* name;
@property (nonatomic,strong) NSString* alignmentMethod;
@property (nonatomic,strong) NSString* stateLabels;
@property (nonatomic)        int       numCharacters;
@property (nonatomic)        int       numTaxa;

- (void)addTaxonData:(RbTaxonData*)td;
- (void)addTaxonName:(NSString*)n;
- (RbDataCell*)cellWithRow:(size_t)r andColumn:(int)c;
- (char)stateWithRow:(size_t)r andColumn:(int)c;
- (void)cleanName:(NSString*)nameStr;
- (void)clear;
- (RbData*)copiedFrom;
- (int)dataSize;
- (void)deleteLastTaxon;
- (void)excludeTaxonIndexed:(int)idx;
- (void)excludeCharacterIndexed:(int)idx;
- (RbTaxonData*)getDataForTaxonIndexed:(int)idx;
- (RbTaxonData*)getDataForTaxonWithName:(NSString*)ns;
- (void)includeAllCharacters;
- (void)includeAllTaxa;
- (int)indexOfTaxonNamed:(NSString*)nme;
- (id)initWithRbData:(RbData*)d;
- (BOOL)isCharacterExcluded:(int)idx;
- (BOOL)isTaxonExcluded:(int)idx;
- (BOOL)isCharacterMissAmbig:(int)idx;
- (BOOL)isTaxonNamePresent:(NSString*)theName;
- (int)maxNumCharacters;
- (int)numExcludedCharacters;
- (int)numExcludedTaxa;
- (int)numCharactersForTaxon:(int)idx;
- (void)print;
- (void)restoreTaxonIndexed:(int)idx;
- (void)restoreCharacterIndexed:(int)idx;
- (void)setCopiedFrom:(RbData*)d;
- (void)setNameOfTaxonWithIndex:(size_t)idx to:(NSString*)newName;
- (void)setStandardMatrixToHave:(int)rows andToHave:(int)columns;
- (void)writeToFile:(NSString*)fn;
- (NSString*)taxonWithIndex:(int)i;

@end
