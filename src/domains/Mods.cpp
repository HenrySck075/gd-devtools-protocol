#include "../WS.hpp"
#include <Geode/Result.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Loader.hpp>

using namespace geode;

$domainMethod(disableMod) {
  if (
    auto m = Loader::get()->getLoadedMod(params["mod"].asString().unwrap())
  )
    m->disable();
  return geode::Ok(matjson::Value::object());
}
$domainMethod(enableMod) {
  if (
    auto m = Loader::get()->getInstalledMod(params["mod"].asString().unwrap())
  )
    m->enable();
  return geode::Ok(matjson::Value::object());
}
$domainMethod(getSettingsItems) {
  matjson::Value ret;
  if (
    auto m = Loader::get()->getInstalledMod(params["mod"].asString().unwrap())
  ) {
    for (auto& p : m->getMetadata().getSettings()) {
      ret.set(p.first,p.second);
    }
  }
  return geode::Ok(matjson::makeObject({
    {"data", ret}
  }));
}
$domainMethod(setSettings) {
  if (
    auto m = Loader::get()->getInstalledMod(params["mod"].asString().unwrap())
  ) {
    for (auto& [k,v] : params["settings"]) {
#define set(type) m->setSettingValue<type>(k, v.as<type>().unwrap())
      if (v.isBool()) set(bool);
      else if (v.asInt().isOk()) set(int64_t);
      else if (v.asDouble().isOk() || v.isNumber()) set(double);
      else if (v.isString()) set(std::string);
#undef set
    }
    return geode::Ok(matjson::Value::object());
  }
  return errors::invalidParameter("Mod not installed.");
}

$execute {
  auto p = Protocol::get();
  p->registerFunction("Mods.disableMod", &disableMod, {"mod"});
  p->registerFunction("Mods.enableMod", &disableMod, {"mod"});
  p->registerFunction("Mods.getSettingsItems", &getSettingsItems, {"mod"});
  p->registerFunction("Mods.setSettings", &setSettings, {"mod", "settings"});
}
