1. Make sure [hyperlight](https://github.com/hyperlight-dev/hyperlight) is cloned adjacent to this repo and that the folder is named "hyperlight". The folder structure should look like this
```bash
$ ls
hyperlight  hyperlight-samples
```
2. `cd` into `hyperlight-samples`, and run all the following commands from within `hyperlight-samples` folder:
    - `just cp-from-adjacent-hl-repo` to copy over header and lib files required for building the js guest
    - `just download-qjs` to download quickjs
    - `just patch-qjs` to patch quickjs.
    - `just build-qjs-guest` to compile the guest
    - `just run samples/pi_bigfloat.js` to compile and run the host cli application, passing in a javascript file to be executed. There are more samples in the samples folder.

### minor tweaks

You might need to increase the MEMORY_LIMIT and BUFFER_LIMIT in `main.rs`, in case the current memory limits are not enough to execute your javascript code without erroring out.