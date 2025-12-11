echo "--- BUILDING MODULES VERSION ---"

echo "Compiling Math-Rational interface..."
/usr/bin/time -p clang++ -std=c++20 -c Math-Rational.cppm -o Math-Rational.o \
    -fmodule-file=std=std.pcm --precompile -o Math-Rational.pcm

echo "Compiling Math interface..."
/usr/bin/time -p clang++ -std=c++20 -c Math.cppm -o Math.o \
    -fmodule-file=std=std.pcm -fmodule-file=Math:Rational=Math-Rational.pcm \
    --precompile -o Math.pcm

echo "Compiling implementation..."
/usr/bin/time -p clang++ -std=c++20 -c Math.cpp -o Math-impl.o \
    -fmodule-file=std=std.pcm -fmodule-file=Math=Math.pcm -fmodule-file=Math:Rational=Math-Rational.pcm

echo "Compiling main.o..."
/usr/bin/time -p clang++ -std=c++20 -c main.cpp -o main_mod.o \
    -fmodule-file=std=std.pcm -fmodule-file=Math=Math.pcm

echo "Linking..."
/usr/bin/time -p clang++ Math-Rational.o Math.o Math-impl.o main_mod.o -o app_modules

ls -lh *.o *.pcm app_modules