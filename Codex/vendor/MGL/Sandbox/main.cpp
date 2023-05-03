#include <cstdlib>
#include <cstdint>
#include <SDL.h>
#include <glad/glad.h>
#include <SDL_opengl.h>
#include <memory>
#include <MGL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Handy macros.
#ifdef _DEBUG
// Define this will make MGL check for OpenGL errors before making any OpenGL calls. 
// This will slow down OpenGL calls so define it only when in Debug mode.
#define MGL_DEBUG 
#endif

#define WIDTH	600
#define HEIGHT	600

#define MAT4(m, x, y) m[y * 4 + x]						// Convert four by four matrix to one dimensional array.
#define PI 3.14159265358979323							// The ratio between circle's diameter and the circumference. This is used to convert from degrees to radians.
#define TO_RADF(d) (float)((float)(d) * (PI / 180.0f))	// Convert degress to radians.
#define ARRAY_LENGTH(x) sizeof(x) / sizeof(x[0])		// Get array length.

void CreateSDLWindow(SDL_Window** window, SDL_GLContext& context, const char* title, int width, int height)
{
	// Initialize SDL.
	SDL_Init(SDL_INIT_EVERYTHING);

	// Set the OpenGL version and profile, also enable double buffering.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Create the window.
	*window = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);

	// Create GL context from the window.
	context = SDL_GL_CreateContext(*window);

	// Load GLAD.
	if (gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		printf("OpenGL loaded\n");
		printf("Vendor:   %s\n", glGetString(GL_VENDOR));
		printf("Renderer: %s\n", glGetString(GL_RENDERER));
		printf("Version:  %s\n", glGetString(GL_VERSION));
	}
	
	// Update the viewport when the window gets resized.
	SDL_AddEventWatch([](void* userdata, SDL_Event* event) -> int 
	{
		if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED) 
		{
			// Get the new window size.
			int width = event->window.data1;
			int height = event->window.data2;
			glViewport(0, 0, width, height);
		}
		return 0;
	}, nullptr);
	
	// Enable VSync
	SDL_GL_SetSwapInterval(2);
}

