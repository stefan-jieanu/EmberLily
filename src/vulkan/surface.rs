use crate::core::window::Window;

use super::{utility, VkInstance};

pub struct Surface {
    loader: ash::extensions::khr::Surface,
    surface: ash::vk::SurfaceKHR,
}

impl Surface {
    pub fn new(instance: &VkInstance, window: &Window) -> Self {
        let surface = unsafe {
            utility::platforms::create_surface(
                instance.entry(),
                instance.instance(),
                window.window(),
            )
            .expect("Failed to create surface.")
        };

        let loader = ash::extensions::khr::Surface::new(instance.entry(), instance.instance());

        Self { surface, loader }
    }

    pub fn loader(&self) -> &ash::extensions::khr::Surface {
        &self.loader
    }

    pub fn surface(&self) -> &ash::vk::SurfaceKHR {
        &self.surface
    }
}

// impl Drop for Surface {
//     fn drop(&mut self) {
//         unsafe { self.loader.destroy_surface(self.surface, None) }
//     }
// }
