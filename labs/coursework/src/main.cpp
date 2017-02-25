#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

map<string, mesh> meshes;
effect eff;
map<string, texture> textures;

map<string, string> textures_link;

directional_light light;
vector<point_light> points(1);
vector<spot_light> spots(1);

free_camera free_cam;
double cursor_x = 0.0;
double cursor_y = 0.0;

bool initialise() {

	// Set input mode - hide the cursor
	glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Capture initial mouse position
	glfwSetCursorPos(renderer::get_window(), cursor_x, cursor_y);

	return true;
}

bool load_content() {



	//// Meshes

	// Create plane mesh
	meshes["plane"] = mesh(geometry_builder::create_plane());

	// Box
	meshes["box"] = mesh(geometry_builder::create_box());
	meshes["box"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
	meshes["box"].get_transform().translate(vec3(-10.0f, 2.5f, -30.0f));;

  // Teapot
	meshes["teapot"] = mesh(geometry("models/teapot.obj"));
	meshes["teapot"].get_transform().scale *= vec3(0.04f);

	// Torus 1
	meshes["torus1"] = mesh(geometry_builder::create_torus(60, 20, 0.2f, 4.0f));

	// Torus 2
	meshes["torus2"] = mesh(geometry_builder::create_torus(60, 20, 0.2f, 5.0f));

	// Torus 3
	meshes["torus3"] = mesh(geometry_builder::create_torus(60, 20, 0.2f, 6.0f));
	meshes["torus3"].get_transform().position += vec3(0.0f, 7.0f, 0.0f);



	//// Set materials

	material mat;

	mat.set_shininess(100.0f);
	meshes["plane"].set_material(mat);

	mat.set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	mat.set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//mat.set_diffuse(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	mat.set_shininess(25.0f);
	meshes["box"].set_material(mat);



	//// Textures

	// Load textures
	textures["test"] = texture("textures/checker.png");
	textures["floor"] = texture("textures/floor.jpg");
	textures["gold"] = texture("textures/gold.jpg");

	// Link textures to meshes
	textures_link["torus1"] = "gold";
	textures_link["torus2"] = "gold";
	textures_link["torus3"] = "gold";
	textures_link["plane"] = "floor";
	textures_link["teapot"] = "gold";



	//// Set lighting values

	// Directional
	light.set_ambient_intensity(vec4(0.3f, 0.3f, 0.3f, 1.0f));
	light.set_light_colour(vec4(1.0f, 1.0f, 0.8f, 1.0f));
	//light.set_direction(normalize(vec3(1.0f, 1.0f, -1.0f))); // ?
	light.set_direction(vec3(1.0f, 1.0f, -1.0f));

	// Point 0
	points[0].set_position(vec3(-10.0f, 5.0f, -10.0f));
	points[0].set_light_colour(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	points[0].set_range(30.0f);

	// Spot 0
	spots[0].set_position(vec3(-17.5f, 15.0f, -25.0f));
	spots[0].set_light_colour(vec4(0.0f, 0.0f, 1.0f, 1.0f));
	spots[0].set_direction(normalize(vec3(1.0f, 0.0f, 1.0f)));
	spots[0].set_range(90.0f);
	spots[0].set_power(1.0f);



	// Load in shaders
	eff.add_shader("shaders/shader.vert", GL_VERTEX_SHADER);

	// Name of fragment shaders required
	vector<string> frag_shaders{ "shaders/shader.frag",
		"shaders/direction.frag", 
		"shaders/point.frag",
		"shaders/spot.frag"/*,
		"shaders/normal_map.frag",
		"shaders/shadow.frag"*/ };
	eff.add_shader(frag_shaders, GL_FRAGMENT_SHADER);

  // Build effect
  eff.build();

  // Set camera properties
  free_cam.set_position(vec3(-3.0f, 5.0f, 20.0f));
	free_cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
	free_cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);
  
  return true;

}

bool update(float delta_time) {

  // Set some camera positions
  if (glfwGetKey(renderer::get_window(), '1')) {
		free_cam.set_position(vec3(-10.0f, 10.0f, 10.0f));
  }
  if (glfwGetKey(renderer::get_window(), '2')) {
		free_cam.set_position(vec3(10.0f, 20.0f, 20.0f));
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
	free_cam.rotate(-delta_x, delta_y);

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
	free_cam.move(dir);

	// Update the camera
	free_cam.update(delta_time);

	// Update cursor pos
	cursor_x = current_x;
	cursor_y = current_y;

	// Rotate the torus
	meshes["teapot"].get_transform().rotate(vec3(half_pi<float>() / 4, 0.0f, 0.0f) * delta_time);
	meshes["torus1"].get_transform().rotate(vec3(half_pi<float>() / 4, 0.0f, 0.0f) * delta_time);
	meshes["torus2"].get_transform().rotate(vec3(0.0f, 0.0f, half_pi<float>() / 4) * delta_time);
	meshes["torus3"].get_transform().rotate(vec3(half_pi<float>() / 4, 0.0f, 0.0f) * delta_time);

  // Update the camera
	free_cam.update(delta_time);

  return true;

}

bool render() {

	// Render meshes
	for (auto &e : meshes) {

		auto m = e.second;

		// Bind effect
		renderer::bind(eff);

		// Create MVP matrix
		auto V = free_cam.get_view();
		auto P = free_cam.get_projection();
		auto M = m.get_transform().get_transform_matrix();
		// Hierarchy
		if (e.first == "torus2")
		{
			M = meshes["torus3"].get_transform().get_transform_matrix() * M;
		}
		else if (e.first == "torus1" || e.first == "teapot")
		{
			M = meshes["torus3"].get_transform().get_transform_matrix() * meshes["torus2"].get_transform().get_transform_matrix() * M;
		}
		
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

		// Bind point lights
		renderer::bind(points, "points");

		// Bind spot lights
		renderer::bind(spots, "spots");

		// Bind light
		renderer::bind(light, "light");

		// Bind texture
		if (textures_link.find(e.first) != textures_link.end())
		{
			renderer::bind(textures[textures_link[e.first]], 0);
		}
		else
		{
			renderer::bind(textures["test"], 0);
		}

		// Set tex uniform
		glUniform1i(eff.get_uniform_location("tex"), 0);

		// Set eye position - Get this from active camera
		glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(free_cam.get_position()));

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