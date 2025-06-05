#pragma once
#include "../Plugin.h"
#include <string>
#include <chrono>

class D2RHUD : public Plugin {
public:
	void OnDraw() override;
	bool OnKeyPressed(short key) override;

private:
	void ShowVersionMessage();
	bool ctrlPressed = false;
	bool altPressed = false;
	bool vPressed = false;
	bool rPressed = false;
};

