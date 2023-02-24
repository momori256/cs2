# What is this?

A HTTP server written in C for [Apache HTTP server benchmarking tool](https://httpd.apache.org/docs/2.4/programs/ab.html).

# How to use

To build and run, enter the following commands.

```sh
cd cs2
make
./a.out 22421 # Specify a port number.
```

Then, benchmark with `ab`.

```sh
ab -c 5 -n 20 localhost:22421/ # 5 client, 20 request.
```

You can use `telnet` to verify this server is working fine. The body of the response will be the request message you typed.

```sh
telnet localhost 22421
```
