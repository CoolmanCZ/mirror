#include "GitCommit.h"

using namespace Upp;

GitCommitting::GitCommitting()
{
	CtrlLayout(*this, "GIT committing");
	this->WhenClose = THISBACK(Close);

	tabs.WhenSet = THISBACK(ToggleSet);

	initdone = 0;

	fetch.Disable();
	fetch.Hide();

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
	ok.SetLabel("Commit");
	ok.WhenPush = THISBACK(Commit);
	close.WhenPush = THISBACK(Close);

	Rect rect = GetWorkArea();
	rect.right = rect.right * .95;
	rect.bottom = rect.bottom * .95;
	SetRect(rect);
	CenterScreen();

	Sizeable().Zoomable();
}

void GitCommitting::MainClose()
{
	initdone = 0;
	Break();
}

void GitCommitting::Close()
{
	if (initdone) {
		int tab = tabs.Get();
		if (commit[tab].GetCmdStatus()) {
			commit[tab].SetCmdStatus(false);
			ToggleSet();
		}
	}
	MainClose();
}

void GitCommitting::Init(Vector<String>& gitdirs, const String& packagepath)
{
	int tab = -1;
	commit.Clear();
    tabs.Reset();

	for (int i = 0 ; i < gitdirs.GetCount(); ++i)
		commit.Add().Init(gitdirs[i]);

	for (int i = 0 ; i < commit.GetCount(); ++i) {
		tabs.Add(commit[i].SizePos(), GetFileName(commit[i].GetGitDir()));
		commit[i].WhenRepositoryChange = THISBACK(DoRepositoryChange);
		commit[i].WhenLeftDouble = THISBACK(GetFilePos);
		commit[i].WhenStatusChange = THISBACK(ToggleSet);

		if (GetGitRoot(packagepath).Compare(commit[i].GetGitDir()) == 0)
			tab = i;
	}

	if (tab > -1 && tab < commit.GetCount())
		tabs.Set(tab);
	else
		Exclamation(Format("Package '%s' is not the part of the Git repository!", GetFileTitle(packagepath)));

	initdone = 1;
	ToggleSet();
}

void GitCommitting::GetFilePos(const String& filepath, int filepos)
{
	WhenLeftDouble(filepath, filepos);
	Close();
}

void GitCommitting::Stash()
{
	if (initdone) {
		int tab = tabs.Get();
		Vector<String> output;

		String message;
		output.Add("Processing stash");
		output.Append(commit[tab].Stash(message).GetOutput());
		output.Add("Stash DONE");
		commit[tab].SetOutput(output).ProcessCommandOutput();
		commit[tab].ShowCommit();
		ToggleSet();
		WhenStash();
	}
}

void GitCommitting::StashApply()
{
	if (initdone) {
		int tab = tabs.Get();
		Vector<String> output;

		output.Add("Processing stash apply");
		output.Append(commit[tab].ProcessStashApply().GetOutput());
		output.Add("Stash apply DONE");
		commit[tab].SetOutput(output).ProcessCommandOutput();
		commit[tab].ShowCommit();
		ToggleSet();
		WhenStash();
	}
}

void GitCommitting::Commit()
{
	if (initdone) {
		if (~commitall) {
			for (int i = 0; i < commit.GetCount(); ++i) {
				tabs.Set(i);
				commit[i].ExecuteCommit(~commitsign);
			}
		} else {
			int tab = tabs.Get();
			commit[tab].ExecuteCommit(~commitsign);
		}
	}
}

void GitCommitting::SaveDiff()
{
	if (initdone) {
		int tab = tabs.Get();
		commit[tab].SaveDiff();
	}
}

