# JavaScript Modules

There are a few modules available to use in the soft backend and your frontend views.

The modules in the `node` folder reflect most if not all features of the native NodeJS modules.

# Node.js Core Modules

| Module            | Description                                                                 |   |
|--------------------|-----------------------------------------------------------------------------|---|
| `assert`          | Provides assertion functions for testing and validating conditions.         | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `async_hooks`     | Provides an API to track asynchronous resources in Node.js.                | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `buffer`          | Handles binary data, dealing with raw memory allocation.                   | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `child_process`   | Provides methods to spawn child processes.                                  | <span style="color: orange; font-weight: bold;">`PARTIAL`</span>  |
| `cluster`         | Enables the creation of multi-core applications.                           | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `console`         | Provides a simple debugging console similar to the browser's.              | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `crypto`          | Offers cryptographic functionality, including OpenSSL wrappers.            | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `dgram`           | Provides UDP datagram socket implementation.                               | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `diagnostics_channel` | A mechanism for context propagation in diagnostics.                   | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `dns`             | Provides DNS resolution functions.                                         | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `domain`          | Helps handle unhandled errors, but is deprecated.                          | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `events`          | Provides the EventEmitter class to handle events.                          | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `fs`              | Handles file system operations.                                            | <span style="color: orange; font-weight: bold;">`PARTIAL`</span>  |
| `fs/promises`     | Promises-based file system API.                                             | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `http`            | Used to create HTTP servers and make HTTP requests.                        | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `http2`           | Provides support for HTTP/2.                                               | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `https`           | Used to create HTTPS servers and make HTTPS requests.                      | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `inspector`       | Provides an API for debugging and profiling.                               | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `module`          | Provides utilities for interacting with Node.js modules.                   | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `net`             | Creates network servers and clients.                                       | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `os`              | Provides information about the operating system.                           | <span style="color: orange; font-weight: bold;">`PARTIAL`</span>  |
| `path`            | Handles and transforms file paths.                                         | <span style="color: orange; font-weight: bold;">`PARTIAL`</span>  |
| `perf_hooks`      | Provides performance monitoring and metrics.                               | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `process`         | Provides information and control over the Node.js process.                | <span style="color: orange; font-weight: bold;">`PARTIAL`</span>  |
| `punycode`        | Converts Unicode strings to Punycode and vice versa.                       | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `querystring`     | Parses and formats URL query strings.                                       | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `readline`        | Interfaces for reading input from a Readable stream, like process.stdin.   | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `repl`            | Provides a Read-Eval-Print-Loop (REPL) implementation.                     | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `stream`          | Handles streaming data.                                                    | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `stream/promises` | Promises-based stream API.                                                 | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `string_decoder`  | Decodes buffer to string.                                                  | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `sys`             | Alias for `util`. Deprecated.                                              | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `timers`          | Handles scheduling of functions (e.g., `setTimeout`, `setInterval`).       | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `timers/promises` | Promises-based timers API.                                                 | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `tls`             | Implements Transport Layer Security (TLS) and Secure Socket Layer (SSL).   | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `trace_events`    | Provides a mechanism to trace events.                                      | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `tty`             | Provides support for terminal interfaces.                                  | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `url`             | URL resolution and parsing utilities.                                      | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `util`            | Provides utilities for debugging and working with objects.                 | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `v8`              | Exposes APIs specific to the V8 JavaScript engine.                        | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `vm`              | Provides APIs to compile and run code within V8 contexts.                | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `wasi`            | WebAssembly System Interface.                                              | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `worker_threads`  | Enables multithreading in Node.js.                                         | <span style="color: red; font-weight: bold;">`NONE`</span>  |
| `zlib`            | Provides compression and decompression utilities.                         | <span style="color: red; font-weight: bold;">`NONE`</span>  |


The modules in the `npm` folder emulate a few specific NPM modules.

| Name       | Description       | Implementation       |
|----------------|----------------|----------------|
| `electron-js`   | ElectronJS emulation module   |  <span style="color: orange; font-weight: bold;">`PARTIAL`</span>    |
| `fs-extra`   | An extension of the `fs` module   | <span style="color: orange; font-weight: bold;">`PARTIAL`</span>   |

The modules in the `sk` folder are modules developed by the Superkraft team with certain specific features.

| Name       | Description       |        |
|----------------|----------------|----------------|
| `application`   | API for application information   |
| `vfs`   | Virtual File System module for file manipulation directly to memory rather than disk   |
| `web`   | Perform GET, POST and download files   |