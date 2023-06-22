use winit::event_loop::EventLoop;

// Default constants
const WINDOW_TITLE: &'static str = "Ember Lily";
const WINDOW_WIDTH: u32 = 800;
const WINDOW_HEIGHT: u32 = 600;

pub struct Window {
    title: String,
    size: (u32, u32),
    window: winit::window::Window,
}

impl Window {
    pub fn new(event_loop: &EventLoop<()>) -> Self {
        let window = winit::window::WindowBuilder::new()
            .with_title(WINDOW_TITLE)
            .with_inner_size(winit::dpi::LogicalSize::new(WINDOW_WIDTH, WINDOW_HEIGHT))
            .build(event_loop)
            .expect("Failed to create window.");

        Window {
            title: String::from(WINDOW_TITLE),
            size: (WINDOW_WIDTH, WINDOW_HEIGHT),
            window,
        }
    }

    pub fn window(&self) -> &winit::window::Window {
        &self.window
    }
}
