#pragma once
#include <memory>
#include <string>
#include <list>
#include <Windows/WindowUI.h>
#include "MVC/Tables.h"
//temp
#include "VieM/Core/Log.h"
class Controller;

class View
{
public:
	View();
	void OnRender();
	void AddListener(const std::shared_ptr<Controller>& controller);
	
public:
	template<class W, class T>
	void Update(std::shared_ptr<T> data)
	{
		for (std::shared_ptr<IWindowUI> window : m_Windows)
		{
			std::shared_ptr<W> win = std::dynamic_pointer_cast<W>(window);
			if (win)
				win->Update(data);
		}
	}

	template<class W>
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
	std::shared_ptr<IWindowUI>&  GetWindow()
	{
		for (std::shared_ptr<IWindowUI>& window : m_Windows)
			if (std::dynamic_pointer_cast<W>(window))
				return window;
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
	void MainWindowRender();
	void ControlPanelRender();
private:
	std::shared_ptr<Controller> m_Controller;
	std::list<std::shared_ptr<IWindowUI>> m_Windows;

	friend class Controller;
};

