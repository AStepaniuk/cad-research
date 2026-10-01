#include "form_section.h"

void gui::components::form_section(const char *label)
{
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::TextDisabled("%s", label);
    ImGui::Spacing();
}
