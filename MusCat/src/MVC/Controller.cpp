#include "Controller.h"
#include "View.h"
#include "Model.h"

using namespace std;


void Controller::OnUpdate()
{

}

void Controller::Run()
{
	m_View->ShowLoginWindow();
}

void Controller::OnLogin(const std::string& username, const std::string& password)
{
	m_View->HideLoginWindow();

	std::string msg;
	bool success = m_Model->Connect(username, password, msg);
	if (!success)
	{
		m_View->ShowErrorMessage(msg);
		Run();
		return;
	}
	m_View->HideErrorMessage();
	m_View->ShowMainWindow();

}
