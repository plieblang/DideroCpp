# DideroCpp

C++ library for fetching and storing currency prices

To build, [install vcpkg](https://github.com/Microsoft/vcpkg).

Then install the `cpprestsdk` and `libmysql` by running `vcpkg install cpprestsdk:x64-windows` and `vcpkg install libmysql:x64-windows` on Windows or `vcpkg install cpprestsdk` and  `vcpkg install libmysql` otherwise.

If the `libmysql` installation fails, [this patch](https://github.com/Microsoft/vcpkg/issues/5046#issuecomment-451047415) may work.
