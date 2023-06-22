use ember_lily::EmberLily;
use winit::event_loop::EventLoop;

fn main() {
    let event_loop = EventLoop::new();
    let app = EmberLily::new(&event_loop);
    app.main_loop(event_loop);
}
