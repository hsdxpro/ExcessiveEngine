////////////////////////////////////////////////////////////////////////////////
//	Zsiroskenyer Team
////////////////////////////////////////////////////////////////////////////////
//	Provides function for physical light calculations and color management.
//
////////////////////////////////////////////////////////////////////////////////

#include "lighting.h"
#include <algorithm>
#include <cmath>

////////////////////////////////////////////////////////////////////////////////
// Spectrum

float IntensitySpectrum::__dummy = 0.0f;

// constructor
IntensitySpectrum::IntensitySpectrum() {
	for (float& i : intensity) {
		i = 0.0f;
	}
}
IntensitySpectrum::IntensitySpectrum(const IntensitySpectrum& other) {
	for (int i = 0; i <= max - min; i++) {
		intensity[i] = other.intensity[i];
	}
}

// generate spectrum
template<class IntensityFunc> // of signature float(*)(float) or compatible
void IntensitySpectrum::Generate(IntensityFunc f) {
	for (int w = min; w <= max; w++) {
		intensity[w - min] = f((float)w);
	}
}

void IntensitySpectrum::BlackBody(float temperature) {
	for (int w = min; w <= max; w++) {
		intensity[w - min] = ::BlackBody(temperature, (float)w);
	}
}

void IntensitySpectrum::Constant(float value) {
	for (float& i : intensity) {
		i = value;
	}
}

void IntensitySpectrum::Scale(float c) {
	for (float& i : intensity) {
		i *= c;
	}
}

// do stuff
int IntensitySpectrum::Peak() const {
	int peakw = 0.0f;
	for (int w = min; w <= max; w++) {
		if (intensity[w - min] > peakw)
			peakw = w;
	}
	return peakw;
}

// convert to whatever
Vec3 IntensitySpectrum::ToRGB() const {
	return LightConvert::XYZToRGB(ToXYZ());
}
Vec3 IntensitySpectrum::ToXYZ() const {
	Vec3 ret(0.0f, 0.0f, 0.0f);
	for (int w = min; w <= max; w++) {
		ret.x += operator[](w) * CIEColorMatch::X(w);
		ret.y += operator[](w) * CIEColorMatch::Y(w);
		ret.z += operator[](w) * CIEColorMatch::Z(w);
	}
	return ret;
}

// operators
IntensitySpectrum& IntensitySpectrum::operator+=(const IntensitySpectrum& other) {
	for (int i = 0; i <= max - min; i++) {
		intensity[i] += other.intensity[i];
	}
	return *this;
}
IntensitySpectrum& IntensitySpectrum::operator-=(const IntensitySpectrum& other) {
	for (int i = 0; i <= max - min; i++) {
		intensity[i] -= other.intensity[i];
	}
	return *this;
}
IntensitySpectrum IntensitySpectrum::operator + (const IntensitySpectrum& other) {
	IntensitySpectrum ret = other;
	ret += other;
	return ret;
}
IntensitySpectrum IntensitySpectrum::operator - (const IntensitySpectrum& other) {
	IntensitySpectrum ret = other;
	ret -= other;
	return ret;
}

IntensitySpectrum& IntensitySpectrum::operator = (const IntensitySpectrum& other) {
	for (int i = 0; i <= max - min; i++) {
		intensity[i] = other.intensity[i];
	}
	return *this;
}

// access intensity by wavelength
float& IntensitySpectrum::operator[](int wavelength) {
	if (min <= wavelength && wavelength <= max) {
		return intensity[wavelength - min];
	}
	else {
		return __dummy;
	}
}
float IntensitySpectrum::operator[](int wavelength) const {
	if (min <= wavelength && wavelength <= max) {
		return intensity[wavelength - min];
	}
	else {
		return 0.0f;
	}
}


////////////////////////////////////////////////////////////////////////////////
// Conversion between color spaces
Vec3 LightConvert::RGBToXYZ(const Vec3& rgb) {
	return Vec3(
		Dot(rgb, Vec3(0.4124, 0.3575, 0.1805)),
		Dot(rgb, Vec3(0.2126, 0.7152, 0.0722)),
		Dot(rgb, Vec3(0.0193, 0.1192, 0.9502))
	);
}
Vec3 LightConvert::XYZToRGB(const Vec3& xyz) {
	return Vec3(
		Dot(xyz, Vec3(3.2406, -1.5372, -0.4986)),
		Dot(xyz, Vec3(-0.9689, 1.8758, 0.0415)),
		Dot(xyz, Vec3(0.0557, -0.2040, 1.0570))
	);
}
Vec3 LightConvert::SpectrumToRGB(const IntensitySpectrum& spectrum) {
	return spectrum.ToRGB();
}
Vec3 LightConvert::SpectrumToXYZ(const IntensitySpectrum& spectrum) {
	return spectrum.ToXYZ();
}


