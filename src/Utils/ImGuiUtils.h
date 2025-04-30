#pragma once

#include <Skateboard.h>

// https://stackoverflow.com/questions/64653747/how-to-center-align-text-horizontally
inline void TextCentered(const std::string& text, const ImVec4& colour = ImVec4(1,1,1,1)) {
    auto windowWidth = ImGui::GetWindowSize().x;
    auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::TextColored(colour, text.c_str());
}