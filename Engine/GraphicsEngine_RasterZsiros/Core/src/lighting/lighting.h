////////////////////////////////////////////////////////////////////////////////
//	Zsiroskenyer Team
////////////////////////////////////////////////////////////////////////////////
//	Provides function for physical light calculations and color management.
//	Light calculations:
//		- Rayleigh scattering on spectrum
//		- Spectrum to CIE XYZ color space
//		- XYZ <--> sRGB conversions
////////////////////////////////////////////////////////////////////////////////


#include "../math/vec3.h"
#include <algorithm>
#include <cassert>


// Spectrum
class IntensitySpectrum {
public:
	// constructor
	IntensitySpectrum();
	IntensitySpectrum(const IntensitySpectrum& other);

	// generate spectrum
	template<class IntensityFunc> // of signature float(*)(float) or compatible
	void Generate(IntensityFunc f);
	void BlackBody(float temperature);
	void Constant(float value);
	void Scale(float c); // scales entire spectrum

	// do stuff
	int Peak() const; // returns peak wavelength

	// convert to whatever
	Vec3 ToRGB() const;
	Vec3 ToXYZ() const;

	// operators
	IntensitySpectrum& operator+=(const IntensitySpectrum& other);
	IntensitySpectrum& operator-=(const IntensitySpectrum& other);
	IntensitySpectrum operator+(const IntensitySpectrum& other);
	IntensitySpectrum operator-(const IntensitySpectrum& other);

	IntensitySpectrum& operator=(const IntensitySpectrum& other);

	// access intensity by wavelength
	float& operator[](int wavelength); // index by wavelenght: ONLY 380-780
	float operator[](int wavelength) const;
	static const int min = 380; // inclusive
	static const int max = 780; // inclusive
private:
	float intensity[max - min + 1];
	static float __dummy;
};


// Conversion
class LightConvert {
public:
	static Vec3 RGBToXYZ(const Vec3& rgb);
	static Vec3 XYZToRGB(const Vec3& xyz);
	static Vec3 SpectrumToRGB(const IntensitySpectrum& spectrum);
	static Vec3 SpectrumToXYZ(const IntensitySpectrum& spectrum);
};


// CIE color matching functions
	// fuck knows which year's standard...
class CIEColorMatch {
public:
	static float X(float wavelength);
	static float Y(float wavelength);
	static float Z(float wavelength);
	static const float lookupTable[81][3];
private:
	template <int offset>
	static inline float SampleLUT(float wavelength) {
		wavelength -= 380.0f;
		wavelength /= 400.0f;
		wavelength = std::max(0.0f, std::min(1.0f, wavelength));
		wavelength *= 80.f;
		int idx = int(wavelength);
		if (idx < 0)
			idx = 0;
		if (idx>79)
			idx = 79;
		assert(idx + 1 < 81 && idx>=0);
		float v = (wavelength - idx)*CIEColorMatch::lookupTable[idx][offset] 
				+ (1 + idx - wavelength)*CIEColorMatch::lookupTable[idx + 1][offset];
		return v;
	}
};

// Black body radiation
double BlackBody(double T, double wavelength);


// Rayleigh scattering
// params:
//	spectrum: scattered away light is removed from this
//	coefficient: how much air, how much it causes scattering, @550nm
void Rayleigh(IntensitySpectrum& spectrum, float coefficient);

inline float Rayleigh(float wavelength, float coefficient) {
	/*
	float halfDistance = 5e2 / pow(wavelength / 780.0f, -4.0f);
	float remaining = pow(2.0f, -distance / halfDistance);
	return remaining;
	*/

	float waveExtCoeff = pow(wavelength / 550.f, 4.0f);
	float remaining = exp(-coefficient / waveExtCoeff);
	return remaining;
}


// Relative air mass
float RelativeAirMass(float angle);