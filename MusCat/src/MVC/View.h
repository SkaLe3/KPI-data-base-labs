#pragma once
#include <memory>
#include <string>
#include <list>
#include <Windows/WindowUI.h>

class Controller;

class View
{
public:
	View() {}
	void OnRender();
	void AddListener(const std::shared_ptr<Controller>& controller);

	// Error
	void ShowErrorMessage() { m_ErrorMessage = true; }
	void HideErrorMessage() { m_ErrorMessage = false; }
	
	// Main Window
	void ShowMainWindow() { m_MainPanelsEnabled = true; }
	void HideMainWindow() { m_MainPanelsEnabled = false; }
public:
	template <class W>
	void ShowWindow()
	{
		for (std::shared_ptr<IWindowUI> window : m_Windows)
			if (std::dynamic_pointer_cast<W>(window))
				window->Show();
	}
	template<class W>
	void HideWindow()
	{
		for (std::shared_ptr<IWindowUI> window : m_Windows)
			if (std::dynamic_pointer_cast<W>(window))
				window->Hide();
	}
	template<class W>
	void AddWindow()
	{
		std::shared_ptr<IWindowUI> window = std::make_shared<W>();
		window->SetCallbackObject(m_Controller);
		m_Windows.push_back(window);
	}
	void RemoveAllWindows();

private:
	void ErrorMessageRender();
	void MainWindowRender();
	void ControlPanelRender();
private:
	std::shared_ptr<Controller> m_Controller;
	
	bool m_ErrorMessage = false;

	bool m_MainPanelsEnabled = false;

	std::list<std::shared_ptr<IWindowUI>> m_Windows;

	friend class Controller;
};

