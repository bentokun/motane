#include <graphics/window.h>

#include <functional>

namespace motane {
	namespace graphics {
		Window::Window() = default;
		Window::Window(const std::string title, const u32 width, const u32 height,
			const u32 x, const u32 y, Window *linkAntWindow, RenderAPI api,
			bool resizeable, bool showMouse)
			: _title(title), _width(width), _height(height), x(x), y(y), linkWindow(linkAntWindow), api(api)
		{
			if (api == RenderAPI::OpenGL) {
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
			}

			glfwWindowHint(GLFW_RESIZABLE, resizeable);
			glfwWindowHint(GLFW_CURSOR, showMouse);

			mainWindow = glfwCreateWindow(width, height, title.data(), nullptr, linkWindow != nullptr ? linkWindow->mainWindow : nullptr);

			glfwSetWindowPos(mainWindow, x, y);  
			
			std::function<void(GLFWwindow*, int, int)> poscallFunc = 
				std::bind(&Window::onWindowPosChange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

			decltype(poscallFunc) sizecallFunc =
				std::bind(&Window::onWindowSizeChange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

			glfwSetWindowPosCallback(mainWindow, *poscallFunc.target<GLFWwindowposfun>());
			glfwSetWindowSizeCallback(mainWindow, *sizecallFunc.target<GLFWwindowsizefun>());
		}

		std::string Window::title() const {
			return _title;
		}

		void Window::title(const std::string newTitle) {
			glfwSetWindowTitle(mainWindow, newTitle.data());

			_title = newTitle;
		}

		// Split the window into 2 other windows. 
		void Window::split() {}

		bool Window::close() const {
			return glfwWindowShouldClose(mainWindow);
		}

		void Window::close(const bool op) {
			glfwSetWindowShouldClose(mainWindow, static_cast<int>(op));
		}

		math::vec2 Window::extent() const {
			return math::vec2(_width, _height);
		}

		void Window::width(const u32 newWidth) { glfwSetWindowSize(mainWindow, newWidth, _height); }
		void Window::height(const u32 newHeight) { glfwSetWindowSize(mainWindow, _width, newHeight); }

		void Window::extent(const math::vec2 newExtent) { glfwSetWindowSize(mainWindow, static_cast<int>(newExtent.x), static_cast<int>(newExtent.y)); }

		math::vec2 Window::pos() const {
			return math::vec2(x, y);
		}

		void Window::pos(const math::vec2 newPos) {
			glfwSetWindowPos(mainWindow, newPos.x, newPos.y);
		}

		void Window::onWindowSizeChange(GLFWwindow *window, int newSizeX, int newSizeY) {
			_width = newSizeX;
			_height = newSizeY;
		}

		void Window::onWindowPosChange(GLFWwindow *window, int newPosX, int newPosY) {
			x = newPosX;
			y = newPosY;
		}
	}
}