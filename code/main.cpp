#include <platform_module.hpp>
#include <window_manager.hpp>
#include <graphics_module.hpp>
#include <primitives.hpp>
#include <time.hpp>
#include <file.hpp>
#include <mat4.hpp>
#include <rgb.hpp>

#include <opengl/vertex_array.hpp>
#include <opengl/commands.hpp>
#include <opengl/shader.hpp>
#include <opengl/pipeline.hpp>

#include <math/functions.hpp>

int32_t main()
{
    using namespace engine;

    PlatformModule::init();

    WindowManager::instance().create({ .size = { 1280, 1024 } }, { });
    WindowManager::instance().display();

    GraphicsModule::init();

    //opengl::Pipeline::enable_lines_mode();

    #pragma region Shaders

    opengl::ShaderStage vertex_stage   { opengl::vertex_stage   };
    opengl::ShaderStage fragment_stage { opengl::fragment_stage };

    vertex_stage.create();
    vertex_stage.source(File::read("default_shader.vert",   std::ios::binary));

    fragment_stage.create();
    fragment_stage.source(File::read("default_shader.frag", std::ios::binary));

    opengl::Shader default_shader;
    default_shader.create();
    default_shader.attach(vertex_stage);
    default_shader.attach(fragment_stage);
    default_shader.link();

    vertex_stage.destroy();
    fragment_stage.destroy();

    #pragma endregion

    #pragma region Plane

    auto [plane_vertices, plane_faces] = editor::Primitives::create_plane(5.0f, 5.0f);

    opengl::Buffer plane_vertex_buffer;
    plane_vertex_buffer.create();
    plane_vertex_buffer.data(buffers::data::create(plane_vertices));

    opengl::Buffer plane_index_buffer;
    plane_index_buffer.create();
    plane_index_buffer.data(buffers::data::create(plane_faces));

    opengl::VertexArray plane_vertex_array;
    plane_vertex_array.create();
    plane_vertex_array.attach_vertices(plane_vertex_buffer, sizeof(editor::vertex));
    plane_vertex_array.attach_indices(plane_index_buffer);
    plane_vertex_array.attribute({ 0, 3, opengl::type_float });

    #pragma endregion
    #pragma region Box

    auto [box_vertices, box_faces] = editor::Primitives::create_box();

    opengl::Buffer box_vertex_buffer;
    box_vertex_buffer.create();
    box_vertex_buffer.data(buffers::data::create(box_vertices));

    opengl::Buffer box_index_buffer;
    box_index_buffer.create();
    box_index_buffer.data(buffers::data::create(box_faces));

    opengl::VertexArray box_vertex_array;
    box_vertex_array.create();
    box_vertex_array.attach_vertices(box_vertex_buffer, sizeof(editor::vertex));
    box_vertex_array.attach_indices(box_index_buffer);
    box_vertex_array.attribute({ 0, 3, opengl::type_float });

    #pragma endregion
    #pragma region Sphere

    auto [sphere_vertices, sphere_faces] = editor::Primitives::create_sphere();

    opengl::Buffer sphere_vertex_buffer;
    sphere_vertex_buffer.create();
    sphere_vertex_buffer.data(buffers::data::create(sphere_vertices));

    opengl::Buffer sphere_index_buffer;
    sphere_index_buffer.create();
    sphere_index_buffer.data(buffers::data::create(sphere_faces));

    opengl::VertexArray sphere_vertex_array;
    sphere_vertex_array.create();
    sphere_vertex_array.attach_vertices(sphere_vertex_buffer, sizeof(editor::vertex));
    sphere_vertex_array.attach_indices(sphere_index_buffer);
    sphere_vertex_array.attribute({ 0, 3, opengl::type_float });

    #pragma endregion

    vec3  camera_position { 0.0f, 2.5, 5.0f };
    const float aspect_ratio = static_cast<float>(WindowManager::instance().width()) /
                               static_cast<float>(WindowManager::instance().height());
    mat4 view;
    view.look_at(camera_position, { });

    mat4 proj;
    proj.perspective(60.0f, aspect_ratio, 0.1f, 100.0f);

    #pragma region Uniform Buffers

    const std::vector camera_data { view, proj };

    opengl::Buffer camera_ubo;
    camera_ubo.create();
    camera_ubo.bind();
    camera_ubo.data(buffers::data::create(camera_data));

    constexpr rgb default_color = rgb::white();

    opengl::Buffer material_ubo;
    material_ubo.create();
    material_ubo.bind(buffers::location::material);
    material_ubo.data(buffers::data::create(&default_color));

    #pragma endregion

    Time time;
    time.init();

    constexpr rgb plane_color   { 0.420f, 0.357f, 0.584f };
    constexpr rgb box_color     { 0.533f, 0.690f, 0.231f };
    constexpr rgb sphere_color  { 0.969f, 0.792f, 0.788f };
    constexpr rgb capsule_color { 0.573f, 0.659f, 0.820f };

    while (WindowManager::instance().is_active())
    {
        time.update();

        constexpr float camera_speed  = 0.25f;
        constexpr float camera_radius = 5.00f;

        camera_position.x = math::sin(Time::total_time() * camera_speed) * -camera_radius;
        camera_position.z = math::cos(Time::total_time() * camera_speed) *  camera_radius;

        view.look_at(camera_position, { });
        camera_ubo.sub_data(buffers::data::create(&view));

        opengl::Commands::clear(1.0f, 0.435f, 0.38f);
        opengl::Commands::clear(opengl::color_buffer | opengl::depth_buffer);

        default_shader.bind();

        #pragma region Plane

        mat4 plane_model_matrix;
        plane_model_matrix.identity();
        default_shader.push_mat4(0, plane_model_matrix);

        material_ubo.sub_data(buffers::data::create(&plane_color));

        plane_vertex_array.bind();
        opengl::Commands::draw_indexed(opengl::triangles, static_cast<int32_t>(plane_faces.size()) * primitives::triangle::elements);

        #pragma endregion
        #pragma region Box

        mat4 box_model_matrix;
        box_model_matrix.identity();
        box_model_matrix.translate({ 1.0f, 1.0f, 0.0f });
        default_shader.push_mat4(0, box_model_matrix);

        material_ubo.sub_data(buffers::data::create(&box_color));

        box_vertex_array.bind();
        opengl::Commands::draw_indexed(opengl::triangles, static_cast<int32_t>(box_faces.size()) * primitives::triangle::elements);

        #pragma endregion
        #pragma region Sphere

        mat4 sphere_model_matrix;
        sphere_model_matrix.identity();
        sphere_model_matrix.translate({ -1.0f, 1.0f, 0.0f });
        default_shader.push_mat4(0, sphere_model_matrix);

        material_ubo.sub_data(buffers::data::create(&sphere_color));

        sphere_vertex_array.bind();
        opengl::Commands::draw_indexed(opengl::triangles, static_cast<int32_t>(sphere_faces.size()) * primitives::triangle::elements);

        #pragma endregion

        WindowManager::instance().update();
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

    camera_ubo.destroy();
    material_ubo.destroy();

    #pragma endregion

    default_shader.destroy();

    WindowManager::instance().destroy();

    return 0;
}