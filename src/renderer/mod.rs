use std::rc::Rc;

use std::collections::HashMap;

pub struct Renderer {
    device: Rc<wgpu::Device>,
    queue: Rc<wgpu::Queue>,
    render_pipelines: HashMap<String, wgpu::RenderPipeline>,
    pipeline_layouts: HashMap<String, wgpu::PipelineLayout>,
}

impl Renderer {
    pub fn new(device: Rc<wgpu::Device>, queue: Rc<wgpu::Queue>) -> Self {
        let render_pipelines: HashMap<String, wgpu::RenderPipeline> = HashMap::new();
        let pipeline_layouts: HashMap<String, wgpu::PipelineLayout> = HashMap::new();

        Self {
            device,
            queue,
            render_pipelines,
            pipeline_layouts,
        }
    }

    pub fn create_pipeline_layout(&mut self, name: &str) {
        let layout = self
            .device
            .create_pipeline_layout(&wgpu::PipelineLayoutDescriptor {
                label: Some("Pipeline layout"),
                bind_group_layouts: &[],
                push_constant_ranges: &[],
            });

        self.pipeline_layouts.insert(String::from(name), layout);
    }

    pub fn create_pipeline(
        &mut self,
        name: &str,
        layout_name: &str,
        vs_shader: &wgpu::ShaderModule,
        fs_shader: &wgpu::ShaderModule,
    ) {
        let layout = self
            .pipeline_layouts
            .get(layout_name)
            .expect("Invalid pipeline layout name");

        let pipeline = self
            .device
            .create_render_pipeline(&wgpu::RenderPipelineDescriptor {
                label: Some("Render Pipeline"),
                layout: Some(layout),
                vertex: wgpu::VertexState {
                    module: &vs_shader,
                    entry_point: "main",
                    // buffers: &[model::ModelVertex::desc()],
                    buffers: &[],
                },
                fragment: Some(wgpu::FragmentState {
                    module: &fs_shader,
                    entry_point: "main",
                    targets: &[Some(wgpu::ColorTargetState {
                        // Get the surface format in the future
                        format: wgpu::TextureFormat::Bgra8UnormSrgb,
                        blend: Some(wgpu::BlendState {
                            color: wgpu::BlendComponent::REPLACE,
                            alpha: wgpu::BlendComponent::REPLACE,
                        }),
                        write_mask: wgpu::ColorWrites::ALL,
                    })],
                }),
                primitive: wgpu::PrimitiveState {
                    topology: wgpu::PrimitiveTopology::TriangleList,
                    strip_index_format: None,
                    front_face: wgpu::FrontFace::Ccw,
                    cull_mode: Some(wgpu::Face::Back),
                    // Setting this to anything other than Fill requires Features::POLYGON_MODE_LINE
                    // or Features::POLYGON_MODE_POINT
                    polygon_mode: wgpu::PolygonMode::Fill,
                    // Requires Features::DEPTH_CLIP_CONTROL
                    unclipped_depth: false,
                    // Requires Features::CONSERVATIVE_RASTERIZATION
                    conservative: false,
                },
                // depth_stencil: Some(wgpu::DepthStencilState {
                //     format: texture::Texture::DEPTH_FORMAT,
                //     depth_write_enabled: true,
                //     depth_compare: wgpu::CompareFunction::Less,
                //     stencil: wgpu::StencilState::default(),
                //     bias: wgpu::DepthBiasState::default(),
                // }),
                depth_stencil: None,
                multisample: wgpu::MultisampleState {
                    count: 1,
                    mask: !0,
                    alpha_to_coverage_enabled: false,
                },
                // If the pipeline will be used with a multiview render pass, this
                // indicates how many array layers the attachments will have.
                multiview: None,
            });

        self.render_pipelines.insert(String::from(name), pipeline);
    }

    pub fn render(
        &mut self,
        surface: &wgpu::Surface,
        pipeline_name: &str,
    ) -> Result<(), wgpu::SurfaceError> {
        let output = surface.get_current_texture()?;
        let view = output
            .texture
            .create_view(&wgpu::TextureViewDescriptor::default());

        let mut encoder = self
            .device
            .create_command_encoder(&wgpu::CommandEncoderDescriptor {
                label: Some("Render Encoder"),
            });

        let pipeline = self
            .render_pipelines
            .get(pipeline_name)
            .expect("Invalid pipeline name");

        let mut render_pass = encoder.begin_render_pass(&wgpu::RenderPassDescriptor {
            label: Some("Render Pass"),
            color_attachments: &[Some(wgpu::RenderPassColorAttachment {
                view: &view,
                resolve_target: None,
                ops: wgpu::Operations {
                    load: wgpu::LoadOp::Clear(wgpu::Color {
                        r: 0.1,
                        g: 0.2,
                        b: 0.6,
                        a: 1.0,
                    }),
                    store: true,
                },
            })],
            depth_stencil_attachment: None,
        });

        render_pass.set_pipeline(pipeline);
        render_pass.draw(0..3, 0..1);

        drop(render_pass);

        self.queue.submit(std::iter::once(encoder.finish()));
        output.present();

        Ok(())
    }
}