void GitCommitting::ToggleSet()
{
	if (initdone) {
		int tab = tabs.Get();
		bool cmdshow = commit[tab].GetCmdStatus();

		if (cmdshow)
			close.SetLabel("Back");
		else
			close.SetLabel("Close");

		commitsign.Show(!cmdshow);
		commitall.Show(!cmdshow);
		stash.Show(!cmdshow);
		stashapply.Show(!cmdshow);
		savediff.Show(!cmdshow);
		ok.Show(!cmdshow);

		if (IsGitDir(commit[tab].GetGitDir())) {
			bool iscommit = commit[tab].IsCommitList();
			bool isstash = commit[tab].IsStashList();
			bool unmergedfiles = commit[tab].IsUnmergedFiles();

			stash.Enable(iscommit);
			stashapply.Enable(isstash);
			savediff.Enable(iscommit);
			commitsign.Enable(iscommit && !unmergedfiles);
			ok.Enable(iscommit && !unmergedfiles);
		} else {
			stash.Disable();
			stashapply.Disable();
			savediff.Disable();
			commitsign.Disable();
			ok.Disable();
		}
	}
}

GitCommit::GitCommit()
{
	CtrlLayout(*this);
	CtrlLayout(cf);
	cf.SetFrame(ViewFrame());

	allowcommit = true;
	cmdshow = false;

	GIT_SELECT="GIT_SELECT";
	GIT_STATUS="GIT_STATUS";
	GIT_FILE="GIT_FILE";

	diff.Horz(diffctrl, cf);
	diff.SetPos(8000);

	splitter.Horz(diff, cmd);
	splitter.Zoom(0);

	cf.statuslist.AddColumn(GIT_SELECT, "").Ctrls<Option>().HeaderTab().Fixed(30);
	cf.statuslist.AddColumn(GIT_STATUS, "Status").SetConvert(Single<ConvStatus>()).HeaderTab().Fixed(90);
	cf.statuslist.AddColumn(GIT_FILE, "Files");
	cf.statuslist.WhenEnterRow = THISBACK(StatusChangeRow);
	cf.statuslist.WhenBar = THISBACK(StatusMenuBar);
	cf.statuslist.MultiSelect();
	cf.statuslist.AutoHideSb();

	cf.difftype.WhenAction = THISBACK(ToggleDiffType);
	cf.difftype.SetData(0);
	cf.difftype.Tip(t_("index - changes in the working tree, cached - changes between the index and your last commit, HEAD - changes in the working tree since your last commit (git commit -a)"));

	diffctrl.WhenLeftDouble = THISBACK(GetFilePos);

	cmd.AddColumn("Command output");
	cmd.GridColor(SColorPaper());
	cmd.AutoHideSb();

	BackPaint();
}

void GitCommit::Init(const String& gitdir)
{
	SetGitDir(gitdir);
	ShowCommit();
}

bool GitCommit::Key(dword key, int count)
{
	if (key == K_ESCAPE) {
		cmdshow = !cmdshow;
		ToggleCmd();
	}
	return Ctrl::Key(key, count);
}

void GitCommit::StatusChangeRow()
{
	if (cf.statuslist.GetCursor() >= 0)
		diffctrl.SetLinePos(cf.statuslist.Get(cf.statuslist.GetCursor(), GIT_FILE));
}

void GitCommit::StatusMenuBar(Bar& bar)
{
	int status_row = cf.statuslist.GetCursor();
	if (status_row < 0)
		return;

	bool add = false, modify = false, deleted = false, reset = false, merge = false;
	bool stage = false, unstage = false;
	String status = cf.statuslist.Get(status_row, GIT_STATUS);

	if (status[0] == 'A') {
		unstage = true;
		if (status[1] == 'M') {
			unstage = false;
			stage = true;
		}
	}

	if (status[0] == 'D')
		deleted = true;

	if ((status[1] == 'M') || (status[1] == 'D'))
		modify = true;

	if ((status[0] == 'U') && (status[1] == 'U')
	 || (status[0] == 'A') && (status[1] == 'A')) {
		reset = true;
		merge = true;
		modify = false;
		deleted = false;
	}

	if (status[0] == '?') {
		add = true;
		modify = false;
		deleted = false;
		reset = false;
	}

	bar.Add(modify,  "Discard changes", THISBACK(CancelCheckout));
	bar.Add(deleted, "Cancel delete", THISBACK(CancelDelete));
	bar.Separator();
	bar.Add(stage,   "Stage for commit", THISBACK(AddToRepository));
	bar.Add(unstage, "Unstage from commit", THISBACK(Unstage));
	bar.Separator();
	bar.Add(merge,   "Add unmerged changes", THISBACK(AddToRepository));
	bar.Add(reset,   "Reset unmerged changes", THISBACK(Reset));
	bar.Separator();
	bar.Add(add,  "Add to the repository", THISBACK(AddToRepository));
	bar.Add(add,  "Add to the .gitignore file", THISBACK(AddToGitignore));
	bar.Add(!add, "Delete from the repository", THISBACK(DelFromRepository));
	bar.Separator();
	bar.Add(add,  "Delete", THISBACK(DelFile));
}

