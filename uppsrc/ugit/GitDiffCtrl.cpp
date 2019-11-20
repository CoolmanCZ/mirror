#include "GitDiffCtrl.h"

using namespace Upp;

GitDiffCtrl::GitDiffCtrl()
{
	SetFrame(ViewFrame());

	Add(hidelinenumber.SetLabel("Hide line number").RightPosZ(80, 100).BottomPosZ(0, 16));
	Add(onepanel.SetLabel("One panel").RightPosZ(0, 70).BottomPosZ(0, 16));
	Add(diffone.SizePos().HSizePosZ(0, 0).VSizePosZ(0, 18));

	diffone.Add(single);
	diffone.Add(difftwo);

	difftwo.Horz(left,right);

	single.AutoHideSb();
	single.NoGutter();
	single.NoChangePaperColor();
	single.WhenLeftDouble = THISBACK1(GetFilePos, PANEL_ONE);
	single.WhenHighlight = THISBACK(DiffHighlight);

	left.NoGutter();
	left.HideSb();
//	left.WhenLeftDouble = THISBACK1(GetFilePos, PANEL_LEFT);
	left.WhenHighlight = THISBACK(DiffHighlight);
	left.WhenScroll = right.ScrollWhen(left);

	right.ShowSb(true);
	right.WhenLeftDouble = THISBACK1(GetFilePos, PANEL_RIGHT);
	right.WhenHighlight = THISBACK(DiffHighlight);
	right.WhenScroll = left.ScrollWhen(right);

	hidelinenumber.WhenAction = THISBACK(ToggleLineNumber);
	hidelinenumber = 0;

	onepanel.WhenAction = THISBACK(TogglePanel);
	onepanel = 0;

	diffone.Zoom(0);

	Clear();

	TogglePanel();
}

int GitDiffCtrl::GetNumber(const String& text, const String& sign)
{
	int x = text.FindFirstOf(sign) + 1;
	int y = text.FindFirstOf(",", x);
	return (ScanInt(text.Mid(x, y-x)));
}

void GitDiffCtrl::GetFilePos(int number, int line, int panel)
{
	int filepos = 0;
	String filepath;

	for (int i = line; i >= 0; --i) {
		String text;
		switch (panel) {
			case PANEL_ONE:	text = single.GetText(i);
				if (filepos == 0) filepos = single.GetNumberDiff(line);
				break;
			case PANEL_LEFT: text = left.GetText(i);
				if (filepos == 0) filepos = left.GetNumberDiff(line);
				break;
			case PANEL_RIGHT: text = right.GetText(i);
				if (filepos == 0) filepos = right.GetNumberDiff(line);
				break;
		}
		if (text.Find(DIFF_FILE_START) == 0) {
			int pos = text.ReverseFind(" ");
			filepath = TrimBoth(text.Mid(pos));
			if (filepath.Find("b/") == 0)
				filepath.Remove(0, 2);
			break;
		}
	}
	WhenLeftDouble(filepath, filepos - 1);
}

void GitDiffCtrl::DiffHighlight(Vector<LineEdit::Highlight>& hl, const WString& ln)
{
	if (ln.GetCount() <= 0)
		return;

	Color bg = SColorPaper();
	Color fg = SColorText();

	switch (ln[0]) {
		case '@': fg = Blue(); bg = LtGray();
			break;
		case '-': fg = Red();
			break;
		case '+': fg = Green();
			break;
		default: fg = Gray();
			break;
	}

	if (ln.Find(DIFF_FILE_START) == 0) {
		bg = LtGray();
		fg = Black();
	}

	for (int i = 0; i < ln.GetCount(); ++i) {
		hl[i].ink = fg;
		hl[i].paper = bg;
	}
}

