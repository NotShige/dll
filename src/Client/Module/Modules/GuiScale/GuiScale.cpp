#include "GuiScale.hpp"

#include "Events/EventManager.hpp"

void GuiScale::onEnable() {
    restored = false;
    Listen(this, SetupAndRenderEvent, &GuiScale::onSetupAndRender)
    Module::onEnable();
}

void GuiScale::onDisable() {
    if (!restored) {
        delayDisable = true;
        return;
    }
    Deafen(this, SetupAndRenderEvent, &GuiScale::onSetupAndRender)

    Module::onDisable();
}

void GuiScale::defaultConfig() {
    Module::defaultConfig("core");
    setDef("guiscale", 2.f);
    
}

void GuiScale::settingsRender(float settingsOffset) {
    float x = Constraints::PercentageConstraint(0.019, "left");
    float y = Constraints::PercentageConstraint(0.10, "top");

    const float scrollviewWidth = Constraints::RelativeConstraint(0.12, "height", true);


    FlarialGUI::ScrollBar(x, y, 140, Constraints::SpacingConstraint(5.5, scrollviewWidth), 2);
    FlarialGUI::SetScrollView(x - settingsOffset, Constraints::PercentageConstraint(0.00, "top"),
                              Constraints::RelativeConstraint(1.0, "width"),
                              Constraints::RelativeConstraint(0.88f, "height"));

    addHeader("GUI Scale");
    addSlider("UI Scale", "", "guiscale", 4.f, 1.f, false);

    FlarialGUI::UnsetScrollView();

    resetPadding();
}

void GuiScale::onSetupAndRender(SetupAndRenderEvent &event) {
    if (!this->isEnabled()) return;
    update();
}

void GuiScale::update() {
    float targetScale = delayDisable ? originalScale : getOps<float>("guiscale");
    auto guiData = SDK::clientInstance->getGuiData();
    if (targetScale == guiData->GuiScale && !delayDisable && !fixResize) return;
    updateScale(targetScale);
}

void GuiScale::updateScale(float newScale) {
    if (restored && !fixResize) return;

    fixResize = false;
    auto guiData = SDK::clientInstance->getGuiData();

    if (originalScale == 0) originalScale = guiData->GuiScale;
    if (newScale == 0) newScale = getOps<float>("guiscale");
    
    float oldScale = guiData->GuiScale;

    auto screenSize = guiData->ScreenSize;
    static auto safeZone = Vec2<float>{ 0.f, 0.f };

    SDK::clientInstance->_updateScreenSizeVariables(&screenSize, &safeZone, newScale < 1.f ? 1.f : newScale);
    SDK::screenView->VisualTree->root->forEachChild([this](std::shared_ptr<UIControl>& control) {
    control->updatePosition();
    });

    if (delayDisable) {
        delayDisable = false;
        restored = true;
    }
}
