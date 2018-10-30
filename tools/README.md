##  Environment Setup

Open the appropriate *Command Prompt* from the *Start* menu.

For example *VS2015 x64 Native Tools Command Prompt*:

```
C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC>
```

Change to your working directory:

```
> cd /to/this/dir
```

If *cmake* command is not available from *Command Prompt*, add it to system *PATH* variable:

```
> set PATH=%PATH%;C:\Program Files (x86)\CMake\bin
```

If *git* command is not available from *Command Prompt*, add it to system *PATH* variable:

```
> set PATH=%PATH%;C:\Program Files\Git\cmd
```

Good. Now you are ready to continue.

##  Build protobuf

```
> build.bat
```
