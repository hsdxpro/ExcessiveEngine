////////////////////////////////////////////////////////////////////////////////
//	Zsiroskenyer Team
////////////////////////////////////////////////////////////////////////////////
//	Vertex format stored as int64
//	Limitations:
//		TODO
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <stdexcept>
#include <vector>


class cVertexFormat {
public:

	// Enumerations
	enum eType : uint32_t {
		INT = 1,
		NORM = 2,
		FLOAT = 3,
	};
	enum eSemantic : uint32_t {
		POSITION = 0,
		NORMAL = 1,
		COLOR = 2,
		TEXCOORD = 3,
	};
	enum eBits : uint32_t {
		_8_BIT = 1,
		_16_BIT = 2,
		_32_BIT = 3,
	};

	// Vertex attrib struct, uncompressed
	struct Attribute {
		eType type;
		eSemantic semantic;
		uint32_t nComponents;
		eBits bitsPerComponent;

		 unsigned GetByteSize(){ 
			return (1 << (size_t(bitsPerComponent) - 1))*nComponents;
		};
	};


	cVertexFormat(const Attribute* attribs, uint32_t nAttribs) : data(0) { Create(attribs, nAttribs);  }
	cVertexFormat(const std::vector<Attribute>& attribs) : data(0) { Create(attribs); }
	cVertexFormat() : data(0) {}

	// Create vertex decl
	 void Create(const std::vector<Attribute>& attribs) {
		Create(attribs.data(), (uint32_t)attribs.size());
	}
	 void Create(const Attribute* attribs, uint32_t nAttribs = 1) {
		data = 0;
		if (nAttribs > 8) {
			throw std::invalid_argument("8 attributes at maximum");
		}
		try {
			for (decltype(nAttribs) i = 0; i < nAttribs; i++) {
				auto& attrib = attribs[i];
				uint64_t aUint = EncodeAttrib(attrib.type, attrib.semantic, attrib.nComponents, attrib.bitsPerComponent);
				data |= (aUint << (8 * i));
			}
		}
		catch (std::exception&) {
			data = 0;
			throw;
		}
	}
	// Decode vertex decl to array
	 std::vector<Attribute> Decode() const {
		Attribute attrib;
		std::vector<Attribute> v;
		for (int i = 0; i < 8; i++) {
			uint8_t aUint = uint8_t(data >> (i * 8));
			if (aUint == 0)
				break;
			DecodeAttrib(aUint, attrib.type, attrib.semantic, attrib.nComponents, attrib.bitsPerComponent);
			v.push_back(attrib);
		}
		return v;
	}
	// Size of vertex structure
	 size_t GetByteSize() const {
		Attribute attrib;
		size_t size = 0;
		for (int i = 0; i < 8; i++) {
			uint8_t aUint = uint8_t(data >> (i * 8));
			if (aUint == 0)
				break;
			DecodeAttrib(aUint, attrib.type, attrib.semantic, attrib.nComponents, attrib.bitsPerComponent);
			size += attrib.GetByteSize();
		}
		return size;
	}

	// Raw data
	 uint64_t Raw() const {
		return data;
	}

	// Compare
	 bool operator==(const cVertexFormat& other) const {
		return other.data == data;
	}
	 bool operator!=(const cVertexFormat& other) const {
		return other.data != data;
	}

	// Compatibility check
	 bool IsSubsetOf(const cVertexFormat& other) const {
		uint8_t d[8];
		uint8_t od[8];
		for (int i = 0; i < 8; i++) {
			d[i] = uint8_t(data >> (8 * i));
			od[i] = uint8_t(other.data >> (8 * i));
		}
		bool knockOut[8] = {true, true, true, true, true, true, true, true};
		int i = 0;
		// for each attrib
		while (d[i] != 0) {
			// look for a pair
			bool match = false;
			for (int j = 0; j < 8; j++) {
				if (d[i] == od[j] && knockOut[j]) {
					match = true;
					knockOut[j] = false;
					break;
				}
			}
			if (!match) {
				return false;
			}
			++i;
		}
		return true;
	}

private:
	// Encode/Decode to/from 8 bits
	 void DecodeAttrib(uint8_t attrib, eType& type, eSemantic& semantic, uint32_t& nComponents, eBits& bitsPerComponent) const {
		type = eType((attrib >> 6) & 3);
		semantic = eSemantic((attrib >> 4) & 3);
		nComponents = ((attrib >> 2) & 3) + 1;
		bitsPerComponent = eBits(attrib & 3);
	}
	 uint8_t EncodeAttrib(eType type, eSemantic semantic, uint32_t nComponents, eBits bitsPerComponent) {
		if (nComponents - 1 > 3)
			throw std::invalid_argument("components must be between 1 and 4");
		if (unsigned(semantic) >= 4)
			throw std::invalid_argument("use enum for semantics, don\'t hack");
		if (unsigned(bitsPerComponent) - 1 >= 4)
			throw std::invalid_argument("use enum for component width, don\'t hack");
		if (unsigned(type) - 1 >= 4)
			throw std::invalid_argument("use enum for type, don\'t hack");
		if (type == eType::FLOAT && bitsPerComponent == eBits::_8_BIT)
			throw std::invalid_argument("float cannot be used with 8 bits");
		if (type == eType::NORM && bitsPerComponent == eBits::_32_BIT)
			throw std::invalid_argument("normalized int cannot be used with 3 bits");
		if (bitsPerComponent != _32_BIT && nComponents == 3)
			throw std::invalid_argument("only 32 bits supported with 3 components");

		uint8_t ret = 0;
		ret |= (uint8_t(type) << 6);
		ret |= (uint8_t(semantic) << 4);
		ret |= ((nComponents-1) << 2);
		ret |= uint8_t(bitsPerComponent);
		return ret;
	}

	// 8x8 bit representation of at max 8 attribs
	uint64_t data;
	
};
