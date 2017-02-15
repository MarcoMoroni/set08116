#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

map<string, mesh> meshes;
texture tex;

geometry geom;
effect eff;
target_camera cam;

bool load_content() {

  // Create box
	meshes["box1"] = mesh(geometry_builder::create_box());
	meshes["box2"] = mesh(geometry_builder::create_box());
	meshes["box3"] = mesh(geometry_builder::create_box());
	meshes["box4"] = mesh(geometry_builder::create_box());
	meshes["box5"] = mesh(geometry_builder::create_box());
	meshes["box6"] = mesh(geometry_builder::create_box());
	meshes["box7"] = mesh(geometry_builder::create_box());

	// Set the transforms for your meshes here
	meshes["box2"].get_transform().position += vec3(0.0f, 0.0f, -1.0f);
	meshes["box3"].get_transform().position += vec3(0.0f, 0.0f, -2.0f);
	meshes["box4"].get_transform().position += vec3(0.0f, 0.0f, -3.0f);
	meshes["box5"].get_transform().position += vec3(0.0f, 1.0f, 0.0f);
	meshes["box6"].get_transform().position += vec3(0.0f, 2.0f, 0.0f);
	meshes["box7"].get_transform().position += vec3(0.0f, 0.0f, 0.0f);

	// Load texture
	tex = texture("textures/check_1.png");

  // Load in shaders
  eff.add_shader("shaders/texture.vert", GL_VERTEX_SHADER);
  eff.add_shader("shaders/texture.frag", GL_FRAGMENT_SHADER);

  // Build effect
  eff.build();

  // Set camera properties
  cam.set_position(vec3(0.0f, 0.0f, 10.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
	auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
	cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);

  return true;

}

bool update(float delta_time) {

		// Set some camera positions
		if (glfwGetKey(renderer::get_window(), '1')) {
				cam.set_position(vec3(50, 10, 50));
		}
		if (glfwGetKey(renderer::get_window(), '2')) {
				cam.set_position(vec3(-50, 10, 50));
		}
		if (glfwGetKey(renderer::get_window(), '3')) {
				cam.set_position(vec3(-50, 10, -50));
		}
		if (glfwGetKey(renderer::get_window(), '4')) {
				cam.set_position(vec3(50, 10, -50));
		}
		if (glfwGetKey(renderer::get_window(), '5')) {
				cam.set_position(vec3(10, 10, 10));
		}

  // Update the camera
  cam.update(delta_time);

  return true;

}

bool render() {

		// Render meshes
		for (auto &e : meshes) {
				auto m = e.second;
				// Bind effect
				renderer::bind(eff);
				// Create MVP matrix
				auto M = m.get_transform().get_transform_matrix();
				auto V = cam.get_view();
				auto P = cam.get_projection();
				auto MVP = P * V * M;
				// Set MVP matrix uniform
				glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
				// Bind texture to renderer
				renderer::bind(tex, 0);
				// Set the texture value for the shader here
				glUniform1i(eff.get_uniform_location("tex"), 0);
				// Render mesh
				renderer::render(m);
		}

		/*
  // Bind effect
  renderer::bind(eff);
  // Create MVP matrix
  mat4 M(1.0f);
  auto V = cam.get_view();
  auto P = cam.get_projection();
  auto MVP = P * V * M;
  // Set MVP matrix uniform
  glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
  // Render geometry
  renderer::render(geom);*/

  return true;
	
}

void main() {

  // Create application
  app application("Graphics Coursework");

  // Set load content, update and render methods
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);

  // Run application
  application.run();

}