void GitCommit::CancelCheckout()
{
	Vector<String> files;
	String names;

	for (int i = 0; i < cf.statuslist.GetCount(); ++i) {
		String status = cf.statuslist.Get(i, GIT_STATUS).ToString();
		if ((cf.statuslist.IsSelected(i) || (cf.statuslist.Get(i, 0) == 1))
		 && (status[1] == 'M' || status[1] == 'D')) {
			files.Add(cf.statuslist.Get(i, GIT_FILE).ToString());
			names = names << DeQtf(cf.statuslist.Get(i, GIT_FILE).ToString()) << "&";
		}
	}

	if (files.GetCount() > 0) {
		String prompt = "Discard all changes in the file(s)?&&" << names;
		if (PromptYesNo(prompt)) {
			Git::CancelCheckout(files);
			ProcessCommandOutput();
			ShowCommit();
		}
		GetStatusList();
	}
}

void GitCommit::CancelDelete()
{
	Vector<String> files;
	String names;

	for (int i = 0; i < cf.statuslist.GetCount(); ++i) {
		String status = cf.statuslist.Get(i, GIT_STATUS).ToString();
		if ((cf.statuslist.IsSelected(i) || (cf.statuslist.Get(i, 0) == 1))
		 && (status[0] == 'D')) {
			files.Add(cf.statuslist.Get(i, GIT_FILE).ToString());
			names = names << DeQtf(cf.statuslist.Get(i, GIT_FILE).ToString()) << "&";
		}
	}

	if (files.GetCount() > 0) {
		String prompt = "Cancel deletion of the file(s)?&&" << names;
		if (PromptYesNo(prompt)) {
			Git::Reset(files, true);
			ProcessCommandOutput();
			ShowCommit();
		}
		GetStatusList();
	}
}

void GitCommit::Unstage()
{
	Vector<String> files;
	String names;

	for (int i = 0; i < cf.statuslist.GetCount(); ++i) {
		String status = TrimBoth(cf.statuslist.Get(i, GIT_STATUS).ToString());
		if ((cf.statuslist.IsSelected(i) || (cf.statuslist.Get(i, 0) == 1))
		 && (status[0] == 'A' && status.GetCount() == 1)) {
			files.Add(cf.statuslist.Get(i, GIT_FILE).ToString());
			names = names << DeQtf(cf.statuslist.Get(i, GIT_FILE).ToString()) << "&";
		}
	}

	if (files.GetCount() > 0) {
		String prompt = "Unstage changes from the commit of the file(s)?&&" << names;
		if (PromptYesNo(prompt)) {
			Git::Unstage(files);
			ProcessCommandOutput();
			ShowCommit();
		}
		GetStatusList();
	}
}

void GitCommit::Reset()
{
	Vector<String> files;
	String names;

	for (int i = 0; i < cf.statuslist.GetCount(); ++i) {
		String status = cf.statuslist.Get(i, GIT_STATUS).ToString();
		if ((cf.statuslist.IsSelected(i) || (cf.statuslist.Get(i, 0) == 1))
		 && ((status[0] == 'U' && status[1] == 'U')
		  || (status[0] == 'A' && status[1] == 'A'))) {
			files.Add(cf.statuslist.Get(i, GIT_FILE).ToString());
			names = names << DeQtf(cf.statuslist.Get(i, GIT_FILE).ToString()) << "&";
		}
	}

	if (files.GetCount() > 0) {
		String prompt = "Reset unmerged changes of the file(s)?&&" << names;
		if (PromptYesNo(prompt)) {
			Git::Reset(files, false);
			ProcessCommandOutput();
			ShowCommit();
		}
		GetStatusList();
	}
}

