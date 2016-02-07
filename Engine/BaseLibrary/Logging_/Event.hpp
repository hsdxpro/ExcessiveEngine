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
	EventParameter() = default;
	EventParameter(const std::string name) : name(name) {}

	std::string name;
	virtual std::string ToString() const { return std::string{}; }
	virtual eEventParameterType Type() const { return eEventParameterType::DEFAULT; }
};


struct EventParameterFloat : public EventParameter {
	EventParameterFloat() = default;
	EventParameterFloat(const std::string name) : EventParameter(name) {}
	EventParameterFloat(const std::string name, float value) : EventParameter(name), value(value) {}

	float value;
	std::string ToString() const override {
		std::stringstream ss;
		ss << value;
		return ss.str();
	}
	eEventParameterType Type() const override { return eEventParameterType::FLOAT; }
};


struct EventParameterInt : public EventParameter {
	EventParameterInt() = default;
	EventParameterInt(const std::string name) : EventParameter(name) {}
	EventParameterInt(const std::string name, int value) : EventParameter(name), value(value) {}

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

	template <class... Args>
	Event(const std::string& message, Args&&... args);

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
	template <size_t Index, class Head, class... Args>
	void AddVariadicParams(Head&& head, Args&&... args);

	template <size_t Index>
	void AddVariadicParams();

	std::vector<std::unique_ptr<EventParameter>> parameters;
	std::string message;
};



template <class... Args>
Event::Event(const std::string& message, Args&&... args) : message(message) {
	parameters.reserve(sizeof...(Args));
	AddVariadicParams<0, Args...>(std::forward<Args>(args)...);
}


template <size_t Index, class Head, class... Args>
void Event::AddVariadicParams(Head&& head, Args&&... args) {
	static_assert(std::is_base_of<EventParameter, Head>::value, "Only EventParameters can be given as argument.");
	PutParameter(head);
	AddVariadicParams<Index + 1, Args...>(std::forward<Args>(args)...);
}

template <size_t Index>
void Event::AddVariadicParams() {

}



} // namespace exc