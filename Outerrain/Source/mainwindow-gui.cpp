#include "mainwindow.h"
#include "imgui\imgui.h"
#include "imgui_opengl.h"
#include "app-stats.h"


static std::string PrettyFormatNumbers(int number)
{
	auto str = std::to_string(number);
	int charCount = str.size();
	if (charCount < 4)
		return str;
	if (charCount == 4)
		str.insert(str.begin() + 1, ' ');
	if (charCount == 5)
		str.insert(str.begin() + 2, ' ');
	if (charCount == 6)
		str.insert(str.begin() + 3, ' ');
	if (charCount == 7)
	{
		str.insert(str.begin() + 1, ' ');
		str.insert(str.begin() + 5, ' ');
	}
	if (charCount == 8)
	{
		str.insert(str.begin() + 2, ' ');
		str.insert(str.begin() + 6, ' ');
	}
	return str;
}

void MainWindow::RenderGUI()
{
	/* Stats panel */
	ImGui::Begin("Statistics");
	std::stringstream cpuStream, gpuStream;
	AppStatistics::StopClock(cpuStream, gpuStream);
	ImGui::Text(cpuStream.str().data());
	ImGui::Text(gpuStream.str().data());
	ImGui::Text((std::string("Vertices : ") + PrettyFormatNumbers(AppStatistics::vertexCount)).c_str());
	ImGui::Text((std::string("Triangles : ") + PrettyFormatNumbers(AppStatistics::triangleCount)).c_str());
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