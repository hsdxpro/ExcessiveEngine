#pragma once

class PipelineData;
class InputPort;
class OutputPort;



class InputPort {
	friend bool LinkPorts(OutputPort* source, InputPort* destination);
	friend class OutputPort;
public:
	virtual void SetData(PipelineData* data) = 0;
	bool Link(OutputPort* source);
	void Unlink();
	OutputPort* GetLink();
private:
	OutputPort* prev;
};



class OutputPort {
	friend bool LinkPorts(OutputPort* source, InputPort* destination);
	friend class InputPort;
public:
	virtual void GetData(PipelineData* data) const = 0;
	bool Link(InputPort* destination);
	void Unlink();
	InputPort* GetLink() const;
private:
	InputPort* next;
};



bool LinkPorts(OutputPort* source, InputPort* destination);