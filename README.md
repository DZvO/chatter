About
----
A console/terminal based 'chat' application. Features are:  

* Usage of the standard "low level" BSD-style socket APIs and high abstraction of them.
* Threading and synchronisation (Not yet done)
* maybe more :)

How to Build
------------
__What you need!__ (at the moment only linux is supported; you can probably get all of these from your package manager)  

> * Standard __binutils__ and __gcc/g++__
> * [Python](http://python.org), either 2.7 or 3.x
> * [Scons](http://scons.org), 2.0.x 
> * [Boost](http://boost.org), any version >1.4 should be ok

__Have everything?__

> Then run "scons", the binarys will be placed in a new directory called "build"
