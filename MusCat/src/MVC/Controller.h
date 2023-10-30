#pragma once
#include <memory>
#include <string>
#include "View.h"
class Model;

class Controller : public std::enable_shared_from_this<Controller>
{
public:
	Controller(const std::shared_ptr<Model>& model, const std::shared_ptr<View>& view)
		: m_Model(model), m_View(view) { }

	~Controller() { m_View->RemoveAllWindows(); }
	void OnUpdate();
	void Run();
	void OnLogin(const std::string& username, const std::string& password);
	std::string& GetErrorMessage() { return m_ErrorMessageText; }
public:
	void CreateWindows();
	template<class W>
	void ShowWindow()
	{
		m_View->ShowWindow<W>();
	}

	template<class W>
	void HideWindow()
	{
		m_View->HideWindow<W>();
	}
private:
	std::shared_ptr<Model> m_Model;
	std::shared_ptr<View> m_View;

	std::string m_ErrorMessageText;


};

