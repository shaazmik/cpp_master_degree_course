echo "--- BUILDING HEADERS VERSION ---"

echo "Compiling Rational.o..."
/usr/bin/time -p clang++ -std=c++2b -c Rational.cpp -o Rational.o

echo "Compiling main.o..."
/usr/bin/time -p clang++ -std=c++2b -c main.cpp -o main.o

echo "Linking..."
/usr/bin/time -p clang++ Rational.o main.o -o app_headers

ls -lh Rational.o main.o app_headers