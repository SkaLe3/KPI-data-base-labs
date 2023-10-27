#pragma once
#include <memory>
#include <string>
class View;
class Model;

class Controller : public std::enable_shared_from_this<Controller>
{
public:
	Controller(const std::shared_ptr<Model>& model, const std::shared_ptr<View>& view) 
		: m_Model(model), m_View(view) {}
	void OnUpdate();
	void Run();
	void OnLogin(const std::string& username, const std::string& password);

private:
	std::shared_ptr<Model> m_Model;
	std::shared_ptr<View> m_View;


};

