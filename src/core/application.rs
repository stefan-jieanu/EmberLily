pub struct Application {}

impl Application {
    pub fn new() -> Self {
        log::trace!("Started application.");
        Self {}
    }

    pub fn input(&mut self, event: &winit::event::WindowEvent) -> bool {
        return false;
    }
}
