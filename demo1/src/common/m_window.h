#ifndef __M_WINDOW_H__
#define __M_WINODW_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <exception>

#include "../_morisa.h"

#ifdef  USE_LOG
#include "../utils/m_log.h"
#endif 

MORISA_NAMESPACE_BEGIN

class m_window
{
public:
	using _reference = GLFWwindow&;
	using _pointer = GLFWwindow*;
	m_window(unsigned glfw_context_version_major = 3, 
			 unsigned glfw_context_version_minor = 3,
			 unsigned width = 800, 
		     unsigned height = 600,
			 const char* title = "OpenGL3.3")
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glfw_context_version_major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glfw_context_version_minor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
		window = glfwCreateWindow(width, height, title, NULL, NULL);

		check_window_create();

		glfwMakeContextCurrent(window);

		check_glad_init();

#ifdef USE_LOG
		m_log("create m_window.");
#endif 
	}

	~m_window()
	{
#ifdef USE_LOG
		m_log("destroy m_window.");
#endif 
	}

	bool is_ok() { return window != nullptr; };


	_pointer operator*() const noexcept { return window; }


	operator bool() const noexcept { return window; }

	void disableCursor() { glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); }

	void bindMouseCallBack(GLFWcursorposfun fun) { glfwSetCursorPosCallback(window, fun); }

	void bindScrollCallBack(GLFWscrollfun fun) { glfwSetScrollCallback(window, fun); }

	void EnableZbuffer(bool enable = true) 
	{
		if (enable) glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);
	}

private:
	_pointer window;

	void check_window_create()
	{
		if (!window)
		{
#ifdef USE_LOG
			m_log("Failed to create GLFW window.");
#endif 
			glfwTerminate();
		}
	}

	void check_glad_init()
	{

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
#ifdef USE_LOG
			m_log("Failed to initialize GLAD.");
#endif 
			glfwTerminate();
		}
	}
};

MORISA_NAMESPACE_END
#endif
