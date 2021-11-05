#include "Application.h"
#include "PanelAssets.h"

#include "Input.h"

#include "mmgr/mmgr.h"

AssetsPanel::AssetsPanel()
{
    active = true;
    scroll = 0;
}

AssetsPanel::~AssetsPanel()
{
}

void AssetsPanel::OnRender(float dt)
{
    if(ImGui::Begin("Assets", &active))
    {
		if (ImGui::IsWindowHovered())
		{
			if (app->input->GetMouseZ() < 0)
			{
				scroll = ImGui::GetScrollY();
				scroll -= app->input->GetMouseZ();
				ImGui::SetScrollY(scroll);
			}
			if (app->input->GetMouseZ() > 0)
			{
				scroll = ImGui::GetScrollY();
				scroll -= app->input->GetMouseZ();
				ImGui::SetScrollY(scroll);
			}
			if (scroll <= 0) scroll = 0;
			else if (scroll >= ImGui::GetScrollMaxY()) scroll = ImGui::GetScrollMaxY();
		}
    }
	ImGui::End();
}