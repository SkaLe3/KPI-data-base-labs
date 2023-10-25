#include "ThemeSerializer.h"
#include "VieM/Core/Log.h"

namespace VieM::UI {



	ThemeSerializer::ThemeSerializer(Theme* theme)
		: m_Theme(theme)
	{

	}

	void ThemeSerializer::Serialize()
	{
		VM_TRACE("Theme Serialized");
	}

	void ThemeSerializer::Deserialize()
	{
		VM_TRACE("Theme Deserialized");
	}

}