////////////////////////////////////////////////////////////////////////////////
// CIE Color matching functions

float CIEColorMatch::X(float wavelength) {
	return SampleLUT<0>(wavelength);
}
float CIEColorMatch::Y(float wavelength) {
	return SampleLUT<1>(wavelength);
}
float CIEColorMatch::Z(float wavelength) {
	return SampleLUT<2>(wavelength);
}

const float CIEColorMatch::lookupTable[81][3] = {
	{0.0014, 0.0000, 0.0065}, {0.0022, 0.0001, 0.0105}, {0.0042, 0.0001, 0.0201},
	{0.0076, 0.0002, 0.0362}, {0.0143, 0.0004, 0.0679}, {0.0232, 0.0006, 0.1102},
	{0.0435, 0.0012, 0.2074}, {0.0776, 0.0022, 0.3713}, {0.1344, 0.0040, 0.6456},
	{0.2148, 0.0073, 1.0391}, {0.2839, 0.0116, 1.3856}, {0.3285, 0.0168, 1.6230},
	{0.3483, 0.0230, 1.7471}, {0.3481, 0.0298, 1.7826}, {0.3362, 0.0380, 1.7721},
	{0.3187, 0.0480, 1.7441}, {0.2908, 0.0600, 1.6692}, {0.2511, 0.0739, 1.5281},
	{0.1954, 0.0910, 1.2876}, {0.1421, 0.1126, 1.0419}, {0.0956, 0.1390, 0.8130},
	{0.0580, 0.1693, 0.6162}, {0.0320, 0.2080, 0.4652}, {0.0147, 0.2586, 0.3533},
	{0.0049, 0.3230, 0.2720}, {0.0024, 0.4073, 0.2123}, {0.0093, 0.5030, 0.1582},
	{0.0291, 0.6082, 0.1117}, {0.0633, 0.7100, 0.0782}, {0.1096, 0.7932, 0.0573},
	{0.1655, 0.8620, 0.0422}, {0.2257, 0.9149, 0.0298}, {0.2904, 0.9540, 0.0203},
	{0.3597, 0.9803, 0.0134}, {0.4334, 0.9950, 0.0087}, {0.5121, 1.0000, 0.0057},
	{0.5945, 0.9950, 0.0039}, {0.6784, 0.9786, 0.0027}, {0.7621, 0.9520, 0.0021},
	{0.8425, 0.9154, 0.0018}, {0.9163, 0.8700, 0.0017}, {0.9786, 0.8163, 0.0014},
	{1.0263, 0.7570, 0.0011}, {1.0567, 0.6949, 0.0010}, {1.0622, 0.6310, 0.0008},
	{1.0456, 0.5668, 0.0006}, {1.0026, 0.5030, 0.0003}, {0.9384, 0.4412, 0.0002},
	{0.8544, 0.3810, 0.0002}, {0.7514, 0.3210, 0.0001}, {0.6424, 0.2650, 0.0000},
	{0.5419, 0.2170, 0.0000}, {0.4479, 0.1750, 0.0000}, {0.3608, 0.1382, 0.0000},
	{0.2835, 0.1070, 0.0000}, {0.2187, 0.0816, 0.0000}, {0.1649, 0.0610, 0.0000},
	{0.1212, 0.0446, 0.0000}, {0.0874, 0.0320, 0.0000}, {0.0636, 0.0232, 0.0000},
	{0.0468, 0.0170, 0.0000}, {0.0329, 0.0119, 0.0000}, {0.0227, 0.0082, 0.0000},
	{0.0158, 0.0057, 0.0000}, {0.0114, 0.0041, 0.0000}, {0.0081, 0.0029, 0.0000},
	{0.0058, 0.0021, 0.0000}, {0.0041, 0.0015, 0.0000}, {0.0029, 0.0010, 0.0000},
	{0.0020, 0.0007, 0.0000}, {0.0014, 0.0005, 0.0000}, {0.0010, 0.0004, 0.0000},
	{0.0007, 0.0002, 0.0000}, {0.0005, 0.0002, 0.0000}, {0.0003, 0.0001, 0.0000},
	{0.0002, 0.0001, 0.0000}, {0.0002, 0.0001, 0.0000}, {0.0001, 0.0000, 0.0000},
	{0.0001, 0.0000, 0.0000}, {0.0001, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000}
};


