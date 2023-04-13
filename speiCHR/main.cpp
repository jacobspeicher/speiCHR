#define STB_IMAGE_RESIZE_IMPLEMENTATION

#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imfilebrowser.h"

#include "include/stb_image.h"
#include "include/stb_image_resize.h"

#include "texture_utility.h"

#include <string>
#include <algorithm>

#define NES_WIDTH 256
#define NES_HEIGHT 240

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

GLFWwindow* Init();
void UI();
void RenderLoop(GLFWwindow* window);

int main(int argc, char* argv) {
	GLFWwindow* window = Init();

	RenderLoop(window);
}

GLFWwindow* Init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1920, 1080, "speiCHR", NULL, NULL);
	if (window == NULL) {
		spdlog::error("window creation failed");
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		spdlog::error("glad failed to load process");
		glfwTerminate();
		return NULL;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	ImGui::StyleColorsDark();

	return window;
}

void RenderLoop(GLFWwindow* window) {
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// start new imgui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		UI();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void UI() {
	static ImGui::FileBrowser fileDialog;
	static std::string image_path = "";

	static int width = 0;
	static int height = 0;

	static GLuint texture = 0;
	static GLuint texture_r = 0;
	static GLuint texture_g = 0;
	static GLuint texture_b = 0;
	static GLuint texture_palatte = 0;
	static unsigned char* initial_data;
	static unsigned char data[NES_WIDTH * NES_HEIGHT * 4];
	static unsigned char data_r[NES_WIDTH * NES_HEIGHT * 4];
	static unsigned char data_g[NES_WIDTH * NES_HEIGHT * 4];
	static unsigned char data_b[NES_WIDTH * NES_HEIGHT * 4];
	static unsigned char data_palatte[NES_WIDTH * NES_HEIGHT * 4];
	
	Color one = { 0, 0, 0 };
	Color two = { 255, 0, 0 };
	Color three = { 0, 255, 0 };
	Color four = { 0, 0, 255 };
	//static Color palatte[4] = { one, two, three, four };

	ImGui::ShowDemoWindow();
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Import Image")) {
				fileDialog.Open();
			}
			if (ImGui::MenuItem("Export CHR")) {

			}
			ImGui::EndMenu();
		}

		if (image_path == "") { ImGui::BeginDisabled(); }
		if (ImGui::Button("Convert")) {

		}
		if (image_path == "") { ImGui::EndDisabled(); }

		ImGui::EndMainMenuBar();
	}

	fileDialog.Display();

	if (fileDialog.HasSelected()) {
		spdlog::info(fileDialog.GetSelected().string());
		image_path = fileDialog.GetSelected().string();

		bool ret = TextureUtility::LoadImageDataFromFile(image_path.c_str(), &initial_data, &width, &height);
		ret = stbir_resize_uint8(initial_data, width, height, 0, data, NES_WIDTH, NES_HEIGHT, 0, 4);
		ret = stbir_resize_uint8(initial_data, width, height, 0, data_r, NES_WIDTH, NES_HEIGHT, 0, 4);
		ret = stbir_resize_uint8(initial_data, width, height, 0, data_g, NES_WIDTH, NES_HEIGHT, 0, 4);
		ret = stbir_resize_uint8(initial_data, width, height, 0, data_b, NES_WIDTH, NES_HEIGHT, 0, 4);
		ret = stbir_resize_uint8(initial_data, width, height, 0, data_palatte, NES_WIDTH, NES_HEIGHT, 0, 4);

		TextureUtility::LoadTextureFromData(data, &texture);
		TextureUtility::ProcessRGB(data_r, 1, 0, 0);
		TextureUtility::LoadTextureFromData(data_r, &texture_r);
		TextureUtility::ProcessRGB(data_g, 0, 1, 0);
		TextureUtility::LoadTextureFromData(data_g, &texture_g);
		TextureUtility::ProcessRGB(data_b, 0, 0, 1);
		TextureUtility::LoadTextureFromData(data_b, &texture_b);

		Color* palatte = TextureUtility::ProcessGeneratePalatte(data);
		TextureUtility::ProcessToPalatte(data_palatte, palatte);
		TextureUtility::LoadTextureFromData(data_palatte, &texture_palatte);

		fileDialog.ClearSelected();
	}

	if (image_path != "") {
		ImGui::Begin("4 Images (original, r, g, b)");
		ImGui::Text("size = %d x %d", NES_WIDTH, NES_HEIGHT);

		ImGui::BeginGroup();
		ImGui::Text("pointer = %p", texture);
		ImGui::Text("data[0] : %d data[1] : %d data[2] : %d data[3] : %d", 
			data[0], data[1], data[2], data[3]);
		ImGui::Image((void*)(intptr_t)texture, ImVec2(NES_WIDTH, NES_HEIGHT));
		ImGui::EndGroup();
		
		ImGui::SameLine();
		
		ImGui::BeginGroup();
		ImGui::Text("pointer = %p", texture_r);
		ImGui::Text("data[0] : %d data[1] : %d data[2] : %d data[3] : %d",
			data_r[0], data_r[1], data_r[2], data_r[3]);
		ImGui::Image((void*)(intptr_t)texture_r, ImVec2(NES_WIDTH, NES_HEIGHT));
		ImGui::EndGroup();

		ImGui::BeginGroup();
		ImGui::Text("pointer = %p", texture_g);
		ImGui::Text("data[0] : %d data[1] : %d data[2] : %d data[3] : %d",
			data_g[0], data_g[1], data_g[2], data_g[3]);
		ImGui::Image((void*)(intptr_t)texture_g, ImVec2(NES_WIDTH, NES_HEIGHT));
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		ImGui::Text("pointer = %p", texture_b);
		ImGui::Text("data[0] : %d data[1] : %d data[2] : %d data[3] : %d",
			data_b[0], data_b[1], data_b[2], data_b[3]);
		ImGui::Image((void*)(intptr_t)texture_b, ImVec2(NES_WIDTH, NES_HEIGHT));
		ImGui::EndGroup();

		ImGui::End();

		ImGui::Begin("4 Color Palatte");
		ImGui::Text("size = %d x %d", NES_WIDTH, NES_HEIGHT);

		ImGui::BeginGroup();
		ImGui::Text("pointer = %p", texture_palatte);
		ImGui::Text("data[0] : %d data[1] : %d data[2] : %d data[3] : %d",
			data_palatte[0], data_palatte[1], data_palatte[2], data_palatte[3]);
		ImGui::Image((void*)(intptr_t)texture_palatte, ImVec2(NES_WIDTH, NES_HEIGHT));
		ImGui::EndGroup();
		
		ImGui::End();
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
}