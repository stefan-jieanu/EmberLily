use crate::core::{adapter::Adapter, surface::Surface};
use crate::renderer::{self, Renderer};

pub struct Application {
    window: winit::window::Window,
    surface: Surface,
    adapter: Adapter,
    // renderer: Renderer,
}

impl Application {
    pub async fn new(window: winit::window::Window) -> Self {
        log::trace!("Started application.");

        let wgpu_instance = wgpu::Instance::new(wgpu::InstanceDescriptor {
            backends: wgpu::Backends::DX12,
            dx12_shader_compiler: Default::default(),
        });

        // # Safety
        // The surface needs to live as long as the window that created it.
        // let surface = unsafe { wgpu_instance.create_surface(&window) }.unwrap();
        let surface = Surface::new(&wgpu_instance, &window);
        let adapter = Adapter::new(&wgpu_instance, &surface).await;

        surface.configure(
            &adapter,
            (window.inner_size().width, window.inner_size().height),
        );

        // let renderer = Renderer::new(adapter.device());

        Self {
            window,
            surface,
            adapter,
            // renderer,
        }
    }

    pub fn resize(&mut self, new_size: winit::dpi::PhysicalSize<u32>) {
        if new_size.width > 0 && new_size.height > 0 {
            self.surface.configure(
                &self.adapter,
                (
                    self.window.inner_size().width,
                    self.window.inner_size().height,
                ),
            );
        }
    }

    pub fn input(&mut self, event: &winit::event::WindowEvent) -> bool {
        return false;
    }

    pub fn update(&mut self) {}

    pub fn render(&mut self) -> Result<(), wgpu::SurfaceError> {
        Ok(())
    }

    pub fn window(&self) -> &winit::window::Window {
        &self.window
    }
}
