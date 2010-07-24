// Meteor - A Nintendo Gameboy Advance emulator
// Copyright (C) 2009 Philippe Daouadi
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "DisassemblerTreeView.hpp"

#include "ameteor.hpp"
#include "ameteor/disassembler/instruction.hpp"

#include "debug.hpp"

DisassemblerTreeView::DisassemblerTreeView () :
	Gtk::HBox(),
	m_lastPos(0xFFFFFFFF),
	m_numRows(0),
	m_instSize(4)
{
	m_refListStore = Gtk::ListStore::create(m_columns);
	m_treeView.set_model(m_refListStore);
	m_treeView.append_column_numeric("Offset", m_columns.offset, "%08X");
	m_treeView.append_column("Operator", m_columns.op);
	m_treeView.append_column("Arguments", m_columns.args);

	m_treeView.set_headers_visible(true);
	m_treeView.set_rules_hint(true);

	Pango::FontDescription font;
	font.set_size(8 * PANGO_SCALE);
	m_treeView.modify_font(font);

	m_scrollbar.set_increments(1., 16.);

	this->pack_start(m_treeView, true, true);
	this->pack_start(m_scrollbar, false, false);

	m_scrollbar.signal_value_changed().connect(sigc::mem_fun(*this,
				&DisassemblerTreeView::on_scroll));
	m_treeView.signal_scroll_event().connect(sigc::mem_fun(*this,
				&DisassemblerTreeView::on_scroll_event));
	m_treeView.signal_key_press_event().connect(sigc::mem_fun(*this,
				&DisassemblerTreeView::on_key_press), false);
}

void DisassemblerTreeView::SetNumInstr (uint8_t numinstr)
{
	uint32_t maxPos = (m_instSize == 4 ? 0x03FFFFFF : 0x07FFFFFF);

	if (numinstr > m_numRows)
	{
		uint32_t scrollpos = (uint32_t)m_scrollbar.get_value();

		if (m_lastPos == 0xFFFFFFFF) // if the object have been just created
			scrollpos = 0x02000000;

		uint8_t cur = numinstr - m_numRows;

		Gtk::TreeIter iter;
		uint32_t pos;
		// add rows after
		for (pos = scrollpos + m_numRows; cur && pos != maxPos;
				--cur, ++pos)
		{
			iter = m_refListStore->append();
			(*iter)[m_columns.offset] = pos*m_instSize;
			FillRow(*iter);
		}

		// add one last row after if we have to
		if (cur)
		{
			iter = m_refListStore->append();
			(*iter)[m_columns.offset] = pos*m_instSize;
			--cur;

			// add rows before if we have to
			for (pos = scrollpos; cur; --cur, --pos)
			{
				iter = m_refListStore->prepend();
				(*iter)[m_columns.offset] = pos*m_instSize;
				FillRow(*iter);
			}
		}
	}
	else if (numinstr < m_numRows)
	{
		uint8_t diff = m_numRows - numinstr;
		for (uint8_t cur = diff; cur; --cur)
		{
			m_refListStore->erase(
					m_refListStore->get_iter(Gtk::TreePath(1u, numinstr)));
		}
	}

	m_numRows = numinstr;
	m_scrollbar.set_range(0, maxPos + 1 - m_numRows);
	if (m_lastPos == 0xFFFFFFFF) // if the object have been just created
		m_scrollbar.set_value(0x02000000);
}

void DisassemblerTreeView::GoTo (uint32_t to)
{
	m_scrollbar.set_value(to >> (m_instSize == 4 ? 2 : 1));
}

void DisassemblerTreeView::Reload ()
{
	on_scroll();
}

void DisassemblerTreeView::SwitchInstructionSet ()
{
	// change size
	m_instSize = (m_instSize == 4 ? 2 : 4);

	// force reload
	m_lastPos = 0xFFFFFFFF;

	// set range
	if (m_instSize == 2)
		m_scrollbar.set_range(0, 0x07FFFFFF + 1 - m_numRows);

	// scroll to new position
	if (m_instSize == 4)
		m_scrollbar.set_value((int)m_scrollbar.get_value() >> 1);
	else
		m_scrollbar.set_value((int)m_scrollbar.get_value() << 1);

	// set range
	if (m_instSize == 4)
		m_scrollbar.set_range(0, 0x03FFFFFF + 1 - m_numRows);
#if 0
	// clear
	uint8_t numRows = m_numRows;
	m_numRows = 0;
	m_refListStore->clear();
	// reload
	SetNumInstr(numRows);
#endif
}

