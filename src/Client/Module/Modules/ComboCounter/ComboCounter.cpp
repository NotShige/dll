#include "ComboCounter.hpp"

#include "Events/EventManager.hpp"
#include "Events/Game/TickEvent.hpp"
#include "Events/Render/RenderEvent.hpp"

void ComboCounter::onEnable() {
    Listen(this, AttackEvent, &ComboCounter::onAttack)
    Listen(this, TickEvent, &ComboCounter::onTick)
    Listen(this, RenderEvent, &ComboCounter::onRender)
    Module::onEnable();
}

void ComboCounter::onDisable() {
    Deafen(this, AttackEvent, &ComboCounter::onAttack)
    Deafen(this, TickEvent, &ComboCounter::onTick)
    Deafen(this, RenderEvent, &ComboCounter::onRender)
    Module::onDisable();
}

void ComboCounter::defaultConfig() {
    setDef("text", (std::string)"Combo: {value}");
    Module::defaultConfig("all");
    
}

void ComboCounter::settingsRender(float settingsOffset) {

    /* Border Start */

    float x = Constraints::PercentageConstraint(0.019, "left");
    float y = Constraints::PercentageConstraint(0.10, "top");

    const float scrollviewWidth = Constraints::RelativeConstraint(0.12, "height", true);


    FlarialGUI::ScrollBar(x, y, 140, Constraints::SpacingConstraint(5.5, scrollviewWidth), 2);
    FlarialGUI::SetScrollView(x - settingsOffset, Constraints::PercentageConstraint(0.00, "top"),
                              Constraints::RelativeConstraint(1.0, "width"),
                              Constraints::RelativeConstraint(0.88f, "height"));

    addHeader("Combo Counter");
    defaultAddSettings("main");
    extraPadding();

    addHeader("Text");
    defaultAddSettings("text");
    extraPadding();

    addHeader("Colors");
    defaultAddSettings("colors");

    addHeader("Misc");
    defaultAddSettings("misc");

    FlarialGUI::UnsetScrollView();

    resetPadding();

}

void ComboCounter::onAttack(AttackEvent &event) {
    if (!this->isEnabled()) return;
    if (std::chrono::high_resolution_clock::now() - last_hit > std::chrono::milliseconds(480)) {
        Combo++;
        last_hit = std::chrono::high_resolution_clock::now();
    }
}

void ComboCounter::onTick(TickEvent &event) {
    if (!this->isEnabled()) return;
    if (!SDK::clientInstance->getLocalPlayer())
        return;

    auto LP = reinterpret_cast<LocalPlayer*>(event.getActor());
    if (LP->getHurtTime() != 0)
        Combo = 0;
    std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - last_hit;
    if (duration.count() >= 15) Combo = 0;
}

void ComboCounter::onRender(RenderEvent &event) {
    if (this->isEnabled()) {
        auto comboStr = FlarialGUI::cached_to_string(Combo);
        this->normalRender(8, comboStr);
    }
}