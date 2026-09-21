#include <device/map.h>
#include <memory/host.h>
#include <memory/store_queue_wrapper.h>
#include <isa.h>
static uint8_t *registers;
static void micro_io_handler(uint32_t offset, int len, bool is_write) {
  Assert(is_write && ((offset == 0 && len == 1) || (offset == 4 && len == 4)),
         "Invalid micro MMIO offset=%u len=%d write=%d", offset, len, is_write);
  store_commit_queue_push(0x10000000 + offset, host_read(registers + offset, len), len, 0);
}
void init_micro(void) {
  if (!registers) {
    registers = new_space(8);
    add_mmio_map_with_diff("micro", 0x10000000, registers, 8, SKIP_FREE, micro_io_handler);
  }
  memset(registers, 0, 8);
  store_queue_reset();
}
