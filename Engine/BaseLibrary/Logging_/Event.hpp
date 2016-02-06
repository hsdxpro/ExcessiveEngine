#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <memory>

namespace exc {


enum class eEventParameterType {
	DEFAULT,
	FLOAT,
	INT,
	RAW,
};



struct EventParameter {
	std::string name;
	virtual std::string ToString() const { return std::string{}; }
	virtual eEventParameterType Type() const { return eEventParameterType::DEFAULT; }
};


struct EventParameterFloat : public EventParameter {
	float value;
	std::string ToString() const override {
		std::stringstream ss;
		ss << value;
		return ss.str();
	}
	eEventParameterType Type() const override { return eEventParameterType::FLOAT; }
};


struct EventParameterInt : public EventParameter {
	int value;
	std::string ToString() const override {
		std::stringstream ss;
		ss << value;
		return ss.str();
	}
	eEventParameterType Type() const override { return eEventParameterType::INT; }
};

struct EventParameterRaw : public EventParameter {
	std::vector<uint8_t> data;
	std::string ToString() const override {
		return "binary data";
	}
	eEventParameterType Type() const override { return eEventParameterType::RAW; }
};



class Event {
public:
	Event();
	Event(const std::string& message);
	Event(std::initializer_list<EventParameter> parameters);
	Event(const std::string& message, std::initializer_list<EventParameter> parameters);

	Event(const Event&);
	Event(Event&&) = default;
	~Event() = default;

	void SetMessage(const std::string& message);
	const std::string& GetMessage() const;

	void PutParameter(const EventParameter& parameter);
	size_t GetNumParameters() const;

	EventParameter& operator[](size_t index);
	const EventParameter& operator[](size_t index) const;
private:
	std::vector<std::unique_ptr<EventParameter>> parameters;
	std::string message;
};



} // namespace exc