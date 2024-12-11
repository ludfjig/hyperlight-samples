1. Make sure you have Hyperlight repo cloned to a repo adjacent to this one, and check out the following Hyperlight branch in that repo
        ``` git fetch origin pull/104/head:musl_subtree && git checkout musl_subtree```, (replace origin with the name of your upstream remote). This step will not be necessary once the PR merges.
2. In this hyperlight-samples repo, run :
3. `just cp-from-adjacent-hl-repo` to copy over header and lib files required for building the guest
4. `just download-qjs` to download quickjs
5. `just patch-qjs` to patch quickjs.
6. `just build-qjs-guest` to compile the guest
6. `just run samples/pi_bigfloat.js` to compile and run the host cli application, passing in a javascript file to be executed. There are more samples in the samples folder.

### minor tweaks

You might need to increase the MEMORY_LIMIT and BUFFER_LIMIT in `main.rs`, in case the current memory limits are not enough to execute your javascript code without erroring out.