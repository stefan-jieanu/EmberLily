use super::adapter::Adapter;

pub struct Surface {
    // Wgpu surface instance
    surface: wgpu::Surface,
    config: Option<wgpu::SurfaceConfiguration>,
}

impl Surface {
    pub fn new(wgpu_instance: &wgpu::Instance, window: &winit::window::Window) -> Self {
        let surface = unsafe { wgpu_instance.create_surface(&window) }.unwrap();
        let config = None;

        Self { surface, config }
    }

    pub fn configure(&mut self, adapter: &Adapter, (width, height): (u32, u32)) {
        let surface_capabilities = self.surface.get_capabilities(adapter.adapter());

        // Shader code will assume sRGB
        let surface_format = surface_capabilities
            .formats
            .iter()
            .copied()
            .filter(|f| f.describe().srgb)
            .next()
            .unwrap_or(surface_capabilities.formats[0]);

        self.config = Some(wgpu::SurfaceConfiguration {
            usage: wgpu::TextureUsages::RENDER_ATTACHMENT,
            format: surface_format,
            width: width,
            height: height,
            present_mode: surface_capabilities.present_modes[0],
            alpha_mode: surface_capabilities.alpha_modes[0],
            view_formats: vec![],
        });

        match &self.config {
            Some(config) => self.surface.configure(&adapter.device(), &config),
            None => {
                panic!("No surface configuration")
            }
        }
    }

    pub fn surface(&self) -> &wgpu::Surface {
        &self.surface
    }

    pub fn config(&self) -> &wgpu::SurfaceConfiguration {
        match &self.config {
            Some(config) => &config,
            None => {
                panic!("No surface configuration")
            }
        }
    }

    pub fn output_texture(&self) -> Result<wgpu::SurfaceTexture, wgpu::SurfaceError> {
        return self.surface.get_current_texture();
    }
}
