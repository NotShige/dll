#pragma once

#include "../Module.hpp"

class DisableMouseWheel : public Module {
public:
	DisableMouseWheel() : Module("Disable Mouse Wheel", "Prevents you from accidentally scrolling through your hotbar",
		IDR_CURSOR_PNG, "") {
		
	};

	void onEnable() override;

	void onDisable() override;

	void defaultConfig() override;

	void settingsRender(float settingsOffset) override;

	bool disableScrolling = true;

	void onMouse(MouseEvent& event);

	void onKey(KeyEvent& event);
};
