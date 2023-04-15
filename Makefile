.PHONY: all
all: compile_commands.json build/src/stm32-bt.elf
	echo "EVERYTHING build" && \
	arm-none-eabi-objcopy -O binary build/src/stm32-bt.elf build/src/stm32-bt.bin

build/docs/diplomski.pdf: docs/diplomski.tex build/docs
	cd docs && pdflatex -mltex -output-directory ../build/docs diplomski.tex;

build/src/stm32-bt.elf: build/src/build.ninja
	cd build/src && ninja

build/src/build.ninja: build/src
	cd build/src && STM32_CUBE_L4_PATH=/opt/STM32CubeL4 cmake -DCMAKE_BUILD_TYPE=Debug -G Ninja ../../src

compile_commands.json: build/src/build.ninja build/src/compile_commands.json
	cp build/src/compile_commands.json .

build/src: build
	mkdir build/src

build/docs: build
	test -d build/docs || mkdir build/docs

build:
	mkdir build

/usr/share/texmf/tex/enumitem:
	sudo pacman -S latex-enumitem

.PHONY: clean
clean:
	@rm build/docs/diplomski.pdf 2> /dev/null || echo "Nothing to remove"
	@rm build/src/* -rdf 2> /dev/null || echo "Nothing to remove"

.PHONY: deps
deps:
	@yay -S arm-none-eabi-gcc arm-none-eabi-gdb arm-none-eabi-newlib arm-none-eabi-binutils openocd ninja
	cd /opt && sudo git https://github.com/STMicroelectronics/STM32CubeL4.git --depth 1

.PHONY: doc-deps
doc-deps:
	@yay -S latex latex-enumitem texlive-core