int main(int argc, char* argv[])
{
	SDL_Window* window = nullptr;
	SDL_GLContext gl_context;
	SDL_Event sdl_event;

	// Function to create the window, the OpenGL context and handle window resizing.
	CreateSDLWindow(&window, gl_context, "Cube Party", WIDTH, HEIGHT);
	
	// Initialize the basic renderer that MGL comes with.
	auto renderer = std::unique_ptr<mgl::Renderer>(new mgl::Renderer(WIDTH, HEIGHT));
	renderer->SetClearColour(0.1f, 0.1f, 0.1f, 1.0f);

	// Create a new shader from a file and then bind it.
	auto shader = std::unique_ptr<mgl::Shader>(new mgl::Shader("default.glsl"));
	shader->Bind();

	// Load the texture and bind it at texture slot 0.
	auto aws_tex = std::unique_ptr<mgl::Texture>(new mgl::Texture("box.png"));
	aws_tex->Bind(0);

	// Create and bind the vertex array.
	auto vao = std::unique_ptr<mgl::VertexArray>(new mgl::VertexArray());
	vao->Bind();

	// The cube's vertex data.
	float vertex_buffer_data[] =
	{
		// Vertex position			// Colour						// Texture coords

		// Front Face
		-0.5f,	-0.5f,	0.5f,		1.0f, 0.0f, 0.0f, 1.0f, 		0.0f, 0.0f,
		-0.5f,	0.5f,	0.5f,		0.0f, 1.0f, 0.0f, 1.0f,			0.0f, 1.0f,
		0.5f,	0.5f,	0.5f,		0.0f, 0.0f, 1.0f, 1.0f,			1.0f, 1.0f,
		
		-0.5f,	-0.5f,	0.5f,		1.0f, 0.0f, 0.0f, 1.0f,			0.0f, 0.0f,
		0.5f,	-0.5f,	0.5f,		1.0f, 1.0f, 0.0f, 1.0f,			1.0f, 0.0f,
		0.5f,	0.5f,	0.5f,		0.0f, 0.0f, 1.0f, 1.0f, 		1.0f, 1.0f,
		
		// Back Face
		-0.5f,	-0.5f,	-0.5f,		1.0f, 0.0f, 0.0f, 1.0f, 		0.0f, 0.0f,
		-0.5f,	0.5f,	-0.5f,		0.0f, 1.0f, 0.0f, 1.0f,			0.0f, 1.0f,
		0.5f,	0.5f,	-0.5f,		0.0f, 0.0f, 1.0f, 1.0f,			1.0f, 1.0f,
		
		-0.5f,	-0.5f,	-0.5f,		1.0f, 0.0f, 0.0f, 1.0f,			0.0f, 0.0f,
		0.5f,	-0.5f,	-0.5f,		1.0f, 1.0f, 0.0f, 1.0f,			1.0f, 0.0f,
		0.5f,	0.5f,	-0.5f,		0.0f, 0.0f, 1.0f, 1.0f, 		1.0f, 1.0f,
		
		// Top Face
		-0.5f,	0.5f,	0.5f,		0.0f, 1.0f, 0.0f, 1.0f,			0.0f, 0.0f,
		-0.5f,	0.5f,	-0.5f,		0.0f, 1.0f, 0.0f, 1.0f, 		0.0f, 1.0f,
		0.5f,	0.5f,	0.5f,		1.0f, 1.0f, 0.0f, 1.0f,			1.0f, 0.0f,
		
		-0.5f,	0.5f,	-0.5f,		0.0f, 1.0f, 0.0f, 1.0f,			0.0f, 1.0f,
		0.5f,	0.5f,	-0.5f,		0.0f, 0.0f, 1.0f, 1.0f, 		1.0f, 1.0f,
		0.5f,	0.5f,	0.5f,		1.0f, 1.0f, 0.0f, 1.0f,			1.0f, 0.0f,
		
		// Bottom Face
		-0.5f,	-0.5f,	0.5f,		1.0f, 0.0f, 0.0f, 1.0f,			0.0f, 0.0f,
		-0.5f,	-0.5f,	-0.5f,		0.0f, 1.0f, 0.0f, 1.0f, 		0.0f, 1.0f,
		0.5f,	-0.5f,	0.5f,		1.0f, 1.0f, 0.0f, 1.0f,			1.0f, 0.0f,
		
		-0.5f,	-0.5f,	-0.5f,		0.0f, 1.0f, 0.0f, 1.0f,			0.0f, 1.0f,
		0.5f,	-0.5f,	-0.5f,		0.0f, 0.0f, 1.0f, 1.0f, 		1.0f, 1.0f,
		0.5f,	-0.5f,	0.5f,		1.0f, 1.0f, 0.0f, 1.0f,			1.0f, 0.0f,
		
		// Left Face
		-0.5f,	-0.5f,	0.5f,		1.0f, 0.0f, 0.0f, 1.0f,			0.0f, 0.0f,
		-0.5f,	-0.5f,	-0.5f,		0.0f, 1.0f, 0.0f, 1.0f,			0.0f, 1.0f,
		-0.5f,	0.5f,	0.5f,		1.0f, 1.0f, 0.0f, 1.0f,			1.0f, 0.0f,
		
		-0.5f,	-0.5f,	-0.5f,		0.0f, 1.0f, 0.0f, 1.0f,			0.0f, 1.0f,
		-0.5f,	0.5f,	-0.5f,		0.0f, 0.0f, 1.0f, 1.0f,			1.0f, 1.0f,
		-0.5f, 	0.5f,	0.5f,		1.0f, 1.0f, 0.0f, 1.0f, 		1.0f, 0.0f,
		
		// Right Face
		0.5f,	-0.5f,	0.5f,		1.0f, 0.0f, 0.0f, 1.0f,			0.0f, 0.0f,
		0.5f,	-0.5f,	-0.5f,		0.0f, 1.0f, 0.0f, 1.0f,			0.0f, 1.0f,
		0.5f,	0.5f,	0.5f,		1.0f, 1.0f, 0.0f, 1.0f,			1.0f, 0.0f,
		
		0.5f,	-0.5f,	-0.5f,		0.0f, 1.0f, 0.0f, 1.0f,			0.0f, 1.0f,
		0.5f,	0.5f,	-0.5f,		0.0f, 0.0f, 1.0f, 1.0f,			1.0f, 1.0f,
		0.5f, 	0.5f,	0.5f,		1.0f, 1.0f, 0.0f, 1.0f, 		1.0f, 0.0f
		
	};

	// Create a vertex buffer instance, bind it and upload our array to the vertex buffer.
	// Make sure to pass in the type of your array as the template argument, in our case its a float.
	auto vbo = std::unique_ptr<mgl::VertexBuffer>(new mgl::VertexBuffer());
	vbo->Bind();
	vbo->SetBuffer<float>(vertex_buffer_data, sizeof(vertex_buffer_data), BufferUsage::DYNAMIC_DRAW);

	// Array consisting of positions of the cubes.
	glm::vec3 cube_locs[] = 
	{
		glm::vec3( 0.0f, 0.0f, 0.0f),
		glm::vec3( 2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3( 2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3( 1.3f, -2.0f, -2.5f),
		glm::vec3( 1.5f, 2.0f, -2.5f),
		glm::vec3( 1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};

	// Create the buffer layout and setup the layout for the vertex buffer's components.
	// Our vertex array has the following components:
	// A position component:			3 floats (3 * 4 = 12 bytes)
	// A colour component:				4 floats (4 * 4 = 16 bytes)
	// A textue coordinate component:	2 floats (2 * 4 = 8 bytes)
	auto layout = std::unique_ptr<mgl::VertexBufferLayout>(new mgl::VertexBufferLayout());
	layout->Push<float>(3); // Our position component float count.
	layout->Push<float>(4); // Our colour component float count.
	layout->Push<float>(2); // Our texture coord float count.

	// Add our vertex buffer and its layout to the vertex array for rendering.
	vao->AddBuffer(vbo.get(), layout.get());

	// Upload an integer uniform with the name "u_Texture" to the shader.
	shader->SetUniform1i("u_Texture", 0);
	
	// The "position" of our "camera" in the world.
	glm::vec3 view_pos(0.0f, 0.0f, -2.0f);

	// Create our model matrix with its diagonal elements being one.
	glm::mat4 model = glm::identity<glm::mat4>();

	// Create the view matrix and translate it with our view_pos vector.
	glm::mat4 view = glm::identity<glm::mat4>();
	view = glm::translate(view, view_pos);
	
	// Create our perspective camera with 90 degrees FOV, aspect ratio of our width and height and clip range of 0.1 to 100.
	glm::mat4 proj = glm::perspective(TO_RADF(90.0f), (float)((float)(WIDTH) / (float)(HEIGHT)), 0.1f, 100.0f);
	
	// Now upload our matricies to the shader.
	shader->SetUniformMat4f("u_Model", model);
	shader->SetUniformMat4f("u_View", view);
	shader->SetUniformMat4f("u_Proj", proj);

	int frame_count = 0;
	bool running = true;
	float angle = 0.0f;
	while (running)
	{
		// Handle events
		while (SDL_PollEvent(&sdl_event))
		{
			switch (sdl_event.type)
			{
				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYDOWN:
				{
					switch (sdl_event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							running = false;
							break;
					}
					break;
				}
			}
		}

		// Clear the back buffer.
		renderer->Clear();

		// Iterate through the cube position array 
		// and translate them at local space using the model matrix.
		// Also, rotate them on all axis and afterwards upload the
		// model matrix to the shader.
		for (int i = 0; i < ARRAY_LENGTH(cube_locs); ++i)
		{
			auto new_model = glm::identity<glm::mat4>();
			new_model = glm::translate(new_model, cube_locs[i]);
			new_model = glm::rotate(new_model, TO_RADF(angle), glm::vec3(1.0f, 1.0f, 1.0f));
			shader->SetUniformMat4f("u_Model", new_model);
			
			// Make a render call by passing our vertex array, shader and the number of triangles to draw.
			// If you're using an index buffer then you can use the index buffer overload of Render() method.
			// For example:
			// uint32_t index_array_data[] =
			// {
			//     0, 1, 2,
			// 	   2, 3, 1
			// };
			// auto ibo = std::unique_ptr<mgl::IndexBuffer>(new mgl::IndexBuffer());
			// ibo->SetBuffer(index_array_data, 6);
			// renderer->Render(vao.get(), ibo.get(), shader.get());
			renderer->Render(vao.get(), shader.get(), sizeof(vertex_buffer_data) / (sizeof(float) * 9));
			angle = (angle < 360.0f) ? ++angle : 0.0f;
		}
		
		// Swap with the back buffer we've been drawing to.
		SDL_GL_SwapWindow(window);
		++frame_count;
	}

	// Cleanup
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}