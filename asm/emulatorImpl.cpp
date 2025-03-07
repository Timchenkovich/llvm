#include <emulator.h>
extern "C" {
#include <sim.h>
};
namespace asmEmulator {
std::array<int64_t, 32> REGS = {0};

void add_(size_t resIdx, int64_t op1, int64_t op2) { REGS[resIdx] = op1 + op2; }

void sub_(size_t resIdx, int64_t op1, int64_t op2) { REGS[resIdx] = op1 - op2; }

void shl_(size_t op0, size_t op1, int64_t op2) { REGS[op0] = REGS[op1] << op2; }

void cmpgt_(size_t resIdx, int64_t op1, int64_t op2) {
  REGS[resIdx] = static_cast<uint8_t>(op1 > op2);
}

void cmpgte_(size_t resIdx, int64_t op1, int64_t op2) {
  REGS[resIdx] = static_cast<uint8_t>(op1 >= op2);
}

void cmplt_(size_t resIdx, int64_t op1, int64_t op2) {
  REGS[resIdx] = static_cast<uint8_t>(op1 < op2);
}

void cmpneq_(size_t resIdx, int64_t op1, int64_t op2) {
  REGS[resIdx] = static_cast<uint8_t>(op1 != op2);
}

void or_(size_t resIdx, int64_t op1, int64_t op2) { REGS[resIdx] = op1 | op2; }

void and_(size_t resIdx, int64_t op1, int64_t op2) { REGS[resIdx] = op1 & op2; }

void slctev_(size_t resIdx, size_t srcIdx, int64_t op1, int64_t op2) {
  REGS[resIdx] = (REGS[srcIdx] & 1) == 0 ? op1 : op2;
}

void mul_(size_t resIdx, int64_t op1, int64_t op2) { REGS[resIdx] = op1 * op2; }

void muladd_(size_t resIdx, size_t reg0, int64_t op1, int64_t op2) {
  REGS[resIdx] = REGS[reg0] * op1 + op2;
}

void mov_(size_t resIdx, int64_t op1) { REGS[resIdx] = op1; }

void select_(size_t resIdx, size_t reg0, int64_t op1, int64_t op2) {
  REGS[resIdx] = REGS[reg0] ? op1 : op2;
}

void flush_() { simFlush(); }

void rnd_(size_t resIdx) { REGS[resIdx] = simRand(); }

void rndrng_(size_t resIdx, int64_t rem) { REGS[resIdx] = simRand() % rem; }

void putpxl_(int64_t op0, int64_t op1, int64_t op2) {
  simPutPixel(op0, op1, op2);
}
}  // namespace asmEmulator