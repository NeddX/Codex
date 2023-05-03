#include <iostream>
#include <SDL.h>
#include <glad/glad.h>
#include <SDL_opengl.h>
#include <cstdint>
#include <iomanip>
#include <memory>
#include <MARFX.h>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef _DEBUG
#define mfx_DEBUG
#endif
#define WIDTH	600
#define HEIGHT	600

#define MAT4(m, x, y) m[y * 4 + x]
#define PI 3.14159265358979323
#define TO_RADF(d) (float)((float)(d) * (PI / 180.0f)) 

void CreateSDLWindow(SDL_Window** window, const char* title, int width, int height)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	*window = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);

	SDL_GLContext context = SDL_GL_CreateContext(*window);

	if (gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		printf("OpenGL loaded\n");
		gladLoadGLLoader(SDL_GL_GetProcAddress);
		printf("Vendor:   %s\n", glGetString(GL_VENDOR));
		printf("Renderer: %s\n", glGetString(GL_RENDERER));
		printf("Version:  %s\n", glGetString(GL_VERSION));
	}
	
	// update the viewport when the window gets resized
	SDL_AddEventWatch([](void* userdata, SDL_Event* event) -> int 
	{
		if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED) 
		{
			// Get the new window size
			int width = event->window.data1;
			int height = event->window.data2;
			glViewport(0, 0, width, height);
		}
		return 0;
	}, nullptr);
	
	SDL_GL_SetSwapInterval(2);
}

int main(int argc, char* argv[])
{
	SDL_Window* window = nullptr;
	SDL_Event sdl_event;
	CreateSDLWindow(&window, "Test", WIDTH, HEIGHT);
	
	auto* renderer = new mfx::Renderer(WIDTH, HEIGHT);
	renderer->SetClearColour(0.1f, 0.1f, 0.1f, 1.0f);

	auto* shader = new mfx::Shader("default.glsl");
	shader->Bind();

	auto* aws_tex = new mfx::Texture("box.png");
	aws_tex->Bind(0);

	auto* vao = new mfx::VertexArray();
	vao->Bind();

	float vertex_buffer_data[] =
	{
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
	auto* vbo = new mfx::VertexBuffer();
	vbo->Bind();
	vbo->SetBuffer<float>(vertex_buffer_data, sizeof(vertex_buffer_data));

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

	auto* layout = new mfx::VertexBufferLayout();
	layout->Push<float>(3);
	layout->Push<float>(4);
	layout->Push<float>(2);

	vao->AddBuffer(vbo, layout);

	shader->SetUniform1i("u_Texture", 0);
	
	glm::vec3 view_pos(0.0f, 0.0f, -2.0f);

	glm::mat4 model = glm::identity<glm::mat4>();
	//model = glm::rotate(model, TO_RADF(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	
	glm::vec3 camera_pos(0.0f, 0.0f, 3.0f);
	glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 camera_direction = glm::normalize(camera_pos - camera_target);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 camera_right = glm::normalize(glm::cross(up, camera_direction));

	glm::mat4 view = glm::identity<glm::mat4>();
	//view = glm::rotate(view, TO_RADF(30), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::translate(view, view_pos);
	
	glm::mat4 proj = glm::perspective(TO_RADF(90.0f), (float)((float)(WIDTH) / (float)(HEIGHT)), 0.1f, 100.0f);
	shader->SetUniformMat4f("u_Model", model);
	shader->SetUniformMat4f("u_View", view);
	shader->SetUniformMat4f("u_Proj", proj);

	bool running = true;
	float angle = 0.0f;
	while (running)
	{
		static int i = 0;
		static float camera_velocity = 0.05f;
		
		// Events
		SDL_PollEvent(&sdl_event);
		switch (sdl_event.type)
		{
			case SDL_KEYDOWN:
				switch (sdl_event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						running = false;
						break;
					case SDLK_w:
						view_pos.z = camera_velocity;
						break;
					case SDLK_s:
						view_pos.z -= camera_velocity;
						break;
					case SDLK_a:
						view_pos.x += camera_velocity;
						break;
					case SDLK_d:
						view_pos.x -= camera_velocity;
						break;
				}
				break;
			case SDL_QUIT:
				running = false;
				break;
			default:
				view_pos.x = 0.0f;
				view_pos.y = 0.0f;
				view_pos.z = 0.0f;
				break;
		}
		
		view = glm::translate(view, view_pos);
		//view = glm::rotate(view, TO_RADF(1), glm::vec3(1.0f, 1.0f, 1.0f));
		shader->SetUniformMat4f("u_View", view);

		renderer->Clear();
		for (int i = 0; i < sizeof(cube_locs) / sizeof(cube_locs[0]); ++i)
		{
			auto new_model = glm::identity<glm::mat4>();
			new_model = glm::translate(new_model, cube_locs[i]);
			new_model = glm::rotate(new_model, TO_RADF(angle), glm::vec3(1.0f, 1.0f, 1.0f));
			shader->SetUniformMat4f("u_Model", new_model);
			
			renderer->Render(vao, shader, sizeof(vertex_buffer_data) / (sizeof(float) * 9));
			angle = (angle < 360.0f) ? ++angle : 0.0f;
		}
		//renderer->Render(vao, ebo, shader);
		
		SDL_GL_SwapWindow(window);
		++i;
	}
	SDL_Quit();

	return 0;
}