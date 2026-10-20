# zephyr-installer-example

A small library made to try [zephyr-installer](https://github.com/nimaltd/zephyr-installer) on, and a starting point for making your own library installable into a Zephyr or nRF Connect SDK application.

It does one useful thing. `example_due()` says yes once every `CONFIG_EXAMPLE_PERIOD_MS` milliseconds and never blocks, so a loop can blink an LED, read a sensor or print a line on a rhythm without sleeping. `example_hello()` prints `CONFIG_EXAMPLE_GREETING`. Both settings are Kconfig symbols. Its `uptime` option adds `example_uptime()`, the time since start up in hours, minutes and seconds.

It also uses what zephyr-installer 2.0.0 brings, so you can see each of them work:

- **A file made once.** Its settings come in `example.conf`, made in your application's folder and yours from then on. No install writes over it.
- **Groups of CMake lines.** One in your `CMakeLists.txt`, above `find_package(Zephyr)`, adds `example.conf` to the build. One in its module puts your application's folder on the library's include path.
- **A step it runs.** `make-id` makes `example_id.h`, this device's own ID, a random number. The installer shows what it runs and asks first.

---

## Try the installer with it

Install the installer once:

```bash
pip install zephyr-installer
```

It needs zephyr-installer 2.0.0 or newer. `pip install --upgrade zephyr-installer` brings an older one up to date.

Then, from your application's folder, the one holding `CMakeLists.txt` and `prj.conf`:

```bash
zephyr-installer nimaltd/zephyr-installer-example
```

Press Enter at its questions, and the library goes into `modules/example/` as a Zephyr module, `CMakeLists.txt` gets a block that adds it to the build and one that adds `example.conf`, `prj.conf` gets `CONFIG_EXAMPLE=y`, and `example.conf` is made. Last, it asks about its step:

```
Run
  make-id      Make this device's own ID, example_id.h
               python scripts/make_id.py example_id.h
               Run it? [Y/n]:
Made example_id.h: device B6D5FDE4.
               done
```

Enter runs it, and `example_id.h` is in your application's folder, with a number of its own.

Then use it in `src/main.c`:

```c
#include <zephyr/kernel.h>

#include "example.h"

int main(void)
{
    example_t tick;

    example_init(&tick);

    while (1)
    {
        if (example_due(&tick))
        {
            example_hello(&tick);
        }

        k_msleep(1);
    }

    return 0;
}
```

Build and run it as always. On `native_sim` it runs on your PC, with no board:

```bash
west build -b native_sim
west build -t run
```

```
Hello from example 1, device B6D5FDE4
Hello from example 2, device B6D5FDE4
Hello from example 3, device B6D5FDE4
```

### Things to try

**Change a setting.** The settings are Kconfig symbols, in `example.conf`, the file the install made in your application's folder. Change them there:

```
CONFIG_EXAMPLE_PERIOD_MS=100
CONFIG_EXAMPLE_GREETING="Hi"
```

Run the install command again, and the file is as you left it: the output says `kept example.conf  yours`. Zephyr reads it after `prj.conf`, so a line here wins over one there. `west build -t menuconfig` shows them too, under **example**.

**Its device ID.** Run the install again, and the step is not asked about: `make-id skipped, example_id.h is already there`, so the ID never changes under you. Delete `example_id.h`, and the next install asks again and makes a new one. Answer for it on the command line instead, with `--run make-id` to run it or `--no-run make-id` to leave it, and `--yes` takes every answer offered, for a script. Nobody to answer, and it is not run; then `example_id()` is 0.

**Look at what it wrote.** Each group of CMake lines sits between two lines naming it, so you can find it, and the next install writes it again:

```cmake
# >>> zephyr-installer: example/conf >>>
# From example's installer.yml, and written again on every install.
list(APPEND EXTRA_CONF_FILE "${CMAKE_CURRENT_SOURCE_DIR}/example.conf")
# <<< zephyr-installer: example/conf <<<
```

The other, `example/id`, is in `modules/example/zephyr/CMakeLists.txt`, and puts your application's folder on the library's include path, which is how `example.c` finds `example_id.h`.

**Add a part, and take it out.** `--with uptime` adds `example_uptime.h` and `example_uptime.c` to the module. `--without uptime` takes them out again.

```bash
zephyr-installer nimaltd/zephyr-installer-example --with uptime
```

```c
example_uptime_t up = example_uptime();   /* up.hours, up.minutes, up.seconds */
```

**Uninstall it.** Its blocks leave `CMakeLists.txt` and `prj.conf`, which are back as they were, and its files go. `example.conf` and `example_id.h` stay, since they are yours, and `example.conf` is no longer in the build. A setting of its you set yourself in `prj.conf` is named, since Zephyr stops on a symbol it no longer knows:

```bash
zephyr-installer --uninstall example
```

**Without internet.** Download this repository as a zip, from the green **Code** button, **Download ZIP**, and give the installer the zip instead of the name:

```bash
zephyr-installer D:/Downloads/zephyr-installer-example-main.zip
```

---

## Make your own library installable

Copy [`installer.yml`](installer.yml) to the root of your library's repository, and change what is yours: its name, its description, its repository, its files and its settings. Every key the installer reads is in it, with a comment saying what it does; the ones this library has no use for are commented out. Only `name` and `files` are required.

Then anyone installs your library with:

```bash
zephyr-installer yourname/yourlibrary
```

Each entry under `zephyr.settings` becomes a Kconfig symbol the user sets, and your code reads it as `CONFIG_<name>`. `once` lists files for the application's own folder, `run` the steps run after the install, with the script in [`scripts/make_id.py`](scripts/make_id.py), and `zephyr.cmake` the groups of CMake lines with where each goes. The [zephyr-installer README](https://github.com/nimaltd/zephyr-installer#for-someone-writing-a-library) has the rest.

---

## API

| Function | What it does |
|---|---|
| `void example_init(example_t *handle)` | Start a period of `CONFIG_EXAMPLE_PERIOD_MS` from now |
| `bool example_due(example_t *handle)` | Whether the period has passed. Returns at once, and starts the next one when it has |
| `uint32_t example_count(const example_t *handle)` | How many times `example_due()` has said yes |
| `uint32_t example_id(void)` | This device's own ID, from `example_id.h`, or 0 when the step was not run |
| `void example_hello(const example_t *handle)` | Print `CONFIG_EXAMPLE_GREETING`, how many periods have passed, and the device's ID |
| `example_uptime_t example_uptime(void)` | With the `uptime` option: hours, minutes and seconds since start up |

Declare one `example_t` for each thing that repeats on its own rhythm. A loop that comes back late is caught up, so the rhythm does not drift.

---

## License

Apache License 2.0. See [LICENSE.md](LICENSE.md).
