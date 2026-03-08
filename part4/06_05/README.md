# Сборка:

```
cd part4/06_05 && mkdir -p output && cd output
cmake -DCMAKE_CXX_COMPILER=g++ .. && cmake --build .
```
# Запуск:

```
cd output
echo ./libshared_v1.so | ./06_05   # → library_v1::test
echo ./libshared_v2.so | ./06_05   # → library_v2::test
```