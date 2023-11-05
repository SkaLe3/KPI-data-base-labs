#pragma once
#include <memory>

class Controller;
class IWindowUI
{
public:
	virtual void OnRender() = 0;
	virtual void Show() { m_NotHidden = true; }
	virtual void Hide() { m_NotHidden = false; }
	virtual bool IsNotHidden() { return m_NotHidden; }
	virtual void SetCallbackObject(std::shared_ptr<Controller>& callback) { m_Callback = callback; }
protected:
	std::shared_ptr<Controller> m_Callback;
	bool m_NotHidden = false;
};