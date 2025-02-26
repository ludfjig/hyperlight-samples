use std::env;
use std::io;
use std::io::Read;
use std::path::PathBuf;
use std::time::Duration;

use hyperlight_host::func::ParameterValue;
use hyperlight_host::func::ReturnType;
use hyperlight_host::func::ReturnValue;
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
    // Read javascript to evalute from stdin
    let mut js_script = String::new();
    io::stdin()
        .read_to_string(&mut js_script)
        .expect("Javascript must be valid UTF-8");

    // we assume the guest binary exists at one level above this "host" directory and is called "quickjs-guest"
    let manifest_dir =
        env::var("CARGO_MANIFEST_DIR").expect("CARGO_MANIFEST_DIR should be set by cargo");
    let guest_path = PathBuf::from(manifest_dir)
        .join("../quickjs-guest")
        .as_os_str()
        .to_string_lossy()
        .to_string();
    let guest_binary = GuestBinary::FilePath(guest_path);

    // configure some sandbox settings
    let mut cfg = SandboxConfiguration::default();
    cfg.set_heap_size(MEMORY_LIMIT as u64);
    cfg.set_output_data_size(BUFFER_LIMIT);
    cfg.set_max_execution_time(Duration::from_secs(10));

    // create the sandbox, then call into it, passing in the javascript to evaluate
    let sandbox = UninitializedSandbox::new(guest_binary, Some(cfg), None, None).unwrap();
    let mut multiusesandbox: MultiUseSandbox = sandbox.evolve(Noop::default()).unwrap();
    let res = multiusesandbox
        .call_guest_function_by_name(
            "EvalScript", // this function is registered in the guest binary
            ReturnType::String,
            Some(vec![ParameterValue::String(js_script)]),
        )
        .unwrap();

    match res {
        ReturnValue::String(s) => {
            println!("{}", s);
        }
        _ => {
            eprintln!("Unexpected return value");
            std::process::exit(1);
        }
    }
}
