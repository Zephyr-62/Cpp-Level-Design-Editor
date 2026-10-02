#pragma once

#include "editor/EditorContext.hpp"
#include "resources/Resource.hpp"
#include "resources/ResourceManager.hpp"

#include <imgui.h>
#include <string>
#include <memory>



class EditorUtils {
public:
    /// @brief Draws a resource picker for a specific resource type T. It displays the currently selected resource and allows the user to change it by selecting from available resources of type T.
    /// @tparam T A resource type that inherits from Resource. The function will only work for types that are registered in the ResourceManager.
    /// @param context the editor context containing the resource manager
    /// @param label the label to display for the resource picker
    /// @param resource a reference to the shared pointer of the resource to be modified
    /// @return true if the resource was changed, false otherwise
    template <typename T>
    static bool DrawResourcePicker(EditorContext& context, const std::string& label, std::shared_ptr<T>& resource)
    {
        std::string popupId = "Select " + label;
        bool resourceChanged = false;

        if (resource) {
            ImGui::Text("%s: %s", label.c_str(), resource->id().c_str());
            ImGui::SameLine();
            if (ImGui::Button(("Change##" + label).c_str())) {
                ImGui::OpenPopup(popupId.c_str());
            }
            
            ImGui::Indent();
            resource->drawInspector(context);
            ImGui::Unindent();
        } else {
            ImGui::Text("%s: (none)", label.c_str());
            ImGui::SameLine();
            if (ImGui::Button(("Select##" + label).c_str())) {
                ImGui::OpenPopup(popupId.c_str());
            }
        }
        

        if (ImGui::BeginPopup(popupId.c_str())) {
            ImGui::Text("Select a %s:", label.c_str()); 

            auto resources = context.resourceManager.getAll<T>();
            if(!resources.empty()){
                ImGui::Separator();         
                for (const auto& candidate : resources) {
                    if (ImGui::Selectable(candidate->id().c_str(), resource == candidate)) {
                        resource = candidate;
                        resourceChanged = true;
                        ImGui::CloseCurrentPopup();
                    }
                }
            }
            else {
                ImGui::Text("No %s resources available.", label.c_str());
            }
            ImGui::EndPopup();
        }

        return resourceChanged;
    }
};