= Cafetine: wannabe pimpl for the masses =

Seriously, you should write a README.

Actually it's a quasi-pimpl, because it cannot guarantee binary compatibility in case of
some implementation detail's change (namely the size of the implementation object)

For now all the documentation is in the file cafetine.h. Check it out!

Usage restrictions:

- do not use it the the Moon is in the seventh house and Jupter aligns to Mars
- use condoms (usually in the singular, in the plural for specific cases)

BUGS:

Yes, this code is probably extremly buggy, as I have not really tested it in real code,
just small example code that do not code all the needs of a real world class.

TODO:

- Benchmark it, compare with other pimpl implementations and see how it behaves
  compared to them.
