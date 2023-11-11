#pragma once
#include <memory>
#include <string>
#include <vector>

class Controller;
class IWindowUI
{
public:
	virtual void OnRender() = 0;
	virtual void Show() { m_NotHidden = true; }
	virtual void Hide() { m_NotHidden = false; }
	virtual bool IsNotHidden() { return m_NotHidden; }
	virtual void SetCallbackObject(std::shared_ptr<Controller>& callback) { m_Callback = callback; }
	virtual void Update(std::vector<std::string> data) {}
protected:
	std::shared_ptr<Controller> m_Callback;
	bool m_NotHidden = false;
};