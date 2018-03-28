#include "mainwindow.h"
#include "imgui\imgui.h"
#include "imgui_opengl.h"

void MainWindow::RenderGUI()
{
	/* Stats panel */
	ImGui::Begin("Statistics");
	std::stringstream cpuStream, gpuStream;
	AppTime::StopClock(cpuStream, gpuStream);
	ImGui::Text(cpuStream.str().data());
	ImGui::Text(gpuStream.str().data());
	ImGui::Text((std::string("Vertices : ") + std::to_string(hfMesh->VerticeCount())).c_str());
	ImGui::Text((std::string("Triangles : ") + std::to_string(hfMesh->IndicesCount())).c_str());
	ImGui::End();

	ImGui::Begin("Examples");
	if (ImGui::Button("Heightfield 1"))
		ExampleScene1();
	ImGui::Spacing();
	if (ImGui::Button("Heightfield 2"))
		ExampleScene2();
	ImGui::Spacing();
	if (ImGui::Button("NoiseField 1"))
		ExampleScene3();
	ImGui::Spacing();
	if (ImGui::Button("NoiseField 2"))
		ExampleScene4();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::SliderInt("Resolution", &settings.resolution, 128, 4096);
	if (ImGui::Button("Generate"))
	{
		ClearScene();
		GenerateTerrainFromSettings();
		UpdateMeshRenderer();
	}

	ImGui::End();
	ImGui::Render();
}