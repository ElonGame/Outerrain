#include "mainwindow.h"
#include "imgui\imgui.h"
#include "imgui_opengl.h"

#include "app-stats.h"

void MainWindow::RenderGUI()
{
	/* Stats panel */
	ImGui::Begin("Statistics");
	std::stringstream cpuStream, gpuStream;
	AppStatistics::StopClock(cpuStream, gpuStream);
	ImGui::Text(cpuStream.str().data());
	ImGui::Text(gpuStream.str().data());
	ImGui::Text((std::string("Vertices : ")	 + std::to_string(AppStatistics::vertexCount)).c_str());
	ImGui::Text((std::string("Triangles : ") + std::to_string(AppStatistics::triangleCount)).c_str());
	ImGui::End();

	ImGui::Begin("Examples");
	if (ImGui::Button("Heightfield 1"))
		Heightfield1Scene();
	ImGui::Spacing();
	if (ImGui::Button("Heightfield 2"))
		Heightfield2Scene();
	ImGui::Spacing();
	if (ImGui::Button("NoiseField 1"))
		NoiseField1Scene();
	ImGui::Spacing();
	if (ImGui::Button("NoiseField 2"))
		NoiseField2Scene();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::SliderInt("Res", &settings.resolution, 128, 2048);
	ImGui::Spacing();
	if (ImGui::Button("Generate"))
	{
		ClearScene();
		GenerateTerrainFromSettings();
		UpdateMeshRenderer();
	}

	ImGui::End();
	ImGui::Render();
}