#include "sdl_renderer.hpp"

#include <SDL.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <constr/string_view.hpp>
#undef main
#include <SDL_syswm.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <core/renderer/vertex_layout.hpp>
#include <core/renderer/memory/allocator.hpp>

namespace {
	std::tuple<byte *, uint32> read_binary(const char *filename) {
		std::fstream f{filename, f.binary | f.in | f.ate};
		if (!f.is_open()) {
			return std::make_tuple(nullptr, 0);
		}

		uint32 size = f.tellg();
		byte *bytes = new byte[size];
		
		f.seekg(0);
		f.read(static_cast<char *>((void*)bytes), size);

		f.close();

		return std::make_tuple(bytes, size);
	}

	tt::uniform_type bgfx_cast(bgfx::UniformType::Enum e) {
		return static_cast<tt::uniform_type>(e);
	}

	tt::texture_format bgfx_cast(bgfx::TextureFormat::Enum e) {
		return static_cast<tt::texture_format>(e);
	}

	bgfx::UniformType::Enum bgfx_cast(tt::uniform_type e) {
		return static_cast<bgfx::UniformType::Enum>(e);
	}

	bgfx::TextureFormat::Enum bgfx_cast(tt::texture_format e) {
		return static_cast<bgfx::TextureFormat::Enum>(e);
	}

	bgfx::Attrib::Enum bgfx_attrib(uint16 e) {
		return static_cast<bgfx::Attrib::Enum>(e);
	}

	bgfx::AttribType::Enum bgfx_attrib_type(uint16 e) {
		return static_cast<bgfx::AttribType::Enum>(e);
	}

	bgfx::VertexLayout bgfx_cast(const tt::vertex_layout &layout) {
		bgfx::VertexLayout result{};

		result.begin();
		for (int8 i = 0; i < layout.size; ++i) {
			auto &detail = layout.details[i];
			result.add(bgfx_attrib(detail.attr), uint8(detail.quantity), bgfx_attrib_type(detail.type), !!detail.normalized, !!detail.as_int);
		}
		result.end();

		return result;
	}

	template<typename HandleType>
	HandleType handle_cast(entt::entity e) {
		return static_cast<HandleType>(static_cast<std::underlying_type_t<entt::entity>>(e));
	}

	template<typename HandleType>
	entt::entity handle_cast(HandleType e) {
		return static_cast<entt::entity>(static_cast<std::underlying_type_t<HandleType>>(e));
	}

	struct uniform {
		bgfx::UniformHandle handle;
		tt::uniform_type type;
	};

	struct texture {
		bgfx::TextureHandle handle;
		uint16 w;
		uint16 h;
		tt::texture_format format;
		uint64 flags;
	};

	struct vertex_buffer {
		vertex_buffer(bgfx::VertexBufferHandle h, tt::vertex_layout l, tt::vertex_buffer_flags f)
			: vb_handle{ h }
			, layout{ l }
			, flags{ f } {
		}
		vertex_buffer(bgfx::DynamicVertexBufferHandle h, tt::vertex_layout l, tt::vertex_buffer_flags f)
			: dvb_handle{ h }
			, layout{ l }
			, flags{ f } {
		}

		union {
			bgfx::VertexBufferHandle vb_handle{};
			bgfx::DynamicVertexBufferHandle dvb_handle;
		};
		tt::vertex_layout layout;
		tt::vertex_buffer_flags flags;
	};

	struct index_buffer {
		index_buffer(bgfx::IndexBufferHandle h, tt::index_buffer_flags f)
			: ib_handle{ h }
			, flags{ f } {
		}
		index_buffer(bgfx::DynamicIndexBufferHandle h, tt::index_buffer_flags f)
			: dib_handle{ h }
			, flags{ f } {
		}

		union {
			bgfx::IndexBufferHandle ib_handle{};
			bgfx::DynamicIndexBufferHandle dib_handle;
		};
		tt::index_buffer_flags flags;
	};
	
