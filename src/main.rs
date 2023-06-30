use ember_lily::Application;

fn main() {
    let event_loop = Application::create_event_loop();

    // Running the app requires passing the same event loop required for
    // creating the window. This is a bit "eh" but needed because
    // running the event loop reuqires ownership of it so the event loop
    // can't really be owned by the Application.
    // TODO: Maybe find a more elegant solution for this in the future
    pollster::block_on(Application::run(event_loop));
}
