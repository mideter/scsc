# scsc

Simple client/server C++.

## Build

```bash
cmake -S . -B build
cmake --build build
```

## Run

Open two terminals:

1. Start server:
```bash
./build/server
```
2. Run client:
```bash
./build/client
```

Expected flow:
- client sends: `Hello from client!`
- server replies: `Hello from server!`

