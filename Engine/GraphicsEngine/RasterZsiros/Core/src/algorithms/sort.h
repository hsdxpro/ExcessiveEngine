////////////////////////////////////////////////////////////////////////////////
//	file: algorithm.h
//	content:
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <xfunctional>



////////////////////////////////////////////////////////////////////////////////
// general sorting algorithm


template<class T, class compare_less>
void merge_sort(T* pvData, int nItems) {
	static compare_less comp;
	// no items, no sorting
	if (nItems<=1)
		return;
	int itemSize = sizeof(T);
	// buffers
	T* pData = pvData;
	T* pTempBuffer = new T[nItems];		assert(pTempBuffer);
	T* targetBuffer=pData;
	T* srcBuffer=pTempBuffer;


	int subListSize = 1;
	int nSubLists = nItems/subListSize + (nItems%subListSize==0 ? 0:1);
	int nResultingSubLists = nSubLists/2;
	int nItemsProcessed=0;


	for (;    nSubLists != 1;    subListSize *= 2,  nSubLists = nItems/subListSize + (nItems%subListSize==0 ? 0:1)) {
		// swap buffers
		{
			T* tmp = targetBuffer;
			targetBuffer = srcBuffer;
			srcBuffer = tmp;
		}

		nItemsProcessed=0;
		// merge all sublists
		for (int subList=0; subList<nSubLists; subList+=2, nItemsProcessed+=subListSize*2) {

			int nItemsRemaining = nItems-nItemsProcessed;
			int size1 = subListSize < nItemsRemaining ? subListSize : nItemsRemaining, 
				size2 = nItemsRemaining-size1 >= subListSize ? subListSize : nItemsRemaining-size1;

			// merges 2 sublists
			T *src1=&srcBuffer[subList*subListSize], *src2=&srcBuffer[(subList+1)*subListSize], *dest=&targetBuffer[subList*subListSize];

			int idx1=0, idx2=0;
			int size_tot=size1+size2;

			for (int i=0; i<size_tot; i++) {
				if (idx1>=size1) {
					for (; idx2<size2; idx2++, i++) {
						memcpy(&dest[i], &src2[idx2], itemSize);
					}
					break;
				}
				else if (idx2>=size2) {
					for (; idx1<size1; idx1++, i++) {
						memcpy(&dest[i], &src1[idx1], itemSize);
					}
					break;
				}
				if (comp(src1[idx1], src2[idx2])) {
					memcpy(&dest[i], &src2[idx2], itemSize);
					idx2++;
				}
				else {
					memcpy(&dest[i], &src1[idx1], itemSize);
					idx1++;
				}
			}
		}
	}
	

	if (targetBuffer==pData) {
		delete[] pTempBuffer;
	}
	else {
		memcpy(pData, targetBuffer, nItems*itemSize);
		delete[] pTempBuffer;
	}
}


template <class T>
void merge_sort(T* pvData, int nItems) {
	merge_sort<T, std::less<T>>(pvData, nItems);
}