1. Make sure you have Hyperlight cloned to a repo adjacent to this one, and check out the following Hyperlight branch
        ``` git fetch origin pull/104/head:musl_subtree && git checkout musl_subtree```
2. In this repo, run `just cp-from-adjacent-hl-repo` to copy over header and lib files required for building the guest
3. In this repo, run `just download-qjs` to download quickjs
4. Run `just patch-qjs` to patch quickjs.
5. Run `just run samples/pi_bigfloat.js` to compile and run the host cli application, passing in a javascript file to be executed.

