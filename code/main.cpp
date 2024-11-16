#include <tools/primitive_generator.hpp>
#include <tools/sprite_generator.hpp>
#include <tools/shader_converter.hpp>

#include <opengl/commands.hpp>
#include <opengl/pipeline.hpp>
#include <opengl/sampler.hpp>

#include <renderer/sprite.hpp>
#include <renderer/model.hpp>

#include <data/material.hpp>
#include <data/camera.hpp>
#include <data/light.hpp>

#include <vertex/sprite.hpp>
#include <vertex/model.hpp>

#include <image/tga.hpp>

#include <window_manager.hpp>
#include <functions.hpp>
#include <graphics.hpp>
#include <platform.hpp>
#include <time.hpp>
#include <file.hpp>

int32_t main()
{
    const std::string assets_folder = "../assets/";

    editor::tools::ShaderConverter::convert("default_base_shader",   assets_folder);
    editor::tools::ShaderConverter::convert("default_model_shader",  assets_folder);
    editor::tools::ShaderConverter::convert("default_sprite_shader", assets_folder);

    engine::Platform::init();

    engine::WindowManager::instance().create({ .size = { 2048, 1024 } }, { });
    engine::WindowManager::instance().open();

    engine::Graphics::init();

    #pragma region Shaders

    #pragma region Default

    engine::opengl::ShaderStage default_vertex_stage   { engine::opengl::shader::vertex   };
    engine::opengl::ShaderStage default_fragment_stage { engine::opengl::shader::fragment };

    default_vertex_stage.create();
    default_vertex_stage.source(engine::File::read("default_base_shader.vert",   std::ios::binary));

    default_fragment_stage.create();
    default_fragment_stage.source(engine::File::read("default_base_shader.frag", std::ios::binary));

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
    default_ui_vertex_stage.source(engine::File::read("default_sprite_shader.vert",   std::ios::binary));

    default_ui_fragment_stage.create();
    default_ui_fragment_stage.source(engine::File::read("default_sprite_shader.frag", std::ios::binary));

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
    model_vertex_stage.source(engine::File::read("default_model_shader.vert",   std::ios::binary));

    model_fragment_stage.create();
    model_fragment_stage.source(engine::File::read("default_model_shader.frag", std::ios::binary));

    engine::opengl::Shader model_shader;
    model_shader.create();
    model_shader.attach(model_vertex_stage);
    model_shader.attach(model_fragment_stage);
    model_shader.link();

    model_vertex_stage.destroy();
    model_fragment_stage.destroy();

    #pragma endregion

    #pragma endregion
    #pragma region Meshes

    constexpr auto  debug_vertex_size = sizeof(editor::vertex::debug);
    constexpr auto  model_vertex_size = sizeof(engine::vertex::model);
    constexpr auto sprite_vertex_size = sizeof(engine::vertex::sprite);

    const std::vector<engine::vertex::attribute> debug_vertex_attributes
    {
        { 0, 3, engine::opengl::type_float  },
        { 1, 3, engine::opengl::type_float, offsetof(editor::vertex::debug, extra) }
    };

    const std::vector<engine::vertex::attribute> model_vertex_attributes
    {
        { 0, 3, engine::opengl::type_float  },
        { 1, 3, engine::opengl::type_float, offsetof(engine::vertex::model, normal) },
        { 2, 2, engine::opengl::type_float, offsetof(engine::vertex::model, uv)     }
    };

    const std::vector<engine::vertex::attribute> sprite_vertex_attributes
    {
        { 0, 2, engine::opengl::type_float  },
        { 1, 2, engine::opengl::type_float, offsetof(engine::vertex::sprite, uv) }
    };

    engine::core::Mesh plane_mesh;
    plane_mesh.create(debug_vertex_size);
    plane_mesh.update(editor::tools::PrimitiveGenerator::create_plane(10.0f, 10.0f));
    plane_mesh.attributes(debug_vertex_attributes);

    engine::core::Mesh box_mesh;
    box_mesh.create(debug_vertex_size);
    box_mesh.update(editor::tools::PrimitiveGenerator::create_box());
    box_mesh.attributes(debug_vertex_attributes);

    engine::core::Mesh sphere_mesh;
    sphere_mesh.create(debug_vertex_size);
    sphere_mesh.update(editor::tools::PrimitiveGenerator::create_sphere());
    sphere_mesh.attributes(debug_vertex_attributes);

    engine::core::Mesh capsule_mesh;
    capsule_mesh.create(debug_vertex_size);
    capsule_mesh.update(editor::tools::PrimitiveGenerator::create_capsule());
    capsule_mesh.attributes(debug_vertex_attributes);

    #pragma endregion
    #pragma region Crate

    constexpr float crate_half_x = 0.5f;
    constexpr float crate_half_y = 0.5f;
    constexpr float crate_half_z = 0.5f;

    const engine::base::geometry<engine::vertex::model, engine::primitive::triangle> crate_geometry
    {
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
        },
        {
            {  0,  1,  2 }, {  2,  3,  0 }, // front face
            {  4,  5,  6 }, {  6,  7,  4 }, //  back face
            {  8,  9, 10 }, { 10, 11,  8 }, //  left face
            { 12, 13, 14 }, { 14, 15, 12 }, // right face
            { 16, 17, 18 }, { 18, 19, 16 }, //    up face
            { 20, 21, 22 }, { 22, 23, 20 }  //  down face
        }
    };

    engine::core::Mesh crate_mesh;
    crate_mesh.create(model_vertex_size);
    crate_mesh.update(crate_geometry);
    crate_mesh.attributes(model_vertex_attributes);

    #pragma endregion
    #pragma region Sprites

    auto sprite_geometry = engine::tools::SpriteGenerator::create({ { }, { 200.0f, 200.0f } });

    engine::core::Mesh sprite_mesh;
    sprite_mesh.create(sprite_vertex_size);
    sprite_mesh.update(sprite_geometry);
    sprite_mesh.attributes(sprite_vertex_attributes);

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

    engine::window::size window_size
    {
        engine::WindowManager::instance().width() / 2,
        engine::WindowManager::instance().height()
    };

    engine::data::camera camera;
    camera.projection.perspective(60.0f, window_size.ratio());

    engine::vec3     camera_position { 0.0f, 2.5, 7.5f };
    camera.view.look(camera_position, { });

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
    camera_buffer.data(engine::buffer::data::create(&camera), engine::opengl::dynamic_draw);

    engine::opengl::Buffer material_buffer;
    material_buffer.create();
    material_buffer.bind(engine::buffer::location::material);
    material_buffer.data(engine::buffer::data::create(&material), engine::opengl::dynamic_draw);

    engine::opengl::Buffer light_buffer;
    light_buffer.create();
    light_buffer.bind(engine::buffer::location::light);
    light_buffer.data(engine::buffer::data::create(&light));

    #pragma endregion
    #pragma region Renderers

    engine::renderer::Model default_base_renderer;
    default_base_renderer.attach(&default_shader);
    default_base_renderer.attach(&material_buffer);

    engine::renderer::Model default_model_renderer;
    default_model_renderer.attach(&model_shader);
    default_model_renderer.attach(&material_buffer);

    #pragma endregion

    engine::WindowManager::instance().on_size([&window_size, &camera_buffer, &camera]
    {
       window_size =
       {
           engine::WindowManager::instance().width() / 2,
           engine::WindowManager::instance().height()
       };

        camera.projection.perspective(60.0f, window_size.ratio());
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

        #pragma region Editor

        engine::opengl::Commands::viewport(0, 0, window_size.width(), window_size.height());

        engine::opengl::Commands::clear(1.0f, 0.5f, 0.0f);
        engine::opengl::Commands::clear(engine::opengl::color_buffer | engine::opengl::depth_buffer);

        default_base_renderer.begin();

        default_base_renderer.draw(&plane_mesh,   plane_matrix,   plane_color);
        default_base_renderer.draw(&box_mesh,     box_matrix,     box_color);
        default_base_renderer.draw(&sphere_mesh,  sphere_matrix,  sphere_color);
        default_base_renderer.draw(&capsule_mesh, capsule_matrix, capsule_color);
        default_base_renderer.draw(&crate_mesh,   crate_matrix,   box_color);

        #pragma endregion
        #pragma region Engine

        const auto [r, g, b] = engine::rgb::black();

        engine::opengl::Commands::viewport(window_size.width(), 0, window_size.width(), window_size.height());

        //engine::opengl::Commands::clear(r, g, b);
        //engine::opengl::Commands::clear(engine::opengl::color_buffer | engine::opengl::depth_buffer);

        default_model_renderer.begin();

        default_model_renderer.draw(&crate_mesh, crate_matrix, &crate_texture);

        #pragma endregion
        #pragma region UI

        engine::mat4 sprite_model;
        sprite_model.identity();

        engine::mat4 ui_view;
        ui_view.identity();

        engine::mat4 ui_projection;
        ui_projection.orthographic(0, static_cast<float>(window_size.width()), static_cast<float>(window_size.height()), 0);

        default_ui_shader.bind();
        default_ui_shader.push(sprite_model);
        default_ui_shader.push(ui_view,       1);
        default_ui_shader.push(ui_projection, 2);

        crate_texture.bind();

        sprite_mesh.bind();

        engine::opengl::Commands::draw_indexed(engine::opengl::triangles, engine::primitive::triangle::elements * sprite_mesh.faces());

        #pragma endregion

        engine::WindowManager::instance().update();
    }

    #pragma region Meshes

    plane_mesh.destroy();
    box_mesh.destroy();
    sphere_mesh.destroy();
    capsule_mesh.destroy();
    crate_mesh.destroy();

    sprite_mesh.destroy();

    #pragma endregion
    #pragma region Buffers

    material_buffer.destroy();
    camera_buffer.destroy();
    light_buffer.destroy();

    #pragma endregion
    #pragma region Shaders

    default_shader.destroy();
    model_shader.destroy();

    #pragma endregion
    #pragma region Textures

    crate_texture.destroy();

    default_sampler.destroy();

    #pragma endregion

    engine::WindowManager::instance().destroy();

    return 0;
}