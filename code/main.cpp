#include <tools/primitive_generator.hpp>
#include <tools/sprite_generator.hpp>
#include <tools/shader_converter.hpp>

#include <opengl/commands.hpp>
#include <opengl/pipeline.hpp>
#include <opengl/sampler.hpp>

#include <graphics/sprite_renderer.hpp>
#include <graphics/model_renderer.hpp>

#include <data/material.hpp>
#include <data/camera.hpp>
#include <data/light.hpp>

#include <math/functions.hpp>

#include <core/window_manager.hpp>
#include <core/vertex/sprite.hpp>
#include <core/vertex/model.hpp>
#include <core/graphics.hpp>
#include <core/platform.hpp>
#include <core/time.hpp>
#include <core/file.hpp>

#include <images/tga.hpp>

int32_t main()
{
    const std::string assets_folder = "../assets/";

    tools::ShaderConverter::convert("default_base_shader",   assets_folder);
    tools::ShaderConverter::convert("default_model_shader",  assets_folder);
    tools::ShaderConverter::convert("default_sprite_shader", assets_folder);

    core::Platform::init();

    core::WindowManager::instance().create({ .size = { 2048, 1024 } }, { });
    core::WindowManager::instance().open();

    core::Graphics::init();

    #pragma region Shaders

    #pragma region Default

    opengl::ShaderStage default_vertex_stage   { opengl::shader::vertex   };
    opengl::ShaderStage default_fragment_stage { opengl::shader::fragment };

    default_vertex_stage.create();
    default_vertex_stage.source(core::File::read("default_base_shader.vert",   std::ios::binary));

    default_fragment_stage.create();
    default_fragment_stage.source(core::File::read("default_base_shader.frag", std::ios::binary));

    opengl::Shader default_shader;
    default_shader.create();
    default_shader.attach(default_vertex_stage);
    default_shader.attach(default_fragment_stage);
    default_shader.link();

    default_vertex_stage.destroy();
    default_fragment_stage.destroy();

    #pragma endregion
    #pragma region UI

    opengl::ShaderStage default_ui_vertex_stage   { opengl::shader::vertex   };
    opengl::ShaderStage default_ui_fragment_stage { opengl::shader::fragment };

    default_ui_vertex_stage.create();
    default_ui_vertex_stage.source(core::File::read("default_sprite_shader.vert",   std::ios::binary));

    default_ui_fragment_stage.create();
    default_ui_fragment_stage.source(core::File::read("default_sprite_shader.frag", std::ios::binary));

    opengl::Shader default_ui_shader;
    default_ui_shader.create();
    default_ui_shader.attach(default_ui_vertex_stage);
    default_ui_shader.attach(default_ui_fragment_stage);
    default_ui_shader.link();

    default_ui_vertex_stage.destroy();
    default_ui_fragment_stage.destroy();

    #pragma endregion
    #pragma region Model

    opengl::ShaderStage model_vertex_stage   { opengl::shader::vertex   };
    opengl::ShaderStage model_fragment_stage { opengl::shader::fragment };

    model_vertex_stage.create();
    model_vertex_stage.source(core::File::read("default_model_shader.vert",   std::ios::binary));

    model_fragment_stage.create();
    model_fragment_stage.source(core::File::read("default_model_shader.frag", std::ios::binary));

    opengl::Shader model_shader;
    model_shader.create();
    model_shader.attach(model_vertex_stage);
    model_shader.attach(model_fragment_stage);
    model_shader.link();

    model_vertex_stage.destroy();
    model_fragment_stage.destroy();

    #pragma endregion

    #pragma endregion
    #pragma region Meshes

    constexpr auto  debug_vertex_size = sizeof(debug::vertex);
    constexpr auto  model_vertex_size = sizeof(core::vertex::model);
    constexpr auto sprite_vertex_size = sizeof(core::vertex::sprite);

    const std::vector<core::vertex::attribute> debug_vertex_attributes
    {
        { 0, 3, opengl::type_float  },
        { 1, 3, opengl::type_float, offsetof(debug::vertex, extra) }
    };

    const std::vector<core::vertex::attribute> model_vertex_attributes
    {
        { 0, 3, opengl::type_float  },
        { 1, 3, opengl::type_float, offsetof(core::vertex::model, normal) },
        { 2, 2, opengl::type_float, offsetof(core::vertex::model, uv)     }
    };

    const std::vector<core::vertex::attribute> sprite_vertex_attributes
    {
        { 0, 2, opengl::type_float  },
        { 1, 2, opengl::type_float, offsetof(core::vertex::sprite, uv) }
    };

    core::Mesh plane_mesh;
    plane_mesh.create(debug_vertex_size);
    plane_mesh.update(tools::PrimitiveGenerator::create_plane(10.0f, 10.0f));
    plane_mesh.attributes(debug_vertex_attributes);

    core::Mesh box_mesh;
    box_mesh.create(debug_vertex_size);
    box_mesh.update(tools::PrimitiveGenerator::create_box());
    box_mesh.attributes(debug_vertex_attributes);

    core::Mesh sphere_mesh;
    sphere_mesh.create(debug_vertex_size);
    sphere_mesh.update(tools::PrimitiveGenerator::create_sphere());
    sphere_mesh.attributes(debug_vertex_attributes);

    core::Mesh capsule_mesh;
    capsule_mesh.create(debug_vertex_size);
    capsule_mesh.update(tools::PrimitiveGenerator::create_capsule());
    capsule_mesh.attributes(debug_vertex_attributes);

    #pragma endregion
    #pragma region Crate

    constexpr float crate_half_x = 0.5f;
    constexpr float crate_half_y = 0.5f;
    constexpr float crate_half_z = 0.5f;

    const core::geometry<core::vertex::model, primitive::triangle> crate_geometry
    {
        {
            { { -crate_half_x, -crate_half_y,  crate_half_z }, math::vec3::front(), { 0.0f, 0.0f } },
            { {  crate_half_x, -crate_half_y,  crate_half_z }, math::vec3::front(), { 1.0f, 0.0f } },
            { {  crate_half_x,  crate_half_y,  crate_half_z }, math::vec3::front(), { 1.0f, 1.0f } },
            { { -crate_half_x,  crate_half_y,  crate_half_z }, math::vec3::front(), { 0.0f, 1.0f } },

            { { -crate_half_x, -crate_half_y, -crate_half_z }, math::vec3::back(), { 0.0f, 0.0f } },
            { { -crate_half_x,  crate_half_y, -crate_half_z }, math::vec3::back(), { 1.0f, 0.0f } },
            { {  crate_half_x,  crate_half_y, -crate_half_z }, math::vec3::back(), { 1.0f, 1.0f } },
            { {  crate_half_x, -crate_half_y, -crate_half_z }, math::vec3::back(), { 0.0f, 1.0f } },

            { { -crate_half_x,  crate_half_y,  crate_half_z }, math::vec3::left(), { 0.0f, 0.0f } },
            { { -crate_half_x,  crate_half_y, -crate_half_z }, math::vec3::left(), { 1.0f, 0.0f } },
            { { -crate_half_x, -crate_half_y, -crate_half_z }, math::vec3::left(), { 1.0f, 1.0f } },
            { { -crate_half_x, -crate_half_y,  crate_half_z }, math::vec3::left(), { 0.0f, 1.0f } },

            { {  crate_half_x,  crate_half_y,  crate_half_z }, math::vec3::right(), { 0.0f, 0.0f } },
            { {  crate_half_x, -crate_half_y,  crate_half_z }, math::vec3::right(), { 1.0f, 0.0f } },
            { {  crate_half_x, -crate_half_y, -crate_half_z }, math::vec3::right(), { 1.0f, 1.0f } },
            { {  crate_half_x,  crate_half_y, -crate_half_z }, math::vec3::right(), { 0.0f, 1.0f } },

            { { -crate_half_x,  crate_half_y, -crate_half_z }, math::vec3::up(), { 0.0f, 0.0f } },
            { { -crate_half_x,  crate_half_y,  crate_half_z }, math::vec3::up(), { 1.0f, 0.0f } },
            { {  crate_half_x,  crate_half_y,  crate_half_z }, math::vec3::up(), { 1.0f, 1.0f } },
            { {  crate_half_x,  crate_half_y, -crate_half_z }, math::vec3::up(), { 0.0f, 1.0f } },

            { { -crate_half_x, -crate_half_y, -crate_half_z }, math::vec3::down(), { 0.0f, 0.0f } },
            { {  crate_half_x, -crate_half_y, -crate_half_z }, math::vec3::down(), { 1.0f, 0.0f } },
            { {  crate_half_x, -crate_half_y,  crate_half_z }, math::vec3::down(), { 1.0f, 1.0f } },
            { { -crate_half_x, -crate_half_y,  crate_half_z }, math::vec3::down(), { 0.0f, 1.0f } },
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

    core::Mesh crate_mesh;
    crate_mesh.create(model_vertex_size);
    crate_mesh.update(crate_geometry);
    crate_mesh.attributes(model_vertex_attributes);

    #pragma endregion
    #pragma region Sprites

    auto sprite_geometry = tools::SpriteGenerator::create({ { }, { 200.0f, 200.0f } });

    core::Mesh sprite_mesh;
    sprite_mesh.create(sprite_vertex_size);
    sprite_mesh.update(sprite_geometry);
    sprite_mesh.attributes(sprite_vertex_attributes);

    #pragma endregion
    #pragma region Samplers

    opengl::Sampler default_sampler;
    default_sampler.create();
    default_sampler.bind();

    default_sampler.parameter(opengl::sampler::wrap_s, opengl::sampler::repeat);
    default_sampler.parameter(opengl::sampler::wrap_t, opengl::sampler::repeat);

    default_sampler.parameter(opengl::sampler::min_filter, opengl::sampler::linear);
    default_sampler.parameter(opengl::sampler::mag_filter, opengl::sampler::linear);

    #pragma endregion
    #pragma region Textures

    auto [crate_image_width, crate_image_height, crate_image_pixels] = images::Tga::load("crate.tga");

    opengl::Texture crate_texture { opengl::texture::texture_2d, opengl::texture::rgb8, crate_image_width, crate_image_height };

    crate_texture.create();
    crate_texture.storage();
    crate_texture.update(opengl::texture::rgb, crate_image_pixels.data());

    #pragma endregion

    core::Time time;
    time.init();

    core::window::size window_size
    {
        core::WindowManager::instance().width() / 2,
        core::WindowManager::instance().height()
    };

    core::data::camera camera;
    camera.projection.perspective(60.0f, window_size.ratio());

    math::vec3       camera_position  { 0.0f, 2.5, 7.5f };
    camera.view.look(camera_position, { });

    core::data::light light
    {
        .extra   { 0.0f, -1.0f, -1.0f },
        .ambient { 0.3f }
    };

    core::data::material material;

    math::mat4 plane_matrix;
    plane_matrix.identity();

    math::mat4 sphere_matrix;
    sphere_matrix.identity();
    sphere_matrix.translate({ 2.0f, 1.0f, -2.0f });

    math::mat4 capsule_matrix;
    capsule_matrix.identity();
    capsule_matrix.translate({ 2.0f, 1.0f, 2.0f });

    constexpr math::rgb plane_color   { 0.5f, 0.5f, 1.0f };
    constexpr math::rgb box_color     { 0.0f, 1.0f, 1.0f };
    constexpr math::rgb sphere_color  { 1.0f, 1.0f, 0.0f };
    constexpr math::rgb capsule_color { 1.0f, 0.5f, 0.5f };

    #pragma region Uniform Buffers

    opengl::Buffer camera_buffer;
    camera_buffer.create();
    camera_buffer.bind();
    camera_buffer.data(core::buffer::data::create(&camera), opengl::dynamic_draw);

    opengl::Buffer material_buffer;
    material_buffer.create();
    material_buffer.bind(core::buffer::location::material);
    material_buffer.data(core::buffer::data::create(&material), opengl::dynamic_draw);

    opengl::Buffer light_buffer;
    light_buffer.create();
    light_buffer.bind(core::buffer::location::light);
    light_buffer.data(core::buffer::data::create(&light));

    #pragma endregion
    #pragma region Renderers

    graphics::ModelRenderer default_base_renderer;
    default_base_renderer.attach(&default_shader);
    default_base_renderer.attach(&material_buffer);

    graphics::ModelRenderer default_model_renderer;
    default_model_renderer.attach(&model_shader);
    default_model_renderer.attach(&material_buffer);

    #pragma endregion

    core::WindowManager::instance().on_size([&window_size, &camera_buffer, &camera]
    {
       window_size =
       {
           core::WindowManager::instance().width() / 2,
           core::WindowManager::instance().height()
       };

        camera.projection.perspective(60.0f, window_size.ratio());
        camera_buffer.update(core::buffer::data::create(&camera.projection), offsetof(core::data::camera, projection));
    });

    while (core::WindowManager::instance().is_active())
    {
        time.update();

        constexpr float camera_speed  = 0.25f;
        constexpr float camera_radius = 7.50f;

        camera_position.x = math::sin(core::Time::total_time() * camera_speed) * -camera_radius;
        camera_position.z = math::cos(core::Time::total_time() * camera_speed) *  camera_radius;

        camera.view.look(camera_position, { });
        camera_buffer.update(core::buffer::data::create(&camera.view));

        math::quat box_orientation;
        box_orientation.rotate({ 0.0f, 1.0f, 0.0f }, core::Time::total_time() * 90.0f);

        math::mat4 box_matrix;
        box_matrix.identity();
        box_matrix.translate({ -2.0f, 1.0f, 2.0f });
        box_matrix *= box_orientation;

        math::quat crate_orientation;
        crate_orientation.rotate({ 1.0f, 0.0f, 0.0f }, core::Time::total_time() * 45.0f);

        math::mat4 crate_matrix;
        crate_matrix.identity();
        crate_matrix.translate({ -2.0f, 1.0f, -2.0f });
        crate_matrix *= crate_orientation;

        #pragma region Editor

        opengl::Commands::viewport(0, 0, window_size.width(), window_size.height());

        opengl::Commands::clear(1.0f, 0.5f, 0.0f);
        opengl::Commands::clear(opengl::color_buffer | opengl::depth_buffer);

        default_base_renderer.begin();

        default_base_renderer.draw(&plane_mesh,   plane_matrix,   plane_color);
        default_base_renderer.draw(&box_mesh,     box_matrix,     box_color);
        default_base_renderer.draw(&sphere_mesh,  sphere_matrix,  sphere_color);
        default_base_renderer.draw(&capsule_mesh, capsule_matrix, capsule_color);
        default_base_renderer.draw(&crate_mesh,   crate_matrix,   box_color);

        #pragma endregion
        #pragma region Engine

        //const auto [r, g, b] = math::rgb::black();

        opengl::Commands::viewport(window_size.width(), 0, window_size.width(), window_size.height());

        //opengl::Commands::clear(r, g, b);
        //opengl::Commands::clear(opengl::color_buffer | opengl::depth_buffer);

        default_model_renderer.begin();

        default_model_renderer.draw(&crate_mesh, crate_matrix, &crate_texture);

        #pragma endregion
        #pragma region UI

        math::mat4 sprite_model;
        sprite_model.identity();

        math::mat4 ui_view;
        ui_view.identity();

        math::mat4 ui_projection;
        ui_projection.orthographic(0, static_cast<float>(window_size.width()), static_cast<float>(window_size.height()), 0);

        default_ui_shader.bind();
        default_ui_shader.push(sprite_model);
        default_ui_shader.push(ui_view,       1);
        default_ui_shader.push(ui_projection, 2);

        crate_texture.bind();

        sprite_mesh.bind();

        opengl::Commands::draw_indexed(opengl::triangles, primitive::triangle::elements * sprite_mesh.elements());

        #pragma endregion

        core::WindowManager::instance().update();
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

    core::WindowManager::instance().destroy();

    return 0;
}