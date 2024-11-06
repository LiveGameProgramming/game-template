#include <opengl/commands.hpp>

#include <data/camera.hpp>
#include <data/material.hpp>
#include <data/light.hpp>

#include <window_manager.hpp>
#include <platform_module.hpp>
#include <graphics_module.hpp>
#include <model_renderer.hpp>
#include <primitives.hpp>
#include <functions.hpp>
#include <time.hpp>
#include <file.hpp>
#include <quat.hpp>

int32_t main()
{
    engine::PlatformModule::init();

    engine::WindowManager::instance().create({ .size = { 1280, 1024 } }, { });
    engine::WindowManager::instance().open();

    engine::GraphicsModule::init();

    #pragma region Shaders

    engine::opengl::ShaderStage vertex_stage   { engine::opengl::vertex_stage   };
    engine::opengl::ShaderStage fragment_stage { engine::opengl::fragment_stage };

    vertex_stage.create();
    vertex_stage.source(engine::File::read("default_shader.vert",   std::ios::binary));

    fragment_stage.create();
    fragment_stage.source(engine::File::read("default_shader.frag", std::ios::binary));

    engine::opengl::Shader default_shader;
    default_shader.create();
    default_shader.attach(vertex_stage);
    default_shader.attach(fragment_stage);
    default_shader.link();

    vertex_stage.destroy();
    fragment_stage.destroy();

    #pragma endregion

    #pragma region Plane

    auto [plane_vertices, plane_faces] = editor::Primitives::create_plane(5.0f, 5.0f);

    engine::opengl::Buffer plane_vertex_buffer;
    plane_vertex_buffer.create();
    plane_vertex_buffer.data(engine::buffers::data::create(plane_vertices));

    engine::opengl::Buffer plane_index_buffer;
    plane_index_buffer.create();
    plane_index_buffer.data(engine::buffers::data::create(plane_faces));

    engine::opengl::VertexArray plane_vertex_array;
    plane_vertex_array.create();
    plane_vertex_array.attach_vertices(plane_vertex_buffer, sizeof(editor::vertex));
    plane_vertex_array.attach_indices(plane_index_buffer);
    plane_vertex_array.attribute({ 0, 3, engine::opengl::type_float });
    plane_vertex_array.attribute({ 1, 3, engine::opengl::type_float, offsetof(editor::vertex, extra) });

    #pragma endregion
    #pragma region Box

    auto [box_vertices, box_faces] = editor::Primitives::create_box();

    engine::opengl::Buffer box_vertex_buffer;
    box_vertex_buffer.create();
    box_vertex_buffer.data(engine::buffers::data::create(box_vertices));

    engine::opengl::Buffer box_index_buffer;
    box_index_buffer.create();
    box_index_buffer.data(engine::buffers::data::create(box_faces));

    engine::opengl::VertexArray box_vertex_array;
    box_vertex_array.create();
    box_vertex_array.attach_vertices(box_vertex_buffer, sizeof(editor::vertex));
    box_vertex_array.attach_indices(box_index_buffer);
    box_vertex_array.attribute({ 0, 3, engine::opengl::type_float });
    box_vertex_array.attribute({ 1, 3, engine::opengl::type_float, offsetof(editor::vertex, extra) });

    #pragma endregion
    #pragma region Sphere

    auto [sphere_vertices, sphere_faces] = editor::Primitives::create_sphere();

    engine::opengl::Buffer sphere_vertex_buffer;
    sphere_vertex_buffer.create();
    sphere_vertex_buffer.data(engine::buffers::data::create(sphere_vertices));

    engine::opengl::Buffer sphere_index_buffer;
    sphere_index_buffer.create();
    sphere_index_buffer.data(engine::buffers::data::create(sphere_faces));

    engine::opengl::VertexArray sphere_vertex_array;
    sphere_vertex_array.create();
    sphere_vertex_array.attach_vertices(sphere_vertex_buffer, sizeof(editor::vertex));
    sphere_vertex_array.attach_indices(sphere_index_buffer);
    sphere_vertex_array.attribute({ 0, 3, engine::opengl::type_float });
    sphere_vertex_array.attribute({ 1, 3, engine::opengl::type_float, offsetof(editor::vertex, extra) });

    #pragma endregion

    engine::Time time;
    time.init();

    engine::vec3 camera_position { 0.0f, 2.5, 5.0f };

    engine::data::camera camera;
    camera.view.look(camera_position, { });
    camera.projection.perspective(60.0f, engine::WindowManager::instance().ratio());

    engine::data::light light
    {
        .extra   { 0.0f, -1.0f, -1.0f },
        .ambient { 0.3f }
    };

    engine::data::material material;

    engine::mat4 plane_matrix;
    plane_matrix.identity();

    engine::mat4 sphere_matrix;
    sphere_matrix.identity();
    sphere_matrix.translate({ -1.0f, 1.0f, 0.0f });

    constexpr engine::rgb plane_color   { 0.5f, 0.5f, 1.0f };
    constexpr engine::rgb box_color     { 0.0f, 1.0f, 1.0f };
    constexpr engine::rgb sphere_color  { 1.0f, 1.0f, 0.0f };
    constexpr engine::rgb capsule_color { 1.0f, 0.0f, 1.0f };

    #pragma region Uniform Buffers

    engine::opengl::Buffer camera_buffer;
    camera_buffer.create();
    camera_buffer.bind();
    camera_buffer.data(engine::buffers::data::create(&camera));

    engine::opengl::Buffer material_buffer;
    material_buffer.create();
    material_buffer.bind(engine::buffers::location::material);
    material_buffer.data(engine::buffers::data::create(&material));

    engine::opengl::Buffer light_buffer;
    light_buffer.create();
    light_buffer.bind(engine::buffers::location::light);
    light_buffer.data(engine::buffers::data::create(&light));

    #pragma endregion

    editor::ModelRenderer model_renderer;
    model_renderer.attach(&default_shader);
    model_renderer.attach(&material_buffer);

    engine::WindowManager::instance().resize([&camera_buffer, &camera]
    {
        camera.projection.perspective(60.0f, engine::WindowManager::instance().ratio());
        camera_buffer.sub_data(engine::buffers::data::create(&camera.projection), offsetof(engine::data::camera, projection));
    });

    while (engine::WindowManager::instance().is_active())
    {
        time.update();

        constexpr float camera_speed  = 0.25f;
        constexpr float camera_radius = 5.00f;

        camera_position.x = engine::sin(engine::Time::total_time() * camera_speed) * -camera_radius;
        camera_position.z = engine::cos(engine::Time::total_time() * camera_speed) *  camera_radius;

        camera.view.look(camera_position, { });
        camera_buffer.sub_data(engine::buffers::data::create(&camera.view));

        engine::quat box_orientation;
        box_orientation.rotate({ 1.0f, 1.0f, 0.0f }, engine::Time::total_time() * 90.0f);

        engine::mat4 box_matrix;
        box_matrix.identity();
        box_matrix.translate({ 1.0f, 1.0f, 0.0f });
        box_matrix *= box_orientation;

        const int32_t width  = engine::WindowManager::instance().width();
        const int32_t height = engine::WindowManager::instance().height();

        engine::opengl::Commands::viewport(0, 0, width, height);
        engine::opengl::Commands::clear(engine::opengl::color_buffer | engine::opengl::depth_buffer);

        model_renderer.bind();
        model_renderer.draw(&plane_vertex_array,  plane_matrix,  plane_color,  static_cast<int32_t>(plane_faces.size()));
        model_renderer.draw(&box_vertex_array,    box_matrix,    box_color,    static_cast<int32_t>(box_faces.size()));
        model_renderer.draw(&sphere_vertex_array, sphere_matrix, sphere_color, static_cast<int32_t>(sphere_faces.size()));

        engine::WindowManager::instance().update();
    }

    #pragma region Primitives

    plane_index_buffer.destroy();
    plane_vertex_buffer.destroy();
    plane_vertex_array.destroy();

    box_index_buffer.destroy();
    box_vertex_buffer.destroy();
    box_vertex_array.destroy();

    sphere_index_buffer.destroy();
    sphere_vertex_buffer.destroy();
    sphere_vertex_array.destroy();

    #pragma endregion
    #pragma region Uniform Buffers

    camera_buffer.destroy();
    material_buffer.destroy();
    light_buffer.destroy();

    #pragma endregion

    default_shader.destroy();

    engine::WindowManager::instance().destroy();

    return 0;
}