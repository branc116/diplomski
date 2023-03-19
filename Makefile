.PHONY: all
all: build/src/stm32-bt.elf
	echo "EVERYTHING build"

build/docs/diplomski.pdf: docs/latex/diplomski.tex
	cd docs/latex && pdflatex -mltex -output-directory ../../build/docs diplomski.tex;

build/src/build.ninja: build/src
	cd build/src && STM32_CUBE_F7_PATH=/opt/stm32cubef7 cmake -G Ninja ../../src

build/src/stm32-bt.elf: build/src/build.ninja
	cd build/src && ninja

build/src: build
	mkdir build/src

build:
	mkdir build

.PHONY: clean
clean:
	@rm build/docs/diplomski.pdf 2> /dev/null || echo "Nothing to remove"
	@rm build/src/* -rdf 2> /dev/null || echo "Nothing to remove"
