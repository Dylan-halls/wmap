"""Build the project."""

import os
from distutils.core import setup, Extension

os.system("# libcurl4-gnutls-dev")

setup(name='wmap', version='0.1',
      ext_modules=[Extension('dirscanner',
                             sources=['auxiliary/dirscanner/dirscanner.c'],
                             extra_link_args=["-lcurl"]),
                   Extension('xsser',
                             sources=['auxiliary/xsser/xsser.c'],
                             extra_link_args=["-lcurl"])
                   ])
