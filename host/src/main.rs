use std::env;
use std::fs::read_to_string;
use std::time::Duration;

use hyperlight_host::sandbox::SandboxConfiguration;
use hyperlight_host::sandbox_state::sandbox::EvolvableSandbox;
use hyperlight_host::sandbox_state::transition::Noop;
use hyperlight_host::GuestBinary;
use hyperlight_host::MultiUseSandbox;
use hyperlight_host::UninitializedSandbox;

// The memory limit for the sandbox's stack, heap, input and output.
// Note: quickjs defines its own memory limits inside the guest
const MEMORY_LIMIT: usize = 32 * 1024 * 1024;
const BUFFER_LIMIT: usize = 4 * 1024 * 1024;

fn main() {
    env_logger::init();
    let file = env::args().nth(1).expect("Usage: <binary> <file_path>");
    let code = read_to_string(&file).expect(format!("Failed to read file {:?}", file).as_str());

    let guest_binary = GuestBinary::FilePath("../quickjs-guest".to_string());
    let mut cfg = SandboxConfiguration::default();
    cfg.set_heap_size(MEMORY_LIMIT as u64);
    cfg.set_output_data_size(BUFFER_LIMIT);
    // cfg.set_input_data_size(BUFFER_LIMIT);
    cfg.set_max_execution_time(Duration::from_secs(100));

    let sandbox = UninitializedSandbox::new(guest_binary, Some(cfg), None, None).unwrap();

    let mut multiusesandbox: MultiUseSandbox = sandbox.evolve(Noop::default()).unwrap();

    let res = multiusesandbox
        .call_guest_function_by_name(
            "EvalScript", // this function is defined in the quickjs guest binary
            hyperlight_host::func::ReturnType::String,
            Some(vec![hyperlight_host::func::ParameterValue::String(code)]),
        )
        .unwrap();

    match res {
        hyperlight_host::func::ReturnValue::String(_s) => {
            println!("{}", _s);
        }
        _ => {
            eprintln!("Unexpected return value");
        }
    }
}