void GitCommit::AddToRepository()
{
	Vector<String> files;
	String names;

	for (int i = 0; i < cf.statuslist.GetCount(); ++i) {
		String status = cf.statuslist.Get(i, GIT_STATUS).ToString();
		if ((cf.statuslist.IsSelected(i) || (cf.statuslist.Get(i, 0) == 1))
		 && ((status[0] == '?' || status[0] == 'U') || (status[0] == 'A' && status[1] == 'M')
		  || (status[0] == 'A' && status[1] == 'A'))) {
			files.Add(cf.statuslist.Get(i, GIT_FILE).ToString());
			names = names << DeQtf(cf.statuslist.Get(i, GIT_FILE).ToString()) << "&";
		}
	}

	if (files.GetCount() > 0) {
		String prompt = "Add the file(s) to the repository?&&" << names;
		if (PromptYesNo(prompt)) {
			Git::AddToRepository(files);
			ProcessCommandOutput();
			ShowCommit();
		}
		GetStatusList();
	}
}

void GitCommit::AddToGitignore()
{
	Vector<String> files;
	String names;

	for (int i = 0; i < cf.statuslist.GetCount(); ++i) {
		String status = cf.statuslist.Get(i, GIT_STATUS).ToString();
		if ((cf.statuslist.IsSelected(i) || (cf.statuslist.Get(i, 0) == 1))
		 && (status[0] == '?' || status[0] == 'U')) {
			files.Add(cf.statuslist.Get(i, GIT_FILE).ToString());
			names = names << DeQtf(cf.statuslist.Get(i, GIT_FILE).ToString()) << "&";
		}
	}

	if (files.GetCount() > 0) {
		String prompt = "Add the file(s) to the .gitignore file?&&" << names;
		if (PromptYesNo(prompt)) {
			Git::AddToGitignore(files);
			ProcessCommandOutput();
			ShowCommit();
		}
		GetStatusList();
	}
}

void GitCommit::DelFromRepository()
{
	Vector<String> files;
	String names;

	for (int i = 0; i < cf.statuslist.GetCount(); ++i) {
		String status = cf.statuslist.Get(i, GIT_STATUS).ToString();
		if ((cf.statuslist.IsSelected(i) || (cf.statuslist.Get(i, 0) == 1))
		 && (status[0] != '?')) {
			files.Add(cf.statuslist.Get(i, GIT_FILE).ToString());
			names = names << DeQtf(cf.statuslist.Get(i, GIT_FILE).ToString()) << "&";
		}
	}

	if (files.GetCount() > 0) {
		String prompt = "Delete the file(s) from the repository?&&" << names;
		if (PromptYesNo(prompt)) {
			Git::DelFromRepository(files);
			ProcessCommandOutput();
			ShowCommit();
		}
		GetStatusList();
	}
}

void GitCommit::DelFile()
{
	Vector<String> files;
	String names;

	for (int i = 0; i < cf.statuslist.GetCount(); ++i) {
		String status = cf.statuslist.Get(i, GIT_STATUS).ToString();
		if ((cf.statuslist.IsSelected(i) || (cf.statuslist.Get(i, 0) == 1))
		 && (status[0] == '?')) {
			files.Add(AppendFileName(GetGitDir(), cf.statuslist.Get(i, GIT_FILE).ToString()));
			names = names << DeQtf(AppendFileName(GetGitDir(), cf.statuslist.Get(i, GIT_FILE).ToString())) << "&";
		}
	}

	if (files.GetCount() > 0) {
		String prompt = "Delete the file(s)/dir(s) from the disk?&&" << names;
		if (PromptYesNo(prompt)) {
			for (int i = 0; i < files.GetCount(); ++i) {
				if (DirectoryExists(files[i]))
					DeleteFolderDeep(files[i]);
				else
					FileDelete(files[i]);
			}
		}
		GetStatusList();
	}
}

