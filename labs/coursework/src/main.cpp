#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

map<string, mesh> meshes;
effect eff;
//target_camera cam;
std::array<texture, 3> textures;
directional_light light;

// ----- test -----
free_camera cam;
double cursor_x = 0.0;
double cursor_y = 0.0;
//-----------------

bool initialise() {

	// Set input mode - hide the cursor
	glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Capture initial mouse position
	glfwSetCursorPos(renderer::get_window(), cursor_x, cursor_y);

	return true;
}

bool load_content() {

  // Create boxes for Penrose triangle
  meshes["box1"] = mesh(geometry_builder::create_box(vec3(4.0f, 1.0f, 1.0f)));
  meshes["box2"] = mesh(geometry_builder::create_box(vec3(1.0f, 1.0f, 4.0f)));
  meshes["box3"] = mesh(geometry_builder::create_box(vec3(1.0f, 3.0f, 1.0f)));
	meshes["teapot"] = mesh(geometry("models/teapot.obj"));

  // Set the transforms for meshes for Penrose triangle
	//meshes["box1"].get_transform().position += vec3(-0.5f, 0.0f, 0.0f);
  //meshes["box2"].get_transform().position += vec3(2.0f, 0.0f, -1.5f);
  //meshes["box3"].get_transform().position += vec3(-3.0f, 1.0f, 0.0f);
	meshes["teapot"].get_transform().scale *= vec3(0.05f);
	meshes["teapot"].get_transform().position += vec3(-5.0f, 0.0f, 6.0f);

	// Load textures
	textures[0] = texture("textures/check_1.png");
	textures[1] = texture("textures/check_2.png");
	textures[2] = texture("textures/check_3.png");

	// Set lighting values
	// ambient intensity (0.3, 0.3, 0.3)
	light.set_ambient_intensity(vec4(0.3f, 0.3f, 0.3f, 1.0f));
	// Light colour white
	light.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	// Light direction (1.0, 1.0, -1.0)
	light.set_direction(vec3(1.0f, 1.0f, -1.0f));

  // Load in shaders
  //eff.add_shader("shaders/texture.vert", GL_VERTEX_SHADER);
  //eff.add_shader("shaders/texture.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/gouraud.vert", GL_VERTEX_SHADER);
	eff.add_shader("shaders/gouraud.frag", GL_FRAGMENT_SHADER);

  // Build effect
  eff.build();

  // Set camera properties
  cam.set_position(vec3(-3.0f, 5.0f, 20.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
  cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);
  
  return true;

}

bool update(float delta_time) {

  // Set some camera positions
  if (glfwGetKey(renderer::get_window(), '1')) {
		cam.set_position(vec3(-10.0f, 10.0f, 10.0f));
  }
  if (glfwGetKey(renderer::get_window(), '2')) {
	  cam.set_position(vec3(10.0f, 20.0f, 20.0f));
  }

	// The ratio of pixels to rotation - remember the fov
	static double ratio_width = quarter_pi<float>() / static_cast<float>(renderer::get_screen_width());
	static double ratio_height =
		(quarter_pi<float>() *
		(static_cast<float>(renderer::get_screen_height()) / static_cast<float>(renderer::get_screen_width()))) /
		static_cast<float>(renderer::get_screen_height());

	double current_x;
	double current_y;
	
	// Get the current cursor position
	glfwGetCursorPos(renderer::get_window(), &current_x, &current_y);

	// Calculate delta of cursor positions from last frame
	double delta_x = current_x - cursor_x;
	double delta_y = current_y - cursor_y;

	// Multiply deltas by ratios - gets actual change in orientation
	delta_x *= ratio_width;
	delta_y *= ratio_height;

	// Rotate cameras by delta
	// delta_y - x-axis rotation
	// delta_x - y-axis rotation
	cam.rotate(-delta_x, delta_y);

	// Use keyboard to move the camera - WSAD
	vec3 dir;
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_W)) {
		dir += vec3(0.0f, 0.0f, 0.5f);
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_A)) {
		dir += vec3(-0.5f, 0.0f, 0.0f);
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_D)) {
		dir += vec3(0.5f, 0.0f, 0.0f);
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_S)) {
		dir += vec3(0.0f, 0.0f, -0.5f);
	}

	// Move camera
	cam.move(dir);

	// Update the camera
	cam.update(delta_time);

	// Update cursor pos
	cursor_x = current_x;
	cursor_y = current_y;

  // Update the camera
  cam.update(delta_time);

  return true;

}

bool render() {

  /*// Render meshes
  for (auto &e : meshes) {
		auto m = e.second;
		// Bind effect
		renderer::bind(eff);
		// Create MVP matrix
		auto M = m.get_transform().get_transform_matrix();
		auto V = cam.get_view();
		auto P = cam.get_projection();
		// ----------------------------- Othographic camera test -----------------------------
		//float zoom = 100.0f;
		//auto P = glm::ortho(-static_cast<float>(renderer::get_screen_width()) / zoom, static_cast<float>(renderer::get_screen_width()) / zoom, static_cast<float>(renderer::get_screen_height()) / zoom, -static_cast<float>(renderer::get_screen_height()) / zoom, 2.414f, 1000.0f);
		// -----------------------------------------------------------------------------------
		auto MVP = P * V * M;
		// Set MVP matrix uniform
		glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
		// Bind texture to renderer
		renderer::bind(textures[1], 0);
		// Set the texture value for the shader here
		glUniform1i(eff.get_uniform_location("tex"), 0);
		// Render mesh
		renderer::render(m);
	}*/

	// Render meshes
	for (auto &e : meshes) {

		auto m = e.second;

		// Bind effect
		renderer::bind(eff);

		// Create MVP matrix
		auto M = m.get_transform().get_transform_matrix();
		auto V = cam.get_view();
		auto P = cam.get_projection();
		// ----------------------------- Othographic camera test -----------------------------
		//float zoom = 100.0f;
		//auto P = glm::ortho(-static_cast<float>(renderer::get_screen_width()) / zoom, static_cast<float>(renderer::get_screen_width()) / zoom, static_cast<float>(renderer::get_screen_height()) / zoom, -static_cast<float>(renderer::get_screen_height()) / zoom, 2.414f, 1000.0f);
		// -----------------------------------------------------------------------------------
		auto MVP = P * V * M;

		// Set MVP matrix uniform
		glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));

		// Set M matrix uniform
		glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));

		// Set N matrix uniform - remember - 3x3 matrix
		glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(m.get_transform().get_normal_matrix()));

		// Bind material
		renderer::bind(m.get_material(), "mat");

		// Bind light
		renderer::bind(light, "light");

		// Bind texture
		renderer::bind(textures[1], 0);

		// Set tex uniform
		glUniform1i(eff.get_uniform_location("tex"), 0);

		// Set eye position - Get this from active camera
		glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(cam.get_position()));

		// Render mesh
		renderer::render(m);
		
	}
  
  return true;
	
}

void main() {

  // Create application
  app application("Graphics Coursework");

  // Set load content, update and render methods
  application.set_load_content(load_content);
	application.set_initialise(initialise);
  application.set_update(update);
  application.set_render(render);

  // Run application
  application.run();

}