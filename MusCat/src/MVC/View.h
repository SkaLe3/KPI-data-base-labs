#pragma once
#include <memory>
#include <string>

class Controller;

class View
{
public:
	View() {}
	void OnRender();
	void AddListener(const std::shared_ptr<Controller>& controller);
	// Login
	void ShowLoginWindow() { m_LoginWindowEnabled = true; }
	void HideLoginWindow() { m_LoginWindowEnabled = false; }

	// Error
	void ShowErrorMessage(const std::string& text) { m_ErrorMessageText = text; m_ErrorMessage = true; }
	void HideErrorMessage() { m_ErrorMessage = false; }
	
	// Main Window
	void ShowMainWindow() { m_MainWindowEnabled = true; }
	void HideMainWindow() { m_MainWindowEnabled = false; }
private:
	void LoginWindowRender();
	void ErrorMessageRender();
	void MainWindowRender();
private:
	std::shared_ptr<Controller> m_Controller;
	bool m_LoginWindowEnabled = false;
	char m_Username[32] = "";
	char m_Password[32] = "";

	std::string m_ErrorMessageText;
	bool m_ErrorMessage = false;

	bool m_MainWindowEnabled = false;


	friend class Controller;
};

