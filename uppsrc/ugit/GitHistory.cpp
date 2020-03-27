#include "GitHistory.h"

using namespace Upp;

GitHistories::GitHistories()
{
	CtrlLayout(*this, "GIT repositories history");
	this->WhenClose = THISBACK(Close);

	commitall.Hide();
	commitsign.Hide();
	savediff.Hide();
	fetch.Hide();
	stash.Hide();
	stashapply.Hide();
	ok.Hide();
	close.WhenPush = THISBACK(Close);

	Rect rect = GetWorkArea();
	rect.right = rect.right * .85;
	rect.bottom = rect.bottom * .85;
	SetRect(rect);
	CenterScreen();

	Sizeable().Zoomable();
}

void GitHistories::Init(Vector<String>& gitdirs)
{
	history.Clear();
	tabs.Reset();

	for (int i = 0 ; i < gitdirs.GetCount(); ++i) {
		history.Add().SetGitDir(gitdirs[i]);
		tabs.Add(history[i].SizePos(), GetFileName(history[i].GetGitDir()));
		history[i].ShowHistory();
	}
}

GitHistory::GitHistory()
{
	CtrlLayout(*this);

	showdiff = 0;
	diffrow = -2;
	diffpos = -1;
	commitpos = -1;

	GIT_TREE = "GIT_TREE";
	GIT_HASH = "GIT_HASH";
	GIT_REFS = "GIT_REFS";
	GIT_SUBJECT = "GIT_SUBJECT";
	GIT_STATUS = "GIT_STATUS";
	GIT_FILE = "GIT_FILE";

	diff.Horz(diffctrl, files);
	diff.SetPos(7000);

	log.AddColumn(GIT_TREE, "Tree").HeaderTab();
	log.AddColumn(GIT_HASH, "Hash").SetConvert(Single<ConvHash>()).HeaderTab();
	log.AddColumn(GIT_REFS, "Refs").HeaderTab();
	log.AddColumn(GIT_SUBJECT, "Subject");
	log.ColumnWidths("10 15 25 100");
	log.WhenEnterRow = THISBACK(LogChangeRow);
	log.WhenBar = THISBACK(StatusMenuBar);

	files.AddColumn(GIT_STATUS, "").HeaderTab();
	files.AddColumn(GIT_FILE, "Files");
	files.ColumnWidths("20 100");
	files.AutoHideSb();
	files.NoGrid();
	files.WhenEnterRow = THISBACK(FilesChangeRow);

	firstparent.WhenAction = THISBACK(ShowHistory);
	showdiff.WhenAction = THISBACK(SwitchChange);

	SwitchChange();

	BackPaint();
}

bool GitHistory::Key(dword key, int count)
{
	if (key == K_ESCAPE) {
		if (splitter.GetZoom() >= 0) {
			splitter.NoZoom();
			log.ScrollIntoCursor();
			LogChangeRow();
		} else {
			splitter.Zoom(0);
		}
	}

	return Ctrl::Key(key, count);
}

void GitHistory::SwitchChange()
{
	if (showdiff.GetData() == 1) {
		if (diffpos == -1) {
			diffpos = 3500;
			commitpos = 7300;
		} else {
			commitpos = splitter.GetPos();
		}

		splitter.Vert(log, diff);
		splitter.SetPos(diffpos);
	} else {
		if (commitpos == -1) {
			diffpos = 3500;
			commitpos = 7300;
		} else {
			diffpos = splitter.GetPos();
		}

		splitter.Vert(log, commitdata);
		splitter.SetPos(commitpos);
	}
}