void GitCommit::GetFilePos(const String& filename, int filepos)
{
	WhenLeftDouble(AppendFileName(GetGitDir(), filename), filepos);
}

void GitCommit::ToggleDiffType()
{
	int type = cf.difftype.GetData();

	Vector<String> output = GetDiff(type).GetOutput();
	if (!GetCmdErrorCode() && output.GetCount() > 0)
		diffctrl.Set(output);
	else
		diffctrl.Clear();
}

void GitCommit::ToggleCmd()
{
	splitter.Zoom(cmdshow);
	splitter.SetFocus();
	WhenStatusChange();
}

void GitCommit::GetStatusList()
{
	DoRepositoryChange();

	allowcommit = false;
	cf.statuslist.Clear();

	Vector<String> list = GetStatus().GetOutput();
	if (!GetCmdErrorCode() && list.GetCount() > 0) {
		for (int i = 0; i < list.GetCount(); ++i) {
			String status = list[i].Left(2);
			String file = list[i].Mid(3);

			if (list[i][2] != ' ') {
				cf.statuslist.Add(0, "?", list[i]);
				continue;
			}

			if ((status[0] != '?') && (status[0] != '!')) {
				allowcommit = true;
			}

			if (file.Find("->") > 0) {
				Vector<String> file_split = Split(file, "->");
				cf.statuslist.Add(0, status, TrimBoth(file_split[1]));
			} else {
				cf.statuslist.Add(0, status, file);
			}
		}
	}
	cf.difftype.Enable(allowcommit);
}

void GitCommit::ProcessCommandOutput()
{
	Vector<String> output = GetOutput();
	if (output.GetCount() > 0) {
		for (int i = 0; i < output.GetCount(); ++i)
			cmd.Add(output[i]);

		cmdshow = true;
	}
	ToggleCmd();
	Sync();
}

void GitCommit::ShowCommit()
{
	if (GetGitDir().IsEmpty())
		return;

	if (IsMergePending()) {
		remotestatus = "Git merge in progress. You have to commit changes or abort the merge.";
		if (!IsGitDir(GetGitDir()))
			remotestatus = "";
		text.Hide();
	} else {
		if (GetCmdErrorCode() == 0)
			remotestatus = GetBranchStatus(GetBranchLocal(), GetBranchLocalRemote());
		text.Show();
	}

	GetStatusList();
	ToggleDiffType();
}

void GitCommit::ExecuteCommit(bool signoff)
{
	if (GetGitDir().IsEmpty())
		return;

	Vector<String> parameters;

	for (int i = 0; i < cf.statuslist.GetCount(); ++i) {
		if (cf.statuslist.Get(i, 0) == 1)
			parameters.Add(cf.statuslist.Get(i, GIT_FILE).ToString());
	}

	String message;
	if (signoff)
		message = Format("\nSigned-off-by: %s <%s>\n", GetGitConfig(true, "user.name"), GetGitConfig(true, "user.email"));

	if (parameters.IsEmpty()) {
		if (!IsCommitList()) {
			Exclamation("[= Commit canceled!&& There aren't files for the commit!]");
			return;
		} else {
			if (!IsMergePending()) {
				String prompt = "[= Commit all changes in the index (use -a parameter)?]";
				if(PromptYesNo(prompt))
					parameters.Add("-a");
				else
					return;
			} else {
				message = GetMergeMessage();
			}
		}
	}

	String msgpath;
	int errorcode = EditCommitMessage(message, parameters, msgpath);
	if (!errorcode)
		Commit(parameters, msgpath).ProcessCommandOutput();

	ShowCommit();
}

void GitCommit::SaveDiff()
{
	int type = cf.difftype.GetData();

	Vector<String> output = GetDiff(type).GetOutput();
	if (!GetCmdErrorCode() && output.GetCount() > 0) {
		String out;
		for (int i = 0; i < output.GetCount(); ++i)
			out << output[i] << "\n";
		SelectSaveFile("*.diff, *.patch\n*.*", out);
	} else
		SetOutput(output).ProcessCommandOutput();
}

// vim: ts=4
