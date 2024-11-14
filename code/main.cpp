#include <opengl/commands.hpp>
#include <opengl/pipeline.hpp>
#include <opengl/sampler.hpp>

#include <renderer/model.hpp>

#include <data/material.hpp>
#include <data/camera.hpp>
#include <data/light.hpp>

#include <vertex/model.hpp>
#include <vertex/ui.hpp>

#include <image/tga.hpp>

#include <window_manager.hpp>
#include <model_renderer.hpp>
#include <primitives.hpp>
#include <functions.hpp>
#include <graphics.hpp>
#include <platform.hpp>
#include <mesh.hpp>
#include <time.hpp>
#include <file.hpp>
#include <quat.hpp>

int32_t main()
{
    engine::Platform::init();

    engine::WindowManager::instance().create({ .size = { 2048, 1024 } }, { });
    engine::WindowManager::instance().open();

    engine::Graphics::init();

    #pragma region Shaders

    #pragma region Default

    engine::opengl::ShaderStage default_vertex_stage   { engine::opengl::shader::vertex   };
    engine::opengl::ShaderStage default_fragment_stage { engine::opengl::shader::fragment };

    default_vertex_stage.create();
    default_vertex_stage.source(engine::File::read("default_shader.vert",   std::ios::binary));

    default_fragment_stage.create();
    default_fragment_stage.source(engine::File::read("default_shader.frag", std::ios::binary));

    engine::opengl::Shader default_shader;
    default_shader.create();
    default_shader.attach(default_vertex_stage);
    default_shader.attach(default_fragment_stage);
    default_shader.link();

    default_vertex_stage.destroy();
    default_fragment_stage.destroy();

    #pragma endregion
    #pragma region UI

    engine::opengl::ShaderStage default_ui_vertex_stage   { engine::opengl::shader::vertex   };
    engine::opengl::ShaderStage default_ui_fragment_stage { engine::opengl::shader::fragment };

    default_ui_vertex_stage.create();
    default_ui_vertex_stage.source(engine::File::read("default_ui_shader.vert",   std::ios::binary));

    default_ui_fragment_stage.create();
    default_ui_fragment_stage.source(engine::File::read("default_ui_shader.frag", std::ios::binary));

    engine::opengl::Shader default_ui_shader;
    default_ui_shader.create();
    default_ui_shader.attach(default_ui_vertex_stage);
    default_ui_shader.attach(default_ui_fragment_stage);
    default_ui_shader.link();

    default_ui_vertex_stage.destroy();
    default_ui_fragment_stage.destroy();

    #pragma endregion
    #pragma region Model

    engine::opengl::ShaderStage model_vertex_stage   { engine::opengl::shader::vertex   };
    engine::opengl::ShaderStage model_fragment_stage { engine::opengl::shader::fragment };

    model_vertex_stage.create();
    model_vertex_stage.source(engine::File::read("model_shader.vert",   std::ios::binary));

    model_fragment_stage.create();
    model_fragment_stage.source(engine::File::read("model_shader.frag", std::ios::binary));

    engine::opengl::Shader model_shader;
    model_shader.create();
    model_shader.attach(model_vertex_stage);
    model_shader.attach(model_fragment_stage);
    model_shader.link();

    model_vertex_stage.destroy();
    model_fragment_stage.destroy();

    #pragma endregion

    #pragma endregion

    #pragma region Plane

    auto [plane_vertices, plane_faces] = editor::Primitives::create_plane(10.0f, 10.0f);

    engine::opengl::Buffer plane_vertex_buffer;
    plane_vertex_buffer.create();
    plane_vertex_buffer.data(engine::buffer::data::create(plane_vertices));

    engine::opengl::Buffer plane_index_buffer;
    plane_index_buffer.create();
    plane_index_buffer.data(engine::buffer::data::create(plane_faces));

    engine::opengl::VertexArray plane_vertex_array;
    plane_vertex_array.create();
    plane_vertex_array.attach_vertices(&plane_vertex_buffer, sizeof(editor::base::vertex));
    plane_vertex_array.attach_indices(&plane_index_buffer);

    plane_vertex_array.attribute({ 0, 3, engine::opengl::type_float  });
    plane_vertex_array.attribute({ 1, 3, engine::opengl::type_float, offsetof(editor::base::vertex, extra) });

    #pragma endregion
    #pragma region Box

    auto [box_vertices, box_faces] = editor::Primitives::create_box();

    engine::opengl::Buffer box_vertex_buffer;
    box_vertex_buffer.create();
    box_vertex_buffer.data(engine::buffer::data::create(box_vertices));

    engine::opengl::Buffer box_index_buffer;
    box_index_buffer.create();
    box_index_buffer.data(engine::buffer::data::create(box_faces));

    engine::opengl::VertexArray box_vertex_array;
    box_vertex_array.create();
    box_vertex_array.attach_vertices(&box_vertex_buffer, sizeof(editor::base::vertex));
    box_vertex_array.attach_indices(&box_index_buffer);

    box_vertex_array.attribute({ 0, 3, engine::opengl::type_float  });
    box_vertex_array.attribute({ 1, 3, engine::opengl::type_float, offsetof(editor::base::vertex, extra) });

    #pragma endregion
    #pragma region Sphere

    auto [sphere_vertices, sphere_faces] = editor::Primitives::create_sphere();

    engine::opengl::Buffer sphere_vertex_buffer;
    sphere_vertex_buffer.create();
    sphere_vertex_buffer.data(engine::buffer::data::create(sphere_vertices));

    engine::opengl::Buffer sphere_index_buffer;
    sphere_index_buffer.create();
    sphere_index_buffer.data(engine::buffer::data::create(sphere_faces));

    engine::opengl::VertexArray sphere_vertex_array;
    sphere_vertex_array.create();
    sphere_vertex_array.attach_vertices(&sphere_vertex_buffer, sizeof(editor::base::vertex));
    sphere_vertex_array.attach_indices(&sphere_index_buffer);

    sphere_vertex_array.attribute({ 0, 3, engine::opengl::type_float  });
    sphere_vertex_array.attribute({ 1, 3, engine::opengl::type_float, offsetof(editor::base::vertex, extra) });

    #pragma endregion
    #pragma region Capsule

    auto [capsule_vertices, capsule_faces] = editor::Primitives::create_capsule();

    engine::opengl::Buffer capsule_vertex_buffer;
    capsule_vertex_buffer.create();
    capsule_vertex_buffer.data(engine::buffer::data::create(capsule_vertices));

    engine::opengl::Buffer capsule_index_buffer;
    capsule_index_buffer.create();
    capsule_index_buffer.data(engine::buffer::data::create(capsule_faces));

    engine::opengl::VertexArray capsule_vertex_array;
    capsule_vertex_array.create();
    capsule_vertex_array.attach_vertices(&capsule_vertex_buffer, sizeof(editor::base::vertex));
    capsule_vertex_array.attach_indices(&capsule_index_buffer);

    capsule_vertex_array.attribute({ 0, 3, engine::opengl::type_float  });
    capsule_vertex_array.attribute({ 1, 3, engine::opengl::type_float, offsetof(editor::base::vertex, extra) });

    #pragma endregion
    #pragma region Crate

    constexpr float crate_half_x = 0.5f;
    constexpr float crate_half_y = 0.5f;
    constexpr float crate_half_z = 0.5f;

    const std::vector<engine::vertex::model> crate_vertices
    {
        { { -crate_half_x, -crate_half_y,  crate_half_z }, engine::vec3::front(), { 0.0f, 0.0f } },
        { {  crate_half_x, -crate_half_y,  crate_half_z }, engine::vec3::front(), { 1.0f, 0.0f } },
        { {  crate_half_x,  crate_half_y,  crate_half_z }, engine::vec3::front(), { 1.0f, 1.0f } },
        { { -crate_half_x,  crate_half_y,  crate_half_z }, engine::vec3::front(), { 0.0f, 1.0f } },

        { { -crate_half_x, -crate_half_y, -crate_half_z }, engine::vec3::back(), { 0.0f, 0.0f } },
        { { -crate_half_x,  crate_half_y, -crate_half_z }, engine::vec3::back(), { 1.0f, 0.0f } },
        { {  crate_half_x,  crate_half_y, -crate_half_z }, engine::vec3::back(), { 1.0f, 1.0f } },
        { {  crate_half_x, -crate_half_y, -crate_half_z }, engine::vec3::back(), { 0.0f, 1.0f } },

        { { -crate_half_x,  crate_half_y,  crate_half_z }, engine::vec3::left(), { 0.0f, 0.0f } },
        { { -crate_half_x,  crate_half_y, -crate_half_z }, engine::vec3::left(), { 1.0f, 0.0f } },
        { { -crate_half_x, -crate_half_y, -crate_half_z }, engine::vec3::left(), { 1.0f, 1.0f } },
        { { -crate_half_x, -crate_half_y,  crate_half_z }, engine::vec3::left(), { 0.0f, 1.0f } },

        { {  crate_half_x,  crate_half_y,  crate_half_z }, engine::vec3::right(), { 0.0f, 0.0f } },
        { {  crate_half_x, -crate_half_y,  crate_half_z }, engine::vec3::right(), { 1.0f, 0.0f } },
        { {  crate_half_x, -crate_half_y, -crate_half_z }, engine::vec3::right(), { 1.0f, 1.0f } },
        { {  crate_half_x,  crate_half_y, -crate_half_z }, engine::vec3::right(), { 0.0f, 1.0f } },

        { { -crate_half_x,  crate_half_y, -crate_half_z }, engine::vec3::up(), { 0.0f, 0.0f } },
        { { -crate_half_x,  crate_half_y,  crate_half_z }, engine::vec3::up(), { 1.0f, 0.0f } },
        { {  crate_half_x,  crate_half_y,  crate_half_z }, engine::vec3::up(), { 1.0f, 1.0f } },
        { {  crate_half_x,  crate_half_y, -crate_half_z }, engine::vec3::up(), { 0.0f, 1.0f } },

        { { -crate_half_x, -crate_half_y, -crate_half_z }, engine::vec3::down(), { 0.0f, 0.0f } },
        { {  crate_half_x, -crate_half_y, -crate_half_z }, engine::vec3::down(), { 1.0f, 0.0f } },
        { {  crate_half_x, -crate_half_y,  crate_half_z }, engine::vec3::down(), { 1.0f, 1.0f } },
        { { -crate_half_x, -crate_half_y,  crate_half_z }, engine::vec3::down(), { 0.0f, 1.0f } },
    };

    const std::vector<engine::primitive::triangle> crate_faces
    {
        {  0,  1,  2 }, {  2,  3,  0 }, // front face
        {  4,  5,  6 }, {  6,  7,  4 }, //  back face
        {  8,  9, 10 }, { 10, 11,  8 }, //  left face
        { 12, 13, 14 }, { 14, 15, 12 }, // right face
        { 16, 17, 18 }, { 18, 19, 16 }, //    up face
        { 20, 21, 22 }, { 22, 23, 20 }  //  down face
    };

    engine::opengl::Buffer crate_vertex_buffer;
    crate_vertex_buffer.create();
    crate_vertex_buffer.data(engine::buffer::data::create(crate_vertices));

    engine::opengl::Buffer crate_index_buffer;
    crate_index_buffer.create();
    crate_index_buffer.data(engine::buffer::data::create(crate_faces));

    engine::opengl::VertexArray crate_vertex_array;
    crate_vertex_array.create();
    crate_vertex_array.attach_vertices(&crate_vertex_buffer, sizeof(engine::vertex::model));
    crate_vertex_array.attach_indices(&crate_index_buffer);

    crate_vertex_array.attribute({ 0, 3, engine::opengl::type_float  });
    crate_vertex_array.attribute({ 1, 3, engine::opengl::type_float, offsetof(engine::vertex::model, normal) });
    crate_vertex_array.attribute({ 2, 2, engine::opengl::type_float, offsetof(engine::vertex::model, uv)     });

    #pragma endregion

    #pragma region UI

    engine::base::geometry<engine::vertex::ui, engine::primitive::triangle> ui_geometry
    {
        {
            { { -100.0f,  100.0f }, { 0.0f, 0.0f } },
            { {  100.0f,  100.0f }, { 1.0f, 0.0f } },
            { {  100.0f, -100.0f }, { 1.0f, 1.0f } },
            { { -100.0f, -100.0f }, { 0.0f, 1.0f } },
        },
        {
            {  0,  1,  2 }, {  2,  3,  0 } // front face
        }
    };

    const std::vector<engine::vertex::attribute> ui_attributes
    {
        { 0, 2, engine::opengl::type_float  },
        { 1, 2, engine::opengl::type_float, offsetof(engine::vertex::ui, uv) }
    };

    engine::Mesh ui_mesh { engine::opengl::triangles };

    ui_mesh.create(sizeof(engine::vertex::ui));
    ui_mesh.attributes(ui_attributes);

    ui_mesh.update(ui_geometry);

    #pragma endregion

    #pragma region Samplers

    engine::opengl::Sampler default_sampler;
    default_sampler.create();
    default_sampler.bind();

    default_sampler.parameter(engine::opengl::sampler::wrap_s, engine::opengl::sampler::repeat);
    default_sampler.parameter(engine::opengl::sampler::wrap_t, engine::opengl::sampler::repeat);

    default_sampler.parameter(engine::opengl::sampler::min_filter, engine::opengl::sampler::linear);
    default_sampler.parameter(engine::opengl::sampler::mag_filter, engine::opengl::sampler::linear);

    #pragma endregion

    #pragma region Textures

    auto [crate_image_width, crate_image_height, crate_image_pixels] = engine::image::Tga::load("crate.tga");

    engine::opengl::Texture crate_texture { engine::opengl::texture::texture_2d, engine::opengl::texture::rgb8, crate_image_width, crate_image_height };

    crate_texture.create();
    crate_texture.storage();
    crate_texture.update(engine::opengl::texture::rgb, crate_image_pixels.data());

    #pragma endregion

    engine::Time time;
    time.init();

    int32_t width  = engine::WindowManager::instance().width() / 2;
    int32_t height = engine::WindowManager::instance().height();

    engine::vec3 camera_position { 0.0f, 2.5, 7.5f };

    engine::data::camera camera;
    camera.view.look(camera_position, { });
    camera.projection.perspective(60.0f, static_cast<float>(width) / static_cast<float>(height));

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
    sphere_matrix.translate({ 2.0f, 1.0f, -2.0f });

    engine::mat4 capsule_matrix;
    capsule_matrix.identity();
    capsule_matrix.translate({ 2.0f, 1.0f, 2.0f });

    constexpr engine::rgb plane_color   { 0.5f, 0.5f, 1.0f };
    constexpr engine::rgb box_color     { 0.0f, 1.0f, 1.0f };
    constexpr engine::rgb sphere_color  { 1.0f, 1.0f, 0.0f };
    constexpr engine::rgb capsule_color { 1.0f, 0.5f, 0.5f };

    #pragma region Uniform Buffers

    engine::opengl::Buffer camera_buffer;
    camera_buffer.create();
    camera_buffer.bind();
    camera_buffer.data(engine::buffer::data::create(&camera));

    engine::opengl::Buffer material_buffer;
    material_buffer.create();
    material_buffer.bind(engine::buffer::location::material);
    material_buffer.data(engine::buffer::data::create(&material));

    engine::opengl::Buffer light_buffer;
    light_buffer.create();
    light_buffer.bind(engine::buffer::location::light);
    light_buffer.data(engine::buffer::data::create(&light));

    #pragma endregion

    editor::ModelRenderer default_model_renderer;
    default_model_renderer.attach(&default_shader);
    default_model_renderer.attach(&material_buffer);

    engine::renderer::Model model_renderer;
    model_renderer.attach(&model_shader);

    engine::WindowManager::instance().resize_callback([&camera_buffer, &camera]
    {
        const int32_t width  = engine::WindowManager::instance().width() / 2;
        const int32_t height = engine::WindowManager::instance().height();

        camera.projection.perspective(60.0f, static_cast<float>(width) / static_cast<float>(height));
        camera_buffer.update(engine::buffer::data::create(&camera.projection), offsetof(engine::data::camera, projection));
    });

    while (engine::WindowManager::instance().is_active())
    {
        time.update();

        constexpr float camera_speed  = 0.25f;
        constexpr float camera_radius = 7.50f;

        camera_position.x = engine::sin(engine::Time::total_time() * camera_speed) * -camera_radius;
        camera_position.z = engine::cos(engine::Time::total_time() * camera_speed) *  camera_radius;

        camera.view.look(camera_position, { });
        camera_buffer.update(engine::buffer::data::create(&camera.view));

        engine::quat box_orientation;
        box_orientation.rotate({ 0.0f, 1.0f, 0.0f }, engine::Time::total_time() * 90.0f);

        engine::mat4 box_matrix;
        box_matrix.identity();
        box_matrix.translate({ -2.0f, 1.0f, 2.0f });
        box_matrix *= box_orientation;

        engine::quat crate_orientation;
        crate_orientation.rotate({ 1.0f, 0.0f, 0.0f }, engine::Time::total_time() * 45.0f);

        engine::mat4 crate_matrix;
        crate_matrix.identity();
        crate_matrix.translate({ -2.0f, 1.0f, -2.0f });
        crate_matrix *= crate_orientation;

        width  = engine::WindowManager::instance().width() / 2;
        height = engine::WindowManager::instance().height();

        #pragma region Editor

        engine::opengl::Commands::viewport(0, 0, width, height);

        engine::opengl::Commands::clear(1.0f, 0.5f, 0.0f);
        engine::opengl::Commands::clear(engine::opengl::color_buffer | engine::opengl::depth_buffer);

        default_model_renderer.bind();

        default_model_renderer.draw(&plane_vertex_array,   plane_matrix,   plane_color,   static_cast<int32_t>(plane_faces.size()));
        default_model_renderer.draw(&box_vertex_array,     box_matrix,     box_color,     static_cast<int32_t>(box_faces.size()));
        default_model_renderer.draw(&sphere_vertex_array,  sphere_matrix,  sphere_color,  static_cast<int32_t>(sphere_faces.size()));
        default_model_renderer.draw(&capsule_vertex_array, capsule_matrix, capsule_color, static_cast<int32_t>(capsule_faces.size()));
        default_model_renderer.draw(&crate_vertex_array,   crate_matrix,   box_color,     static_cast<int32_t>(crate_faces.size()));

        #pragma endregion
        #pragma region Engine

        const auto [r, g, b] = engine::rgb::black();

        engine::opengl::Commands::viewport(width, 0, width, height);

        engine::opengl::Commands::clear(r, g, b);
        //engine::opengl::Commands::clear(engine::opengl::color_buffer | engine::opengl::depth_buffer);

        model_renderer.bind();

        model_renderer.draw(&crate_vertex_array, &crate_texture, crate_matrix, static_cast<int32_t>(crate_faces.size()));

        #pragma endregion
        #pragma region UI

        engine::mat4 ui_model;
        ui_model.identity();
        ui_model.translate({ 100, 100, 0 });

        engine::mat4 ui_view;
        ui_view.identity();

        engine::mat4 ui_projection;
        ui_projection.orthographic(0, static_cast<float>(width), static_cast<float>(height), 0);

        default_ui_shader.bind();
        default_ui_shader.push(ui_model);
        default_ui_shader.push(ui_view,       1);
        default_ui_shader.push(ui_projection, 2);

        crate_texture.bind();

        ui_mesh.bind();

        engine::opengl::Commands::draw_indexed(engine::opengl::triangles, engine::primitive::triangle::elements * ui_mesh.faces());

        #pragma endregion

        engine::WindowManager::instance().update();
    }

    #pragma region Buffers

    ui_mesh.destroy();

    plane_index_buffer.destroy();
    plane_vertex_buffer.destroy();
    plane_vertex_array.destroy();

    box_index_buffer.destroy();
    box_vertex_buffer.destroy();
    box_vertex_array.destroy();

    sphere_index_buffer.destroy();
    sphere_vertex_buffer.destroy();
    sphere_vertex_array.destroy();

    capsule_index_buffer.destroy();
    capsule_vertex_buffer.destroy();
    capsule_vertex_array.destroy();

    crate_index_buffer.destroy();
    crate_vertex_buffer.destroy();
    crate_vertex_array.destroy();

    material_buffer.destroy();
    camera_buffer.destroy();
    light_buffer.destroy();

    #pragma endregion
    #pragma region Shaders

    default_shader.destroy();
    model_shader.destroy();

    #pragma endregion

    crate_texture.destroy();

    default_sampler.destroy();

    engine::WindowManager::instance().destroy();

    return 0;
}
