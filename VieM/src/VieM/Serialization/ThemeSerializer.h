#pragma once
#include "VieM/UI/Theme.h"

namespace VieM::UI {


	class ThemeSerializer
	{
	public:
		ThemeSerializer(Theme* theme);
		void Serialize();
		void Deserialize();
	private:
		Theme* m_Theme;
	};



}