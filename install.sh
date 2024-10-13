bin="$1/$2.elf"
echo "Bin: $bin"
probe-rs download --chip STM32F427IIHx "$bin"
