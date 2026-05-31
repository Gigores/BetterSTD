CC := "gcc"

files := "$(find src -name \"*.c\")"
tests := "$(find tests -name \"*.c\")"
xargs := "$(xargs < compile_flags.txt)"

build:
	mkdir -p build
	gcc {{ xargs }} {{ files }} -c -o build/btrstd.a

test: build
	@mkdir -p build/tests
	@for test in {{ tests }}; do gcc {{ xargs }} build/btrstd.a $test -o build/$test; echo "TESTING $test"; build/$test; done
