#include "GitBlame.h"

using namespace UPP;

GitBlame::GitBlame()
{
	CtrlLayout(*this);

	this->WhenClose = THISBACK(Close);
	close.WhenPush = THISBACK(Close);
	ok.Hide();
	status.Hide();

	GIT_HASH = "HASH";
	GIT_HASH_FILE = "HASH_FILE";
	GIT_AUTHOR = "AUTHOR";
	GIT_AUTHOR_TIME = "AUTHOR_TIME";
	GIT_FILENAME = "FILENAME";

	split.Horz(blame, lines);
	split.SetPos(2300);
	splitter.Vert(split, commitdata);
	splitter.SetPos(8000);

	lines.WhenLeftDown = THISBACK(SetLine);
	lines.SetReadOnly();

	blame.AddColumn(GIT_HASH, "Changeset").SetConvert(Single<ConvHash>()).HeaderTab().Hide();
	blame.AddColumn(GIT_HASH_FILE, "Previous file").HeaderTab().Hide();
	blame.AddColumn(GIT_AUTHOR, "Author").HeaderTab();
	blame.AddColumn(GIT_AUTHOR_TIME, "Author time").SetConvert(Single<ConvDateTime>()).HeaderTab();
	blame.AddColumn(GIT_FILENAME, "Filename").HeaderTab().Hide();
	blame.ColumnWidths("80 0 100 80 0");
	blame.WhenEnterRow = THISBACK(ShowDetails);
	blame.WhenBar = THISBACK(StatusMenuBar);
	blame.HideSb();
	blame.NoGrid();
	blame.Header(false);

	Font f = lines.GetFont();
	f.Height(f.GetHeight() - 2);
	lines.SetFont(f);
	int cy = lines.GetFontSize().cy;
	blame.SetLineCy(cy);

	lines.WhenAction = lines.WhenScroll = [=] {
		blame.SetVirtualCount(lines.GetLineCount() + 2); // unfortunately, we need 2 more lines to cover LineEdit end of text
		blame.ScrollTo(lines.GetScrollPos().y * cy);
	};
	lines.WhenSel = [=] {
		if(dosel) // avoid infinite recursion
			return;
		dosel++;
		blame.SetCursor(lines.GetCursorLine());
		blame.WhenAction();
		dosel--;
	};

	blame.WhenScroll = [=] {
		lines.SetScrollPos(Point(0, blame.GetScroll() / cy));
	};
	blame.WhenSel = [=] {
		if(dosel) // avoid infinite recursion
			return;
		dosel++;
		int i = blame.GetCursor();
		if(i >= 0 && i <= lines.GetLength()) {
			lines.SetCursor(lines.GetPos(i));
			lines.WhenAction();
		}
		dosel--;
	};

	Rect rect = GetWorkArea();
	rect.right = rect.right * .95;
	rect.bottom = rect.bottom * .95;
	SetRect(rect);
	CenterScreen();

	Sizeable().Zoomable();
	BackPaint();
}

bool GitBlame::Key(dword key, int count)
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

void GitBlame::StatusMenuBar(Bar& bar)
{
	int row = blame.GetCursor();
	if (row < 0)
		return;

	if (blame.HeaderTab(GIT_HASH).IsVisible())
		bar.Add("View hash", GitImg::checkmark(), THISBACK1(ViewColumn, GIT_HASH));
	else
		bar.Add("View hash", THISBACK1(ViewColumn, GIT_HASH));

	if (blame.HeaderTab(GIT_AUTHOR).IsVisible())
		bar.Add("View author", GitImg::checkmark(), THISBACK1(ViewColumn, GIT_AUTHOR));
	else
		bar.Add("View author", THISBACK1(ViewColumn, GIT_AUTHOR));

	if (blame.HeaderTab(GIT_AUTHOR_TIME).IsVisible())
		bar.Add("View date", GitImg::checkmark(), THISBACK1(ViewColumn, GIT_AUTHOR_TIME));
	else
		bar.Add("View date",   THISBACK1(ViewColumn, GIT_AUTHOR_TIME));
}

void GitBlame::ViewColumn(const Id &id)
{
	if (blame.HeaderTab(id).IsVisible())
		blame.HeaderTab(id).Hide();
	else
		blame.HeaderTab(id).Show();
}

