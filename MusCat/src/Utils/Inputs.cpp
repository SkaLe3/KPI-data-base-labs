#include "Inputs.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "misc/cpp/imgui_stdlib.h"
#include <regex>
#include <memory>
#include <iostream>
namespace Inputs
{

	bool InputDate(const char* label, std::string& date)
	{
		bool edited = false;

		auto validator = [](ImGuiInputTextCallbackData* data)
		{
			data->BufDirty = true;
			int writeIdx = 0;
			// keep only digits and dashes
			for (int readIdx = 0; readIdx < data->BufTextLen; readIdx++) {
				if (std::isdigit(data->Buf[readIdx]) || (data->Buf[readIdx] == '-' && (readIdx == 4 || readIdx == 7))) {
					data->Buf[writeIdx] = data->Buf[readIdx];
					writeIdx++;
				}
				else
					data->CursorPos--;
			}
			data->Buf[writeIdx] = '\0';
			data->BufTextLen = writeIdx;

			// validate dashes
			int readIdx = 0;
			while (readIdx < data->BufTextLen)
			{
				if (data->Buf[readIdx] != '-' && (readIdx == 4 || readIdx == 7))
				{
					data->BufTextLen++;
					data->Buf[data->BufTextLen] = '\0';
					data->CursorPos++;
					for (int i = data->BufTextLen - 1; i > readIdx; i-- )
					{
						data->Buf[i] = data->Buf[i - 1];
					}
					data->Buf[readIdx] = '-';
				}
				readIdx++;
			}
			
			if (data->BufTextLen > 10)
			{
				data->BufTextLen = 10;
				data->Buf[data->BufTextLen] = '\0';
			}


			return 0;

		};




		static ImVec4 color = { 0.7f, 0.5f, 0.5f, 1.0f };
		// Check for valid characters while typing
		ImGui::PushID(label);
		ImGui::PushStyleColor(ImGuiCol_Text, color);

		if (ImGui::InputText(label, &date, ImGuiInputTextFlags_CallbackAlways, validator, &date))
		{
			edited = true;

			ImGui::ClearActiveID();
			ImGui::SetKeyboardFocusHere(-1);
		}
		static const std::regex validDateFormat(R"(^\d{4}-\d{2}-\d{2}$)");
		// Validate the date format after each character input
		if (edited)
		{
			if (!std::regex_match(date, validDateFormat))
			{
				color = { 0.7f, 0.5f, 0.5f, 1.0f };
			}
			else 
				color = { 0.7f, 0.8f, 0.7f, 1.0f };
		}
		

		ImGui::PopStyleColor();
		ImGui::PopID();

		return edited;
	}


}