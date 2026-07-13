*This project has been created as part of the 42 curriculum by jomunoz, mrapp-he, pbongiov.*

# ft_irc

## Description

`ft_irc` is a C++98 implementation of a small IRC server, built for the 42 curriculum. The goal of the project is to understand network programming by creating a TCP server that can accept multiple IRC clients, authenticate them with a server password, parse IRC-style commands, and relay messages between users and channels.

The server uses non-blocking sockets and `poll()` to handle several clients in a single process. It implements the core workflow expected from the 42 `ft_irc` subject:

- client registration with `PASS`, `NICK`, and `USER`;
- private messages between users with `PRIVMSG`;
- channel creation and joining with `JOIN`;
- channel messages and user lists;
- channel operator actions with `KICK`, `INVITE`, `TOPIC`, and `MODE`;
- channel modes `+i`, `+t`, `+k`, `+o`, and `+l`;
- numeric replies and errors for the implemented command set;
- clean removal of disconnected clients from their channels.

This project is not a full production IRC daemon. It does not aim to implement every IRC RFC command, server-to-server links, TLS, persistent services, account registration, or IRCv3 extensions. The implemented behavior is intentionally focused on the mandatory `ft_irc` requirements and the commands listed above.

## Technical Overview

The code is organized around three main classes:

- `Server`: owns the listening socket, the `poll()` loop, connected clients, channels, command dispatching, replies, errors, and message delivery.
- `Client`: stores each connection's socket, registration state, nickname, username, buffers, and joined channels.
- `Channel`: stores channel users, operators, invited clients, topic, key, invite-only state, topic restriction, and user limit.

The server receives data into each client's receive buffer, processes complete IRC messages separated by `\r\n`, dispatches supported commands through a command map, and queues outgoing replies in each client's send buffer. Writable sockets are flushed when `poll()` reports `POLLOUT`.

An optional `Bot` executable is also present. It connects as a normal IRC client and answers private messages with simple commands such as `!hello`, `!time`, `!help`, `!joke`, and `!list`.

## Instructions

### Requirements

- A Unix-like system.
- `make`.
- A C++ compiler supporting C++98, such as `c++` or `g++`.
- An IRC client for manual testing, for example `irssi`, HexChat, WeeChat, or a raw TCP tool such as `nc`.

### Compilation

Build the IRC server:

```sh
make
```

This creates the executable:

```text
./ircserv
```

Build the optional bot:

```sh
make bot
```

Useful Makefile targets:

```sh
make clean    # remove object files
make fclean   # remove object files and executables
make re       # rebuild the server from scratch
make val      # rebuild and run the server with valgrind on port 8080
```

### Running the Server

The server expects exactly two arguments:

```sh
./ircserv <port> <password>
```

Example:

```sh
./ircserv 6667 pass
```

The accepted port range in this implementation is `1024` to `65535`.

### Connecting With an IRC Client

With `irssi`, one possible connection command is:

```text
/connect -password pass 127.0.0.1 6667
```

Then try:

```text
/join #chat
/msg #chat hello everyone
/topic #chat Project discussion
```

The first user to create a channel becomes a channel operator and can use operator-only commands such as:

```text
/mode #chat +i
/invite othernick #chat
/kick othernick #chat reason
```

### Raw Protocol Example

IRC messages are line-based and must end with `\r\n`. A minimal manual session can be sent with `nc`:

```sh
printf 'PASS pass\r\nNICK alice\r\nUSER alice 0 * :Alice\r\nJOIN #chat\r\nPRIVMSG #chat :hello\r\n' | nc 127.0.0.1 6667
```

For interactive raw testing:

```sh
nc 127.0.0.1 6667
```

Then type commands in the IRC format:

```text
PASS pass
NICK alice
USER alice 0 * :Alice
JOIN #chat
PRIVMSG #chat :hello
```

Depending on your terminal and `nc` version, manual input may send only `\n`; real IRC clients are usually easier for end-to-end testing because they send the expected `\r\n` delimiter.

### Running the Optional Bot

Build it first:

```sh
make bot
```

Connect it to a local server:

```sh
./bot <port> <password> <bot_name>
```

Example:

```sh
./bot 6667 pass helperbot
```

Or connect it to a specific host:

```sh
./bot <host> <port> <password> <bot_name>
```

The bot responds to private messages:

```text
/msg helperbot !list
/msg helperbot !hello
/msg helperbot !time
/msg helperbot !help
/msg helperbot !joke
```

## Supported IRC Commands

| Command | Purpose | Notes |
| --- | --- | --- |
| `PASS` | Authenticate with the server password. | Required before registration is complete. |
| `NICK` | Set or change nickname. | Nicknames are limited to 9 characters and checked for basic IRC validity. |
| `USER` | Set username information. | Completes registration when `PASS` and `NICK` are also valid. |
| `JOIN` | Join or create a channel. | The first user in a new channel becomes operator. |
| `PRIVMSG` | Send a private or channel message. | Channel messages require membership in the target channel. |
| `KICK` | Remove a user from a channel. | Channel operator only. |
| `INVITE` | Invite a user to a channel. | Channel operator only. Used with invite-only channels. |
| `TOPIC` | View or change a channel topic. | Restricted to operators when channel mode `+t` is enabled. |
| `MODE` | Change supported channel modes. | Channel operator only. |

Supported channel modes:

| Mode | Meaning |
| --- | --- |
| `+i` / `-i` | Enable or disable invite-only access. |
| `+t` / `-t` | Restrict or unrestrict topic changes to channel operators. |
| `+k <key>` / `-k` | Set or remove a channel password. |
| `+o <nick>` / `-o <nick>` | Give or remove channel operator privileges. |
| `+l <limit>` / `-l` | Set or remove a user limit. |

## Project Structure

```text
.
|-- Makefile
|-- main.cpp
|-- lib/
|   |-- Bot.hpp
|   |-- Channel.hpp
|   |-- Client.hpp
|   |-- Server.hpp
|   |-- defines.hpp
|   `-- irc.hpp
`-- src/
    |-- Bot/
    |-- Channel/
    |-- Client/
    |-- Server/
    `-- utils.cpp
```

## Resources

- [RFC 1459 - Internet Relay Chat Protocol](https://datatracker.ietf.org/doc/html/rfc1459): original IRC protocol reference, useful for message format, registration, channels, replies, and classic commands.
- [RFC 2812 - Internet Relay Chat: Client Protocol](https://datatracker.ietf.org/doc/html/rfc2812): client-side IRC protocol reference, useful for command syntax and numeric replies.
- [RFC 2811 - Internet Relay Chat: Channel Management](https://datatracker.ietf.org/doc/html/rfc2811): reference for IRC channel concepts and modes.
- [Linux `poll(2)` manual page](https://man7.org/linux/man-pages/man2/poll.2.html): reference for the multiplexing system call used by the server loop.
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/): practical reference for sockets, `bind()`, `listen()`, `accept()`, and TCP client/server programming.

### AI Usage

AI assistance was used for this README update to inspect the repository structure, summarize the implemented classes and command handlers, and draft English documentation that matches the current codebase. It was not used here to add new server features or modify the C++ source files. Any protocol behavior described in this README was checked against the existing implementation in `main.cpp`, `lib/`, and `src/`.
