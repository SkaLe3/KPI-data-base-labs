#pragma once
#include <VieM/Core/Image.h>
#include <memory>



class TilesPanel
{

public:
	TilesPanel() { }
	void DrawTile(bool* selected);
	void OnRender();
public:
	bool m_Enabled = false;

};

