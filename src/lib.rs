use std::rc::Rc;

use winit::{
    event::{ElementState, Event, KeyboardInput, VirtualKeyCode, WindowEvent},
    event_loop::{ControlFlow, EventLoop},
    window::Window,
};

mod core;
mod renderer;

use renderer::Renderer;

pub struct Application {
    window: Window,
    size: winit::dpi::PhysicalSize<u32>,
    surface: wgpu::Surface,
    surface_config: wgpu::SurfaceConfiguration,
    device: Rc<wgpu::Device>,
    queue: Rc<wgpu::Queue>,
    renderer: Renderer,
}

impl Application {
    const WINDOW_TITLE: &str = "EmberLily";
    const WINDOW_WIDTH: u32 = 800;
    const WINDOW_HEIGHT: u32 = 600;

    async fn new(event_loop: &EventLoop<()>) -> Self {
        let window = Application::create_window(event_loop);
        let size = window.inner_size();

        // Init logger for wgpu
        let log_level = "error";
        env_logger::Builder::from_env(env_logger::Env::default().default_filter_or(log_level))
            .init();

        // Instance handle to the GPU
        let instance = wgpu::Instance::new(wgpu::InstanceDescriptor {
            backends: wgpu::Backends::DX12,
            dx12_shader_compiler: Default::default(),
        });

        // # Safety
        // The surface needs to live as long as the window that created it
        let surface = unsafe { instance.create_surface(&window) }.unwrap();

        let adapter = instance
            .request_adapter(&wgpu::RequestAdapterOptions {
                power_preference: wgpu::PowerPreference::default(),
                compatible_surface: Some(&surface),
                force_fallback_adapter: false,
            })
            .await
            .unwrap();

        let (device, queue) = adapter
            .request_device(
                &wgpu::DeviceDescriptor {
                    label: None,
                    features: wgpu::Features::empty(),
                    limits: wgpu::Limits::default(),
                },
                None, // Trace path
            )
            .await
            .unwrap();

        let surface_caps = surface.get_capabilities(&adapter);
        // Assume srgb surfaces in shaders
        let surface_format = surface_caps
            .formats
            .iter()
            .copied()
            .find(|f| f.is_srgb())
            .unwrap_or(surface_caps.formats[0]);
        let surface_config = wgpu::SurfaceConfiguration {
            usage: wgpu::TextureUsages::RENDER_ATTACHMENT,
            format: surface_format,
            width: size.width,
            height: size.height,
            present_mode: surface_caps.present_modes[0],
            alpha_mode: surface_caps.alpha_modes[0],
            view_formats: vec![],
        };
        surface.configure(&device, &surface_config);

        let vs_shader =
            device.create_shader_module(wgpu::include_spirv!("shaders/bare_bones.vert.spv"));
        let fs_shader =
            device.create_shader_module(wgpu::include_spirv!("shaders/bare_bones.frag.spv"));

        let device_ptr = Rc::new(device);
        let queue_ptr = Rc::new(queue);

        let mut renderer = Renderer::new(Rc::clone(&device_ptr), Rc::clone(&queue_ptr));
        renderer.create_pipeline_layout("simple_layout");
        renderer.create_pipeline("simple_pipeline", "simple_layout", &vs_shader, &fs_shader);

        Self {
            window,
            size,
            surface,
            surface_config,
            device: device_ptr,
            queue: queue_ptr,
            renderer,
        }
    }

    fn create_window(event_loop: &winit::event_loop::EventLoop<()>) -> Window {
        let size: winit::dpi::PhysicalSize<u32> = (Self::WINDOW_WIDTH, Self::WINDOW_HEIGHT).into();
        let window = winit::window::WindowBuilder::new()
            .with_inner_size(size)
            .with_title(Self::WINDOW_TITLE)
            .build(&event_loop)
            .expect("Could not build winow");
        window
    }

    fn resize(&mut self, new_size: winit::dpi::PhysicalSize<u32>) {
        if new_size.width > 0 && new_size.height > 0 {
            self.size = new_size;
            self.surface_config.width = new_size.width;
            self.surface_config.height = new_size.height;
            self.surface.configure(&self.device, &self.surface_config);
        }
    }

    fn input(&mut self, event: &WindowEvent) -> bool {
        false
    }

    fn update(&mut self) {}

    fn render(&mut self) -> Result<(), wgpu::SurfaceError> {
        self.renderer.render(&self.surface, "simple_pipeline");
        Ok(())
    }

    pub async fn run(event_loop: winit::event_loop::EventLoop<()>) {
        let mut app = Application::new(&event_loop).await;
        // Run event loop
        event_loop.run(move |event, _, control_flow| match event {
            Event::WindowEvent { event, .. } => {
                if !app.input(&event) {
                    match event {
                        WindowEvent::CloseRequested => *control_flow = ControlFlow::Exit,
                        WindowEvent::KeyboardInput { input, .. } => match input {
                            KeyboardInput {
                                virtual_keycode,
                                state,
                                ..
                            } => match (virtual_keycode, state) {
                                (Some(VirtualKeyCode::Escape), ElementState::Pressed) => {
                                    *control_flow = ControlFlow::Exit
                                }
                                _ => {}
                            },
                        },
                        _ => {}
                    }
                }
            }

            Event::MainEventsCleared => {
                app.window.request_redraw();
            }

            Event::RedrawRequested(_window_id) => {
                app.update();
                match app.render() {
                    Result::Ok(_) => {}
                    // Reconfigure the surface if it's lost or outdated
                    Err(wgpu::SurfaceError::Lost | wgpu::SurfaceError::Outdated) => {
                        app.resize(app.size)
                    }
                    // The system is out of memory, we should probably quit
                    Err(wgpu::SurfaceError::OutOfMemory) => *control_flow = ControlFlow::Exit,
                    // We're ignoring timeouts
                    Err(wgpu::SurfaceError::Timeout) => log::warn!("Surface timeout"),
                }
            }
            _ => (),
        })
    }

    pub fn create_event_loop() -> winit::event_loop::EventLoop<()> {
        winit::event_loop::EventLoop::new()
    }
}
