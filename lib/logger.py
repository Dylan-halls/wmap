"""
Logger.

Logging functions.
"""


def status(data):
    """Log status."""
    text = "\033[01;34m[i]\033[0m " + data
    print(text)

def success(data):
    """Log success."""
    text = "\033[01;32m[+]\033[0m " + data
    print(text)

def verbose(wmap, data):
    """Log verbose output"""
    if wmap.verbose is True:
        text = "\033[33m\t-\033[0m " + data
        print(text)
