// Copyright 2020 Valentin Vanelslande
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <fstream>
#include <iostream>
#include <sstream>
#include "common_types.h"
#include "json/single_include/nlohmann/json.hpp"

#ifdef _WIN32
#define VVCTRE_PLUGIN_EXPORT extern "C" __declspec(dllexport)
#else
#define VVCTRE_PLUGIN_EXPORT extern "C"
#endif

static const char* required_function_names[] = {
    "vvctre_settings_set_button",
    "vvctre_settings_set_analog",
    "vvctre_settings_set_motion_device",
    "vvctre_settings_set_touch_device",
    "vvctre_settings_set_cemuhookudp_address",
    "vvctre_settings_set_cemuhookudp_port",
    "vvctre_settings_set_cemuhookudp_pad_index",
};

typedef void (*vvctre_settings_set_button_t)(int index, const char* params);
typedef void (*vvctre_settings_set_analog_t)(int index, const char* params);
typedef void (*vvctre_settings_set_motion_device_t)(const char* params);
typedef void (*vvctre_settings_set_touch_device_t)(const char* params);
typedef void (*vvctre_settings_set_cemuhookudp_address_t)(const char* value);
typedef void (*vvctre_settings_set_cemuhookudp_port_t)(u16 value);
typedef void (*vvctre_settings_set_cemuhookudp_pad_index_t)(u8 value);

static vvctre_settings_set_button_t vvctre_settings_set_button;
static vvctre_settings_set_analog_t vvctre_settings_set_analog;
static vvctre_settings_set_motion_device_t vvctre_settings_set_motion_device;
static vvctre_settings_set_touch_device_t vvctre_settings_set_touch_device;
static vvctre_settings_set_cemuhookudp_address_t vvctre_settings_set_cemuhookudp_address;
static vvctre_settings_set_cemuhookudp_port_t vvctre_settings_set_cemuhookudp_port;
static vvctre_settings_set_cemuhookudp_pad_index_t vvctre_settings_set_cemuhookudp_pad_index;

VVCTRE_PLUGIN_EXPORT int GetRequiredFunctionCount() {
    return 7;
}

VVCTRE_PLUGIN_EXPORT const char** GetRequiredFunctionNames() {
    return required_function_names;
}

VVCTRE_PLUGIN_EXPORT void PluginLoaded(void* core, void* plugin_manager,
                                       void* required_functions[]) {
    vvctre_settings_set_button = (vvctre_settings_set_button_t)required_functions[0];
    vvctre_settings_set_analog = (vvctre_settings_set_analog_t)required_functions[1];
    vvctre_settings_set_motion_device = (vvctre_settings_set_motion_device_t)required_functions[2];
    vvctre_settings_set_touch_device = (vvctre_settings_set_touch_device_t)required_functions[3];
    vvctre_settings_set_cemuhookudp_address =
        (vvctre_settings_set_cemuhookudp_address_t)required_functions[4];
    vvctre_settings_set_cemuhookudp_port =
        (vvctre_settings_set_cemuhookudp_port_t)required_functions[5];
    vvctre_settings_set_cemuhookudp_pad_index =
        (vvctre_settings_set_cemuhookudp_pad_index_t)required_functions[6];
}

VVCTRE_PLUGIN_EXPORT void InitialSettingsOpening() {
    std::ifstream file("controls.json");
    if (!file.fail()) {
        try {
            std::ostringstream oss;
            oss << file.rdbuf();

            const nlohmann::json json = nlohmann::json::parse(oss.str());

            std::array<std::string, 17> buttons =
                json["buttons"].get<std::array<std::string, 17>>();
            std::array<std::string, 2> analogs = json["analogs"].get<std::array<std::string, 2>>();

            for (std::size_t i = 0; i < buttons.size(); ++i) {
                vvctre_settings_set_button(static_cast<int>(i), buttons[i].c_str());
            }

            for (std::size_t i = 0; i < analogs.size(); ++i) {
                vvctre_settings_set_analog(static_cast<int>(i), analogs[i].c_str());
            }

            vvctre_settings_set_motion_device(json["motion_device"].get<std::string>().c_str());
            vvctre_settings_set_touch_device(json["touch_device"].get<std::string>().c_str());
            vvctre_settings_set_cemuhookudp_address(
                json["cemuhookudp_address"].get<std::string>().c_str());
            vvctre_settings_set_cemuhookudp_port(json["cemuhookudp_port"].get<u16>());
            vvctre_settings_set_cemuhookudp_pad_index(json["cemuhookudp_pad_index"].get<u8>());
        } catch (nlohmann::json::exception&) {
        }
    }
}