static bool SmallDiff(const char *s1, const char *s2)
{
	for (;;) {
		while(*s1 && (byte)*s1 <= ' ')
			s1++; // NOLINT: C code
		while(*s2 && (byte)*s2 <= ' ')
			s2++; // NOLINT: C code
		if(!*s1 || !*s2)
			return !*s1 && !*s2;
		if(*s1 != *s2)
			return false;
		while(*s1 && *s1 == *s2)
			s1++, s2++; // NOLINT: C code
		if((byte)s1[-1] <= ' ') // NOLINT: C code
			continue;
		if((byte)*s1 > ' ' || (byte)*s2 > ' ')
			return false;
	}
}

void GitDiffCtrl::SetTwo(Vector<String>& ll, Vector<String>& rl)
{
	Array<TextSection> sections = CompareLineMaps(ll, rl);

	left.SetLeft();
	right.SetDiffBgColor(GreenDiffBg());
	right.SetDiffBgColorBold(GreenDiffBgBold());

	int outln = left.GetCount();
	int firstdiff = -1;
	int leftnumber = 0, rightnumber = 0;
	for(int i = 0; i < sections.GetCount(); i++) {
		const TextSection& sec = sections[i];
		bool diff = !sec.same;
		if(firstdiff < 0 && diff)
			firstdiff = outln;
		int maxcount = max(sec.count1, sec.count2);
		left.AddCount(maxcount);
		int l = 0;
		for(l = 0; l < sec.count1; l++) {
			bool shownull = false;
			if (ll[sec.start1 + l].Find(DIFF_FILE_START) == 0)
				shownull = true;
			if (ll[sec.start1 + l].Left(4).Compare("--- ") == 0 || ll[sec.start1 + l].Left(4).Compare("+++ ") == 0)
				shownull = true;
			if (ll[sec.start1 + l].Left(3).Compare("@@ ") == 0 || ll[sec.start1 + l].Left(4).Compare("@@@ ") == 0) {
				shownull = true;
				leftnumber = GetNumber(ll[sec.start1 + l], "-");
			}
			int level = (diff ? l < sec.count2 && SmallDiff(ll[sec.start1 + l], rl[sec.start2 + l]) ? 1 : 2 : 0);
			left.Set(outln + l, ll[sec.start1 + l], diff, shownull ? Null : outln + l + 1, level, diff && l < sec.count2 ? rl[sec.start2 + l] : Null, shownull ? Null : leftnumber++);
		}
		for(; l < maxcount; l++)
			left.Set(outln + l, Null, diff, Null, 2, Null, Null);
		right.AddCount(maxcount);
		for(l = 0; l < sec.count2; l++) {
			bool shownull = false;
			if (rl[sec.start2 + l].Find(DIFF_FILE_START) == 0)
				shownull = true;
			if (rl[sec.start2 + l].Left(4).Compare("--- ") == 0 || rl[sec.start2 + l].Left(4).Compare("+++ ") == 0)
				shownull = true;
			if (rl[sec.start2 + l].Left(3).Compare("@@ ") == 0 || rl[sec.start2 + l].Left(4).Compare("@@@ ") == 0) {
				shownull = true;
				rightnumber = GetNumber(rl[sec.start2 + l], "+");
			}
			int level = (diff ? l < sec.count1 && SmallDiff(rl[sec.start2 + l], ll[sec.start1 + l]) ? 1 : 2 : 0);
			right.Set(outln + l, rl[sec.start2 + l], diff, shownull ? Null : outln + l + 1, level, diff && l < sec.count1 ? ll[sec.start1 + l] : Null, shownull ? Null : rightnumber++);
		}
		for(; l < maxcount; l++)
			right.Set(outln + l, Null, diff, Null, 2, Null, Null);
		outln += maxcount;
	}
	left.ClearSelection();
	right.ClearSelection();
}

