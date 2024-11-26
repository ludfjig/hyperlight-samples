use std::fs::read_to_string;
use std::path::PathBuf;
use std::time::Duration;

use clap::Parser;
use hyperlight_host::sandbox::SandboxConfiguration;
use hyperlight_host::sandbox_state::sandbox::EvolvableSandbox;
use hyperlight_host::sandbox_state::transition::Noop;
use hyperlight_host::GuestBinary;
use hyperlight_host::MultiUseSandbox;
use hyperlight_host::UninitializedSandbox;

/// Simple example of running javascript in Hyperlight
#[derive(Parser, Debug)]
#[command(version, about, long_about = None)]
#[group(multiple = false, required = true)]
struct Args {
    /// Javascript code to evaluate
    #[arg(short, long, conflicts_with = "file")]
    eval: Option<String>,
    /// File to run
    #[arg(short, long, conflicts_with = "eval")]
    file: Option<PathBuf>,
}

// The memory limit for the sandbox's stack, heap, input and output.
// Note: quickjs defines its own memory limits inside the guest
const MEMORY_LIMIT: usize = 32 * 1024 * 1024;

fn main() {
    let args = Args::parse();

    let code = match (args.eval, args.file) {
        (Some(code), None) => code,
        (None, Some(file)) => {
            read_to_string(&file).expect(format!("Failed to read file {:?}", file).as_str())
        }
        _ => unreachable!("Either --eval or --file must be provided"), // should not happen due to clap
    };

    let quickjs_guest_binary = GuestBinary::FilePath("../guest".to_string());
    let mut cfg = SandboxConfiguration::default();
    cfg.set_stack_size(MEMORY_LIMIT as u64);
    cfg.set_heap_size(MEMORY_LIMIT as u64);
    cfg.set_input_data_size(MEMORY_LIMIT);
    cfg.set_output_data_size(MEMORY_LIMIT);
    cfg.set_max_execution_time(Duration::from_secs(10));

    let sandbox = UninitializedSandbox::new(quickjs_guest_binary, Some(cfg), None, None).unwrap();
    let mut multiusesandbox: MultiUseSandbox = sandbox.evolve(Noop::default()).unwrap();

    multiusesandbox
        .call_guest_function_by_name(
            "EvalScript", // this function is defined in the quickjs guest binary
            hyperlight_host::func::ReturnType::Int,
            Some(vec![hyperlight_host::func::ParameterValue::String(code)]),
        )
        .unwrap();
}
