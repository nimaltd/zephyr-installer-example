"""
Makes example_id.h: this device's own ID, a random 32 bit number.

The installer runs this as example's make-id step, in the project's folder,
once the library's files are in, and only when the user says yes. Its argument
is the file to make. The step names that file under unless, so the installer
skips it when the file is there already, and this checks again for anyone who
runs it by hand: an ID that changed under a device already in use would be
worse than none.
"""

import secrets
import sys
from pathlib import Path

TEMPLATE = """\
/*
 * Made once by example's make-id step: this device's own ID, a random 32 bit
 * number. It is yours now, and no install changes it. Delete this file, and
 * the next install offers to make a new one.
 */

#ifndef EXAMPLE_ID_H
#define EXAMPLE_ID_H

#define EXAMPLE_DEVICE_ID           0x{device:08X}UL

#endif /* EXAMPLE_ID_H */
"""


def main(argv):
    target = Path(argv[1]) if len(argv) > 1 else Path("example_id.h")

    if target.exists():
        print(f"{target.as_posix()} is there already, so it is kept.")
        return 0

    device = secrets.randbits(32)
    target.parent.mkdir(parents=True, exist_ok=True)

    with open(target, "w", encoding="utf-8", newline="\n") as file:
        file.write(TEMPLATE.format(device=device))

    print(f"Made {target.as_posix()}: device {device:08X}.")

    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
