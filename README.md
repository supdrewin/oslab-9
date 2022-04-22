# OS Lab 9

## Dependencies

### List

- gcc (or other compiler support `C++20`)
- ncurses6 (for raw terminal support)
- llvm (optional except `macOS`)
- gmake (required if `FreeBSD`)

### Install

- Arch Linux

  - required

    ``` shell
    $ pacman -S base-devel ncurses
    ```

  - optional

    ``` shell
    $ pacman -S clang lld libc++ libunwind
    ```

- Debian

  - required

    ``` shell
    $ apt install build-essential libncurses-dev
    ```

  - optional

    ``` shell
    $ apt install clang lld libc++-dev libunwind-dev
    ```

- FreeBSD

  ``` shell
  $ pkg install gmake ncurses
  ```

- macOS

  ``` shell
  $ brew install llvm ncurses
  ```

  and then write following into your `.bashrc`/`.zshrc`:

  ``` shell
  $ export PATH="/usr/local/opt/ncurses/bin:$PATH"
  ```

- Windows

  no yet supported

## Make

### Build

- FreeBSD

  ``` shell
  $ gmake
  ```

- Others

  ``` shell
  $ make
  ```

  if you want to build with `Clang`:

  ``` shell
  $ CXX=clang++ make
  ```

### Clean

- FreeBSD

  ``` shell
  $ gmake clean
  ```

- Others

  ``` shell
  $ make clean
  ```

### Test

``` shell
$ ./impl1.exe
$ ./impl2.exe
```