void GitHistory::StatusMenuBar(Bar& bar)
{
	int row = log.GetCursor();
	if (row < 0)
		return;

	bool tag =  log.Get(row, GIT_REFS).ToString().Find("tag:") >= 0 ? true : false;
	bool hash = log.Get(row, GIT_HASH).ToString().GetCount() > 0;
	bool diff = diffctrl.GetCount() > 0;

	bar.Add(hash,         "Create branch", THISBACK(BranchAdd));
	bar.Separator();
	bar.Add(hash,         "Tag insert", THISBACK(TagInsert));
	bar.Add(hash && tag,  "Tag delete", THISBACK1(TagAction, false));
	bar.Add(hash && tag,  "Tag show",   THISBACK1(TagAction, true));
	bar.Separator();
	bar.Add(diff,         "Save diff",  THISBACK(SaveDiff));
	bar.Add(diff,         "Save GIT patch",  THISBACK(SavePatch));
}

void GitHistory::BranchAdd()
{
	int row = log.GetCursor();
	String id = log.Get(row, GIT_HASH).ToString();

	ProcessBranchAdd(id);

	if (IsOutput())
		ProcessCommandOutput();
	else {
		diffrow = row;
		ShowHistory();
	}
}

void GitHistory::TagInsert()
{
	int row = log.GetCursor();

	String hash = log.Get(row, GIT_HASH).ToString().Left(GIT_HASH_SIZE);

	WithGitConfigParamLayout2<TopWindow> tag;
	CtrlLayout(tag, "Add new tag to the commit '" + hash + "'");

	tag.Acceptor(tag.ok, IDOK);
	tag.ok.Ok().SetLabel("Insert");
	tag.Rejector(tag.cancel, IDCANCEL);
	tag.cancel.Cancel();
	tag.WhenClose = tag.Rejector(IDCANCEL);
	tag.param1text.SetText("Tag name:");
	tag.param2text.SetText("Description:");

	for (;;) {
		int c = tag.RunAppModal();

		if (c == IDOK) {
			String name = TrimBoth(tag.param1.GetData().ToString());
			String desc = TrimBoth(tag.param2.GetData().ToString());
			if (name.GetCount() > 0 && desc.GetCount() > 0) {
				TagAdd(hash, name, desc);
				break;
			} else {
				Exclamation("Parameters can't be empty!");
			}
		}

		if (c == IDCANCEL)
			break;
	}

	if (IsOutput())
		ProcessCommandOutput();
	else {
		diffrow = row;
		ShowHistory();
	}
}

void GitHistory::TagAction(bool show)
{
	Vector<String> output;
	int row = log.GetCursor();

	String hash = log.Get(row, GIT_HASH).ToString().Left(GIT_HASH_SIZE);
	String refs = log.Get(row, GIT_REFS).ToString();
	String title = Format("%s the tag(s) in the commit '%s'",  show ? "Show" : "Delete", hash);

	WithGitCmdOutputLayout<TopWindow> tag;
	CtrlLayout(tag, title);

	tag.Sizeable().Zoomable();
	tag.Acceptor(tag.ok, IDOK);
	tag.ok.Ok().SetLabel("Delete");
	tag.Rejector(tag.close, IDCANCEL);
	tag.close.Cancel().SetLabel("Cancel");
	tag.WhenClose = tag.Rejector(IDCANCEL);

	tag.cmd.AddColumn("Tag name");
	tag.cmd.AddColumn("Tag description");
	tag.cmd.NoWantFocus();

	if (show) {
		tag.ok.Hide();
		tag.close.SetLabel("Close");
	}

	if (refs.GetCount() >= 2) {
		refs.TrimLast();
		refs = refs.Mid(1);
	}

	Vector<String> tmp = Split(refs, ",");
	for (int i = 0; i < tmp.GetCount(); ++i) {
		if (tmp[i].Find("tag:") >= 0) {
			String name = TrimBoth(tmp[i].Mid(5));
			String desc;

			output = TagGetDesc(name).GetOutput();
			if (output.GetCount() > 0) {
				desc = output[0];
				desc = TrimBoth(desc.Mid(desc.Find(" ")));
				output.Clear();
			}
			tag.cmd.Add(name, desc);
		}
	}
	tag.cmd.GoBegin();

	bool error = false;
	bool deleted = false;
	for (;;) {
		int c = tag.RunAppModal();

		if (c == IDOK) {
			int row = tag.cmd.GetCursor();
			if (row >= 0) {
				output = TagDelete(tag.cmd.Get(row, 0).ToString()).GetOutput();
				tag.cmd.Remove(row);

				error = CheckError(output);
				if (!error && !deleted)
					deleted = true;
				if (error)
					SetOutput(output).ProcessCommandOutput();

				if (tag.cmd.GetCount() == 0)
					break;
			}
		}

		if (c == IDCANCEL)
			break;
	}

	if (deleted) {
		diffrow = row;
		ShowHistory();
	}
}

