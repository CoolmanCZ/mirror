#include "ide.h"

#include "ugit/GitBlame.h"
#include "ugit/GitCommit.h"
#include "ugit/GitCommitData.h"
#include "ugit/GitConfig.h"
#include "ugit/GitConfigBranch.h"
#include "ugit/GitHistory.h"
#include "ugit/GitFileHistory.h"
#include "ugit/GitMerge.h"

void Ide::ExecuteGitInitRepository()
{
	String path = GetFileDirectory(PackagePath(actualpackage));
	if (DirectoryExists(AppendFileName(path, ".git")))
		return;

	WithGitConfigDropListLayout<TopWindow> gitdir;
	CtrlLayout(gitdir, "Select a directory for GIT init");
	gitdir.Acceptor(gitdir.ok, IDOK);
	gitdir.Rejector(gitdir.cancel, IDCANCEL);
	gitdir.dir.Add(path).SetIndex(0);

	String path0;
	while (path != path0) {
		path0 = path;
		DirectoryUp(path);
		if (DirectoryExists(AppendFileName(path, ".git")))
			break;
		if (!path.IsEmpty())
			gitdir.dir.Add(path);
	}

	int c = gitdir.RunAppModal();
	if (c == IDOK) {
		path = gitdir.dir.Get().ToString();
		Git git(path);
		Vector<String> output = git.InitRepository().GetOutput();
		ProcessCommandOutput(output);
		if (git.GetCmdErrorCode())
			PutConsole(Format("Initialization of the GIT repository in the directory '%s' failed!", path));
		else
			ExecuteGitConfig();
		ScanWorkspace();
	}
}

void Ide::ExecuteGitConfig()
{
	Vector<String> dirs = GitDirs(true);
	static GitConfiguration configuration;

	if (configuration.IsOpen())
		configuration.SetFocus();
	else {
		configuration.Init(dirs, PackagePath(actualpackage));
		configuration.OpenMain();
	}
}

void Ide::ExecuteGitHistory()
{
	Vector<String> dirs = GitDirs(true);
	static GitHistories history;

	if (history.IsOpen())
		history.SetFocus();
	else {
		history.Init(dirs);
		history.OpenMain();
	}
	SyncGitBranchList();
}

void Ide::ExecuteGitStatus()
{
	Git git(GetGitRoot(PackagePath(actualpackage)));

	if (!git.GetGitDir().IsEmpty()) {
		ConsoleClear();
		ConsoleShow();
		PutConsole(Format("GIT status of the repository: %s", git.GetGitDir()));
		ConsoleSync();

		Vector<String> output = git.GetStatusVerbose().GetOutput();
		for (int i = 0; i < output.GetCount(); ++i) {
			PutConsole(output[i]);
		}
		PutConsole("Status DONE");
	}
}

void Ide::ExecuteGitFetch()
{
	Vector<String> output;
	Vector<String> dirs = GitDirs(true);
	Git git;

	ConsoleClear();
	ConsoleShow();
	for (int i = 0; i < dirs.GetCount(); ++i) {
		PutConsole(Format("Fetching data from all repositories in the GIT directory: %s", dirs[i]));
		ConsoleSync();

		output = git.SetGitDir(dirs[i]).FetchAll().GetOutput();
		for (int j = 0; j < output.GetCount(); ++j) {
			PutConsole(output[j]);
		}
		PutConsole("Fetch DONE");
	}
}

void Ide::ExecuteGitMerge()
{
	editor.CloseAssist();
	SaveFile();

	Vector<String> dirs = GitDirs(true);
	GitMerging merging;

	merging.WhenStash = THISBACK(ScanWorkspace);
	merging.Init(dirs);
	merging.Run();
}

