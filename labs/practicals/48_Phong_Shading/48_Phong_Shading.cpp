#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

map<string, mesh> meshes;
effect eff;
texture tex, earth, bricks, lava;
//target_camera cam;
directional_light light;

free_camera cam;
double cursor_x = 0.0;
double cursor_y = 0.0;
double angleLuna = 0.0f;

vec3 bulbPos = vec3(0);
bool up = true;

// before load_content
bool initialise() {
	// *********************************
	// Set input mode - hide the cursor
	glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Capture initial mouse position
	glfwGetCursorPos(renderer::get_window(), &cursor_x, &cursor_y);
	// *********************************
	return true;
}

bool load_content() {

	// Create plane mesh
	meshes["plane"] = mesh(geometry_builder::create_plane());

	// Create scene
	meshes["box"] = mesh(geometry_builder::create_box());
	meshes["tetra"] = mesh(geometry_builder::create_tetrahedron());
	meshes["pyramid"] = mesh(geometry_builder::create_pyramid());
	meshes["disk"] = mesh(geometry_builder::create_disk(20));
	meshes["cylinder"] = mesh(geometry_builder::create_cylinder(20, 20));
	meshes["sphere"] = mesh(geometry_builder::create_sphere(80, 80));
	meshes["luna"] = mesh(geometry_builder::create_sphere(40, 40));
	meshes["torus"] = mesh(geometry_builder::create_torus(45, 45, 1.0f, 5.0f));

	// Transform objects
	meshes["box"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
	meshes["box"].get_transform().translate(vec3(-10.0f, 2.5f, -30.0f));
	meshes["tetra"].get_transform().scale = vec3(4.0f, 4.0f, 4.0f);
	meshes["tetra"].get_transform().translate(vec3(-30.0f, 10.0f, -10.0f));
	meshes["pyramid"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
	meshes["pyramid"].get_transform().translate(vec3(-10.0f, 7.5f, -30.0f));
	meshes["disk"].get_transform().scale = vec3(3.0f, 1.0f, 3.0f);
	meshes["disk"].get_transform().translate(vec3(-10.0f, 11.5f, -30.0f));
	meshes["disk"].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));
	meshes["cylinder"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
	meshes["cylinder"].get_transform().translate(vec3(-25.0f, 2.5f, -25.0f));
	meshes["sphere"].get_transform().scale = vec3(2.5f, 2.5f, 2.5f);
	meshes["sphere"].get_transform().translate(vec3(-25.0f, 10.0f, -25.0f));
	meshes["sphere"].get_transform().rotate(vec3(-half_pi<float>(), 0.0f, quarter_pi<float>() / 2.0f));
	meshes["luna"].get_transform().scale = vec3(0.7f, 0.7f, 0.7f);
	meshes["luna"].get_transform().translate(vec3(-25.0f, 10.0f, -29.0f));
	meshes["luna"].get_transform().rotate(vec3(-half_pi<float>(), 0.0f, quarter_pi<float>() / 2.0f));
	meshes["torus"].get_transform().translate(vec3(-25.0f, 10.0f, -25.0f));
	meshes["torus"].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));

	// *********************************
	// Set materials
	// - all emissive is black
	// - all specular is white
	// - all shininess is 25
	// Red box
	material objectMaterial;
	objectMaterial.set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	objectMaterial.set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	objectMaterial.set_shininess(25.0f);
	objectMaterial.set_diffuse(vec4(0.5f, 0.5f, 0.5f, 1.0f));
	meshes["box"].set_material(objectMaterial);
	// Green tetra
	objectMaterial.set_diffuse(vec4(0.0f, 1.0f, 0.0f, 1.0f));
	meshes["tetra"].set_material(objectMaterial);
	// Blue pyramid
	objectMaterial.set_diffuse(vec4(0.0f, 0.0f, 1.0f, 1.0f));
	meshes["pyramid"].set_material(objectMaterial);
	// Yellow disk
	objectMaterial.set_diffuse(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	meshes["disk"].set_material(objectMaterial);
	// Magenta cylinder
	objectMaterial.set_diffuse(vec4(1.0f, 0.0f, 1.0f, 1.0f));
	meshes["cylinder"].set_material(objectMaterial);
	// Cyan sphere

	objectMaterial.set_shininess(35);
	objectMaterial.set_diffuse(vec4(0.5f, 0.5f, 0.5f, 1.0f));
	meshes["sphere"].set_material(objectMaterial);
	// White torus
	objectMaterial.set_diffuse(vec4(0.8f, 0.8f, 0.8f, 1.0f));
	meshes["torus"].set_material(objectMaterial);
	// *********************************
	// Load texture
	tex = texture("textures/checked.gif");
	/* earth = texture("textures/earth.jpg");
	bricks = texture("textures/brick_diffuse.jpg");
	lava = texture("textures/lavatile.jpg");*/

	// *********************************
	// ambient intensity (0.3, 0.3, 0.3)
	light.set_ambient_intensity(vec4(0.3f, 0.3f, 0.3f, 1.0f));
	// Light colour white
	light.set_light_colour(vec4(0.6f, 0.6f, 0.6f, 1.0f));
	// Light direction (1.0, 1.0, -1.0)
	light.set_direction(vec3(1.0f, 1.0f, -1.0f));
	// Load in shaders
	eff.add_shader("48_Phong_Shading/phong.vert", GL_VERTEX_SHADER);
	eff.add_shader("48_Phong_Shading/phong.frag", GL_FRAGMENT_SHADER);
	// Build effect
	eff.build();
	// *********************************

	// Set camera properties
	cam.set_position(vec3(-50.0f, 10.0f, -50.0f));
	cam.set_target(vec3(0.0f, 0.0f, 0.0f));
	cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
	return true;
}

