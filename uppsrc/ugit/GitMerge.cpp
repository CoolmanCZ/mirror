#include "GitMerge.h"

using namespace Upp;

GitMerging::GitMerging()
{
	CtrlLayout(*this, "GIT merging");
	this->WhenClose = THISBACK(MainClose);

	tabs.WhenSet = THISBACK(ToggleSet);

	initdone = 0;

	fetch.SetImage(GitImg::GitFetch());
	fetch.WhenPush = THISBACK(Fetch);

	commitsign.Disable();
	commitsign.Hide();

	stash.Disable();
	stash.SetImage(GitImg::GitStash());
	stash.WhenPush = THISBACK(Stash);

	stashapply.Disable();
	stashapply.SetImage(GitImg::GitUnstash());
	stashapply.WhenPush = THISBACK(StashApply);

	savediff.Disable();
	savediff.WhenPush = THISBACK(SaveDiff);

	ok.Disable();
	ok.SetLabel("Merge");
	ok.WhenPush = THISBACK(Merge);
	close.WhenPush = THISBACK(Close);

	commitall.Hide();

	Rect rect = GetWorkArea();
	rect.right = rect.right * .85;
	rect.bottom = rect.bottom * .85;
	SetRect(rect);
	CenterScreen();

	Sizeable().Zoomable();
}

void GitMerging::MainClose()
{
	initdone = 0;
	Break();
}

void GitMerging::Close()
{
	if (initdone) {
		int tab = tabs.Get();
		if (merge[tab].GetCmdStatus()) {
			merge[tab].SetCmdStatus(false);
			ToggleSet();
		} else {
			MainClose();
		}
	} else {
		Break();
	}
}

void GitMerging::Init(Vector<String>& GitDirs)
{
	merge.Clear();
    tabs.Reset();

	for (int i = 0 ; i < GitDirs.GetCount(); ++i)
		merge.Add().Init(GitDirs[i]);

	for (int i = 0 ; i < merge.GetCount(); ++i) {
		tabs.Add(merge[i].SizePos(), GetFileName(merge[i].GetGitDir()));
		merge[i].WhenStatusChange = THISBACK(ToggleSet);
	}

	initdone = 1;
	ToggleSet();
}

void GitMerging::Fetch()
{
	if (initdone) {
		int tab = tabs.Get();
		Vector<String> output;

		output.Add("Fetching data from all repositories in the GIT directory");
		output.Append(merge[tab].FetchAll().GetOutput());
		output.Add("Fetch DONE");
		merge[tab].SetOutput(output).ProcessCommandOutput();
		merge[tab].BranchChangeRow();
	}
}

void GitMerging::Stash()
{
	if (initdone) {
		int tab = tabs.Get();
		Vector<String> output;

		output.Add("Processing stash");
		output.Append(merge[tab].Stash("").GetOutput());
		output.Add("Stash DONE");
		merge[tab].SetOutput(output).ProcessCommandOutput();
		ToggleSet();
		WhenStash();
	}
}

void GitMerging::StashApply()
{
	if (initdone) {
		int tab = tabs.Get();
		Vector<String> output;

		output.Add("Processing stash apply");
		output.Append(merge[tab].ProcessStashApply().GetOutput());
		output.Add("Stash apply DONE");
		merge[tab].SetOutput(output).ProcessCommandOutput();
		ToggleSet();
		WhenStash();
	}
}

void GitMerging::Merge()
{
	if (initdone) {
		int tab = tabs.Get();
		merge[tab].ExecuteMerge(~commitsign);
	}
}

void GitMerging::SaveDiff()
{
	if (initdone) {
		int tab = tabs.Get();
		merge[tab].SaveDiff();
	}
}


