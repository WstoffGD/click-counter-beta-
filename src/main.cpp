#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
using namespace geode::prelude;

static int64_t getTotal() {
    auto mod = Mod::get();
    if (!mod->hasSavedValue("total-clicks")) {
        int start = GameStatsManager::sharedState()->getStat("1");
        mod->setSavedValue<int64_t>("total-clicks", start);
    }
    return mod->getSavedValue<int64_t>("total-clicks");
}

class $modify(ClickPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        auto box = CCLayerColor::create(ccc4(255, 255, 255, 60), 150.f, 45.f);
        box->setPosition({10.f, 10.f});
        box->setID("click-box"_spr);
        m_uiLayer->addChild(box);

        auto label = CCLabelBMFont::create(fmt::format("{}", getTotal()).c_str(), "bigFont.fnt");
        label->setScale(0.6f);
        label->setPosition({85.f, 32.f});
        label->setID("click-label"_spr);
        m_uiLayer->addChild(label);
        return true;
    }
};

class $modify(ClickPlayerObject, PlayerObject) {
    bool pushButton(PlayerButton btn) {
        auto pl = PlayLayer::get();
        if (pl && btn == PlayerButton::Jump && this == pl->m_player1) {
            Mod::get()->setSavedValue<int64_t>("total-clicks", getTotal() + 1);

            if (auto box = typeinfo_cast<CCLayerColor*>(pl->m_uiLayer->getChildByID("click-box"_spr))) {
                box->stopAllActions();
                box->setOpacity(210);
                box->runAction(CCFadeTo::create(0.25f, 60));
            }
            if (auto lbl = typeinfo_cast<CCLabelBMFont*>(pl->m_uiLayer->getChildByID("click-label"_spr))) {
                lbl->setString(fmt::format("{}", getTotal()).c_str());
            }
        }
        return PlayerObject::pushButton(btn);
    }
};
