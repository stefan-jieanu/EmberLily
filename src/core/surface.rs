use super::adapter::Adapter;

pub struct Surface {
    // Wgpu surface instance
    surface: wgpu::Surface,
}

impl Surface {
    pub fn new(wgpu_instance: &wgpu::Instance, window: &winit::window::Window) -> Self {
        let surface = unsafe { wgpu_instance.create_surface(&window) }.unwrap();

        Self { surface }
    }

    pub fn configure(&self, adapter: &Adapter, (width, height): (u32, u32)) {
        let surface_capabilities = self.surface.get_capabilities(adapter.adapter());

        // Shader code will assume sRGB
        let surface_format = surface_capabilities
            .formats
            .iter()
            .copied()
            .filter(|f| f.describe().srgb)
            .next()
            .unwrap_or(surface_capabilities.formats[0]);

        let surface_config = wgpu::SurfaceConfiguration {
            usage: wgpu::TextureUsages::RENDER_ATTACHMENT,
            format: surface_format,
            width: width,
            height: height,
            present_mode: surface_capabilities.present_modes[0],
            alpha_mode: surface_capabilities.alpha_modes[0],
            view_formats: vec![],
        };

        self.surface.configure(adapter.device(), &surface_config);
    }

    pub fn surface(&self) -> &wgpu::Surface {
        &self.surface
    }

    pub fn output_texture(&self) -> Result<wgpu::SurfaceTexture, wgpu::SurfaceError> {
        return self.surface.get_current_texture();
    }
}
