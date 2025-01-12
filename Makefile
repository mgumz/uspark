
all: uspark-c uspark-rust
clean:
	make -C c-lang clean
	cd rust-lang && cargo clean

uspark-c:
	make -C c-lang

uspark-rust:
	cd rust-lang && cargo build --release

.PHONY: all uspark-c uspark-rust