void GitMerging::ToggleSet()
{
	if (initdone) {
		int tab = tabs.Get();
		bool cmdshow = merge[tab].GetCmdStatus();
		if (cmdshow)
			close.SetLabel("Back");
		else
			close.SetLabel("Close");

		fetch.Show(!cmdshow);
		commitsign.Show(!cmdshow);
		stash.Show(!cmdshow);
		stashapply.Show(!cmdshow);
		savediff.Show(!cmdshow);
		ok.Show(!cmdshow);

		if (IsGitDir(merge[tab].GetGitDir())) {
			bool allowmerge = merge[tab].GetMergeStatus();
			bool iscommit = merge[tab].IsCommitList();
			bool isstash = merge[tab].IsStashList();
			bool isdiff = merge[tab].IsDiffOutput();

			fetch.Enable();
			stash.Enable(iscommit);
			stashapply.Enable(isstash);
			savediff.Enable(isdiff);
			commitsign.Enable(!iscommit && allowmerge);
			ok.Enable(!iscommit && allowmerge);
		} else {
			fetch.Disable();
			stash.Disable();
			stashapply.Disable();
			savediff.Disable();
			commitsign.Disable();
			ok.Disable();
		}
	}
}

GitMerge::GitMerge()
{
	CtrlLayout(*this);
	CtrlLayout(mb);
	mb.SetFrame(ViewFrame());
	mb.commit.Set(1);

	allowmerge = false;
	cmdshow = false;

	mb.local.WhenAction = THISBACK(BranchChangeRow);
	mb.remote.WhenAction = THISBACK(BranchChangeRow);

	GIT_FILE = "GIT_FILE";

	mb.files.AddColumn(GIT_FILE, "Files");
	mb.files.AutoHideSb();
	mb.files.NoGrid();
	mb.files.WhenEnterRow = THISBACK(FilesChangeRow);

	diff.Horz(diffctrl, mb);
	diff.SetPos(7500);

	splitter.Vert(diff, cmd);
	splitter.Zoom(0);

	cmd.AddColumn("Command output");
	cmd.GridColor(SColorPaper());
	cmd.AutoHideSb();

	BackPaint();
}

void GitMerge::Init(const String& gitdir)
{
	SetGitDir(gitdir);
	ShowMerge();
	BranchChangeRow();
}

bool GitMerge::Key(dword key, int count)
{
	if (key == K_ESCAPE) {
		cmdshow = !cmdshow;
		ToggleCmd();
	}
	return Ctrl::Key(key, count);
}

void GitMerge::BranchChangeRow()
{
	String mergefrom = mb.remote.Get();
	String mergeto = mb.local.Get();

	if (mergeto.GetCount() > 0 && mergefrom.GetCount() > 0) {
		if (mergefrom.Find("*") == 0)
			mergefrom = mergefrom.Mid(2);
		if (mergeto.Find("*") == 0)
			mergeto = mergeto.Mid(2);

		ShowDiff(mergefrom, mergeto);
		remotestatus = GetBranchStatus(mergeto, mergefrom);
	}

	mb.commit.Enable(allowmerge);
	mb.noff.Enable(allowmerge);
}

void GitMerge::FilesChangeRow()
{
	int row = mb.files.GetCursor();
	if (row >= 0)
		diffctrl.SetLinePos(mb.files.Get(row, GIT_FILE));
}

void GitMerge::ShowDiff(const String& mergefrom, const String& mergeto)
{
	mb.files.Clear();
	allowmerge = false;

	Vector<String> output = GetMergeDiff(mergeto, mergefrom).GetOutput();
	if (GetCmdErrorCode()) {
		diffctrl.Clear();
	} else {
		diffctrl.Set(output);
		if (output.GetCount() > 0) {
			if (GetBranchLocal().Compare(mergeto) == 0)
				allowmerge = true;
			output = GetMergeDiff(mergeto, mergefrom, true).GetOutput();
			if (!GetCmdErrorCode()) {
				for (int i = 0; i < output.GetCount(); ++i)
					mb.files.Add(output[i]);
			}
		}
	}
	WhenStatusChange();
}