void GitBlame::ShowDetails()
{
	int row = blame.GetCursor();
	if (row < 0)
		return;

	lines.GotoLine(row);
	commitdata.ShowCommitData(GetGitDir(), blame.Get(row, GIT_HASH).ToString(), blame.Get(row, GIT_HASH_FILE).ToString());
}

void GitBlame::SetLine()
{
	int row = lines.GetCursorLine();
	if (row < 0)
		return;

	blame.SetCursor(row);
}

void GitBlame::ShowBlame(const String& filepath)
{
	String title = "GIT Blame: " + filepath;
	Title(title);

	SetGitDir(GetGitRoot(filepath));

	Vector<String> output = Blame(filepath).GetOutput();
	blame.Clear();
	lines.Clear();

	bool hashDone = false;
	bool lineDone = false;
	String hash, hashPrev, hashFile, summary, fileName, line;
	String author, authorEmail, committer, committerEmail;
	int64 authorTime = 0;
	// int64 committerTime; // not used yet
	// int authorTz, committerTz; // not used yet

	for (int i = 0; i < output.GetCount(); ++i) {
		int size;

		if (!hashDone) {
			size = output[i].FindFirstOf(" ");
			hash = TrimBoth(output[i].Left(size));
			hashDone = true;
			continue;
		}
		if (output[i].Find("author ") == 0) {
			size = output[i].FindFirstOf(" ");
			author = TrimBoth(output[i].Mid(size));
			continue;
		}
		if (output[i].Find("author-mail ") == 0) {
			size = output[i].FindFirstOf(" ");
			authorEmail = TrimBoth(output[i].Mid(size));
			continue;
		}
		if (output[i].Find("author-time ") == 0) {
			size = output[i].FindFirstOf(" ");
			authorTime = ScanInt64(output[i].Mid(size));
			continue;
		}
		if (output[i].Find("author-tz ") == 0) {
			// not used yet
			// size = output[i].FindFirstOf(" ");
			// authorTz = ScanInt(output[i].Mid(size));
			continue;
		}
		if (output[i].Find("committer ") == 0) {
			size = output[i].FindFirstOf(" ");
			committer = TrimBoth(output[i].Mid(size));
			continue;
		}
		if (output[i].Find("committer-mail ") == 0) {
			size = output[i].FindFirstOf(" ");
			committerEmail = TrimBoth(output[i].Mid(size));
			continue;
		}
		if (output[i].Find("committer-time ") == 0) {
			// not used yet
			// size = output[i].FindFirstOf(" ");
			// committerTime = ScanInt64(output[i].Mid(size));
			continue;
		}
		if (output[i].Find("committer-tz ") == 0) {
			// not used yet
			// size = output[i].FindFirstOf(" ");
			// committerTz = ScanInt(output[i].Mid(size));
			continue;
		}
		if (output[i].Find("summary ") == 0) {
			size = output[i].FindFirstOf(" ");
			summary = TrimBoth(output[i].Mid(size));
			continue;
		}
		if (output[i].Find("boundary ") == 0) {
			continue;
		}
		if (output[i].Find("previous ") == 0) {
			size = output[i].FindFirstOf(" ");
			int sizeNext = output[i].FindFirstOf(" ", size + 1);
			hashPrev = TrimBoth(output[i].Mid(size, sizeNext - size));
			hashFile = TrimBoth(output[i].Mid(sizeNext));
			continue;
		}
		if (output[i].Find("filename ") == 0) {
			size = output[i].FindFirstOf(" ");
			fileName = output[i].Mid(size);
			if (i < (output.GetCount() - 1)) {
				++i;
				line = output[i].Mid(1);
				lineDone = true;
			}
		}

		if (lineDone) {
			lines.Insert(lines.GetLength(), line + "\n");
			blame.Add(hash, hashFile, author, authorTime, fileName);
			hashDone = false;
			lineDone = false;
		}
	}

	if (blame.GetCount() > 0) {
		blame.SetCursor(0);

		String h = EditorSyntax::GetSyntaxForFilename(GetFileName(filepath));
		lines.Highlight(h);
	}
}

// vim: ts=4
