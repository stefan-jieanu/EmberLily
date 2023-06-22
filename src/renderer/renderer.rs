use std::{collections::VecDeque, rc::Rc};

use super::{RenderPipeline, Sprite};

pub struct Renderer {
    device: Rc<wgpu::Device>,
    queue: Rc<wgpu::Queue>,
}

impl Renderer {
    pub fn new(device: Rc<wgpu::Device>, queue: Rc<wgpu::Queue>) -> Self {
        Self { device, queue }
    }

    pub fn flush(&mut self, output: wgpu::SurfaceTexture, pipeline: &RenderPipeline) {
        let view = output
            .texture
            .create_view(&wgpu::TextureViewDescriptor::default());
        let mut encoder = self
            .device
            .create_command_encoder(&wgpu::CommandEncoderDescriptor {
                label: Some("Render Encoder"),
            });

        // for s in sprites.iter() {
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
        // render_pass.set_vertex_buffer(0, sprite.vertex_buffer().slice(..));
        // render_pass.set_index_buffer(sprite.index_buffer().slice(..), wgpu::IndexFormat::Uint16);
        render_pass.draw(0..Sprite::NUM_VERTICES, 0..1);
        // render_pass.draw_indexed(0..Sprite::NUM_INDICES, 0, 0..1);

        drop(render_pass);
        // }

        self.queue.submit(std::iter::once(encoder.finish()));
        output.present();
    }
}
