#include "GitFileHistory.h"

using namespace Upp;

GitFileHistory::GitFileHistory()
{
	CtrlLayout(*this);

	this->WhenClose = THISBACK(Close);
	close.WhenPush = THISBACK(Close);
	ok.Hide();
	status.Hide();

	GIT_HASH = "HASH";
	GIT_AUTHOR = "AUTHOR";
	GIT_AUTHOR_TIME = "AUTHOR_TIME";
	GIT_DIFF="GIT_DIFF";

	commitoutput.Horz(diffctrl, commitlist);
	commitoutput.SetPos(7000);

	splitter.Vert(commitoutput, commitdata);
	splitter.SetPos(7300);

	commitlist.AddColumn(GIT_HASH, "Changeset").SetConvert(Single<ConvHash>()).HeaderTab().Hide();
	commitlist.AddColumn(GIT_AUTHOR, "Author").HeaderTab();
	commitlist.AddColumn(GIT_AUTHOR_TIME, "Author time").SetConvert(Single<ConvDateTime>()).HeaderTab();
	commitlist.AddColumn(GIT_DIFF, "Diff").HeaderTab().Hide();
	commitlist.ColumnWidths("0 100 100 0");
	commitlist.EvenRowColor();
	commitlist.AutoHideSb();
	commitlist.WhenEnterRow = THISBACK(ShowDetails);

	Rect rect = GetWorkArea();
	rect.right = rect.right * .95;
	rect.bottom = rect.bottom * .95;
	SetRect(rect);
	CenterScreen();

	Sizeable().Zoomable();
	BackPaint();
}

bool GitFileHistory::Key(dword key, int count)
{
	if (key == K_ESCAPE) {
		if (splitter.GetZoom() >= 0) {
			splitter.NoZoom();
		} else {
			splitter.Zoom(0);
		}
	}

	return Ctrl::Key(key, count);
}

void GitFileHistory::ShowDetails()
{
	int row = commitlist.GetCursor();
	if (row < 0)
		return;

	StringStream ss(commitlist.Get(row, GIT_DIFF));
	Vector<String> diff;
	while (!ss.IsEof()) {
		diff.Add(ss.GetLine());
	}
	if (diff.GetCount() > 0)
		diffctrl.Set(diff);
	else
		diffctrl.Clear();

	commitdata.ShowCommitData(GetGitDir(), commitlist.Get(row, GIT_HASH).ToString());
}

void GitFileHistory::ShowFileHistory(const String& filepath)
{
	String title = "GIT file history: " + filepath;
	Title(title);

	SetGitDir(GetGitRoot(filepath));

	Vector<String> output = GetFileLog(filepath).GetOutput();
	commitlist.Clear();
	diffctrl.Clear();

	Vector<String> data;
	String diff;
	int done = 0;
	int diffstart = 0;

	int numparam = 3;
	for (int i = 0; i < output.GetCount(); ++i) {
		String line = output[i];
		if (line.Find("fatal: Not a git repository") == 0) {
			Exclamation(line);
			break;
		}

		if (line.Find("commit ") == 0) {
			if (done == 1 && diffstart == 1) {
				diffstart = 0;
				if (data.GetCount() == numparam ) {
					commitlist.Add(data[0].Mid(7), data[1], ScanInt64(data[2]), diff);
				}
				data.Clear();
				diff.Clear();
			}
			data = Split(line, " >< ");
			done = 1;
			continue;
		}

		if (line.Find("diff ") == 0)
			diffstart = 1;

		if (diffstart == 1) {
			diff.Cat() << line << '\n';
		}
	}

	if (data.GetCount() == numparam ) {
		commitlist.Add(data[0].Mid(7), data[1], ScanInt64(data[2]), diff);
	}

	if (commitlist.GetCount() > 0) {
		commitlist.SetCursor(0);
	}
}

// vim: ts=4
