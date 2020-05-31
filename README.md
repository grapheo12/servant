# Servant

New player in the market of HTTP Servers.

Wait! We aren't there yet!

This is an under-development alpha version of a HTTP server. (I may add support for Gopher too!)
But before that it needs to establish a stable connection to the internet and parse and send
HTTP requests and responses respectively without leaking memory.

Everything is kinda built from scratch, taking reference from "Beej's Guide to Network Programming",
and the man pages and obviously StackOverflow!!.

I repeat! It is not a production server. It's more of a pursuit to learn C++ and system calls
and network programming.

However, if you really want to try it out, just fork/clone the repo and run:

```bash

make

```

`bin/servant` is the executable you are looking for.

To build the tests, run:

```bash

make test

```

Tests will be built inside `bin/tests/`.

Contributions are always welcome!