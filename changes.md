# Changes

## What I changed

- Enabled the required compiler and warning flags in [Makefile](Makefile).
- Ignored `SIGPIPE` during server startup in [src/Server/ServerService.cpp](src/Server/ServerService.cpp).
- Added nickname validation in [src/Server/ServerCommands.cpp](src/Server/ServerCommands.cpp).
- Replaced the custom registration text with standard IRC welcome numerics in [src/Server/ServerCommands.cpp](src/Server/ServerCommands.cpp).
- Added standard JOIN-side replies for topic, names, and end-of-names in [src/Server/ServerCommands.cpp](src/Server/ServerCommands.cpp).
- Fixed the TOPIC query response format in [src/Server/ServerCommandsCh.cpp](src/Server/ServerCommandsCh.cpp).

## Why

- The subject requires `-Wall -Wextra -Werror` and C++98; leaving them commented out would make the submission non-compliant.
- A disconnected client can raise `SIGPIPE` on `send()`, which can terminate the server even if sockets are non-blocking.
- Invalid nicknames should be rejected with `ERR_ERRONEUSNICKNAME` instead of being accepted blindly.
- IRC clients expect standard numeric replies when registration completes and when a channel join succeeds; the previous custom message was not protocol-compatible.
- The previous TOPIC query path built the wrong wire format for `331` because it routed the reply through the generic helper.