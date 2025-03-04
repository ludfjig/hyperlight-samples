# Run javascript in Hyperlight using quickjs
This is a small demo application of how to use [Hyperlight](https://github.com/hyperlight-dev/hyperlight) to run isolated JavaScript in a microVM using quickjs. 

## Requirements
- A x86_64 linux machine (or WSL) with `clang` installed (I used 18.1.8 but I believe most versions should work).
- Rust installed (`rustup show` must have active rustc version >=1.80, if not run `rustup update`)
- `just` command runner installed (`cargo install just`) 

## Steps to build and run 
1. Clone this repo 
```
git clone git@github.com:ludfjig/hyperlight-samples.git
```
2. `cd` into `hyperlight-samples`, and run all the following commands from within `hyperlight-samples` folder:
    - `just download-hl-headers-and-lib` to download over header and lib artifacts from Hyperlight github repo release page
    - `just download-qjs` to download and extract quickjs source code
    - `just patch-qjs` to patch quickjs source code
    - `just build-qjs-guest` to compile the guest
    - `just run samples/pi_bigfloat.js` to compile and run the host cli application, passing in a javascript file to be executed. This specific javascript program calculates 1 million digits of pi. There are more samples in the samples folder. You can also `cd` into host directory an run cargo run from there directly, for example `cd host && echo "1+1" | cargo run`

### minor tweaks

You might need to increase the MEMORY_LIMIT and BUFFER_LIMIT in `main.rs`, in case the current memory limits are not enough to execute your particular javascript.