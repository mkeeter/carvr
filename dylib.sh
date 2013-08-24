#!/bin/sh
mkdir -p $1.app/Contents/Frameworks
for lib in `otool -L $1.app/Contents/MacOS/carvr|
            sed -n -E "s:.*/usr/local/lib/([^ ]*).*:\1:p"`
do
    rm -f $1.app/Contents/Frameworks/$lib
    cp /usr/local/lib/$lib $1.app/Contents/Frameworks
    install_name_tool -change /usr/local/lib/$lib \
            @executable_path/../Frameworks/$lib   \
            $1.app/Contents/MacOS/$1
done
