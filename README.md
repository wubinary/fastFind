# FastFind (ff)

FastFind (ff) is a fast file pattern matching executer for linux.

# Install
```cmd
# build ff && unitest ff
> git clone https://github.com/wubinary/fastFind.git && cd fastFind
> ./build.sh Release && ./run.sh

# run ff
> export PATH=~/.local/bin/:$PATH
> ff <Pattern> <RootPath>
```

# Usage
```cmd
NAME
    ff - fast file name pattern matching.

USAGE
    ff <Pattern> <RootPath>

OPTIONS
    -p, --pattern, $0
        File pattern that you are searching for.

    -r, --root, $1
        Root directory that file matching searched.

    -v, --version
        Display version information and exit.  No other output is generated.

    -h, --help
        Display help text and exit. No other output generated.
```

# Demo
> ff usage
<img src="docs/ff_demo.gif?raw=true" width="1200px">

> find ~ -name "\*mai\*" V.S. ff mai ~
<img src="docs/compare_demo.gif?raw=true" width="1200px">
