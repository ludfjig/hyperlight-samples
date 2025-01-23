# Run javascript in Hyperlight using quickjs

## Requirements
- A linux machine with clang installed (I used 18.1.8 but I believe most versions should work)
- `rustup show` must have active rustc version >=1.80, if not run `rustup update`

## Steps to build and run 
1. Clone this repo 
```
git clone git@github.com:ludfjig/hyperlight-samples.git
```
2. Make sure [hyperlight](https://github.com/hyperlight-dev/hyperlight) is cloned adjacent to this repo
```
git clone git@github.com:hyperlight-dev/hyperlight.git
``` 
The default `main` branch of hyperlight should work, but in case it doesn't, try checking out commit `003bc162f9fa2fb43eb849fdd3781f5780f318eb`, which is the SHA this sample was most recently tested on. The folder structure should look like this
```bash
$ ls
hyperlight  hyperlight-samples
```
2. `cd` into `hyperlight-samples`, and run all the following commands from within `hyperlight-samples` folder:
    - `just cp-from-adjacent-hl-repo` to copy over header and lib artifacts from hyperlight repo, required for building the js guest
    - `just download-qjs` to download and extract quickjs source code
    - `just patch-qjs` to patch quickjs source code
    - `just build-qjs-guest` to compile the guest
    - `just run samples/pi_bigfloat.js` to compile and run the host cli application, passing in a javascript file to be executed. This specific javascript program calculates 1 million digits of pi. There are more samples in the samples folder. The `date.js` file uses Date.now(), which is stubbed out since the guest currently has not way of getting the current date/time, but it should be easy to implement this using a host function (which is what we do for hyperlight-js). 

### minor tweaks

You might need to increase the MEMORY_LIMIT and BUFFER_LIMIT in `main.rs`, in case the current memory limits are not enough to execute your javascript code without erroring out.