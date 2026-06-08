CC := "gcc"

tests   := "$(find tests -name \"*.c\")"
headers := "$(find include/btrstd -name \"*.h\")"
xargs   := "$(xargs < compile_flags.txt)"

build:
	mkdir -p build
	gcc {{ xargs }} src/_btrstd.c -c -o build/btrstd.a

test: build
	mkdir -p build/tests
	for test in {{ tests }}; do gcc {{ xargs }} -fsanitize=address build/btrstd.a $test -o build/$test; echo "TESTING $test"; build/$test; done
