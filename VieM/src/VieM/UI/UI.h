#pragma once

#include "Theme.h"
#include "VieM/Core/Image.h"


#include <imgui.h>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <functional>

namespace VieM {
	class Window;
}
namespace VieM::UI {



	class TitleBar
	{
	public:
		TitleBar(Window* window);
		~TitleBar() {
			m_AppHeaderIcon.reset();
			m_IconClose.reset();
			m_IconMinimize.reset();
			m_IconMaximize.reset();
			m_IconRestore.reset();
		}

		void Draw(float& outTitlebarHeight);
		void DrawMenubar();
		const bool IsHovered() const { return m_TitleBarHovered; }
		bool& GetHovered(){ return m_TitleBarHovered; }


		void SetMenubarCallback(const std::function<void()>& menubarCallback) { m_MenubarCallback = menubarCallback; }
		const std::function<void()>& GetMenubarCallback() const { return m_MenubarCallback; }
	public:
		//Titlebar
		float Height = 30.0f; // vertical size
		float VerticalOffset = -5.0f; // Icons, menubar, logo
		// Logo
		int LogoWidth = 0;
		int LogoHeight = 0;
		glm::vec2 LogoOffsetValues {16.0f, 5.0f};

		// Other
		glm::vec2 MenubarOffsetValues = { 16.0f, 6.0f };
		float ButtonsAreaWidth = 94;
		float VerticalTitleOffset = 2.0f;

		glm::vec2 ButtonSize = { 14.0f, 14.0f };
		// Add color balues for button, title
	public:
		std::shared_ptr<VieM::Image> m_AppHeaderIcon;
		std::shared_ptr<VieM::Image> m_IconClose;
		std::shared_ptr<VieM::Image> m_IconMinimize;
		std::shared_ptr<VieM::Image> m_IconMaximize;
		std::shared_ptr<VieM::Image> m_IconRestore;
	private:
		Window* m_WindowHandle;
		bool m_TitleBarHovered = false;
		std::function<void()> m_MenubarCallback;
		friend class Menubar;
	};



	class Settings
	{
	public:
		Settings() = default;
		~Settings();
		// Style
		void SetWindowRounding();

		// Fonts
		ImFont* GetFont(std::string& fontName);
		void SetDefaultFont(std::string& fontName);
		void LoadFont(std::string& fontName, std::string& filePath, float fontSize);

		// Themes
		bool SetTheme(const std::string& themeName);
		void AddTheme(Theme* theme, const std::string& themeName);
		Theme* GetCurrentTheme();

	public:
		std::unordered_map<std::string, ImFont*> GetFontsStorage() { return m_Fonts; }
		std::unordered_map<std::string, Theme*> GetThemesStorage() { return m_Themes; }
	private:
		std::unordered_map<std::string, ImFont*> m_Fonts;
		std::unordered_map<std::string, Theme*> m_Themes;

		std::string m_CurrentTheme;



	};

}