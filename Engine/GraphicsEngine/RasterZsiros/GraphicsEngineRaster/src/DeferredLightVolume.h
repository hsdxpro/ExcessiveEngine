////////////////////////////////////////////////////////////////////////////////
//	Zsiroskenyer Team
////////////////////////////////////////////////////////////////////////////////
//	Light volume geometries nicely hardcoded :)
//	UGLY UGLY UGLY UGLY ugly ugly ugly ugly ... ... ... ...
////////////////////////////////////////////////////////////////////////////////

#pragma warning(disable: 4305)

////////////////////////////////////////////////////////////////////////////////
// vertex buffers for light volumes

////////////////////////////////////////
// directional - fullscreen quad
static float vbDataLightDirectional[] = {
	// fullscreen quad
	-1.f,	-1.f,	1.f,	1,		0.0002f,	0.0002f, 0,	0,
	-1.f,	1.f,	1.f,	1,		0.0002f,	0.9998f, 0,	0,
	1.f,	1.f,	1.f,	1,		0.9998f,	0.9998f, 0,	0,

	-1.f,	-1.f,	1.f,	1,		0.0002f,	0.0002f, 0,	0,
	1.f,	-1.f,	1.f,	1,		0.9998f,	0.0002f, 0,	0,
	1.f,	1.f,	1.f,	1,		0.9998f,	0.9998f, 0,	0,
};
static unsigned ibDataLightDirectional[] = {
	1,0,2,	3,4,5,
	0,1,2,	4,3,5,
};


////////////////////////////////////////
// spot - cone
static float vbDataLightSpot[] = {
	0.000000,	1.100000,	0.000000,	1,		0/*base*/,			0	,			0, 0,
	0.000000,	0.000000,	0.000000,	1,		0/*base*/,			0	,			0, 0,
	1.100000,	1.100000,	0.000000,	1,		180	,				1.0	,			0, 0,
	0.777818,	1.100000,	-0.777818,	1,		-135,				1.0	,			0, 0,
	0.000000,	1.100000,	-1.100000,	1,		-90	,				1.0	,			0, 0,
	-0.777818,	1.100000,	-0.777818,	1,		-45	,				1.0	,			0, 0,
	-1.100000,	1.100000,	0.000000,	1,		0	,				1.0	,			0, 0,
	-0.777818,	1.100000,	0.777818,	1,		45	,				1.0	,			0, 0,
	0.000000,	1.100000,	1.100000,	1,		90	,				1.0	,			0, 0,
	0.777818,	1.100000,	0.777817,	1,		135	,				1.0	,			0, 0,
	0.583364,	1.100000,	-0.583363,	1,		-135,				0.75,			0, 0,
	0.388909,	1.100000,	-0.388909,	1,		-135,				0.50,			0, 0,
	0.194454,	1.100000,	-0.194454,	1,		-135,				0.15,			0, 0,
	0.275001,	1.100000,	0.000000,	1,		180	,				0.25,			0, 0,
	0.550000,	1.100000,	0.000000,	1,		180	,				0.50,			0, 0,
	0.825000,	1.100000,	0.000000,	1,		180	,				0.75,			0, 0,
	0.000000,	1.100000,	-0.825000,	1,		-90	,				0.75,			0, 0,
	0.000000,	1.100000,	-0.550000,	1,		-90	,				0.50,			0, 0,
	0.000000,	1.100000,	-0.275000,	1,		-90	,				0.25,			0, 0,
	-0.583364,	1.100000,	-0.583363,	1,		-45	,				0.75,			0, 0,
	-0.388909,	1.100000,	-0.388909,	1,		-45	,				0.50,			0, 0,
	-0.194454,	1.100000,	-0.194454,	1,		-45	,				0.25,			0, 0,
	-0.825000,	1.100000,	-0.000000,	1,		0	,				0.75,			0, 0,
	-0.550000,	1.100000,	-0.000000,	1,		0	,				0.50,			0, 0,
	-0.275001,	1.100000,	-0.000000,	1,		0	,				0.25,			0, 0,
	-0.583364,	1.100000,	0.583364,	1,		45	,				0.75,			0, 0,
	-0.388908,	1.100000,	0.388909,	1,		45	,				0.50,			0, 0,
	-0.194454,	1.100000,	0.194455,	1,		45	,				0.25,			0, 0,
	0.000000,	1.100000,	0.825000,	1,		90	,				0.75,			0, 0,
	0.000000,	1.100000,	0.550000,	1,		90	,				0.50,			0, 0,
	0.000000,	1.100000,	0.275000,	1,		90	,				0.25,			0, 0,
	0.583364,	1.100000,	0.583363,	1,		135	,				0.75,			0, 0,
	0.388909,	1.100000,	0.388909,	1,		135	,				0.50,			0, 0,
	0.194454,	1.100000,	0.194454,	1,		135	,				0.25,			0, 0
};
static unsigned ibDataLightSpot[] = {
	1,	3,	2,
	11,	14,	15,
	15,	10,	11,
	1,	4,	3,
	17,	11,	10,
	10,	16,	17,
	1,	5,	4,
	20,	17,	16,
	16,	19,	20,
	1,	6,	5,
	23,	20,	19,
	19,	22,	23,
	1,	7,	6,
	26,	23,	22,
	22,	25,	26,
	1,	8,	7,
	29,	26,	25,
	25,	28,	29,
	1,	9,	8,
	32,	29,	28,
	28,	31,	32,
	1,	2,	9,
	14,	32,	31,
	31,	15,	14,
	12,	0,	13,
	12,	13,	14,
	14,	11,	12,
	2,	3,	10,
	10,	15,	2,
	18,	0,	12,
	18,	12,	11,
	11,	17,	18,
	3,	4,	16,
	16,	10,	3,
	21,	0,	18,
	21,	18,	17,
	17,	20,	21,
	4,	5,	19,
	19,	16,	4,
	24,	0,	21,
	24,	21,	20,
	20,	23,	24,
	5,	6,	22,
	22,	19,	5,
	27,	0,	24,
	27,	24,	23,
	23,	26,	27,
	6,	7,	25,
	25,	22,	6,
	30,	0,	27,
	30,	27,	26,
	26,	29,	30,
	7,	8,	28,
	28,	25,	7,
	33,	0,	30,
	33,	30,	29,
	29,	32,	33,
	8,	9,	31,
	31,	28,	8,
	13,	0,	33,
	13,	33,	32,
	32,	14,	13,
	9,	2,	15,
	15,	31,	9
};


