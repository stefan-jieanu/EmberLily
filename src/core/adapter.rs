use super::surface::Surface;
use std::rc::Rc;

pub struct Adapter {
    // Wgpu adapter instance
    adapter: wgpu::Adapter,
    device: Rc<wgpu::Device>,
    queue: Rc<wgpu::Queue>,
}

impl Adapter {
    pub async fn new(wgpu_instance: &wgpu::Instance, surface: &Surface) -> Self {
        let adapter = wgpu_instance
            .request_adapter(&wgpu::RequestAdapterOptionsBase {
                power_preference: wgpu::PowerPreference::default(),
                compatible_surface: Some(surface.surface()),
                force_fallback_adapter: false,
            })
            .await
            .unwrap();

        let (device, queue) = adapter
            .request_device(
                &wgpu::DeviceDescriptor {
                    features: wgpu::Features::empty(),
                    limits: wgpu::Limits::default(),
                    label: None,
                },
                None,
            )
            .await
            .unwrap();

        Self {
            adapter,
            device: Rc::new(device),
            queue: Rc::new(queue),
        }
    }

    pub fn device(&self) -> Rc<wgpu::Device> {
        Rc::clone(&self.device)
    }

    pub fn queue(&self) -> Rc<wgpu::Queue> {
        Rc::clone(&self.queue)
    }

    pub fn adapter(&self) -> &wgpu::Adapter {
        &self.adapter
    }
}
