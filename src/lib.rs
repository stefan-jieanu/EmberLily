mod core;
use crate::core::*;
use winit::event::{ElementState, Event, KeyboardInput, VirtualKeyCode, WindowEvent};
use winit::event_loop::ControlFlow;

pub async fn run() {
    env_logger::Builder::from_env(env_logger::Env::default().default_filter_or("trace")).init();

    let event_loop: winit::event_loop::EventLoop<()> = winit::event_loop::EventLoop::new();
    let window_builder: winit::window::WindowBuilder = Default::default();
    let window = window_builder
        .with_inner_size(winit::dpi::LogicalSize {
            width: 800,
            height: 600,
        })
        .with_title("EmberLily")
        .build(&event_loop)
        .expect("Could not create window!");

    let application: Application = Application::new();

    event_loop.run(move |event, _, control_flow| match event {
        Event::WindowEvent {
            ref event,
            window_id,
        } if window_id == window.id() => match event {
            WindowEvent::CloseRequested
            | WindowEvent::KeyboardInput {
                input:
                    KeyboardInput {
                        state: ElementState::Pressed,
                        virtual_keycode: Some(VirtualKeyCode::Escape),
                        ..
                    },
                ..
            } => *control_flow = ControlFlow::Exit,
            _ => {}
        },
        _ => {}
    });
}
