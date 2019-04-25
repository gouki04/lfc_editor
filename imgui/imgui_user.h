namespace lfc
{
    struct MatchTime;
}

namespace ImGui
{
    IMGUI_API void          FixedColumns(int count, ...);                                       // 
    IMGUI_API void          FixedColumnsV(int count, va_list args);                             // 
    IMGUI_API void          ShowColumnsNorm();                                                  // 

    IMGUI_API void          LabelTextColored(const ImVec4& col, const char* label, const char* fmt, ...) IM_PRINTFARGS(2);    // display text+label aligned the same way as value+label widgets
    IMGUI_API void          LabelTextColoredV(const ImVec4& col, const char* label, const char* fmt, va_list args);    // display text+label aligned the same way as value+label widgets

    IMGUI_API bool          MatchTimeEdit(const char* label, lfc::MatchTime &time);

    IMGUI_API bool          DateEdit(const char* label, int *year, int *month, int *day);
}