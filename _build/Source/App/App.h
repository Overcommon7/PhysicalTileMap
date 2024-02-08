#pragma once
class Editor;

class App
{
public:
	App();
	~App();

	
	bool Run();

private:
	static inline bool isCreated = false;
	Editor* editor;
	bool shouldRestart = false;
};

