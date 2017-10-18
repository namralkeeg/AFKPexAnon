# AFKPexAnon
Application that anonymize's Bethesda's compiled Papyrus script files (.pex).

Copyright (c) 2017 Larry Lopez

See LICENSE file for licensing information.

Dependencies
============

* Boost 1.65.1 http://www.boost.org/
* keeg 1.0.0 https://github.com/namralkeeg/keeg
* A C++14 compiler. (for Windows you need at least Visual Studio 2015 or mingw 5.3)
* Qt Creator (qmake)


### Commandline Options
```
Generic Options:
  -h [ --help ]                         Help Screen
  -c [ --config ] arg (=afkpexanon.cfg) Use configuration file.
  --version                             Show application version information.

Config Options:
  -s [ --source ] arg (=.)              Source Folder(s), defaults to current
                                        folder.
  -b [ --backup ]                       Enables the creation of backup Files.
  -m [ --mask ] arg (=*)                Character to mask computer and user
                                        name. Defaults to *
  -r [ --recursive ]                    Recursively process all subfolders.
  --verbose                             Enables verbose output mode.
```