////////////////////////////////////////
// point - sphere
static float vbDataLightPoint[] = {
	0.000000,	0.000000,	0.540000,	1.f,	0.f, 0.f, 0.f, 0.f,
	0.482991,	0.000000,	0.241495,	1.f,	0.f, 0.f, 0.f, 0.f,
	0.149252,	0.459351,	0.241495,	1.f,	0.f, 0.f, 0.f, 0.f,
	-0.390748,	0.283895,	0.241495,	1.f,	0.f, 0.f, 0.f, 0.f,
	-0.390748,	-0.283895,	0.241495,	1.f,	0.f, 0.f, 0.f, 0.f,
	0.149252,	-0.459351,	0.241495,	1.f,	0.f, 0.f, 0.f, 0.f,
	0.390748,	0.283895,	-0.241495,	1.f,	0.f, 0.f, 0.f, 0.f,
	-0.149252,	0.459351,	-0.241495,	1.f,	0.f, 0.f, 0.f, 0.f,
	-0.482991,	-0.000000,	-0.241495,	1.f,	0.f, 0.f, 0.f, 0.f,
	-0.149252,	-0.459351,	-0.241495,	1.f,	0.f, 0.f, 0.f, 0.f,
	0.390748,	-0.283895,	-0.241495,	1.f,	0.f, 0.f, 0.f, 0.f,
	0.000000,	0.000000,	-0.540000,	1.f,	0.f, 0.f, 0.f, 0.f,
	0.283895,	0.000000,	0.459351,	1.f,	0.f, 0.f, 0.f, 0.f,
	0.087728,	0.270000,	0.459351,	1.f,	0.f, 0.f, 0.f, 0.f,
	-0.229676,	0.166869,	0.459351,	1.f,	0.f, 0.f, 0.f, 0.f,
	-0.229676,	-0.166869,	0.459351,	1.f,	0.f, 0.f, 0.f, 0.f,
	0.087728,	-0.270000,	0.459351,	1.f,	0.f, 0.f, 0.f, 0.f,
	0.371623,	0.270000,	0.283895,	1.f,	0.f, 0.f, 0.f, 0.f,
	-0.141947,	0.436869,	0.283895,	1.f,	0.f, 0.f, 0.f, 0.f,
	-0.459351,	-0.000000,	0.283895,	1.f,	0.f, 0.f, 0.f, 0.f,
	-0.141947,	-0.436869,	0.283895,	1.f,	0.f, 0.f, 0.f, 0.f,
	0.371623,	-0.270000,	0.283895,	1.f,	0.f, 0.f, 0.f, 0.f,
	0.513570,	0.166869,	0.000000,	1.f,	0.f, 0.f, 0.f, 0.f,
	-0.000000,	0.540000,	0.000000,	1.f,	0.f, 0.f, 0.f, 0.f,
	-0.513570,	0.166869,	0.000000,	1.f,	0.f, 0.f, 0.f, 0.f,
	-0.317404,	-0.436869,	0.000000,	1.f,	0.f, 0.f, 0.f, 0.f,
	0.317404,	-0.436869,	0.000000,	1.f,	0.f, 0.f, 0.f, 0.f,
	0.513571,	-0.166869,	0.000000,	1.f,	0.f, 0.f, 0.f, 0.f,
	0.317404,	0.436869,	0.000000,	1.f,	0.f, 0.f, 0.f, 0.f,
	-0.317404,	0.436869,	0.000000,	1.f,	0.f, 0.f, 0.f, 0.f,
	-0.513570,	-0.166869,	0.000000,	1.f,	0.f, 0.f, 0.f, 0.f,
	0.000000,	-0.540000,	0.000000,	1.f,	0.f, 0.f, 0.f, 0.f,
	0.141947,	0.436869,	-0.283895,	1.f,	0.f, 0.f, 0.f, 0.f,
	-0.371623,	0.270000,	-0.283895,	1.f,	0.f, 0.f, 0.f, 0.f,
	-0.371623,	-0.270000,	-0.283895,	1.f,	0.f, 0.f, 0.f, 0.f,
	0.141947,	-0.436869,	-0.283895,	1.f,	0.f, 0.f, 0.f, 0.f,
	0.459351,	0.000000,	-0.283895,	1.f,	0.f, 0.f, 0.f, 0.f,
	0.229676,	0.166869,	-0.459351,	1.f,	0.f, 0.f, 0.f, 0.f,
	-0.087728,	0.270000,	-0.459351,	1.f,	0.f, 0.f, 0.f, 0.f,
	-0.283895,	-0.000000,	-0.459351,	1.f,	0.f, 0.f, 0.f, 0.f,
	-0.087728,	-0.270000,	-0.459351,	1.f,	0.f, 0.f, 0.f, 0.f,
	0.229676,	-0.166869,	-0.459351,	1.f,	0.f, 0.f, 0.f, 0.f
};
static unsigned ibDataLightPoint[] = {
	0,	12,	13,
	12,	1,	17,
	12,	17,	13,
	13,	17,	2,
	0,	13,	14,
	13,	2,	18,
	13,	18,	14,
	14,	18,	3,
	0,	14,	15,
	14,	3,	19,
	14,	19,	15,
	15,	19,	4,
	0,	15,	16,
	15,	4,	20,
	15,	20,	16,
	16,	20,	5,
	0,	16,	12,
	16,	5,	21,
	16,	21,	12,
	12,	21,	1,
	1,	27,	22,
	27,	10,	36,
	27,	36,	22,
	22,	36,	6,
	2,	28,	23,
	28,	6,	32,
	28,	32,	23,
	23,	32,	7,
	3,	29,	24,
	29,	7,	33,
	29,	33,	24,
	24,	33,	8,
	4,	30,	25,
	30,	8,	34,
	30,	34,	25,
	25,	34,	9,
	5,	31,	26,
	31,	9,	35,
	31,	35,	26,
	26,	35,	10,
	6,	28,	22,
	28,	2,	17,
	28,	17,	22,
	22,	17,	1,
	7,	29,	23,
	29,	3,	18,
	29,	18,	23,
	23,	18,	2,
	8,	30,	24,
	30,	4,	19,
	30,	19,	24,
	24,	19,	3,
	9,	31,	25,
	31,	5,	20,
	31,	20,	25,
	25,	20,	4,
	10,	27,	26,
	27,	1,	21,
	27,	21,	26,
	26,	21,	5,
	11,	38,	37,
	38,	7,	32,
	38,	32,	37,
	37,	32,	6,
	11,	39,	38,
	39,	8,	33,
	39,	33,	38,
	38,	33,	7,
	11,	40,	39,
	40,	9,	34,
	40,	34,	39,
	39,	34,	8,
	11,	41,	40,
	41,	10,	35,
	41,	35,	40,
	40,	35,	9,
	11,	37,	41,
	37,	6,	36,
	37,	36,	41,
	41,	36,	10
};