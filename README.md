# DideroCpp

C++ library for fetching and storing currency prices

To build, [install vcpkg](https://github.com/Microsoft/vcpkg).

Then install `cpprestsdk` and `libmysql` by running `vcpkg install cpprestsdk:x64-windows` and `vcpkg install libmysql:x64-windows` on Windows or `vcpkg install cpprestsdk` and  `vcpkg install libmysql` otherwise.

If the `libmysql` installation fails, [this patch](https://github.com/Microsoft/vcpkg/issues/5046#issuecomment-451047415) may work.

Requires the [Microsoft Visual Studio Installer Projects](https://marketplace.visualstudio.com/items?itemName=visualstudioclient.MicrosoftVisualStudio2017InstallerProjects) VS extension to build the installer and the [MSVC redistributable package](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads) for VS 2019 to install onto a client.

See [this page](https://docs.microsoft.com/en-us/cpp/windows/walkthrough-deploying-a-visual-cpp-application-by-using-a-setup-project?view=vs-2019) for documentation on building an installer file.