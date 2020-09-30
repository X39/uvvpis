#pragma once
#include "../core/instruction.h"

#include <vector>
#include <sstream>

namespace uvvpis
{

	class vpu
	{
	public:
#pragma pack(push, 1)
		struct reg
		{
			enum flags
			{
				cf = 0b0000'0000'0000'0001,
				pf = 0b0000'0000'0000'0010,
				af = 0b0000'0000'0000'0100,
				zf = 0b0000'0000'0000'1000,
				sf = 0b0000'0000'0001'0000,
				of = 0b0000'0000'0010'0000
			};
			union target
			{
				uint8_t uint8;
				uint16_t uint16;
				int8_t int8;
				int16_t int16;
			};
		private:
			uint16_t START;
		public:
			uint16_t pc;
			uint16_t fr;
			uint8_t r0;
			uint8_t r1;
			uint8_t r2;
			uint8_t r3;
			uint8_t r4;
			uint8_t r5;
			uint8_t r6;
			uint8_t r7;

			reg() :
				START(0),
				pc(0),
				fr(0),
				r0(0),
				r1(0),
				r2(0),
				r3(0),
				r4(0),
				r5(0),
				r6(0),
				r7(0),
				END(0)
			{}
		private:
			uint16_t END;
		public:
			target* decode(uint8_t address)
			{
				switch (address)
				{
				default:			return reinterpret_cast<target*>(&START);
				case 0x0: END = 0;	return reinterpret_cast<target*>(&END);
				case 0x1:			return reinterpret_cast<target*>(&pc);
				case 0x2:			return reinterpret_cast<target*>(&fr);
				case 0x3:			return reinterpret_cast<target*>(&r0);
				case 0x4:			return reinterpret_cast<target*>(&r1);
				case 0x5:			return reinterpret_cast<target*>(&r2);
				case 0x6:			return reinterpret_cast<target*>(&r3);
				case 0x7:			return reinterpret_cast<target*>(&r4);
				case 0x8:			return reinterpret_cast<target*>(&r5);
				case 0x9:			return reinterpret_cast<target*>(&r6);
				case 0xA:			return reinterpret_cast<target*>(&r7);
				}
			}
		};
#pragma pack(pop)
	private:
		static constexpr size_t pow_2_16 = 65536;
		reg m_reg;
		std::vector<uint16_t> ram;

		static void print_registers_binary(std::stringstream& sstream, uint16_t val)
		{
			sstream << "0b" <<
				((val & 0b1000'0000'0000'0000) ? '1' : '0') <<
				((val & 0b0100'0000'0000'0000) ? '1' : '0') <<
				((val & 0b0010'0000'0000'0000) ? '1' : '0') <<
				((val & 0b0001'0000'0000'0000) ? '1' : '0') <<
				'_' <<
				((val & 0b0000'1000'0000'0000) ? '1' : '0') <<
				((val & 0b0000'0100'0000'0000) ? '1' : '0') <<
				((val & 0b0000'0010'0000'0000) ? '1' : '0') <<
				((val & 0b0000'0001'0000'0000) ? '1' : '0') <<
				'_' <<
				((val & 0b0000'0000'1000'0000) ? '1' : '0') <<
				((val & 0b0000'0000'0100'0000) ? '1' : '0') <<
				((val & 0b0000'0000'0010'0000) ? '1' : '0') <<
				((val & 0b0000'0000'0001'0000) ? '1' : '0') <<
				'_' <<
				((val & 0b0000'0000'0000'1000) ? '1' : '0') <<
				((val & 0b0000'0000'0000'0100) ? '1' : '0') <<
				((val & 0b0000'0000'0000'0010) ? '1' : '0') <<
				((val & 0b0000'0000'0000'0001) ? '1' : '0');
		}
		static void print_registers_binary(std::stringstream& sstream, uint8_t val)
		{
			sstream << "0b" <<
				((val & 0b1000'0000) ? '1' : '0') <<
				((val & 0b0100'0000) ? '1' : '0') <<
				((val & 0b0010'0000) ? '1' : '0') <<
				((val & 0b0001'0000) ? '1' : '0') <<
				'_' <<
				((val & 0b0000'1000) ? '1' : '0') <<
				((val & 0b0000'0100) ? '1' : '0') <<
				((val & 0b0000'0010) ? '1' : '0') <<
				((val & 0b0000'0001) ? '1' : '0');
		}
	public:
		vpu() : ram(pow_2_16), m_reg() {}
		void execute(instruction inst);
		const reg registers() const { return m_reg; }

		void print_registers(std::stringstream& sstream, const char* lineprefix = "") const
		{
			sstream << lineprefix << "PC: "; print_registers_binary(sstream, m_reg.pc); sstream << " " << (uint16_t)m_reg.pc << '\n';
			sstream << lineprefix << "FR: "; print_registers_binary(sstream, m_reg.fr); sstream << " " << (uint16_t)m_reg.fr << '\n';
			sstream << lineprefix << "R0: "; print_registers_binary(sstream, m_reg.r0); sstream << " " << (uint16_t)m_reg.r0 << '\n';
			sstream << lineprefix << "R1: "; print_registers_binary(sstream, m_reg.r1); sstream << " " << (uint16_t)m_reg.r1 << '\n';
			sstream << lineprefix << "R2: "; print_registers_binary(sstream, m_reg.r2); sstream << " " << (uint16_t)m_reg.r2 << '\n';
			sstream << lineprefix << "R3: "; print_registers_binary(sstream, m_reg.r3); sstream << " " << (uint16_t)m_reg.r3 << '\n';
			sstream << lineprefix << "R4: "; print_registers_binary(sstream, m_reg.r4); sstream << " " << (uint16_t)m_reg.r4 << '\n';
			sstream << lineprefix << "R5: "; print_registers_binary(sstream, m_reg.r5); sstream << " " << (uint16_t)m_reg.r5 << '\n';
			sstream << lineprefix << "R6: "; print_registers_binary(sstream, m_reg.r6); sstream << " " << (uint16_t)m_reg.r6 << '\n';
			sstream << lineprefix << "R7: "; print_registers_binary(sstream, m_reg.r7); sstream << " " << (uint16_t)m_reg.r7;
		}
	};
}