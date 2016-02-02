#include "PipelinePorts.h"

////////////////////////////////////////////////////////////////////////////////
// InputPort

bool InputPort::Link(OutputPort* source) {
	source->Unlink();
	return LinkPorts(source, this);

}

void InputPort::Unlink() {
	prev->next = nullptr;
	prev = nullptr;
}

OutputPort* InputPort::GetLink() {
	return prev;
}

////////////////////////////////////////////////////////////////////////////////
// OutputPort

bool OutputPort::Link(InputPort* destination) {
	destination->Unlink();
	return LinkPorts(this, destination);
}

void OutputPort::Unlink() {
	next->prev = nullptr;
	next = nullptr;
}

InputPort* OutputPort::GetLink() const {
	return nullptr;
}


////////////////////////////////////////////////////////////////////////////////
// Linking

bool LinkPorts(OutputPort* source, InputPort* destination) {
	source->next = destination;
	destination->prev = source;
	return true;
}