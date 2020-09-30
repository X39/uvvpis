#pragma once
#include <string>
#include <string_view>
#include <sstream>
#include <iomanip>

namespace uvvpis
{
	enum class opcode
	{
		NOP,
		HLT,
		EXT,
		ADD,
		SUB,
		RSHIFT,
		LSHIFT,
		MUL,
		DIV,
		RMD,
		INC,
		DEC,
		XOR,
		AND,
		OR,
		NOR,
		NAND,
		XNOR,
		NOT,
		SET,
		LOAD,
		STORE,
		JUMP,
		BCY,
		BNC,
		BZR,
		BNZ,
		BNG,
		BPL
	};
	enum class modifiers
	{
		EMPTY = 0,

		Destination08 = 0b0000,
		Destination16 = 0b0001,
		DESTINATION   = 0b0001,

		ArgumentR08   = 0b0000,
		ArgumentR16   = 0b0010,
		ArgumentI08   = 0b0100,
		ArgumentI16   = 0b0110,
		ARGUMENT      = 0b0110,

		UseCarryFlag  = 0b1000,
	};

	static inline modifiers operator|(const modifiers l, const modifiers r) { return (modifiers)((unsigned int)l | (unsigned int)r); }
	static inline modifiers operator&(const modifiers l, const modifiers r) { return (modifiers)((unsigned int)l & (unsigned int)r); }
	struct instruction
	{
		opcode op : 8;
		modifiers modifier : 4;
		unsigned int destination : 4;
		unsigned int argument : 16;

		instruction(enum opcode opcode, modifiers modifier, unsigned int destination, unsigned int argument) :
			op(opcode),
			modifier(modifier),
			destination(destination),
			argument(argument)
		{}

		instruction(enum opcode opcode, modifiers modifier, unsigned int destination) :
			op(opcode),
			modifier(modifier),
			destination(destination),
			argument(0)
		{}
		instruction(enum opcode opcode) :
			op(opcode),
			modifier(modifiers::EMPTY),
			destination(0),
			argument(0)
		{}
		instruction() :
			op(opcode::NOP),
			modifier(modifiers::EMPTY),
			destination(0),
			argument(0)
		{}

		bool is_set(modifiers mask, modifiers mod)
		{
			return ((unsigned int)modifier & (unsigned int)mask) == (unsigned int)mod;
		}
	};

	inline std::string_view to_string_view(const opcode& op)
	{
		using namespace std::string_view_literals;
		switch (op)
		{
		case opcode::NOP: return "NOP"sv;
		case opcode::HLT: return "HLT"sv;
		case opcode::EXT: return "EXT"sv;
		case opcode::ADD: return "ADD"sv;
		case opcode::SUB: return "SUB"sv;
		case opcode::RSHIFT: return "RSHIFT"sv;
		case opcode::LSHIFT: return "LSHIFT"sv;
		case opcode::MUL: return "MUL"sv;
		case opcode::DIV: return "DIV"sv;
		case opcode::RMD: return "RMD"sv;
		case opcode::INC: return "INC"sv;
		case opcode::DEC: return "DEC"sv;
		case opcode::XOR: return "XOR"sv;
		case opcode::AND: return "AND"sv;
		case opcode::OR: return "OR"sv;
		case opcode::NOR: return "NOR"sv;
		case opcode::NAND: return "NAND"sv;
		case opcode::XNOR: return "XNOR"sv;
		case opcode::NOT: return "NOT"sv;
		case opcode::SET: return "SET"sv;
		case opcode::LOAD: return "LOAD"sv;
		case opcode::STORE: return "STORE"sv;
		case opcode::JUMP: return "JUMP"sv;
		case opcode::BCY: return "BCY"sv;
		case opcode::BNC: return "BNC"sv;
		case opcode::BZR: return "BZR"sv;
		case opcode::BNZ: return "BNZ"sv;
		case opcode::BNG: return "BNG"sv;
		case opcode::BPL: return "BPL"sv;
		default: "UNKNOWN"sv;
		}
	}
	inline std::string to_string(const opcode& op) { return std::string(to_string_view(op)); }
	inline std::string to_string(const instruction& inst)
	{
		std::stringstream sstream;
		sstream <<
			to_string_view(inst.op) << std::string(6 - to_string_view(inst.op).length() > 0 ? 6 - to_string_view(inst.op).length() : 0, ' ')  << " " <<
			(((inst.modifier & modifiers::ARGUMENT) == modifiers::ArgumentI08) ? " i8" :
				((inst.modifier & modifiers::ARGUMENT) == modifiers::ArgumentI16) ? "i16" :
				((inst.modifier & modifiers::ARGUMENT) == modifiers::ArgumentR08) ? " r8" : "r16") << "," <<
			(((inst.modifier & modifiers::DESTINATION) == modifiers::Destination08) ? " d8" : "d16") << " " <<
			"0x" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << inst.destination << " "
			"0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << inst.argument;
		
		return sstream.str();
	}
}