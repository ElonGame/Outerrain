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
	size_t frameCount = instanceRenderer->GetFrameCount();
	size_t vertexCount = hfMesh->VertexCount();
	size_t triangleCount = hfMesh->TriangleCount();
	ImGui::Text((std::string("Vertices : ")	 + std::to_string(vertexCount)).c_str());
	ImGui::Text((std::string("Triangles : ") + std::to_string(triangleCount)).c_str());
	ImGui::Text((std::string("Instances : ") + std::to_string(frameCount)).c_str());
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