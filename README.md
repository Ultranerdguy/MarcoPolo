# Marcopolo
A simple client/server tool that establishes a connection between machines and periodically sends a simple message.

This was originally created as a project for learning socket programming in Linux, as well as differences between TCP and UDP.

## Usage
- Build the project using `make` to produce two binaries `marco` and `polo`.
- Run the `polo` binary to create a simple server listening server, specifying the port and whether to use tcp or udp.
- On a separate machine (or the same machine, I'm not your boss), run the `marco` binary to create a client, passing the ip address of the first machine, and providing the same port and option of tcp/udp as the first machine.

### Parameters
`polo <port> <tcp|udp>`

`marco <ipv4> <port> <tcp|udp>`

