#!/usr/bin/env python3

"""
Wmap.

Webapp Mapper, automatic webapp vulnerability scanner and exploiter.
"""

import xsser
import dirscanner
from lib.logger import status, success, verbose
from progressbar import ProgressBar, Percentage, Bar, ETA


class Wmap(object):
    """Main controlling class for wmap."""

    def __init__(self, target, verbose=False):
        """Handle setup for class."""
        super(Wmap, self).__init__()
        self.target = target
        self.verbose = verbose
        self.scan_begun = False

    def _dirscan(self):
        # to scan with ssl pass 1 insted of 0 to the function.
        ssl = 0
        self.urls = []
        intresting_urls = []
        self.scan_begun = True
        widgets = [Percentage(), ' ',
                   Bar(marker='█', left='[', right=']'), ' ', ETA()]
        # change to sccept diffrent wordlists
        with open("wordlists/directorys/common.txt", "r") as file:
            data = file.read().splitlines()
            pbar = ProgressBar(widgets=widgets, maxval=len(data))
            pbar.start()
            for line in data:
                if ssl == 0:
                    url = "http://"+self.target+"/"+line
                elif ssl == 1:
                    url = "https://"+self.target+"/"+line
                scandata = dirscanner.scan(url, ssl)
                if scandata[url] != 404:
                    intresting_urls.append(scandata)
                self.urls.append(scandata)
                pbar.update(data.index(line))
        pbar.finish()
        return intresting_urls

    def _xss_scan(self):
        xsser.scan()

    def start(self):
        """Start the scan."""
        status("Starting directory scan")
        self.intresting_urls = self._dirscan()
        if self.intresting_urls is not 0:
            success("found %d urls to take a look at..." %
                    (len(self.intresting_urls)))
        for urldic in self.intresting_urls:
            for key in urldic:
                verbose(self, "[%d] %s" % (urldic[key], key))
        self._xss_scan()


if __name__ == '__main__':
    wmap = Wmap("127.0.0.1", verbose=True)
    wmap.start()
