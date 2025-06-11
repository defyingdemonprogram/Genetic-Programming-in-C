## Installing SDL

1. **Download and extract SDL:**

    ```bash
    wget https://github.com/libsdl-org/SDL/releases/download/release-3.2.16/SDL3-3.2.16.tar.gz
    tar -xvzf SDL3-3.2.16.tar.gz
    cd SDL3-3.2.16
    ```

2. **Create a custom installation directory:**

    ```bash
    mkdir -p ~/opt/c/sdl
    ```

3. **Build and install SDL:**

    ```bash
    mkdir build && cd build
    cmake .. -DCMAKE_INSTALL_PREFIX=$HOME/opt/c/sdl
    make -j$(nproc)
    make install
    ```
