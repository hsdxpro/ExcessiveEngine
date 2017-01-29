#pragma once

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define DEAD_END_STACK_SIZE 128
#define DEAD_END_STACK_MASK (DEAD_END_STACK_SIZE - 1)

// The sizeofthese data types control the memory usage
typedef uint8_t AdjacencyType;
#define MAX_ADJACENCY UINT8_MAX


typedef size_t VertexIndexType;
typedef size_t TriangleIndexType;
typedef size_t ArrayIndexType;

/*
typedef uint16_t VertexIndexType;
typedef uint16_t TriangleIndexType;
typedef uint16_t ArrayIndexType;
*/

#define ISEMITTED(x)(emitted[(x) >> 3]  & (1<<(x & 7)))
#define SETEMITTED(x)(emitted[(x) >> 3] |= (1<<(x & 7)))

// Find the next non − local vertex to continue from
int skipDeadEnd(const AdjacencyType* liveTriangles, const VertexIndexType* deadEndStack, int& deadEndStackPos, int& deadEndStackStart, int nVertices, int& i);

// Find the next vertex to continue from
int getNextVertex(int nVertices, int& i, int k, const VertexIndexType* nextCandidates, int numNextCandidates, const ArrayIndexType* cacheTime, int s, const AdjacencyType* liveTriangles, const VertexIndexType* deadEndStack, int& deadEndStackPos, int& deadEndStackStart);

// The main reordering function
VertexIndexType* tipsify(const VertexIndexType* indices, int nTriangles, int nVertices, int k);