	struct framebuffer {
		bgfx::FrameBufferHandle handle;
	};

	struct shader {
		bgfx::ShaderHandle handle;
		byte *bytes;
		uint32 size;
		constr::string_view name;
	};

	struct shader_program {
		bgfx::ProgramHandle handle;
	};
}    // namespace

namespace tt {
	int sdl_renderer_manager::init() {
		const int WIDTH = 640;
		const int HEIGHT = 480;

		// Initialize SDL systems
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
			return EXIT_FAILURE;
		}
		else {
			//Create a window
			window = SDL_CreateWindow("Window Title",
				SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				WIDTH,
				HEIGHT,
				SDL_WINDOW_SHOWN);
			if (window == nullptr) {
				std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
				return EXIT_FAILURE;
			}
		}

		{
			bgfx::Init bgfx_init{};
			if (auto platform_result = get_platform_data(window, bgfx_init.platformData)) {
				return platform_result;
			}

			// TODO: This should be choosen through configuration.
			bgfx_init.type = bgfx::RendererType::Vulkan;

			// TODO: This should be choosen through configuration.
			bgfx_init.resolution.height = HEIGHT;
			bgfx_init.resolution.width = WIDTH;
			bgfx_init.resolution.reset = BGFX_RESET_VSYNC;

			// Render an empty frame
			bgfx::renderFrame();

			bgfx::init(bgfx_init);

			// Enable debug text.
			bgfx::setDebug(BGFX_DEBUG_TEXT /*| BGFX_DEBUG_STATS*/);

			// Set view rectangle for 0th view
			bgfx::setViewRect(0, 0, 0, uint16_t(WIDTH), uint16_t(HEIGHT));

			// Clear the view rect
			bgfx::setViewClear(0,
				BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
				0x333333FF,
				1.0f,
				0);

			// Set empty primitive on screen
			bgfx::touch(0);
		}

		gl::allocator().init();

