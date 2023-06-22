pub mod adapter;
pub mod surface;
mod utility;

use ash::vk;
use std::ffi::CString;
use std::os::raw::c_void;
use std::ptr;

const VALIDATION: utility::ValidationInfo = utility::ValidationInfo {
    is_enable: true,
    required_validation_layers: ["VK_LAYER_KHRONOS_validation"],
};

pub struct VkInstance {
    entry: ash::Entry,
    instance: ash::Instance,
    debug_utils_loader: ash::extensions::ext::DebugUtils,
    debug_messenger: vk::DebugUtilsMessengerEXT,
}

impl VkInstance {
    pub fn new() -> Self {
        // init vulkan stuff
        let entry = unsafe { ash::Entry::load().unwrap() };
        let instance = Self::create_instance(&entry);
        let (debug_utils_loader, debug_messenger) =
            utility::setup_debug_utils(VALIDATION.is_enable, &entry, &instance);

        VkInstance {
            entry,
            instance,
            debug_utils_loader,
            debug_messenger,
        }
    }

    pub fn instance(&self) -> &ash::Instance {
        &self.instance
    }

    pub fn entry(&self) -> &ash::Entry {
        &self.entry
    }

    fn create_instance(entry: &ash::Entry) -> ash::Instance {
        // Leave this as default, change it to make the app seem more official
        let app_info = vk::ApplicationInfo::default();

        // This create info is used to debug issues in vk::createInstance and vk::destroyInstance
        let debug_utils_create_info = utility::populate_debug_messenger_create_info();

        let extension_names = utility::required_extension_names();

        // Map Rust str to CString
        let required_validation_layer_raw_names: Vec<CString> = VALIDATION
            .required_validation_layers
            .iter()
            .map(|layer_name| CString::new(*layer_name).unwrap())
            .collect();

        // Map CString to *
        let enable_layer_names: Vec<*const i8> = required_validation_layer_raw_names
            .iter()
            .map(|layer_name| layer_name.as_ptr())
            .collect();

        let create_info = vk::InstanceCreateInfo {
            s_type: vk::StructureType::INSTANCE_CREATE_INFO,
            p_next: if VALIDATION.is_enable {
                &debug_utils_create_info as *const vk::DebugUtilsMessengerCreateInfoEXT
                    as *const c_void
            } else {
                ptr::null()
            },
            flags: vk::InstanceCreateFlags::empty(),
            p_application_info: &app_info,
            pp_enabled_layer_names: if VALIDATION.is_enable {
                enable_layer_names.as_ptr()
            } else {
                ptr::null()
            },
            enabled_layer_count: if VALIDATION.is_enable {
                enable_layer_names.len()
            } else {
                0
            } as u32,
            pp_enabled_extension_names: extension_names.as_ptr(),
            enabled_extension_count: extension_names.len() as u32,
        };

        let instance: ash::Instance = unsafe {
            entry
                .create_instance(&create_info, None)
                .expect("Failed to create vulkan instance")
        };

        instance
    }
}

impl Drop for VkInstance {
    fn drop(&mut self) {
        println!("I am debuggin");
        unsafe {
            if VALIDATION.is_enable {
                self.debug_utils_loader
                    .destroy_debug_utils_messenger(self.debug_messenger, None);
            }
            self.instance.destroy_instance(None);
        }
    }
}
