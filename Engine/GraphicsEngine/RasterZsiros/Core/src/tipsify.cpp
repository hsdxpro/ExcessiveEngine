#include "tipsify.h"

// Find the next non − local vertex to continue from
int skipDeadEnd(const AdjacencyType* liveTriangles, const VertexIndexType* deadEndStack, int& deadEndStackPos, int& deadEndStackStart, int nVertices, int& i)
{
	// Next in dead − endstack
	while ((deadEndStackPos&  DEAD_END_STACK_MASK) != deadEndStackStart)
	{
		int d = deadEndStack[(--deadEndStackPos)&  DEAD_END_STACK_MASK];

		// Check for live triangles
		if (liveTriangles[d] > 0)
			return d;
	}

	// Next in ManagerInput order
	while (i + 1 < nVertices)
	{
		// Cursor sweeps list only once
		i++;

		// Check for live triangles
		if (liveTriangles[i] > 0)
			return i;
	}

	// We are done !
	return -1;
}


// Find the next vertex to continue from
int getNextVertex(int nVertices, int& i, int k, const VertexIndexType* nextCandidates, int numNextCandidates, const ArrayIndexType* cacheTime, int s, const AdjacencyType* liveTriangles, const VertexIndexType* deadEndStack, int& deadEndStackPos, int& deadEndStackStart)
{
	// Best candidate
	int n = -1;

	// and priority
	int m = -1;

	for (int j = 0; j < numNextCandidates; j++)
	{
		int v = nextCandidates[j];
		// Must have live triangles

		if (liveTriangles[v] > 0)
		{
			// Initial priority
			int p = 0;

			// In cache even after fanning ?
			if (s - cacheTime[v] + 2 * liveTriangles[v] <= k)
				// Priority is position in cache
				p = s - cacheTime[v];

			// Keep best candidate
			if (p > m)
			{
				m = p;
				n = v;
			}
		}
	}

	// Reached a dead − end?
	if (n == -1)
	{
		// Get non − local vertex
		n = skipDeadEnd(liveTriangles, deadEndStack, deadEndStackPos, deadEndStackStart, nVertices, i);
	}

	return n;
}

// The main reordering function
VertexIndexType* tipsify(const VertexIndexType* indices, int nTriangles, int nVertices, int k)
{
	// Vertex − triangle adjacency

	// Count the occur rances of each vertex
	AdjacencyType* numOccurrances = new AdjacencyType[nVertices];
	memset(numOccurrances, 0, sizeof (AdjacencyType)* nVertices);
	for (int i = 0; i < 3 * nTriangles; i++)
	{
		int v = indices[i];
		if (numOccurrances[v] == MAX_ADJACENCY)
		{
			// Unsupported mesh
			// vertex shared by too many triangles
			delete[] numOccurrances;
			return nullptr;
		}

		numOccurrances[v]++;
	}

	// Find the offsets into the adjacency array for each vertex
	int sum = 0;
	ArrayIndexType* offsets = new ArrayIndexType[nVertices + 1];

	int maxAdjacency = 0;
	for (int i = 0; i < nVertices; i++)
	{
		offsets[i] = sum;
		sum += numOccurrances[i];

		if (numOccurrances[i] > maxAdjacency)
			maxAdjacency = numOccurrances[i];

		numOccurrances[i] = 0;
	}

	offsets[nVertices] = sum;

	// Add the triangle indices to the vertices it refers to
	TriangleIndexType* adjacency = new TriangleIndexType[3 * nTriangles];

	for (int i = 0; i < nTriangles; i++)
	{
		const VertexIndexType* vptr = &indices[3 * i];

		adjacency[offsets[vptr[0]] + numOccurrances[vptr[0]]] = i;
		numOccurrances[vptr[0]]++;

		adjacency[offsets[vptr[1]] + numOccurrances[vptr[1]]] = i;
		numOccurrances[vptr[1]]++;

		adjacency[offsets[vptr[2]] + numOccurrances[vptr[2]]] = i;
		numOccurrances[vptr[2]]++;
	}

	// Per − vertex live triangle counts
	AdjacencyType* liveTriangles = numOccurrances;

	// Per − vertex caching time stamps
	ArrayIndexType* cacheTime = new ArrayIndexType[nVertices];
	memset(cacheTime, 0, sizeof(ArrayIndexType)* nVertices);

	// Dead − end vertex stack
	VertexIndexType* deadEndStack = new VertexIndexType[DEAD_END_STACK_SIZE];
	memset(deadEndStack, 0, sizeof(VertexIndexType)* DEAD_END_STACK_SIZE);
	int deadEndStackPos = 0;
	int deadEndStackStart = 0;

	// Per triangle emitted flag
	uint8_t* emitted = new uint8_t[(nTriangles + 7) / 8];
	memset(emitted, 0, sizeof (uint8_t)* ((nTriangles + 7) / 8));

	// Empty output buffer
	TriangleIndexType* outputTriangles = new TriangleIndexType[nTriangles];

	int outputPos = 0;

	// Arbitrary starting vertex
	int f = 0;
	// Time stamp and cursor
	int s = k + 1;
	int i = 0;

	VertexIndexType* nextCandidates = new VertexIndexType[3 * maxAdjacency];

	// For all valid fanning vertices
	while (f >= 0)
	{
		// 1 − ring of next candidates
		int numNextCandidates = 0;
		int startOffset = offsets[f];
		int endOffset = offsets[f + 1];

		for (int offset = startOffset; offset < endOffset; offset++)
		{
			int t = adjacency[offset];
			if (!ISEMITTED(t))
			{
				const VertexIndexType* vptr = &indices[3 * t];
				// Output triangle

				outputTriangles[outputPos++] = t;

				for (int j = 0; j < 3; j++)
				{
					int v = vptr[j];

					// Add to dead − end stack
					deadEndStack[(deadEndStackPos++)&  DEAD_END_STACK_MASK] = v;

					if ((deadEndStackPos&  DEAD_END_STACK_MASK) == (deadEndStackStart&  DEAD_END_STACK_MASK))
						deadEndStackStart = (deadEndStackStart + 1)&  DEAD_END_STACK_MASK;

					// Register as candidate
					nextCandidates[numNextCandidates++] = v;

					// Decrease live triangle count
					liveTriangles[v]--;

					// If not in cache
					if (s - cacheTime[v] > k)
					{
						// Set time stamp
						cacheTime[v] = s;

						// Increment time stamp
						s++;
					}
				}

				// Flag triangle as emitted
				SETEMITTED(t);
			}
		}

		// Select next fanning vertex
		f = getNextVertex(nVertices, i, k, nextCandidates, numNextCandidates, cacheTime, s, liveTriangles, deadEndStack, deadEndStackPos, deadEndStackStart);
	}

	// Clean up
	delete[] nextCandidates;
	delete[] emitted;
	delete[] deadEndStack;
	delete[] cacheTime;
	delete[] adjacency;
	delete[] offsets;
	delete[] numOccurrances;

	// Convert the triangle index array into a full triangle list
	VertexIndexType* outputIndices = new VertexIndexType[3 * nTriangles];
	outputPos = 0;

	for (int i = 0; i < nTriangles; i++)
	{
		int t = outputTriangles[i];
		for (int j = 0; j < 3; j++)
		{
			int v = indices[3 * t + j];
			outputIndices[outputPos++] = v;
		}
	}
	delete[] outputTriangles;
	return outputIndices;
}