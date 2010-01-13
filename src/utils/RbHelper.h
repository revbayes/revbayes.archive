#ifndef RbHelper_H
#define RbHelper_H

namespace RbHelper {

template<typename T>
T *** newThreeDArray(unsigned f , unsigned s , unsigned t);
template<typename T>
T ** newTwoDArray(unsigned f , unsigned s);
template<typename T>
void deleteThreeDArray(T ***& ptr);
template<typename T>
void deleteTwoDArray(T **& ptr);

template<typename T> T*** newThreeDArray(unsigned f , unsigned s , unsigned t) {

	assert(f > 0 && s > 0 && t> 0);
	const unsigned twoDStride = s*t;
	T ***ptr;
	ptr = new T **[f];
	ptr[0] = new T *[f * s];
	ptr[0][0] = new T[f * s * t];
	for (unsigned sIt = 1 ; sIt < s ; sIt++)
		ptr[0][sIt] = ptr[0][sIt-1] + t ;
	for (unsigned fIt = 1 ; fIt < f ; fIt ++)
		{
		ptr[fIt] = ptr[fIt -1] +  s ;
		ptr[fIt][0] = ptr[fIt -1][0] + twoDStride;
		for (unsigned sIt = 1 ; sIt < s ; sIt++)
			ptr[fIt][sIt] = ptr[fIt][sIt-1] + t ;
		}
	return ptr;
}

template<typename T> void deleteThreeDArray(T*** &ptr) {

	if (ptr)
		{
		if (*ptr)
			{
			delete [] **ptr;
			delete [] *ptr;
			}
		delete [] ptr;
		}
	ptr = NULL;
}

template<typename T> T** newTwoDArray(unsigned f , unsigned s) {

	assert(f > 0 && s > 0);
	T **ptr;
	ptr = new T *[f];
	*ptr = new T [f * s];
	for (unsigned fIt = 1 ; fIt < f ; fIt ++)
		ptr[fIt] = ptr[fIt -1] +  s ;
	return ptr;
}

template<typename T> inline void deleteTwoDArray(T** &ptr) {

	if (ptr)
		{
		delete [] *ptr;
		delete [] ptr;
		ptr = NULL;
		}
}

}

#endif
