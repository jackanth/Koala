![Graph example](./screenshots/graph.svg)

# Koala

A thread-safe, header-only C++17 library for managing object lifetimes and relationships.

If serialization is enabled, Koala depends on the cereal library v1.2.2 (https://github.com/USCiLab/cereal). For hierarchy visualization, the  dot application is required. On Ubuntu 18.04, this can be installed via `apt install graphviz`.

## Building
Installing the headers and a `koala-config.cmake` file can be done using cmake, e.g.:
```
cd Koala
mkdir build
cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/prefix
make install
```

If building with cereal support, the file `cmake/FindCereal.cmake` will be used to locate it:
```
cd Koala
mkdir build
cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/prefix -DTARGET_CEREAL_SUPPORT=ON
make install
```

The `libunwind-dev` package is required for building tests, e.g. on Ubuntu 18.04:
```
apt install libunwind-dev
cd Koala
mkdir build
cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/prefix -DBUILD_TESTS=ON
make install
```

## License
(c) 2018 Jack Anthony
This source code is subject to the terms and conditions defined in the file LICENSE.