void GitMerge::ProcessCommandOutput()
{
	Vector<String> output = GetOutput();
	if (output.GetCount() > 0) {
		for (int i = 0; i < output.GetCount(); ++i)
			cmd.Add(output[i]);

		cmdshow = true;
		ToggleCmd();
	}
	Sync();
}

void GitMerge::ToggleCmd()
{
	splitter.Zoom(cmdshow);
	splitter.SetFocus();
	WhenStatusChange();
}

void GitMerge::ShowMerge()
{
	if (!IsGitDir(GetGitDir()))
		return;

	Vector<String> local = GetBranchAll().GetOutput();
	int localerrorcode = GetCmdErrorCode();
	Vector<String> remote = GetBranchAll(false).GetOutput();
	int remoteerrorcode = GetCmdErrorCode();

	if (!remoteerrorcode && !localerrorcode) {
		for (int i = 0; i < local.GetCount(); ++i) {
			String tmp = TrimBoth(local[i]);
			if (tmp.Find("*") == 0)
				actualbranch = tmp;
			mb.local.Add(tmp);
			mb.remote.Add(tmp);
		}

		for (int i = 0; i < remote.GetCount(); ++i){
			if (remote[i].Find("->") >= 0)
				continue;
			mb.remote.Add(TrimBoth(remote[i]));
		}

		if (actualbranch.GetCount() > 0)
			mb.local.SetIndex(mb.local.Find(actualbranch));

		String currentremote = GetBranchLocalRemote();
		if (!GetCmdErrorCode() && currentremote.GetCount() > 0)
			mb.remote.SetIndex(mb.remote.Find(currentremote));
	}
}

void GitMerge::ExecuteMerge(bool signoff)
{
	if (IsCommitList()) {
		Exclamation("You have to commit or stash the changes before merge!");
		return;
	}

	String mergefrom = mb.remote.Get();
	String mergeto = mb.local.Get();

	if (mergeto.Compare(mergefrom) == 0) {
		Exclamation("Can not merge to the same branch!");
		return;
	}

	Vector<String> output;
	int errorcode = Merge(mergefrom, mb.noff.Get()).GetCmdErrorCode();
	output.Append(GetOutput());

	if (!errorcode && mb.commit.Get() == 1) {
		String message = Format("Merge remote-tracking branch '%s'", mergefrom);
		if (signoff)
			message.Cat() << Format("\nSigned-off-by: %s <%s>\n", GetGitConfig(true, "user.name"), GetGitConfig(true, "user.email"));
		Vector<String> parameters;
		String msgpath;
		errorcode = EditCommitMessage(message, parameters, msgpath);
		if (!errorcode)
			output.Append(Commit(parameters, msgpath).GetOutput());
		else {
			CmdExecute("merge --abort");
			output.Append(GetOutput());
			output.Add("Merge aborted!");
		}
	}

	SetOutput(output).ProcessCommandOutput();
	BranchChangeRow();
}

void GitMerge::SaveDiff()
{
	String mergefrom = mb.remote.Get();
	String mergeto = mb.local.Get();

	if (mergeto.GetCount() > 0 && mergefrom.GetCount() > 0) {
		if (mergefrom.Find("*") == 0)
			mergefrom = mergefrom.Mid(2);
		if (mergeto.Find("*") == 0)
			mergeto = mergeto.Mid(2);
	}

	if (mergeto.Compare(mergefrom) == 0) {
		Exclamation("Can not create diff for to the same branches!");
		return;
	}

	Vector<String> output = GetMergeDiff(mergeto, mergefrom).GetOutput();
	if (!GetCmdErrorCode() && output.GetCount() > 0) {
		String out;
		for (int i = 0; i < output.GetCount(); ++i)
			out << output[i] << "\n";
		SelectSaveFile("*.diff, *.patch\n*.*", out);
	} else
		SetOutput(output).ProcessCommandOutput();
}

// vim: ts=4
