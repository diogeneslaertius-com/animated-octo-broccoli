#include <algorithm>
#include <iostream>
#include "Shader.h"
#include "Camera.h"
#include "Render.h"
#include "Mechanics.h"
#include "Model.h"
#include "Terrain.h"
#include "Beam.h"

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "GLEW initialization failed!\n";
		glfwTerminate();
		return -1;
	}

	std::cout << "Opengl ver " << glGetString(GL_VERSION) << std::endl;
	glfwSwapInterval(1);
	Shader shader("vertex.glsl", "fragment.glsl");

	Renderer::InitGLSettings();
	Renderer::CheckError();

	const float FOV = 45.0f;
	const float NEAR_PLANE = 0.1f;
	const float FAR_PLANE = 100.0f;

	// Инициализация камеры
	glm::vec3 cameraPos(0.0f, 0.0f, 3.0f);
	Camera camera(cameraPos);
	Mechanics mechanicsHandler(&camera);

	// Настройка ввода (захват мыши)
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowUserPointer(window, &mechanicsHandler);
	glfwSetCursorPosCallback(window, Mechanics::MouseCallback);

	std::vector<Beam> beams;

	// при выстреле
	

	Model myModel("assets/cube.obj");
	Terrain terrain(100.0f, 0.0f);
	float lastTime = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		float currentTime = (float)glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		mechanicsHandler.Update(deltaTime);
		// --- Логика обновления ---
		Renderer::UpdateViewport(window);
		glm::mat4 view = camera.ProcessInput(window, deltaTime);
		glm::mat4 projection = camera.GetProjectionMatrix(window, FOV, NEAR_PLANE, FAR_PLANE);

		
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::rotate(model, currentTime, glm::vec3(0.0f, 1.0f, 0.0f));

		// Логика стрельбы
		bool hit = false;
		bool shot = mechanicsHandler.CheckForShot(window);
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
			mechanicsHandler.RewindTime(2.0f); // отменить 2 секунды
		}

		// --- Рендеринг ---
		Renderer::Clear(0.259f, 0.667f, 1.0f, 1.0f);

		shader.Bind();
		shader.SetUniformMat4f("projection", projection);
		shader.SetUniformMat4f("view", view);
		shader.SetUniform1f("time", currentTime);

		// 1. Рендерим Террейн (статичный, зеленый)
		glm::mat4 terrain_model = glm::mat4(1.0f);
		shader.SetUniformMat4f("model", terrain_model);
		shader.SetUniform4f("u_Color", 0.1f, 0.8f, 0.1f, 1.0f); // Установили цвет для террейна
		terrain.Draw();

		// 2. Рендерим Кубик (вращающийся, синий)
		glm::mat4 cube_model = glm::mat4(1.0f);
		cube_model = glm::translate(cube_model, glm::vec3(0.0f, 0.0f, -5.0f));
		cube_model = glm::rotate(cube_model, currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
		shader.SetUniformMat4f("model", cube_model);
		shader.SetUniform4f("u_Color", 0.0f, 0.0f, 1.0f, 1.0f); // Установили цвет для кубика
		myModel.Draw();

		// Логика выстрела
		if (mechanicsHandler.CheckForShot(window)) {
			// Создаем луч с розовым цветом
			beams.emplace_back(camera.GetPosition(), camera.GetFront(), 20.0f, 0.2f, glm::vec3(1.0f, 0.2f, 1.0f));
		}

		// 3. Рендерим все лучи (каждый со своей позицией и цветом)
		for (auto it = beams.begin(); it != beams.end();) {
			it->Update(deltaTime);
			if (!it->IsAlive()) 
			{
				it = beams.erase(it);
			}
			else {
				// Прямо перед отрисовкой луча устанавливаем ЕГО матрицу и ЕГО цвет
				shader.SetUniformMat4f("model", it->GetModelMatrix()); // Нужен метод, который вернет матрицу модели луча
				glm::vec3 color = it->GetColor(); // Нужен метод, который вернет цвет луча
				shader.SetUniform4f("u_Color", color.r, color.g, color.b, 1.0f);

				it->Draw(); // Теперь луч нарисуется правильно
				++it;
			}
		}
		/*if (hit && shot)
			shader.SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
		else
			shader.SetUniform4f("u_Color", 0.0f, 0.0f, 1.0f, 1.0f);*/

		shader.SetUniform1f("time", currentTime);

		Renderer::CheckError();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}