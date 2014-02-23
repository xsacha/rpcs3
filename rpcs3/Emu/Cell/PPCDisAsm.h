#pragma once

#include "Emu/CPU/CPUDisAsm.h"
#include "Gui/DisAsmFrame.h"
#include "Emu/Memory/Memory.h"

class PPCDisAsm : public CPUDisAsm
{
protected:
	PPCDisAsm(CPUDisAsmMode mode) : CPUDisAsm(mode)
	{
	}

	virtual u32 DisAsmBranchTarget(const s32 imm)=0;

	void DisAsm_V4(const wxString& op, u32 v0, u32 v1, u32 v2, u32 v3)
	{
		Write(wxString::Format("%s v%d,v%d,v%d,v%d", FixOp(op).wx_str(), v0, v1, v2, v3));
	}
	void DisAsm_V3_UIMM(const wxString& op, u32 v0, u32 v1, u32 v2, u32 uimm)
	{
		Write(wxString::Format("%s v%d,v%d,v%d,%u #%x", FixOp(op).wx_str(), v0, v1, v2, uimm, uimm));
	}
	void DisAsm_V3(const wxString& op, u32 v0, u32 v1, u32 v2)
	{
		Write(wxString::Format("%s v%d,v%d,v%d", FixOp(op).wx_str(), v0, v1, v2));
	}
	void DisAsm_V2_UIMM(const wxString& op, u32 v0, u32 v1, u32 uimm)
	{
		Write(wxString::Format("%s v%d,v%d,%u #%x", FixOp(op).wx_str(), v0, v1, uimm, uimm));
	}
	void DisAsm_V2(const wxString& op, u32 v0, u32 v1)
	{
		Write(wxString::Format("%s v%d,v%d", FixOp(op).wx_str(), v0, v1));
	}
	void DisAsm_V1_SIMM(const wxString& op, u32 v0, s32 simm)
	{
		Write(wxString::Format("%s v%d,%d #%x", FixOp(op).wx_str(), v0, simm, simm));
	}
	void DisAsm_V1(const wxString& op, u32 v0)
	{
		Write(wxString::Format("%s v%d", FixOp(op).wx_str(), v0));
	}
	void DisAsm_V1_R2(const wxString& op, u32 v0, u32 r1, u32 r2)
	{
		Write(wxString::Format("%s v%d,r%d,r%d", FixOp(op).wx_str(), v0, r1, r2));
	}
	void DisAsm_CR1_F2_RC(const wxString& op, u32 cr0, u32 f0, u32 f1, bool rc)
	{
		Write(wxString::Format("%s%s cr%d,f%d,f%d", FixOp(op).wx_str(), wxString(rc ? "." : "").wx_str(), cr0, f0, f1));
	}
	void DisAsm_CR1_F2(const wxString& op, u32 cr0, u32 f0, u32 f1)
	{
		DisAsm_CR1_F2_RC(op, cr0, f0, f1, false);
	}
	void DisAsm_INT1_R2(const wxString& op, u32 i0, u32 r0, u32 r1)
	{
		Write(wxString::Format("%s %d,r%d,r%d", FixOp(op).wx_str(), i0, r0, r1));
	}
	void DisAsm_INT1_R1_IMM(const wxString& op, u32 i0, u32 r0, s32 imm0)
	{
		Write(wxString::Format("%s %d,r%d,%d #%x", FixOp(op).wx_str(), i0, r0, imm0, imm0));
	}
	void DisAsm_INT1_R1_RC(const wxString& op, u32 i0, u32 r0, bool rc)
	{
		Write(wxString::Format("%s%s %d,r%d", FixOp(op).wx_str(), wxString(rc ? "." : "").wx_str(), i0, r0));
	}
	void DisAsm_INT1_R1(const wxString& op, u32 i0, u32 r0)
	{
		DisAsm_INT1_R1_RC(op, i0, r0, false);
	}
	void DisAsm_F4_RC(const wxString& op, u32 f0, u32 f1, u32 f2, u32 f3, bool rc)
	{
		Write(wxString::Format("%s%s f%d,f%d,f%d,f%d", FixOp(op).wx_str(), wxString(rc ? "." : "").wx_str(), f0, f1, f2, f3));
	}
	void DisAsm_F3_RC(const wxString& op, u32 f0, u32 f1, u32 f2, bool rc)
	{
		Write(wxString::Format("%s%s f%d,f%d,f%d", FixOp(op).wx_str(), wxString(rc ? "." : "").wx_str(), f0, f1, f2));
	}
	void DisAsm_F3(const wxString& op, u32 f0, u32 f1, u32 f2)
	{
		DisAsm_F3_RC(op, f0, f1, f2, false);
	}
	void DisAsm_F2_RC(const wxString& op, u32 f0, u32 f1, bool rc)
	{
		Write(wxString::Format("%s%s f%d,f%d", FixOp(op).wx_str(), wxString(rc ? "." : "").wx_str(), f0, f1));
	}
	void DisAsm_F2(const wxString& op, u32 f0, u32 f1)
	{
		DisAsm_F2_RC(op, f0, f1, false);
	}
	void DisAsm_F1_R2(const wxString& op, u32 f0, u32 r0, u32 r1)
	{
		if(m_mode == CPUDisAsm_CompilerElfMode)
		{
			Write(wxString::Format("%s f%d,r%d,r%d", FixOp(op).wx_str(), f0, r0, r1));
			return;
		}

		Write(wxString::Format("%s f%d,r%d(r%d)", FixOp(op).wx_str(), f0, r0, r1));
	}
	void DisAsm_F1_IMM_R1_RC(const wxString& op, u32 f0, s32 imm0, u32 r0, bool rc)
	{
		if(m_mode == CPUDisAsm_CompilerElfMode)
		{
			Write(wxString::Format("%s%s f%d,r%d,%d #%x", FixOp(op).wx_str(), wxString(rc ? "." : "").wx_str(), f0, r0, imm0, imm0));
			return;
		}

		Write(wxString::Format("%s%s f%d,%d(r%d) #%x", FixOp(op).wx_str(), wxString(rc ? "." : "").wx_str(), f0, imm0, r0, imm0));
	}
	void DisAsm_F1_IMM_R1(const wxString& op, u32 f0, s32 imm0, u32 r0)
	{
		DisAsm_F1_IMM_R1_RC(op, f0, imm0, r0, false);
	}
	void DisAsm_F1_RC(const wxString& op, u32 f0, bool rc)
	{
		Write(wxString::Format("%s%s f%d", FixOp(op).wx_str(), wxString(rc ? "." : "").wx_str(), f0));
	}
	void DisAsm_R1_RC(const wxString& op, u32 r0, bool rc)
	{
		Write(wxString::Format("%s%s r%d", FixOp(op).wx_str(), wxString(rc ? "." : "").wx_str(), r0));
	}
	void DisAsm_R1(const wxString& op, u32 r0)
	{
		DisAsm_R1_RC(op, r0, false);
	}
	void DisAsm_R2_OE_RC(const wxString& op, u32 r0, u32 r1, u32 oe, bool rc)
	{
		Write(wxString::Format("%s%s%s r%d,r%d", FixOp(op).wx_str(), wxString(oe ? "o" : "").wx_str(), wxString(rc ? "." : "").wx_str(), r0, r1));
	}
	void DisAsm_R2_RC(const wxString& op, u32 r0, u32 r1, bool rc)
	{
		DisAsm_R2_OE_RC(op, r0, r1, false, rc);
	}
	void DisAsm_R2(const wxString& op, u32 r0, u32 r1)
	{
		DisAsm_R2_RC(op, r0, r1, false);
	}
	void DisAsm_R3_OE_RC(const wxString& op, u32 r0, u32 r1, u32 r2, u32 oe, bool rc)
	{
		Write(wxString::Format("%s%s%s r%d,r%d,r%d", FixOp(op).wx_str(), wxString(oe ? "o" : "").wx_str(), wxString(rc ? "." : "").wx_str(), r0, r1, r2));
	}
	void DisAsm_R3_INT2_RC(const wxString& op, u32 r0, u32 r1, u32 r2, s32 i0, s32 i1, bool rc)
	{
		Write(wxString::Format("%s%s r%d,r%d,r%d,%d,%d", FixOp(op).wx_str(), wxString(rc ? "." : "").wx_str(), r0, r1, r2, i0, i1));
	}
	void DisAsm_R3_RC(const wxString& op, u32 r0, u32 r1, u32 r2, bool rc)
	{
		DisAsm_R3_OE_RC(op, r0, r1, r2, false, rc);
	}
	void DisAsm_R3(const wxString& op, u32 r0, u32 r1, u32 r2)
	{
		DisAsm_R3_RC(op, r0, r1, r2, false);
	}
	void DisAsm_R2_INT3_RC(const wxString& op, u32 r0, u32 r1, s32 i0, s32 i1, s32 i2, bool rc)
	{
		Write(wxString::Format("%s%s r%d,r%d,%d,%d,%d", FixOp(op).wx_str(), wxString(rc ? "." : "").wx_str(), r0, r1, i0, i1, i2));
	}
	void DisAsm_R2_INT3(const wxString& op, u32 r0, u32 r1, s32 i0, s32 i1, s32 i2)
	{
		DisAsm_R2_INT3_RC(op, r0, r1, i0, i1, i2, false);
	}
	void DisAsm_R2_INT2_RC(const wxString& op, u32 r0, u32 r1, s32 i0, s32 i1, bool rc)
	{
		Write(wxString::Format("%s%s r%d,r%d,%d,%d", FixOp(op).wx_str(), wxString(rc ? "." : "").wx_str(), r0, r1, i0, i1));
	}
	void DisAsm_R2_INT2(const wxString& op, u32 r0, u32 r1, s32 i0, s32 i1)
	{
		DisAsm_R2_INT2_RC(op, r0, r1, i0, i1, false);
	}
	void DisAsm_R2_INT1_RC(const wxString& op, u32 r0, u32 r1, s32 i0, bool rc)
	{
		Write(wxString::Format("%s%s r%d,r%d,%d", FixOp(op).wx_str(), wxString(rc ? "." : "").wx_str(), r0, r1, i0));
	}
	void DisAsm_R2_INT1(const wxString& op, u32 r0, u32 r1, s32 i0)
	{
		DisAsm_R2_INT1_RC(op, r0, r1, i0, false);
	}
	void DisAsm_R2_IMM(const wxString& op, u32 r0, u32 r1, s32 imm0)
	{
		if(m_mode == CPUDisAsm_CompilerElfMode)
		{
			Write(wxString::Format("%s r%d,r%d,%d  #%x", FixOp(op).wx_str(), r0, r1, imm0, imm0));
			return;
		}

		Write(wxString::Format("%s r%d,%d(r%d)  #%x", FixOp(op).wx_str(), r0, imm0, r1, imm0));
	}
	void DisAsm_R1_IMM(const wxString& op, u32 r0, s32 imm0)
	{
		Write(wxString::Format("%s r%d,%d  #%x", FixOp(op).wx_str(), r0, imm0, imm0));
	}
	void DisAsm_IMM_R1(const wxString& op, s32 imm0, u32 r0)
	{
		Write(wxString::Format("%s %d,r%d  #%x", FixOp(op).wx_str(), imm0, r0, imm0));
	}
	void DisAsm_CR1_R1_IMM(const wxString& op, u32 cr0, u32 r0, s32 imm0)
	{
		Write(wxString::Format("%s cr%d,r%d,%d  #%x", FixOp(op).wx_str(), cr0, r0, imm0, imm0));
	}
	void DisAsm_CR1_R2_RC(const wxString& op, u32 cr0, u32 r0, u32 r1, bool rc)
	{
		Write(wxString::Format("%s%s cr%d,r%d,r%d", FixOp(op).wx_str(), wxString(rc ? "." : "").wx_str(), cr0, r0, r1));
	}
	void DisAsm_CR1_R2(const wxString& op, u32 cr0, u32 r0, u32 r1)
	{
		DisAsm_CR1_R2_RC(op, cr0, r0, r1, false);
	}
	void DisAsm_CR2(const wxString& op, u32 cr0, u32 cr1)
	{
		Write(wxString::Format("%s cr%d,cr%d", FixOp(op).wx_str(), cr0, cr1));
	}
	void DisAsm_INT3(const wxString& op, const int i0, const int i1, const int i2)
	{
		Write(wxString::Format("%s %d,%d,%d", FixOp(op).wx_str(), i0, i1, i2));
	}
	void DisAsm_INT1(const wxString& op, const int i0)
	{
		Write(wxString::Format("%s %d", FixOp(op).wx_str(), i0));
	}
	void DisAsm_BRANCH(const wxString& op, const int pc)
	{
		Write(wxString::Format("%s 0x%x", FixOp(op).wx_str(), DisAsmBranchTarget(pc)));
	}
	void DisAsm_BRANCH_A(const wxString& op, const int pc)
	{
		Write(wxString::Format("%s 0x%x", FixOp(op).wx_str(), pc));
	}
	void DisAsm_B2_BRANCH(const wxString& op, u32 b0, u32 b1, const int pc)
	{
		Write(wxString::Format("%s %d,%d,0x%x ", FixOp(op).wx_str(), b0, b1, DisAsmBranchTarget(pc)));
	}
	void DisAsm_CR_BRANCH(const wxString& op, u32 cr, const int pc)
	{
		Write(wxString::Format("%s cr%d,0x%x ", FixOp(op).wx_str(), cr, DisAsmBranchTarget(pc)));
	}
};