void GitDiffCtrl::SetOne(Vector<String>& sl)
{
	int number = 0;

	single.SetLeft();
	for (int i = 0; i < sl.GetCount(); ++i) {
		bool shownull = false;
		bool deleted = false;
		if (sl[i].Find(DIFF_FILE_START) == 0)
			shownull = true;

		if (sl[i].Left(4).Compare("--- ") == 0 || sl[i].Left(4).Compare("+++ ") == 0)
			shownull = true;

		if (sl[i].Left(3).Compare("@@ ") == 0 || sl[i].Left(4).Compare("@@@ ") == 0) {
			shownull = true;
			number = GetNumber(sl[i], "+") - 1;
		}

		if (sl[i][0] == '-') {
			deleted = true;
			--number;
		}

		single.Set(i, sl[i], false, shownull ? Null : i, Null, Null, shownull ? Null : deleted ? -1 : number);
		++number;
	}
	single.SetCount(sl.GetCount());
	single.ClearSelection();
}

void GitDiffCtrl::Clear()
{
	llines.Clear();
	rlines.Clear();
	slines.Clear();
	left.SetCount(0);
	right.SetCount(0);
	single.SetCount(0);

	hidelinenumber.Disable();
	onepanel.Disable();
}

void GitDiffCtrl::Set(Vector<String>& lines)
{
	Clear();

	if (lines.IsEmpty())
		return;

	hidelinenumber.Enable();
	onepanel.Enable();

	Progress pi("Parsing diff output");
	int limit = min(lines.GetCount(), 512000);
	pi.Set(0, limit);
	bool insertlines = false;
	bool diffstarted = false;
	for (int i = 0; i < limit; ++i) {
		pi.Step();
		if (lines[i].Find(DIFF_FILE_START) == 0
		 || lines[i].Left(4).Compare("--- ") == 0
		 || lines[i].Left(4).Compare("+++ ") == 0) {
			slines.Add(lines[i]);
			llines.Add(lines[i]);
			rlines.Add(lines[i]);
			diffstarted = true;
			continue;
		}
		
		if (lines[i].Left(3).Compare("@@ ") == 0
		 || lines[i].Left(4).Compare("@@@ ") == 0) {
			slines.Add(lines[i]);
			llines.Add(lines[i]);
			rlines.Add(lines[i]);
			insertlines = true;
			continue;
		}

		if (insertlines && lines[i][0] == '-') {
			slines.Add(lines[i]);
			llines.Add(lines[i].Mid(1));
			continue;
		}

		if (insertlines && lines[i][0] == '+') {
			slines.Add(lines[i]);
			rlines.Add(lines[i].Mid(1));
			continue;
		}

		if (insertlines && lines[i][0] == ' ') {
			slines.Add(lines[i]);
			llines.Add(lines[i].Mid(1));
			rlines.Add(lines[i].Mid(1));
			continue;
		}

		if (diffstarted && insertlines) {
			SetTwo(llines, rlines);
			llines.Clear();
			rlines.Clear();
			diffstarted = false;
		}

		insertlines = false;
	}
	SetTwo(llines, rlines);
	pi.Step();
	SetOne(slines);
	pi.Close();
}

void GitDiffCtrl::SetLinePos(const String& text)
{
	for (int i = 0; i < single.GetCount(); ++i) {
		if ((single.GetText(i).Find(DIFF_FILE_START) == 0) && (single.GetText(i).Find(text) > 0)) {
			single.SetSb(i);
			break;
		}
	}
	for (int i = 0; i < right.GetCount(); ++i) {
		if ((right.GetText(i).Find(DIFF_FILE_START) == 0) && (right.GetText(i).Find(text) > 0)) {
			right.SetSb(i);
			break;
		}
	}
}

void GitDiffCtrl::TogglePanel()
{
	if (~onepanel == 1) {
		diffone.Zoom(0);
		single.SetSelection(0, 0);
		single.SetSb(0);
	} else {
		diffone.Zoom(1);
		right.SetSelection(0, 0);
		right.SetSb(0);
	}
}

void GitDiffCtrl::ToggleLineNumber()
{
	if (~hidelinenumber == 1) {
		single.HideLineNumber();
		left.HideLineNumber();
		right.HideLineNumber();
	} else {
		single.ShowLineNumber(true);
		left.ShowLineNumber(true);
		right.ShowLineNumber(true);
	}
}

// vim: ts=4