////////////////////////////////////////////////////////////////////////////////
// Black body radiation
double BlackBody(double T, double wavelength) {
	wavelength /= 1e9; // pre-scale wavelength to meters

	static const double h = 6.62606957e-34; // Planck constant
	static const double c = 299792458.0; // speed of light in vacuum
	static const double k = 1.3806488e-23; // Boltzmann constant

	double exparg = h*c / (k*wavelength*T);
	double exppart = std::exp(exparg) - 1.0;
	double constpart = (2.0*h*c*c);
	double powpart = pow(wavelength, -5.0);
	double v = constpart * powpart / exppart;
	return v;
}


////////////////////////////////////////////////////////////////////////////////
// Rayleigh scattering
// params:
//	spectrum: scattered away light is removed from this
//	coefficient: how much air, how much it causes scattering, @550nm
void Rayleigh(IntensitySpectrum& spectrum, float coefficient) {
	for (int w = spectrum.min; w <= spectrum.max; w++) {
		spectrum[w] = spectrum[w] * Rayleigh((float)w, coefficient);
	}
}

////////////////////////////////////////////////////////////////////////////////
// Relative air mass
//
float RelativeAirMass(float angle) {
	// lookup table
	static const float lookupTable[] = {
		35.34702265702, 25.03345901695, 18.82383912497, 14.841587689799, 12.135143856789,
		10.206460817283, 8.7774380923265, 7.683854750501, 6.8241957946941, 6.132986854173,
		5.5666123169945, 5.0948231937226, 4.6964061279255, 4.3558205102075, 4.0616019565882,
		3.8051054723326, 3.5796902068684, 3.3800910357009, 3.2022826862325, 3.0429022724957,
		2.8993342165155, 2.7693535785862, 2.6512279629063, 2.5434457692177, 2.4447161928055,
		2.3540201773158, 2.2704066034852, 2.1931791161385, 2.1215564387377, 2.0550969801963,
		1.9932402595197, 1.9355277013486, 1.8816366045042, 1.8311253779001, 1.7838581473555, 
		1.7394103060566, 1.6976629640952, 1.6583273888379, 1.6212507218316, 1.5862631203506,
		1.5532117259418, 1.5219606644247, 1.4923740616189, 1.464350011889, 1.4377866095995,
		1.4126159176602, 1.3886850776181, 1.3659771052006, 1.3443900947722, 1.3238730935154,
		1.3043411800673, 1.2857943544278, 1.2681307109616, 1.2513162811237, 1.2353340806413,
		1.2200991881518, 1.2055946193825, 1.1917864057882, 1.1786235945514, 1.1661741227623,
		1.1542341791501, 1.1429736064404, 1.1322055776351, 1.1219980298244, 1.1123000101906,
		1.1030945344611, 1.0943646183634, 1.08612724617, 1.0783314650634, 1.0709602907707,
		1.0640137232922, 1.0574747783552, 1.0513434559598, 1.0455688032882, 1.0402017731581,
		1.0352083970244, 1.0305547063419, 1.0262237168382, 1.0222663813309, 1.01868269982,
		1.0153707666701, 1.0124494717891, 1.0097829409966, 1.0074900642006, 1.0055029043106,
		1.0038214613268, 1.0024457352492, 1.0013757260777, 1.0005944495397, 1.0001698427256,
		1, 1,
	};

	// convert angle to index
	angle /= 1.5707963267949f;
	angle = std::max(0.0f, std::min(1.0f, angle)) * 90.0f;
	int idx1 = (int)abs(angle);
	int idx2 = idx1 + 1;

	float sample1 = lookupTable[idx1];
	float sample2 = lookupTable[idx2];

	return (angle - (float)idx1)*sample2 + ((float)idx2 - angle)*sample1;
}