		return 0;
	}

	void sdl_renderer_manager::shutdown() {
		gl::allocator().shutdown();

		bgfx::shutdown();
		
		// Free up window
		SDL_DestroyWindow(window);

		// Shutdown SDL
		SDL_Quit();
	}

	uint32 sdl_renderer_manager::height() const {
		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		return h;
	}

	uint32 sdl_renderer_manager::width() const {
		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		return w;
	}

	void sdl_renderer_manager::begin() {
	}

	void sdl_renderer_manager::end() {
	}

	void sdl_renderer_manager::submit(const shader_program_handle &program_handle, uint8 view, uint32 depth, uint8 flags) {
		auto &program = registry.get<::shader_program>(handle_cast(program_handle));
		bgfx::submit(view, program.handle, depth, flags);
	}

	void sdl_renderer_manager::present() {
		bgfx::frame();

		// Garbage Collection for the gl allocator
		gl::allocator().cleanup();
	}

	uniform_handle sdl_renderer_manager::create_uniform(uniform_type type, const char *name) {
		auto handle = bgfx::createUniform(name, bgfx_cast(type));

		auto entity = registry.create();
		registry.emplace<::uniform>(entity, handle, type);

		return handle_cast<uniform_handle>(entity);
	}

	texture_handle sdl_renderer_manager::create_texture_2d(uint16 w, uint16 h, texture_format format, uint64 flags, const char *name) {
		auto handle = bgfx::createTexture2D(w, h, false, 0, bgfx_cast(format), flags);
		if (name) {
			bgfx::setName(handle, "Ultralight Surface");
		}

		auto entity = registry.create();
		registry.emplace<::texture>(entity, handle, w, h, format, flags);

		return handle_cast<texture_handle>(entity);
	}

	texture_handle sdl_renderer_manager::create_texture_2d(uint16 w, uint16 h, texture_format format, uint16 num_layers, uint64 flags, const char *name = nullptr) {
		auto handle = bgfx::createTexture2D(w, h, true, num_layers, bgfx_cast(format), flags);
		if (name) {
			bgfx::setName(handle, name);
		}

		auto entity = registry.create();
		registry.emplace<::texture>(entity, handle, w, h, format, flags);

		return handle_cast<texture_handle>(entity);
	}

	framebuffer_handle sdl_renderer_manager::create_framebuffer(const texture_handle *textures, uint8 texture_count, bool destroy_textures) {
		static constexpr const uint8 MAX_TEXTURES = 20;
		bgfx::TextureHandle handles[MAX_TEXTURES];
		auto count = std::min(MAX_TEXTURES, texture_count);
		for (uint8 i = 0; i < count; ++i) {
			auto entity = handle_cast(textures[i]);
			handles[i] = registry.get<::texture>(entity).handle;
		}

		auto handle = bgfx::createFrameBuffer(texture_count, handles, true);

		auto entity = registry.create();
		registry.emplace<::framebuffer>(entity, handle);

		return handle_cast<framebuffer_handle>(entity);
	}

	vertex_buffer_handle sdl_renderer_manager::create_vertex_buffer(const void *mem, uint32 size, const vertex_layout &layout, vertex_buffer_flags flags) {
		auto converted_layout = bgfx_cast(layout);

		auto handle = bgfx::createVertexBuffer(bgfx::makeRef(mem, size), converted_layout, flags);

		auto entity = registry.create();
		registry.emplace<::vertex_buffer>(entity, handle, layout, flags);

		return handle_cast<vertex_buffer_handle>(entity);
	}

	dyn_vertex_buffer_handle sdl_renderer_manager::create_dyn_vertex_buffer(const void *mem, uint32 size, const vertex_layout &layout, vertex_buffer_flags flags) {
		auto converted_layout = bgfx_cast(layout);

		auto handle = bgfx::createDynamicVertexBuffer(bgfx::makeRef(mem, size * layout.stride), converted_layout, flags);

		auto entity = registry.create();
		registry.emplace<::vertex_buffer>(entity, handle, layout, flags);

		return handle_cast<dyn_vertex_buffer_handle>(entity);
	}

	index_buffer_handle sdl_renderer_manager::create_index_buffer(const void *mem, uint32 size, index_buffer_flags flags) {
		auto handle = bgfx::createIndexBuffer(bgfx::makeRef(mem, size), flags);

		auto entity = registry.create();
		registry.emplace<::index_buffer>(entity, handle, flags);

		return handle_cast<index_buffer_handle>(entity);
	}

	dyn_index_buffer_handle sdl_renderer_manager::create_dyn_index_buffer(const void *mem, uint32 size, index_buffer_flags flags) {
		auto stride = (flags & IB_INDEX_32) == IB_INDEX_32 ? 4 : 2;
		auto handle = bgfx::createDynamicIndexBuffer(bgfx::makeRef(mem, size * stride), flags);

		auto entity = registry.create();
		registry.emplace<::index_buffer>(entity, handle, flags);

		return handle_cast<dyn_index_buffer_handle>(entity);
	}

	shader_handle sdl_renderer_manager::create_shader(const char *name, shader_stage stage) {
		static const std::filesystem::path root{ std::filesystem::absolute(".") / "assets" /  "shaders"};
		
		auto root_path = root;
		switch (bgfx::getRendererType()) {
			case bgfx::RendererType::Noop:
			case bgfx::RendererType::Direct3D9: root_path /= "dx9"; break;
			case bgfx::RendererType::Direct3D11:
			case bgfx::RendererType::Direct3D12: root_path /= "dx11"; break;
			case bgfx::RendererType::Agc:
			case bgfx::RendererType::Gnm: root_path /= "pssl"; break;
			case bgfx::RendererType::Metal: root_path /= "metal"; break;
			case bgfx::RendererType::Nvn: root_path /= "nvn"; break;
			case bgfx::RendererType::OpenGL: root_path /= "glsl"; break;
			case bgfx::RendererType::OpenGLES: root_path /= "essl"; break;
			case bgfx::RendererType::Vulkan: root_path /= "spirv"; break;
			case bgfx::RendererType::WebGPU: root_path /= "spirv"; break;

			case bgfx::RendererType::Count: {
				entt::entity entity = entt::null;
				return handle_cast<shader_handle>(entity);
			}
		}

		auto shader = root_path / name;
		auto as_str = shader.string();
		auto [bytes, size] = read_binary(as_str.c_str());

		::shader wrapper{};

		wrapper.bytes = bytes;
		wrapper.size = size;
		wrapper.name = constr::string_view(name);
		wrapper.handle = bgfx::createShader(bgfx::makeRef(bytes, size));
		bgfx::setName(wrapper.handle, name);

		auto entity = registry.create();
		registry.emplace<::shader>(entity, wrapper);

		return handle_cast<shader_handle>(entity);
	}

	shader_program_handle sdl_renderer_manager::create_shader_program(shader_handle vertex_handle, shader_handle fragment_handle) {
		auto &vertex = registry.get<::shader>(handle_cast(vertex_handle));
		auto &fragment = registry.get<::shader>(handle_cast(fragment_handle));
		auto handle = bgfx::createProgram(vertex.handle, fragment.handle);

		auto entity = registry.create();
		registry.emplace<::shader_program>(entity, handle);

		return handle_cast<shader_program_handle>(entity);
	}

	shader_program_handle sdl_renderer_manager::create_shader_program(shader_handle compute_handle) {
		auto &compute = registry.emplace<::shader>(handle_cast(compute_handle));
		auto handle = bgfx::createProgram(compute.handle);

		auto entity = registry.create();
		registry.emplace<::shader_program>(entity, handle);

		return handle_cast<shader_program_handle>(entity);
	}

	void sdl_renderer_manager::update_texture_2d(
		texture_handle handle,
		uint16 layers,
		uint16 mip_layer,
		uint16 offset_x,
		uint16 offset_y,
		uint16 w,
		uint16 h,
		const void *mem,
		uint32 size,
		uint16 pitch) {
		auto &texture = registry.get<::texture>(handle_cast(handle));
		auto mem_ref = bgfx::makeRef(mem, size);
		bgfx::updateTexture2D(texture.handle, layers, mip_layer, offset_x, offset_y, w, h, mem_ref, pitch);
	}

	void sdl_renderer_manager::set_vertex_buffer(dyn_vertex_buffer_handle handle, uint8 stream, int32 start, int32 count) {
		auto &vert_buffer = registry.get<::vertex_buffer>(handle_cast(handle));
		bgfx::setVertexBuffer(stream, vert_buffer.dvb_handle, start, count);
	}

	void sdl_renderer_manager::set_vertex_buffer(vertex_buffer_handle handle, uint8 stream) {
		auto &vert_buffer = registry.get<::vertex_buffer>(handle_cast(handle));
		bgfx::setVertexBuffer(stream, vert_buffer.vb_handle);
	}
	
	void sdl_renderer_manager::set_index_buffer(dyn_index_buffer_handle handle, int32 start, int32 count) {
		auto &buffer = registry.get<::index_buffer>(handle_cast(handle));
		bgfx::setIndexBuffer(buffer.dib_handle, start, count);
	}

	void sdl_renderer_manager::set_index_buffer(index_buffer_handle handle) {
		auto &buffer = registry.get<::index_buffer>(handle_cast(handle));
		bgfx::setIndexBuffer(buffer.ib_handle);
	}

	void sdl_renderer_manager::set_texture(uniform_handle uniform_handle, texture_handle texture_handle, uint16 stage) {
		auto &uniform = registry.get<::uniform>(handle_cast(uniform_handle));
		auto &texture = registry.get<::texture>(handle_cast(texture_handle));

		bgfx::setTexture(stage, uniform.handle, texture.handle);
	}

	void sdl_renderer_manager::set_state(uint64 state, uint32 rgba) {
		bgfx::setState(state, rgba);
	}

	void sdl_renderer_manager::set_view_rect(uint8 view, uint16 x, uint16 y, uint16 width, uint16 height) {
		bgfx::setViewRect(view, x, y, width, height);
	}

	void sdl_renderer_manager::set_view_transform(uint8 view_id, const float *view, const float *proj) {
		bgfx::setViewTransform(view_id, view, proj);
	}

	void sdl_renderer_manager::touch(uint8 view) {
		bgfx::touch(view);
	}

	void sdl_renderer_manager::destroy(framebuffer_handle handle) {
		auto entity = handle_cast(handle);
		auto &data = registry.get<::framebuffer>(entity);
		bgfx::destroy(data.handle);

		registry.destroy(entity);
	}
	
	void sdl_renderer_manager::destroy(texture_handle handle) {
		auto entity = handle_cast(handle);
		auto &data = registry.get<::texture>(entity);
		bgfx::destroy(data.handle);

		registry.destroy(entity);
	}

	void sdl_renderer_manager::destroy(uniform_handle handle) {
		auto entity = handle_cast(handle);
		auto &data = registry.get<::uniform>(entity);
		bgfx::destroy(data.handle);

		registry.destroy(entity);
	}

	void sdl_renderer_manager::destroy(vertex_buffer_handle handle) {
		auto entity = handle_cast(handle);
		auto &data = registry.get<::vertex_buffer>(entity);
		bgfx::destroy(data.vb_handle);

		registry.destroy(entity);
	}
	
	void sdl_renderer_manager::destroy(dyn_vertex_buffer_handle handle) {
		auto entity = handle_cast(handle);
		auto &data = registry.get<::vertex_buffer>(entity);
		bgfx::destroy(data.dvb_handle);

		registry.destroy(entity);
	}

	void sdl_renderer_manager::destroy(index_buffer_handle handle) {
		auto entity = handle_cast(handle);
		auto &data = registry.get<::index_buffer>(entity);
		bgfx::destroy(data.ib_handle);

		registry.destroy(entity);
	}
	
	void sdl_renderer_manager::destroy(dyn_index_buffer_handle handle) {
		auto entity = handle_cast(handle);
		auto &data = registry.get<::index_buffer>(entity);
		bgfx::destroy(data.dib_handle);

		registry.destroy(entity);
	}

	void sdl_renderer_manager::destroy(shader_handle handle) {
		auto entity = handle_cast(handle);
		auto &data = registry.get<::shader>(entity);
		bgfx::destroy(data.handle);
		delete[] data.bytes;

		registry.destroy(entity);
	}

	void sdl_renderer_manager::destroy(shader_program_handle handle) {
		auto entity = handle_cast(handle);
		auto &data = registry.get<::shader_program>(entity);
		bgfx::destroy(data.handle);

		registry.destroy(entity);
	}

	void sdl_renderer_manager::update(dyn_index_buffer_handle handle, byte *mem, int16 mem_size, int32 start) {
		auto &buffer = registry.get<::index_buffer>(handle_cast(handle));
		auto stride = (buffer.flags & IB_INDEX_32) == IB_INDEX_32 ? 4 : 2;
		bgfx::update(buffer.dib_handle, start, bgfx::makeRef(mem, mem_size * stride));
	}

	void sdl_renderer_manager::update(dyn_vertex_buffer_handle handle, byte *mem, int16 mem_size, int32 start) {
		auto &buffer = registry.get<::vertex_buffer>(handle_cast(handle));
		bgfx::update(buffer.dvb_handle, start, bgfx::makeRef(mem, mem_size * buffer.layout.stride));
	}

	renderer_manager &get_renderer() {
		static sdl_renderer_manager renderer{};
		return renderer;
	}
}    // namespace tt
