# PI Game Engine

## Running locally
Clone the repo to your local system and navigate to the root of the project:
```bash
git clone --recursive https://github.com/Ritam727/pi_game_engine.git
cd pi_game_engine/
```

### On Linux
To build the project, run the following commands:
```bash
mkdir build
cd build/
cmake .. --preset=debug/release # any one of release or debug
cmake --build . - --j4 
```

Run the application using:
```bash
./pi_game_engine # from inside the build folder
```
