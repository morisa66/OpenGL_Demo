#include "common/m_window.h"
#include "common/m_shader.h"
#include "common/m_camera.h"
#include "utils/m_img.h"
#include "asset/data.h"

using namespace morisa;

m_window window(3, 3, 1920, 1080, "moirsa");
m_camera camera;
m_shader shader("shader/v.vert", "shader/f.frag");

int main()
{
    window.EnableZbuffer();
    window.disableCursor();
    window.bindMouseCallBack([](GLFWwindow* _window, double posX, double posY)
    { camera.mouseCallBack(_window, posX, posY); });

    window.bindScrollCallBack([](GLFWwindow* _window, double offsetX, double offsetY) 
	{ camera.scrollCallBack(_window, offsetX, offsetY); });

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	unsigned texture1 = m_load_img2D("asset/1.png");
	unsigned texture2 = m_load_img2D("asset/2.jpg");

	shader.active();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);


	while (!glfwWindowShouldClose(*window))
	{
		curTime = (float)glfwGetTime();
		camera.keyEvent(*window, curTime - lastTime);
		lastTime = curTime;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		shader.active();

		glm::mat4 view = camera.getViewMat();
		shader.setMat4("view", view);
		glm::mat4 projection = glm::perspective(glm::radians(camera.getFov()), (GLfloat)1920 / (GLfloat)1080, 0.1f, 100.0f);
		shader.setMat4("projection", projection);
	
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle) + (GLfloat)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glfwSwapBuffers(*window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

