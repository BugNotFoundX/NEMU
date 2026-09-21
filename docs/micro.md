# micro 整数参考模型

本维护分支使用正常 NEMU 组织与执行流程。RV32 没有私有 runtime：

```text
src/cpu/difftest/ref.c → cpu_exec → isa/riscv32/instr + system
                                     → memory/vaddr + paddr → 官方 store queue
```

`configs/riscv{32,64}-micro-ref_defconfig` 选择整数 REF、128 MiB RAM、入口
`0x80000000`、store 比较、REF 状态接口和 micro MMIO。`src/device/micro.c`
只提供 UART/finish 两个写寄存器，使用正常 MMIO map 和 store_commit_queue_push。
RV32 的 `difftest/ref.c` 负责 132 字节 GPR/PC 传输；公共 init_v2 校验大小。

直接构建：

```sh
make NEMU_HOME="$PWD" git_commit= riscv32-micro-ref_defconfig
make NEMU_HOME="$PWD" git_commit= -j4
```

Windrise 的 `make micro-ref PROFILE=rv32im|rv64im` 会同步当前工作源码到独立构建目录，
避免两种 ISA 的配置和对象互相污染，并关闭历史自动提交 hook。

修复包括 RV32 的 JALR bit 0、移位、除零/有符号除法溢出、新版 decode/hostcall 接口；
公共执行和内存路径中真正属于 RV64 的 CSR/向量/AMO/触发器行为按 ISA 区分。
RV32 特权系统尚非完整实现；本轮微核整数测试拒绝 CSR、架构异常和中断。
香山寄存器 checkpoint 格式仍只支持 RV64，RV32 显式拒绝序列化该格式。

维护参考：`git show b4150414`（公共 REF 与 ISA CSR hook）、
`git show 2e69db15`（store 队列封装）。新能力应继续落在相同职责边界中。
