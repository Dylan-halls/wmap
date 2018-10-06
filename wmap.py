#!/usr/bin/env python3

"""
Wmap.

Webapp Mapper, automatic webapp vulnerability scanner and exploiter.
"""

import dirscanner


class Wmap(object):
    """Main controlling class for wmap."""

    def __init__(self, *args):
        """Handle setup for class."""
        super(Wmap, self).__init__()
        self.args = args
        self.target = self.args[0]

    def start(self):
        """Start the scan."""
        # to scan with ssl pass 1 insted of 0 to the function.
        self.urls = dirscanner.scan(self.target, 0)
        print(self.urls)


if __name__ == '__main__':
    wmap = Wmap("127.0.0.1")
    wmap.start()
