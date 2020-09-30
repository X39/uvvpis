#include "vpu.h"

#define OP(OUTER_PRE, INNER_PRE, INNER_POST, OUTER_POST, W_OUTER_PRE, W_INNER_PRE, W_INNER_POST, W_OUTER_POST, CF, PF, AF, ZF, SF, OF) \
if (dest_wide)\
{\
    tmp_dest = W_OUTER_PRE##(W_INNER_PRE##dest->uint16##W_INNER_POST)##W_OUTER_POST;\
    if (CF) { m_reg.fr &= ~(unsigned int)reg::flags::cf; }\
    if (PF) { m_reg.fr &= ~(unsigned int)reg::flags::pf; }\
    if (AF) { m_reg.fr &= ~(unsigned int)reg::flags::af; }\
    if (ZF) { m_reg.fr &= ~(unsigned int)reg::flags::zf; }\
    if (SF) { m_reg.fr &= ~(unsigned int)reg::flags::sf; }\
    if (OF) { m_reg.fr &= ~(unsigned int)reg::flags::of; }\
    if (CF && (tmp_dest & 0x0001'0000)) { m_reg.fr |= (unsigned int)reg::flags::cf; }\
    if (PF && ((tmp_dest & 0x0000'FFFF) % 2 == 0)) { m_reg.fr |= (unsigned int)reg::flags::pf; }\
    if (AF && (tmp_dest & 0x0000'0010)) { m_reg.fr |= (unsigned int)reg::flags::af; }\
    if (ZF && ((tmp_dest & 0x0000'FFFF) == 0)) { m_reg.fr |= (unsigned int)reg::flags::zf; }\
    if (SF && (tmp_dest & 0x0000'8000)) { m_reg.fr |= (unsigned int)reg::flags::sf; }\
    if (OF && (tmp_dest & 0x0000'8000 && (!(dest->uint16 & 0x8000) || !(arg.uint16 & 0x8000)))) { m_reg.fr |= (unsigned int)reg::flags::of; }\
    if (OF && (!(tmp_dest & 0x0000'8000) && ((dest->uint16 & 0x8000) || (arg.uint16 & 0x8000)))) { m_reg.fr |= (unsigned int)reg::flags::of; }\
    dest->uint16 = (uint16_t)tmp_dest;\
}\
else\
{\
    tmp_dest = OUTER_PRE##(INNER_PRE##dest->uint8##INNER_POST)##OUTER_POST;\
    if (CF) { m_reg.fr &= ~(unsigned int)reg::flags::cf; }\
    if (PF) { m_reg.fr &= ~(unsigned int)reg::flags::pf; }\
    if (AF) { m_reg.fr &= ~(unsigned int)reg::flags::af; }\
    if (ZF) { m_reg.fr &= ~(unsigned int)reg::flags::zf; }\
    if (SF) { m_reg.fr &= ~(unsigned int)reg::flags::sf; }\
    if (OF) { m_reg.fr &= ~(unsigned int)reg::flags::of; }\
    if (CF && (tmp_dest & 0x0000'0100)) { m_reg.fr |= (unsigned int)reg::flags::cf; }\
    if (PF && ((tmp_dest & 0x0000'00FF) % 2 == 0)) { m_reg.fr |= (unsigned int)reg::flags::pf; }\
    \
    if (ZF && ((tmp_dest & 0x0000'00FF) == 0)) { m_reg.fr |= (unsigned int)reg::flags::zf; }\
    if (SF && (tmp_dest & 0x0000'0080)) { m_reg.fr |= (unsigned int)reg::flags::sf; }\
    if (OF && (tmp_dest & 0x0000'0080 && (!(dest->uint8 & 0x0080) || !(arg.uint8 & 0x0080)))) { m_reg.fr |= (unsigned int)reg::flags::of; }\
    if (OF && (!(tmp_dest & 0x0000'0080) && ((dest->uint8 & 0x0080) || (arg.uint8 & 0x0080)))) { m_reg.fr |= (unsigned int)reg::flags::of; }\
    dest->uint8 = (uint8_t)tmp_dest;\
}

#define OP_BINARY(SIGN, PRE)\
if (arg_wide) { OP(PRE,, SIGN arg.uint16,,PRE,, SIGN arg.uint16,,true,true,true,true,true,true) }\
else { OP(PRE,, SIGN arg.uint8,,PRE,, SIGN arg.uint8,,true,true,true,true,true,true) }
#define OP_BINARY_NO_FLAGS(SIGN, PRE)\
if (arg_wide) { OP(PRE,, SIGN arg.uint16,,PRE,, SIGN arg.uint16,,false,false,false,false,false,false) }\
else { OP(PRE,, SIGN arg.uint8,,PRE,, SIGN arg.uint8,,false,false,false,false,false,false) }
#define OP_BINARY_NO_CF(SIGN, PRE)\
if (arg_wide) { OP(PRE, , SIGN arg.uint16, , PRE, , SIGN arg.uint16, , false,true,true,true,true,true) }\
else { OP(PRE, , SIGN arg.uint8, , PRE, , SIGN arg.uint8, , false,true,true,true,true,true) }

#define OP_BRANCH(PRE, FLAG)\
int16_t val;\
if (dest_wide) { val = dest->int16; }\
else { val = dest->int8; }\
if (PRE(m_reg.fr & reg::flags::FLAG)) {\
    m_reg.pc += val;\
}\

void uvvpis::vpu::execute(instruction inst)
{
    m_reg.pc++;
    auto dest = m_reg.decode(inst.destination);
    uint32_t tmp_dest;
    reg::target arg;
    arg.uint16 = inst.argument;
    if (inst.is_set(modifiers::ARGUMENT, modifiers::ArgumentR08) ||
        inst.is_set(modifiers::ARGUMENT, modifiers::ArgumentR16))
    {
        auto res = m_reg.decode(inst.argument);
        arg = *res;
    }
    bool arg_wide = (
        inst.is_set(modifiers::ARGUMENT, modifiers::ArgumentI16) ||
        inst.is_set(modifiers::ARGUMENT, modifiers::ArgumentR16)
        ) &&
        inst.is_set(modifiers::DESTINATION, modifiers::Destination16);
    bool dest_wide = (
        inst.is_set(modifiers::ARGUMENT, modifiers::ArgumentI16) ||
        inst.is_set(modifiers::ARGUMENT, modifiers::ArgumentR16)
        ) &&
        inst.is_set(modifiers::DESTINATION, modifiers::Destination16);
    switch (inst.op)
    {
    case uvvpis::opcode::NOP: {} break;
    case uvvpis::opcode::HLT: {} break;
    case uvvpis::opcode::EXT: {} break;
    case uvvpis::opcode::ADD: { OP_BINARY(+,); } break;
    case uvvpis::opcode::SUB: { OP_BINARY(-, ); } break;
    case uvvpis::opcode::RSHIFT: { OP_BINARY(>> , ); } break;
    case uvvpis::opcode::LSHIFT: { OP_BINARY(<< , ); } break;
    case uvvpis::opcode::MUL: { OP_BINARY(*,); } break;
    case uvvpis::opcode::DIV: { OP_BINARY(/,); } break;
    case uvvpis::opcode::RMD: { OP_BINARY(%,); } break;
    case uvvpis::opcode::INC: { arg.uint16 = 1; OP_BINARY_NO_CF(+,); } break;
    case uvvpis::opcode::DEC: { arg.uint16 = 1; OP_BINARY_NO_CF(-,); } break;
    case uvvpis::opcode::XOR: { OP_BINARY(^,); } break;
    case uvvpis::opcode::AND: { OP_BINARY(&,); } break;
    case uvvpis::opcode::OR: { OP_BINARY(|,); } break;
    case uvvpis::opcode::NOR: { OP_BINARY(+,~); } break;
    case uvvpis::opcode::NAND: { OP_BINARY(+,~); } break;
    case uvvpis::opcode::XNOR: { OP_BINARY(^,~); } break;
    case uvvpis::opcode::NOT: { arg.uint16 = dest->uint16; OP_BINARY(|, ~); } break;
    case uvvpis::opcode::SET: { OP_BINARY_NO_FLAGS(=,); } break;
    case uvvpis::opcode::LOAD: {
        uint16_t val;
        if (arg_wide) { val = arg.uint16; }
        else { val = arg.uint8; }
        if (dest_wide) { dest->uint16 = ram[val]; }
        else { dest->uint8 = (uint8_t)ram[val]; }
    } break;
    case uvvpis::opcode::STORE: {
        uint16_t val;
        if (dest_wide) { val = dest->uint16; }
        else { val = dest->uint8; }
        if (arg_wide) { ram[val] = arg.uint16; }
        else { ram[val] = arg.uint8; }
    } break;
    case uvvpis::opcode::JUMP: {
        uint16_t val;
        if (dest_wide) { val = dest->uint16; }
        else { val = dest->uint8; }
        m_reg.pc = val;
    } break;
    case uvvpis::opcode::BCY: { OP_BRANCH( , cf); } break;
    case uvvpis::opcode::BNC: { OP_BRANCH(!, cf); } break;
    case uvvpis::opcode::BZR: { OP_BRANCH( , zf); } break;
    case uvvpis::opcode::BNZ: { OP_BRANCH(!, zf); } break;
    case uvvpis::opcode::BNG: { OP_BRANCH( , sf); } break;
    case uvvpis::opcode::BPL: { OP_BRANCH(!, sf); } break;
    default:
        break;
    }
}