void DisassemblerTreeView::on_scroll ()
{
	uint32_t scrollpos = (uint32_t)m_scrollbar.get_value();

	/*if (scrollpos == m_lastPos)
		return;*/

	if (scrollpos > m_lastPos && (scrollpos - m_lastPos) < (uint32_t)m_numRows)
	{
		int8_t diff = scrollpos - m_lastPos;
		for (int8_t i = diff; i > 0; --i)
		{
			m_refListStore->erase(m_refListStore->get_iter(Gtk::TreePath(1u, 0)));
		}
		uint32_t pos = scrollpos;
		pos += m_numRows - diff;
		Gtk::TreeIter iter;
		for (int8_t i = diff; i > 0; --i, ++pos)
		{
			iter = m_refListStore->append();
			(*iter)[m_columns.offset] = pos*m_instSize;
			FillRow(*iter);
		}
	}
	else if (scrollpos < m_lastPos &&
			(m_lastPos - scrollpos) < (uint32_t)m_numRows)
	{
		int8_t diff = m_lastPos - scrollpos;
		for (int8_t i = diff; i > 0; --i)
		{
			m_refListStore->erase(
					m_refListStore->get_iter(Gtk::TreePath(1u, m_numRows - diff)));
		}
		uint32_t pos = m_lastPos-1;
		Gtk::TreeIter iter;
		for (int8_t i = diff; i > 0; --i, --pos)
		{
			iter = m_refListStore->prepend();
			(*iter)[m_columns.offset] = pos*m_instSize;
			FillRow(*iter);
		}
	}
	else
	{
		m_refListStore->clear();
		Gtk::TreeIter iter;
		uint32_t pos = scrollpos;
		for (uint8_t i = 0; i < m_numRows; ++i, ++pos)
		{
			iter = m_refListStore->append();
			(*iter)[m_columns.offset] = pos*m_instSize;
			FillRow(*iter);
		}
	}

	m_lastPos = scrollpos;
}

bool DisassemblerTreeView::on_scroll_event (GdkEventScroll* event)
{
	switch (event->direction)
	{
		case GDK_SCROLL_UP:
			m_scrollbar.set_value(m_scrollbar.get_value() - 1);
			break;
		case GDK_SCROLL_DOWN:
			m_scrollbar.set_value(m_scrollbar.get_value() + 1);
			break;
		default :
			break;
	}

	return true;
}

void DisassemblerTreeView::FillRow (const Gtk::TreeRow& row)
{
	uint32_t offset = row.get_value(m_columns.offset);

	if (offset >= 0x08000000 && offset < 0x0E000000 ||
			offset >= 0x02000000 && offset < 0x02040000 ||
			offset >= 0x03000000 && offset < 0x03008000)
	{
		AMeteor::Disassembler::Instruction inst;
		if (m_instSize == 4)
			inst.ParseArm(offset, AMeteor::_memory.Read32(offset));
		else
			inst.ParseThumb(offset, AMeteor::_memory.Read16(offset));
		row[m_columns.op] = inst.GetOperator();
		row[m_columns.args] = inst.GetArguments();
	}
	else if (offset < 0x00004000)
	{
		if (AMeteor::_memory.HasBios())
		{
			AMeteor::Disassembler::Instruction inst;
			if (m_instSize == 4)
				inst.ParseArm(offset, AMeteor::_memory.Read32(offset));
			else
				inst.ParseThumb(offset, AMeteor::_memory.Read16(offset));
			row[m_columns.op] = inst.GetOperator();
			row[m_columns.args] = inst.GetArguments();
		}
		else
			row[m_columns.op] = "BIOS memory";
	}
	else if (offset >= 0x04000000 && offset < 0x04000400)
		row[m_columns.op] = "I/O registers";
	else if (offset >= 0x05000000 && offset < 0x08000000)
		row[m_columns.op] = "Display memory";
	else
		row[m_columns.op] = "Bad address";
}

bool DisassemblerTreeView::on_key_press (GdkEventKey* event)
{
	switch (event->keyval)
	{
		case GDK_Up:
			m_scrollbar.set_value(m_scrollbar.get_value() - 1);
			break;
		case GDK_Down:
			m_scrollbar.set_value(m_scrollbar.get_value() + 1);
			break;
		case GDK_Page_Up:
			m_scrollbar.set_value(m_scrollbar.get_value() - m_numRows);
			break;
		case GDK_Page_Down:
			m_scrollbar.set_value(m_scrollbar.get_value() + m_numRows);
			break;
		default:
			return false;
	}
	return true;
}
