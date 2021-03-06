# Visual Studio 2015 Build Instructions

## Important

We do not recommend Windows for rippled production use at this time. Currently, the Ubuntu
platform has received the highest level of quality assurance, testing, and support.

## Prerequisites

To clone the source code repository, create branches for inspection or modification,
build rippled under Visual Studio, and run the unit tests you will need these
software components:

* [Visual Studio 2015](https://www.visualstudio.com/)
* [Git for Windows](http://git-scm.com/)
* [Google Protocol Buffers Compiler](https://code.google.com/p/protobuf/source/checkout)
* [OpenSSL Library](README.md#install-openssl)
  * [ActivePerl](https://www.activestate.com/activeperl/downloads)
    (Recommended to build OpenSSL.)
* [Boost 1.59 library](http://www.boost.org/users/download/)
* [Node.js](http://nodejs.org/download/)

Any version of Visual Studio 2015 may be used, including the Visual Studio Community
Edition which is available under a free license.

## Install Software

### Install Visual Studio 2015

If not already installed on your system, download your choice of installers from the
[Visual Studio 2015 Download](https://www.visualstudio.com/downloads/download-visual-studio-vs)
page, run the installer, and follow the directions.
The **Visual Studio 2015 Community** edition is available for free, while paid editions
may be used for an free initial trial period.

### Install Git for Windows

Git is a distributed revision control system. The Windows version also provides the
bash shell and many Windows versions of Unix commands. While there are other
varieties of Git (such as TortoiseGit, which has a native Windows interface and
integrates with the Explorer shell), we recommend installing
[Git for Windows](https://git-scm.com/) since
it provides a Unix-like command line environment useful for running shell scripts.
Use of the bash shell under Windows is mandatory for running the unit tests.

* NOTE: To gain full featured access to the
  [git-subtree](https://blogs.atlassian.com/2013/05/alternatives-to-git-submodule-git-subtree/)
  functionality used in the rippled repository we suggest Git version 1.8.3.2 or later.

### Install Google Protocol Buffers Compiler

Building rippled requires **protoc.exe** version 2.5.1 or later. At your option you
may build it yourself from the sources in the
[Google Protocol Buffers](https://github.com/google/protobuf) repository,
or you may download a
[protoc.exe](https://ripple.github.io/Downloads/protoc/2.5.1/protoc.exe)
([alternate link](https://github.com/ripple/Downloads/raw/gh-pages/protoc/2.5.1/protoc.exe))
precompiled Windows executable from the
[Ripple Organization](https://github.com/ripple).

Either way, once you have the required version of **protoc.exe**, copy it into
a folder in your command line `%PATH%`.

* **NOTE:** If you use an older version of the compiler, the build will fail with
  errors related to a mismatch of the version of protocol buffer headers versus
  the compiler.
  
### Install ActivePerl

If not already installed on your system, download your choice of installers from the
[Activeperl Download](https://www.activestate.com/activeperl/downloads)
page, run the installer, and follow the directions.

## Configure Dependencies

### Install OpenSSL

1. Download OpenSSL *v1.0.2d or higher* source https://www.openssl.org/source/
2. Unpack the source archive into a temporary folder.
3. Open `cmd.exe`. Change the the folder where you unpacked OpenSSL.
4. Build the 64-bit libraries: (
  [Reference 1](http://developer.covenanteyes.com/building-openssl-for-visual-studio/),
  [Reference 2](http://www.p-nand-q.com/programming/windows/building_openssl_with_visual_studio_2013.html))

    ```powershell
    "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\vcvars32.bat"
    "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\amd64\vcvars64.bat"
     
    perl Configure VC-WIN64A --prefix=C:\lib\openssl-VC-64
    ms\do_win64a
    nmake -f ms\nt.mak
    nmake -f ms\nt.mak install
    
    copy tmp32\lib.pdb C:\lib\openssl-VC-64\lib\
    ```
5. Optionally, delete the temporary folder.

* NOTE: Since rippled links statically to OpenSSL, it does not matter where the OpenSSL
  .DLL files are placed, or what version they are. rippled does not use or require any
  external .DLL files to run other than the standard operating system ones.

### Build Boost

After downloading boost and unpacking it, open a **Developer Command Prompt** for
Visual Studio, change to the directory containing boost, then bootstrap the build tools:

```powershell
cd C:\lib\boost_1_59_0
bootstrap
```

The rippled application is linked statically to the standard runtimes and external
dependencies on Windows, to ensure that the behavior of the executable is not
affected by changes in outside files. Therefore, it is necessary to build the
required boost static libraries using this command:

```powershell
bjam --toolset=msvc-14.0 --build-type=complete variant=debug,release link=static runtime-link=static address-model=64
```

Building the boost libraries may take considerable time. When the build process
is completed, take note of both the reported compiler include paths and linker
library paths as they will be required later.

* NOTE: If older versions of Visual Studio are also installed, the build may fail.
  If this happens, make sure that only Visual Studio 2015 is installed. Due to
  defects in the uninstallation procedures of these Microsoft products, it may
  be necessary to start with a fresh install of the operating system with only
  the necessary development environment components installed to have a successful build.

### Clone the rippled repository

If you are familiar with cloning github repositories, just follow your normal process
and clone `git@github.com:ripple/rippled.git`. Otherwise follow this section for instructions.

1. If you don't have a github account, sign up for one at
  [github.com](https://github.com/).
2. Make sure you have Github ssh keys. For help see
  [generating-ssh-keys](https://help.github.com/articles/generating-ssh-keys).

Open the "Git Bash" shell that was installed with "Git for Windows" in the
step above. Navigate to the directory where you want to clone rippled (git
bash uses `/c` for windows's `C:` and forward slash where windows uses
backslash, so `C:\Users\joe\projs` would be `/c/Users/joe/projs` in git bash).
Now clone the repository and optionally switch to the *master* branch.
Type the following at the bash prompt:

```powershell
git clone git@github.com:ripple/rippled.git
cd rippled
git checkout master
```

* If you receive an error about not having the "correct access rights"
  make sure you have Github ssh keys, as described above.

### Configure Library Paths

Open the solution file located at **Builds/Visual Studio 2015/ripple.sln**
and select the "View->Other Windows->Property Manager" to bring up the Property Manager.
Expand the *debug | x64* section (or similar section on 32-windows) and
double click the *Microsoft.Cpp.x64.user* property sheet to bring up the
*Property Pages* dialog (these sections will be called *Win32* instead of
*x64* on 32-bit windows). These are global properties applied to all
64-bit build targets:

![Visual Studio 2015 Global Properties](images/VS2015x64Properties.png)

Go to *C/C++, General, Additional Include Directories* and add the
location of the boost installation:

![Visual Studio 2015 Include Directories](images/VS2015x64IncludeDirs.png)

Then, go to *Linker, General, Additional Library Directories* and add
the location of the compiled boost libraries reported at the completion
of building the boost libraries:

![Visual Studio 2015 Library Directories](images/VS2015x64LibraryDirs.png)

Follow the same procedure for adding the `Additional Include Directories`
and `Additional Library Directories` required for OpenSSL. In our example
these directories are **C:\lib\openssl-VC-64\include** and
**C:\lib\openssl-VC-64\lib** respectively.

# Setup Environment

## Create a working directory for rippled.cfg

The rippled server uses the [Rippled.cfg](https://wiki.ripple.com/Rippled.cfg)
file to read its configuration parameters. This section describes setting up
a directory to hold the config file. The next sections describe how to tell
the rippled server where that file is.

1. Create a directory to hold the configuration file. In this example, the
  ripple config directory was created in `C:\Users\joe\ripple\config`.
2. Copy the example config file located in `doc\rippled-example.cfg` to the
  new directory and rename it "rippled.cfg".
3. Read the rippled.cfg file and edit as appropriate.

## Change the Visual Studio Projects Debugging Properties

1. If not already open, open the solution file located at **Builds/Visual Studio 2015/Ripple.sln**
2. Select the correct solution platform in the solution platform dropdown (either *x64*
  or *Win32* depending on machine type).
3. Select the "Project->Properties" menu item to bring up RippleD's Properties Pages
4. In "Configuration Properties" select "Debugging".
5. In the upper-left Configurations drop down, select "All Configurations".
6. In "Debugger to Launch" select "Local Windows Debugger".

### Tell rippled where to find the configuration file.

The `--conf` command-line switch to tell rippled where to find this file.
In the "Command Arguments" field in the properties dialog (that you opened
in the above section), add: `--conf="C:/Users/joe/ripple/config/rippled.cfg"`
(of course replacing that path with the path you set up above).

![Visual Studio 2013 Command Args Prop Page](images/VSCommandArgsPropPage.png)

### Set the _NO_DEBUG_HEAP Environment Variable

Rippled can run very slowly in the debugger when using the Windows Debug Heap.
Set the `_NO_DEBUG_HEAP` environment variable to one to disable the debug heap.
In the "Environment" field (that you opened in the above section), add:
`_NO_DEBUG_HEAP=1`

![Visual Studio 2013 No Debug Heap Prop Page](images/NoDebugHeapPropPage.png)

# Build

After these steps are complete, rippled should be ready to build. Simply
set rippled as the startup project by right clicking on it in the
Visual Studio Solution Explorer, choose **Set as Startup Project**,
and then choose the **Build->Build Solution** menu item.

# Unit Tests (Recommended)

The external rippled unit tests are written in Javascript using Node.js,
and utilize the mocha unit test framework. To run the unit tests, it
will be necessary to perform the following steps:

## Install Node.js

[Install Node.js](http://nodejs.org/download/). We recommend the Windows
installer (**.msi** file) as it takes care of updating the *PATH* environment
variable so that scripts can find the command. On Windows systems,
**Node.js** comes with **npm**. A separate installation of **npm**
is not necessary.

## Create node_modules

Open a windows console. From the root of your local rippled repository
directory, invoke **npm** to bring in the necessary components:

```
npm install
```

If you get an error that looks like

```
Error: ENOENT, stat 'C:\Users\username\AppData\Roaming\npm'
```

simply create the indicated folder and try again.

## Create a test config.js

From a *bash* shell (installed with Git for Windows), copy the
example configuration file into the appropriate location:

```
cp test/config-example.js test/config.js
```

Edit your version of test/config.js to reflect the correct path to the rippled executable:

```
exports.default_server_config = {
  // Where to find the binary.
  rippled_path: path.resolve(__dirname, "../build/msvc.debug/rippled.exe")
};
```

Also in **test/config.js**, change any occurrences of the
IP address *0.0.0.0* to *127.0.0.1*.

## Run Tests

From a windows console, run the unit tests:

```
npm test
```

Alternatively, run an individual test using mocha:

```
sh
node_modules/mocha/bin/mocha test/account_tx-test.js
```

* NOTE: The version of ripple-lib provided by the npm install
  facility is usually slightly behind the develop branch of the
  authoritative ripple-lib repository. Therefore, some tests might fail.

## Development ripple-lib

To use the latest branch of **ripple-lib** during the unit tests,
first clone the repository in a new location outside of your rippled
repository. Then update the submodules. After, run **npm install**
to set up the **node_modules** directory. Finally, install the
**grunt** command line tools required to run **grunt** and
build **ripple-lib**.

```
git clone git@github.com:ripple/ripple-lib.git
cd ripple-lib
git submodule update --init
npm install
npm install -g grunt-cli
grunt
```

Now link this version of **ripple-lib** into the global packages:

```
sudo npm link
```

To make rippled use the newly linked global **ripple-lib** package
instead of the one installed under **node_modules**, change
directories to the local rippled repository and delete the old
**ripple-lib** then link to the new one:

```
sh
rm -rf node_modules/ripple-lib
npm link ripple-lib
```