bool update(float delta_time) {
	cout << "FPS: " << 1.0f / delta_time << endl;
	/*
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
	}*/

	static double ratio_width = quarter_pi<float>() / static_cast<float>(renderer::get_screen_width());
	static double ratio_height =
		(quarter_pi<float>() *
		(static_cast<float>(renderer::get_screen_height()) / static_cast<float>(renderer::get_screen_width()))) /
		static_cast<float>(renderer::get_screen_height());

	double current_x = 0;
	double current_y = 0;
	double delta_x;
	double delta_y;
	vec3 pos = vec3(0);
	vec3 lunaPos = vec3(0);

	// *********************************
	// Get the current cursor position
	glfwGetCursorPos(renderer::get_window(), &current_x, &current_y);
	// Calculate delta of cursor positions from last frame
	delta_x = current_x - cursor_x;
	delta_y = current_y - cursor_y;
	// Multiply deltas by ratios - gets actual change in orientation
	delta_x = delta_x * ratio_width;
	delta_y = delta_y * ratio_height;

	// Rotate cameras by delta
	// delta_y - x-axis rotation
	// delta_x - y-axis rotation
	cam.rotate(delta_x, -delta_y);
	// Use keyboard to move the camera - WSAD
	if (glfwGetKey(renderer::get_window(), 'W')) {
		pos += vec3(0.0f, 0.0f, 20.0f) * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), 'S')) {
		pos += vec3(0.0f, 0.0f, -20.0f) * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), 'A')) {
		pos += vec3(-20.0f, 0.0f, 0.0f) * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), 'D')) {
		pos += vec3(20.0f, 0.0f, 0.0f) * delta_time;
	}
	// Move camera
	cam.move(pos);
	// Update cursor pos
	cursor_x = current_x;
	cursor_y = current_y;
	// *********************************

	// Rotate the sphere
	meshes["sphere"].get_transform().rotate(vec3(0.0f, 0.0f, quarter_pi<float>()) * delta_time);
	meshes["luna"].get_transform().rotate(vec3(0.0f, 0.0f, -quarter_pi<float>()) * delta_time);
	lunaPos = vec3((cos(angleLuna)*5.0f), 0.0f, (sin(angleLuna)*5.0f));
	meshes["luna"].get_transform().position += lunaPos*delta_time;
	angleLuna += 0.01f;
	cout << bulbPos.y << endl;

	// Update the camera
	cam.update(delta_time);

	return true;
}

bool render() {
	// Render meshes
	for (auto &e : meshes) {
		auto m = e.second;
		auto N = m.get_transform().get_normal_matrix();
		// Bind effect
		renderer::bind(eff);
		// Create MVP matrix
		auto M = m.get_transform().get_transform_matrix();
		auto V = cam.get_view();
		auto P = cam.get_projection();
		auto MVP = P * V * M;
		// Set MVP matrix uniform
		glUniformMatrix4fv(eff.get_uniform_location("MVP"), // Location of uniform
			1,                               // Number of values - 1 mat4
			GL_FALSE,                        // Transpose the matrix?
			value_ptr(MVP));                 // Pointer to matrix data

											 // Set M matrix uniform

		glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));

		// *********************************
		// Set N matrix uniform - remember - 3x3 matrix
		glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(N));
		// Bind material
		renderer::bind(m.get_material(), "mat");
		// Bind light
		renderer::bind(light, "light");
		// Bind texture
		renderer::bind(tex, 0);
		/*renderer::bind(earth, 1);
		renderer::bind(lava, 2);
		renderer::bind(bricks, 3);*/
		// Set tex uniform
		if (e.first == "sphere") {
			glUniform1i(eff.get_uniform_location("tex"), 1);
		}
		else if (e.first == "torus") {
			glUniform1i(eff.get_uniform_location("tex"), 2);
		}
		else if (e.first == "box") {
			glUniform1i(eff.get_uniform_location("tex"), 3);
		}
		else {
			glUniform1i(eff.get_uniform_location("tex"), 0);
		}
		// Set eye position - Get this from active camera
		glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(cam.get_position()));
		// Render mesh
		renderer::render(m);
		// *********************************
	}

	return true;
}

void main() {
	// Create application
	app application("48_Phong_Shading");
	// Set load content, update and render methods
	application.set_initialise(initialise);
	application.set_load_content(load_content);
	application.set_update(update);
	application.set_render(render);
	// Run application
	application.run();
}