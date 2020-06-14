// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

static const char* required_function_names[] = {"vvctre_gui_begin", "vvctre_gui_end",
                                                "vvctre_gui_text"};

using vvctre_gui_begin_t = bool (*)(const char* name);
using vvctre_gui_end_t = void (*)();
using vvctre_gui_text_t = bool (*)(const char* text);

static vvctre_gui_begin_t vvctre_gui_begin;
static vvctre_gui_end_t vvctre_gui_end;
static vvctre_gui_text_t vvctre_gui_text;

#ifdef _WIN32
#define VVCTRE_PLUGIN_EXPORT extern "C" __declspec(dllexport)
#else
#define VVCTRE_PLUGIN_EXPORT extern "C"
#endif

VVCTRE_PLUGIN_EXPORT int GetRequiredFunctionCount() {
    return 3;
}

VVCTRE_PLUGIN_EXPORT const char** GetRequiredFunctionNames() {
    return required_function_names;
}

VVCTRE_PLUGIN_EXPORT void PluginLoaded(void* core, void* plugin_manager,
                                       void* required_functions[]) {
    vvctre_gui_begin = (vvctre_gui_begin_t)required_functions[0];
    vvctre_gui_end = (vvctre_gui_end_t)required_functions[1];
    vvctre_gui_text = (vvctre_gui_text_t)required_functions[2];
}

VVCTRE_PLUGIN_EXPORT void BeforeDrawingFPS() {
    if (vvctre_gui_begin("Plugin")) {
        vvctre_gui_text("Your plugin works!");
    }
    vvctre_gui_end();
}
