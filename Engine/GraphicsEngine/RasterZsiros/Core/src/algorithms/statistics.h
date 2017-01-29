////////////////////////////////////////////////////////////////////////////////
//	file: statistics.h
//	content: math functions to compute statistics such as avarage, sum, etc.
//
////////////////////////////////////////////////////////////////////////////////


// avarage function
template <class T, class R>
R average(T* list, size_t len, size_t treshold=128) {
	// brute-force compute
	if (len<=treshold) {
		T sum = (T)0;
		for (size_t i=0; i<len; i++) {
			sum += list[i];
		}
		return R(sum)/R(treshold);
	}
	// divide into two subgroups
	size_t len1 = len/2;
	size_t len2 = len-len1;

	R weight1 = R(len1)/R(len);
	R weight2 = R(len2)/R(len);

	return weight1*average<T,R>(list,len1,treshold) + weight2*average<T,R>(list+len1,len2,treshold);
}