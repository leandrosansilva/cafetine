= Cafetine: wannabe pimpl for the masses =

Seriously, you should write a README.

For now all the documentation is in the file cafetine.h. Check it out!

Usage restrictions:

- do not use it the the Moon is in the seventh house and Jupter aligns to Mars
- use condoms (usually in the singular, in the plural for specific cases)

BUGS:

Yes, this code is probably extremly buggy, as I have not really tested it in real code,
just small example code that do not code all the needs of a real world class.

- As a friend of me pointed out, the code provides no protection against the class
binary interface, provided by usual pimpl implementations.
I don't know how to fix it (I tried adding a versioning template parameter, which change
the symbol names, but for some reason the linker is able to find and call them!!!).
That reminds me I really don't know how the linker works.

TODO:

- Benchmark it, compare with other pimpl implementations and see how it behaves
  compared to them.
