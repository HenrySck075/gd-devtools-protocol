#include "state.hpp"
#include "../../../external/tinyjs/TinyJS.h"
#undef inline // kill yourself
#include <Geode/DefaultInclude.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/ModMetadata.hpp>

geode::Mod* representer;

static void pushLog(TinyJS::Variable* msg, geode::Severity severity) {
  if (msg->isString()) {
    std::string j = msg->getString();
    geode::log::logImpl(severity, representer, "{}", j);
  }
  msg->getReturnVar()->setUndefined();
}

$jsMethod(Console_log) {
  pushLog(v, geode::Severity::Info);
}

$jsMethod(Console_debug) {
  pushLog(v, geode::Severity::Debug);
}

$jsMethod(Console_warn) {
  pushLog(v, geode::Severity::Warning);
}

$jsMethod(Console_error) {
  pushLog(v, geode::Severity::Error);
}


$execute{
  geode::ModMetadata meta("henrysck075.puppeteer.js");

  meta.setName("JavaScript");
  meta.setVersion(geode::VersionInfo{7,7,7});
  meta.setDescription("The representation of the JavaScript console output from GD DevTools Protocol.");
  meta.setDevelopers(geode::Mod::get()->getDevelopers());

  representer = new geode::Mod(meta);

  auto s = getState();
  {
    js_newcfunction(s, Console_log, "console.log", 1);
    js_defproperty(s, -2, "log", JS_DONTENUM);

    js_newcfunction(s, Console_debug, "console.debug", 1);
    js_defproperty(s, -2, "debug", JS_DONTENUM);

    js_newcfunction(s, Console_warn, "console.warn", 1);
    js_defproperty(s, -2, "warn", JS_DONTENUM);

    js_newcfunction(s, Console_error, "console.error", 1);
    js_defproperty(s, -2, "error", JS_DONTENUM);

  }
  //js_newcconstructor(s, new_Console, new_Console, "Console", 0);
  js_defglobal(s, "console", JS_DONTENUM);

  //js_dostring(s, "var console = new Console()");
/*
  js_getglobal(s, "Console");
  if (js_pconstruct(s, 0)) {
    geode::log::error("Error creating Console object.");
  } else {
    auto console = js_toobject(s, -1);
    js_pop(s,1);

    js_pushobject(s, console);
    js_setglobal(s, "console");
  }
*/
}
