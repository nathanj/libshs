libshs - A Small HTTP Server C++ API
====================================

Description
-----------
libshs is a C++ API for embedding a HTTP server in C++ programs. Its primary
goal is to make it easy to add a web interface to new or existing C++ programs.

Examples
--------
Examples of how to use the library are stored in the `src/examples` directory.
A simple hello world example can be viewed in [example1.cc][1] while a more
complex form application can be viewed in [example2.cc][2].

Download
--------
There are currently no released versions of this library.

The latest code can be retrieved via git.

    $ git clone git://github.com/nathanj/libshs.git

Dependencies
------------
There are no dependencies for this library.

Install
-------
    $ make
	# make install

This will install the static library and the include files.

If you want to use libshs in your program, you may want to copy `src/libshs`
into your codebase instead of linking with the library.

Portability
-----------
I have only tested on Linux. Other operating systems may or may not work.
Patches are welcome.

  [1]: http://github.com/nathanj/libshs/tree/master/src/examples/example1.cc
  [2]: http://github.com/nathanj/libshs/tree/master/src/examples/example2.cc