void Ide::ExecuteGitCommit()
{
	editor.CloseAssist();
	SaveFile();

	Vector<String> dirs = GitDirs(true);
	GitCommitting commiting;

	commiting.WhenStash = THISBACK(ScanWorkspace);
	commiting.WhenRepositoryChange = THISBACK(ScanWorkspace);
	commiting.WhenLeftDouble = THISBACK(SetGitFilePos);
	commiting.Init(dirs, PackagePath(actualpackage));
	commiting.Run();
}

void Ide::SetGitFilePos(String filename, int filepos)
{
	if ((filename.GetCount() > 0) && (filepos > 0)) {
		EditFile(filename);
		editor.SetCursor(editor.GetPos(filepos));
		editor.SetFocus();
	}
}

void Ide::ExecuteGitStash()
{
	editor.CloseAssist();
	SaveFile();

	Git git(GetGitRoot(PackagePath(actualpackage)));

	ConsoleClear();
	ConsoleShow();
	PutConsole("Stashing the changes");
	ConsoleSync();

	Vector<String> output = git.Stash("").GetOutput();
	for (int i = 0; i < output.GetCount(); ++i) {
		PutConsole(output[i]);
	}
	PutConsole("Stash DONE");
	ScanWorkspace();
}

void Ide::ExecuteGitStashApply()
{
	Git git(GetGitRoot(PackagePath(actualpackage)));

	ConsoleClear();
	ConsoleShow();
	PutConsole("Applying stashed changes");
	ConsoleSync();

	Vector<String> output = git.ProcessStashApply().GetOutput();
	for (int i = 0; i < output.GetCount(); ++i) {
		PutConsole(output[i]);
	}
	PutConsole("Stash apply DONE");
	ScanWorkspace();
}

void Ide::ExecuteGitPush()
{
	Vector<String> output;
	Vector<String> dirs = GitDirs(true);
	Git git;

	ConsoleClear();
	ConsoleShow();
	for (int i = 0; i < dirs.GetCount(); ++i) {
		PutConsole(Format("Pushing data to all repositories in the directory: %s", dirs[i]));
		ConsoleSync();

		output = git.SetGitDir(dirs[i]).PushAll().GetOutput();
		for (int j = 0; j < output.GetCount(); ++j) {
			PutConsole(output[j]);
		}
		PutConsole("Push DONE");
	}
}

void Ide::ExecuteGitFileHistory()
{
	if(IsNull(editfile))
		return;

	static GitFileHistory filehistory;

	if (filehistory.IsOpen())
		filehistory.Close();

	filehistory.ShowFileHistory(editfile);
	filehistory.OpenMain();
}

void Ide::ExecuteGitBlame()
{
	if(IsNull(editfile))
		return;

	static GitBlame blame;

	if (blame.IsOpen())
		blame.Close();

	blame.ShowBlame(editfile);
	blame.OpenMain();
}

void Ide::ExecuteGitUpdateStatus()
{
	Vector<String> dirs = GitDirs(true);
	Git git;

	git.ProcessUpdateStatus(dirs);
}

void Ide::ExecuteGitPatchApply()
{
	WithGitPatchLayout<TopWindow> dlg;
	CtrlLayout(dlg, "Select a GIT repository and a patch file");
	dlg.Acceptor(dlg.ok, IDOK);
	dlg.ok.Ok();
	dlg.Rejector(dlg.cancel, IDCANCEL);
	dlg.cancel.Cancel();

	Vector<String> dirs = GitDirs(true);
	for (int i = 0; i < dirs.GetCount(); ++i)
		dlg.gitdir.Add(dirs[i]);
	dlg.gitdir.GoBegin();

	OpenFileButton  openpatchfile;
	openpatchfile.Attach(dlg.patchfile);
	openpatchfile.Title(dlg.patchtext.GetText());

	int c = dlg.RunAppModal();
	if (c == IDOK) {
		Vector<String> output;
		if (IsGitDir(dlg.gitdir.GetData())) {
			Git git(GetGitRoot(dlg.gitdir.GetData()));
			if (git.IsPatchApplyPending())
				output.Add("GIT patch is pending! Solve the problem first.");
			else {
				if (FileExists(dlg.patchfile.GetData().ToString())) {
					output.Add(Format("Applying GIT patch in the directory: %s", git.GetGitDir()));
					output.Append(git.PatchApply(dlg.patchfile.GetData()).GetOutput());
					output.Add(Format("GIT patch apply %s", git.GetCmdErrorCode() ? "failed" : "successful"));
				}
				else
					output.Add(Format("File '%s' doesn't exist!", DeQtf(dlg.patchfile.GetData().ToString())));
			}
		} else
			output.Add(Format("'%s' is not a GIT repository!", DeQtf(dlg.gitdir.GetData().ToString())));
		ProcessCommandOutput(output);
	}
	openpatchfile.Detach();
	SyncGitBranchList();
}

