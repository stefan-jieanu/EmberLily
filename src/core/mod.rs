pub mod window;

use window::Window;
use winit::{
    event::{ElementState, Event, KeyboardInput, VirtualKeyCode, WindowEvent},
    event_loop::{ControlFlow, EventLoop},
};

use crate::vulkan::{adapter::Adapter, surface::Surface, VkInstance};

pub struct EmberLily {
    window: Window,
    vk_instance: VkInstance,
    device: Adapter,
    surface: Surface,
}

impl EmberLily {
    pub fn new(event_loop: &EventLoop<()>) -> Self {
        let window = Window::new(event_loop);
        let vk_instance = VkInstance::new();
        let surface = Surface::new(&vk_instance, &window);
        let device = Adapter::new(&vk_instance, &surface);

        Self {
            window,
            vk_instance,
            device,
            surface,
        }
    }

    pub fn main_loop(self, event_loop: EventLoop<()>) {
        event_loop.run(move |event, _, control_flow| match event {
            Event::WindowEvent { event, .. } => match event {
                WindowEvent::CloseRequested => *control_flow = ControlFlow::Exit,
                WindowEvent::KeyboardInput { input, .. } => match input {
                    KeyboardInput {
                        virtual_keycode,
                        state,
                        ..
                    } => match (virtual_keycode, state) {
                        (Some(VirtualKeyCode::Escape), ElementState::Pressed) => {
                            dbg!();
                            *control_flow = ControlFlow::Exit
                        }
                        _ => {}
                    },
                },
                _ => {}
            },
            _ => (),
        });
    }
}
