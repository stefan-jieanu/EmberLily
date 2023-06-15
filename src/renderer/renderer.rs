use std::rc::Rc;

use super::RenderPipeline;

pub struct Renderer {
    device: Rc<wgpu::Device>,
    queue: Rc<wgpu::Queue>,
}

impl Renderer {
    pub fn new(device: Rc<wgpu::Device>, queue: Rc<wgpu::Queue>) -> Self {
        Self { device, queue }
    }

    pub fn submit(&mut self) {}

    pub fn flush(&mut self, output: wgpu::SurfaceTexture, pipeline: &RenderPipeline) {
        let view = output
            .texture
            .create_view(&wgpu::TextureViewDescriptor::default());
        let mut encoder = self
            .device
            .create_command_encoder(&wgpu::CommandEncoderDescriptor {
                label: Some("Render Encoder"),
            });

        let mut render_pass = encoder.begin_render_pass(&wgpu::RenderPassDescriptor {
            label: Some("Render pass"),
            color_attachments: &[Some(wgpu::RenderPassColorAttachment {
                view: &view,
                resolve_target: None,
                ops: wgpu::Operations {
                    load: wgpu::LoadOp::Clear(wgpu::Color {
                        r: 0.1,
                        g: 0.3,
                        b: 0.8,
                        a: 1.0,
                    }),
                    store: true,
                },
            })],
            depth_stencil_attachment: None,
        });

        render_pass.set_pipeline(pipeline.pipeline());
        render_pass.draw(0..3, 0..1);

        drop(render_pass);

        self.queue.submit(std::iter::once(encoder.finish()));
        output.present();
    }
}
