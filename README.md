# zephyr-installer-example

A small library made to try [zephyr-installer](https://github.com/nimaltd/zephyr-installer) on, and a starting point for making your own library installable into a Zephyr or nRF Connect SDK application.

It does one useful thing. `example_due()` says yes once every `CONFIG_EXAMPLE_PERIOD_MS` milliseconds and never blocks, so a loop can blink an LED, read a sensor or print a line on a rhythm without sleeping. `example_hello()` prints `CONFIG_EXAMPLE_GREETING`. Both settings are Kconfig symbols, set in `prj.conf`. Its `uptime` option adds `example_uptime()`, the time since start up in hours, minutes and seconds.

---

## Try the installer with it

Install the installer once:

```bash
pip install zephyr-installer
```

Then, from your application's folder, the one holding `CMakeLists.txt` and `prj.conf`:

```bash
zephyr-installer nimaltd/zephyr-installer-example
```

Press Enter at its questions, and the library goes into `modules/example/` as a Zephyr module, `CMakeLists.txt` gets a line that adds it to the build, and `prj.conf` gets `CONFIG_EXAMPLE=y`.

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
Hello from example 1
Hello from example 2
Hello from example 3
```

### Things to try

**Change a setting.** The settings are Kconfig symbols. Set them in `prj.conf`, below the installer's block, or in `west build -t menuconfig`, under **example**:

```
CONFIG_EXAMPLE_PERIOD_MS=100
CONFIG_EXAMPLE_GREETING="Hi"
```

Run the install command again, and your lines are still there: the installer writes only its own block.

**Add a part, and take it out.** `--with uptime` adds `example_uptime.h` and `example_uptime.c` to the module. `--without uptime` takes them out again.

```bash
zephyr-installer nimaltd/zephyr-installer-example --with uptime
```

```c
example_uptime_t up = example_uptime();   /* up.hours, up.minutes, up.seconds */
```

**Uninstall it.** Its blocks leave `CMakeLists.txt` and `prj.conf`, which are back as they were, and its files go. A setting of its you set yourself is named, since Zephyr stops on a symbol it no longer knows:

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

Each entry under `zephyr.settings` becomes a Kconfig symbol the user sets in `prj.conf`, and your code reads it as `CONFIG_<name>`. The [zephyr-installer README](https://github.com/nimaltd/zephyr-installer#for-someone-writing-a-library) has the rest.

---

## API

| Function | What it does |
|---|---|
| `void example_init(example_t *handle)` | Start a period of `CONFIG_EXAMPLE_PERIOD_MS` from now |
| `bool example_due(example_t *handle)` | Whether the period has passed. Returns at once, and starts the next one when it has |
| `uint32_t example_count(const example_t *handle)` | How many times `example_due()` has said yes |
| `void example_hello(const example_t *handle)` | Print `CONFIG_EXAMPLE_GREETING`, and how many periods have passed |
| `example_uptime_t example_uptime(void)` | With the `uptime` option: hours, minutes and seconds since start up |

Declare one `example_t` for each thing that repeats on its own rhythm. A loop that comes back late is caught up, so the rhythm does not drift.

---

## License

Apache License 2.0. See [LICENSE.md](LICENSE.md).
