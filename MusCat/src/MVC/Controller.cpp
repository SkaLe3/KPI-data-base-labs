#include "Controller.h"
#include "View.h"
#include "Model.h"
#include "Windows/WLogin.h"
using namespace std;


void Controller::OnUpdate()
{

}

void Controller::Run()
{
	CreateWindows();
	ShowWindow<WLogin>();
}

void Controller::OnLogin(const std::string& username, const std::string& password)
{
	HideWindow<WLogin>();

	bool success = m_Model->Connect(username, password, m_ErrorMessageText);
	if (!success)
	{
		m_View->ShowErrorMessage();
		ShowWindow<WLogin>();
		return;
	}
	m_View->HideErrorMessage();
	m_View->ShowMainWindow();

	if (!m_Model->CreateTables(m_ErrorMessageText))
	{
		m_View->ShowErrorMessage();
		return;
	}
	m_View->HideErrorMessage();

}

void Controller::CreateWindows()
{
	m_View->AddWindow<WLogin>();
}