void Ide::ExecuteGitPatchAbort()
{
	Vector<String> output;

	WithGitConfigDropListLayout<TopWindow> gitdir;
	CtrlLayout(gitdir, "Select a directory for GIT patch abort");
	gitdir.Acceptor(gitdir.ok, IDOK);
	gitdir.Rejector(gitdir.cancel, IDCANCEL);

	Vector<String> dirs = GitDirs(true);
	for (int i = 0; i < dirs.GetCount(); ++i) {
		Git git(dirs[i]);
		if (git.IsPatchApplyPending())
			gitdir.dir.Add(dirs[i]);
	}
	gitdir.dir.GoBegin();

	if (gitdir.dir.GetCount() > 0) {
		int c = gitdir.RunAppModal();
		if (c == IDOK) {
			Git git(gitdir.dir.GetData());
			if (git.IsPatchApplyPending()) {
				output.Append(git.PatchAbort().GetOutput());
				output.Add("Patch abort DONE");
			}
			else
				output.Add(Format("There is not a pending GIT patch in the directory '%s'", git.GetGitDir()));
		}
		ProcessCommandOutput(output);
		SyncGitBranchList();
	}
}

void Ide::ExecuteGitConfigBranch()
{
	editor.CloseAssist();
	SaveFile();

	GitBranches branches;
	Vector<String> dirs = GitDirs(true);

	branches.Init(dirs);
	branches.Run();
	SyncGitBranchList();
}

void Ide::SyncGitBranchList()
{
	gitbranchlist.Clear();

	String gitdir = GetGitRoot(editfile);
	if (gitdir.IsEmpty())
		gitdir = GetGitRoot(PackagePath(actualpackage));

	Git git(gitdir);
	Vector<String> list = git.GetBranchAll().GetOutput();
	if (!git.GetCmdErrorCode() && list.GetCount() > 0) {
		for(int i = 0; i < list.GetCount(); ++i) {
			gitbranchlist.Add(list[i]);
			if (list[i].Find('*') >= 0)
				gitbranchlist = list[i];
		}
	}
	patchpending = git.IsPatchApplyPending();
}

void Ide::OnGitBranchList()
{
	String branch = ~gitbranchlist;

	if (branch.Find("*") < 0) {
		Git git(GetGitRoot(PackagePath(actualpackage)));

		if (CheckGit()) {
			Vector<String> output = git.BranchCheckout(TrimBoth(branch)).GetOutput();
			ProcessCommandOutput(output);
			SyncGitBranchList();
		}
	}
}

void Ide::SetGitBranchList()
{
	Git git(GetGitRoot(PackagePath(actualpackage)));

	if (CheckGit()) {
		Vector<String> output = git.ProcessBranchAdd(git.GetBranchLocal()).GetOutput();
		ProcessCommandOutput(output);
		SyncGitBranchList();
	}
}

void Ide::ProcessCommandOutput(const Vector<String>& output)
{
	if (output.GetCount() > 0) {
		ConsoleClear();
		ConsoleShow();
		for (int j = 0; j < output.GetCount(); ++j)
			PutConsole(output[j]);
	}
}