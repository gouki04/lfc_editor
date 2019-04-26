#include "data\MatchTime.hpp"

//-------------------------------------------------------------------------
// Forward Declarations
//-------------------------------------------------------------------------

static void             PushMultiItemsWidthsWithPrefix(float prefix, int components, float w_full = 0.0f);

static void PushMultiItemsWidthsWithPrefix(float prefix, int components, float w_full)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    const ImGuiStyle& style = GImGui->Style;
    if (w_full <= 0.0f)
        w_full = ImGui::CalcItemWidth();
    w_full = w_full - prefix - style.ItemInnerSpacing.x;
    const float w_item_one = ImMax(1.0f, (float)(int)((w_full - (style.ItemInnerSpacing.x) * (components - 1)) / (float)components));
    const float w_item_last = ImMax(1.0f, (float)(int)(w_full - (w_item_one + style.ItemInnerSpacing.x) * (components - 1)));
    window->DC.ItemWidthStack.push_back(w_item_last);
    for (int i = 0; i < components - 1; i++)
        window->DC.ItemWidthStack.push_back(w_item_one);
    window->DC.ItemWidthStack.push_back(prefix);
    window->DC.ItemWidth = window->DC.ItemWidthStack.back();
}

// Add a label+text combo aligned to other label+value widgets
void ImGui::LabelTextColoredV(const ImVec4& col, const char* label, const char* fmt, va_list args)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const float w = CalcItemWidth();

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImRect value_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2));
    const ImRect total_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w + (label_size.x > 0.0f ? style.ItemInnerSpacing.x : 0.0f), style.FramePadding.y * 2) + label_size);
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, NULL))
        return;

    // Render
    const char* value_text_begin = &g.TempBuffer[0];
    const char* value_text_end = value_text_begin + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
    PushStyleColor(ImGuiCol_Text, col);
    RenderTextClipped(value_bb.Min, value_bb.Max, value_text_begin, value_text_end, NULL, ImVec2(0.0f, 0.5f));
    PopStyleColor();
    if (label_size.x > 0.0f)
        RenderText(ImVec2(value_bb.Max.x + style.ItemInnerSpacing.x, value_bb.Min.y + style.FramePadding.y), label);
}

void ImGui::LabelTextColored(const ImVec4& col, const char* label, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    LabelTextColoredV(col, label, fmt, args);
    va_end(args);
}

bool ImGui::MatchTimeEdit(const char* label, lfc::MatchTime &time)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    bool value_changed = false;
    BeginGroup();
    PushID(label);
    PushMultiItemsWidthsWithPrefix(10.0f, 2);
    //PushMultiItemsWidths(3);

    ImGui::PushID(0); value_changed |= ImGui::Checkbox("##v", &time.is_penalty_shootout); SameLine(0, g.Style.ItemInnerSpacing.x); PopID(); PopItemWidth();
    ImGui::PushID(1); value_changed |= ImGui::SliderInt("##v", &time.local_time, 0, 120); SameLine(0, g.Style.ItemInnerSpacing.x); PopID(); PopItemWidth();
    ImGui::PushID(2); value_changed |= ImGui::SliderInt("##v", &time.injury_time, 0, 15); SameLine(0, g.Style.ItemInnerSpacing.x); PopID(); PopItemWidth();

    PopID();

    TextUnformatted(label, FindRenderedTextEnd(label));
    EndGroup();

    return value_changed;
}

bool ImGui::DateEdit(const char* label, int *year, int *month, int *day)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const float w_extra = 0.0f; // ColorSquareSize() + style.ItemInnerSpacing.x;
    const float w_items_all = CalcItemWidth() - w_extra;
    const char* label_display_end = FindRenderedTextEnd(label);

    const int components = 3;

    BeginGroup();
    PushID(label);

    // Convert to the formats we need
    int i[3] = { *year, *month, *day };
    int min_i[3] = { 1980, 1, 1 };
    int max_i[3] = { 2018, 12, 31 };

    bool value_changed = false;
    bool value_changed_as_float = false;

    // RGB/HSV 0..255 Sliders
    const float w_item_one = ImMax(1.0f, (float)(int)((w_items_all - (style.ItemInnerSpacing.x) * (components - 1)) / (float)components));
    const float w_item_last = ImMax(1.0f, (float)(int)(w_items_all - (w_item_one + style.ItemInnerSpacing.x) * (components - 1)));

    const char* ids[3] = { "##YEAR", "##MONTH", "##DAY" };

    PushItemWidth(w_item_one);
    for (int n = 0; n < components; n++)
    {
        if (n > 0)
            SameLine(0, style.ItemInnerSpacing.x);
        if (n + 1 == components)
            PushItemWidth(w_item_last);
        value_changed |= DragInt(ids[n], &i[n], 1.0f, min_i[n], max_i[n]);
    }
    PopItemWidth();
    PopItemWidth();

    if (label != label_display_end)
    {
        SameLine(0, style.ItemInnerSpacing.x);
        TextUnformatted(label, label_display_end);
    }

    if (value_changed)
    {
        *year = i[0];
        *month = i[1];
        *day = i[2];
    }

    PopID();
    EndGroup();

    return value_changed;
}

void ImGui::FixedColumns(int count, ...)
{
    va_list args;
    va_start(args, count);
    FixedColumnsV(count, args);
    va_end(args);
}

void ImGui::FixedColumnsV(int count, va_list args)
{
    Columns(count);

    ImGuiWindow* window = GetCurrentWindowRead();
    int columns_count = GetColumnsCount();

    window->DC.CurrentColumns->Columns[0].OffsetNorm = 0.0;
    for (int column_index = 1; column_index < columns_count + 1; column_index++)
    {
        double t = va_arg(args, double);
        window->DC.CurrentColumns->Columns[column_index].OffsetNorm =
            window->DC.CurrentColumns->Columns[column_index - 1].OffsetNorm + t;
    }
}

void ImGui::ShowColumnsNorm()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    int columns_count = GetColumnsCount();

    for (int column_index = 1; column_index < columns_count + 1; column_index++)
    {
        ImGui::Text("%.2f", window->DC.CurrentColumns->Columns[column_index].OffsetNorm - window->DC.CurrentColumns->Columns[column_index - 1].OffsetNorm);
        ImGui::NextColumn();
    }
}