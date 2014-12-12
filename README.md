pcghash
=======

An attempt to use RNGs to resolve hash collisions.  In particular built upon
the [PCG family](http://www.pcg-random.org/).

The idea is that the to-be-hashed value is used to seed an RNG.  An initial
random number is drawn as slot candidate and used if there's no collision.
Upon a hash collision the next random is drawn and used as slot candidate,
and so on until either a free slot is found or the random number sequence
becomes periodic.