bool GitHistory::CheckError(const Vector<String>& test)
{
	bool error = false;
	for (int i = 0; i < test.GetCount(); ++i) {
		if (test[i].Find("error:") == 0) {
			error = true;
			break;
		}
	}
	return (error);
}

void GitHistory::SaveDiff()
{
	int row = log.GetCursor();
	String hash = log.Get(row, GIT_HASH).ToString();

	if (hash.GetCount() > 0)
		SaveOutput(hash, typediff);
}

void GitHistory::SavePatch()
{
	int row = log.GetCursor();
	String hash = log.Get(row, GIT_HASH).ToString();

	if (hash.GetCount() > 0)
		SaveOutput(hash, typepatch);
}

void GitHistory::SaveOutput(const String& hash, int type)
{
	String ext;
	switch (type) {
		case typepatch:
			ext = "patch";
			GetPatch(hash);
			break;
		default:
			ext = "diff";
			GetDiff(hash);
	}
	
	FileSel file;
	file.Set(AppendFileName(GetHomeDirectory(), hash + "." + ext));
	file.ExecuteSaveAs("Select filename to save the GIT " + ext);

	Vector<String> data = GetOutput();
	if (!GetCmdErrorCode()) {
		String s;
		for (int i = 0; i < data.GetCount(); ++i)
			s << data[i] << "\n";
		SaveFile(file.Get(), s);
	}
}

void GitHistory::LogChangeRow()
{
	if (splitter.GetZoom() == 0)
		return;

	showdiff.Disable();

	int row = log.GetCursor();
	String hash = log.Get(row, GIT_HASH).ToString();

	SetCommitData(row);
 
	if (diffrow != row) {
		files.Clear();
		diffctrl.Clear();
	}

	if (hash.GetCount() > 0 && files.GetCount() == 0) {
		Vector<String> data = GetDiff(hash).GetOutput();
		if (!GetCmdErrorCode()) {
			diffctrl.Set(data);

			data = GetDiff(hash, true).GetOutput();
			for (int i = 0; i < data.GetCount(); ++i) {
				Vector<String> tmp = Split(data[i], "\t");
				files.Add(tmp[0], tmp[1]);
			}
			files.GoBegin();
		}
	}

	diffrow = row;
	showdiff.Enable();
}

void GitHistory::FilesChangeRow()
{
	int row = files.GetCursor();
	if (row >= 0)
		diffctrl.SetLinePos(files.Get(row, GIT_FILE));
}

void GitHistory::SetCommitData(int row)
{
	commitdata.ShowCommitData(GetGitDir(), log.Get(row, GIT_HASH).ToString());
}

void GitHistory::ShowHistory()
{
	if (GetGitDir().IsEmpty())
		return;

	log.Clear();
	Vector<String> list = GetLog(firstparent.Get()).GetOutput();

	if (!GetCmdErrorCode()) {
		for (int i = 0; i < list.GetCount(); ++i) {
			Vector<String> tmp = Split(list[i], "><");
			if (tmp.GetCount() > 3) {
				log.Add(tmp[0], TrimBoth(tmp[1]), TrimBoth(tmp[2]), TrimBoth(tmp[3]));
			} else {
				log.Add(tmp[0], "", "", "");
			}
		}
	}
	if (diffrow < 0 || diffrow > log.GetCount())
		log.GoBegin();
	else
		log.SetCursor(diffrow);

	showdiff.Enable(log.GetCount());
	firstparent.Enable(log.GetCount());
}

// vim: ts=4
