#include "GitApp.h"

#include "GitBlame.h"
#include "GitCommit.h"
#include "GitCommitData.h"
#include "GitConfig.h"
#include "GitConfigBranch.h"
#include "GitFileHistory.h"
#include "GitHistory.h"
#include "GitMerge.h"

using namespace Upp;

GitApp::GitApp()
{
	CtrlLayout(*this);

	this->WhenClose = THISBACK(Close);
	close.WhenPush = THISBACK(Close);
	ok.Hide();

	AddFrame(menubar);
	AddFrame(toolbar);

	cmd.AddColumn("Command output");
	cmd.GridColor(SColorPaper());

	menubar.Set(THISBACK(MainMenu));
	toolbar.Set(THISBACK(GitMenu));

	Sizeable().Zoomable();
	BackPaint();
}

void GitApp::MainMenu(Bar& menu)
{
	menu.Add("Setup", THISBACK(GitSetup));
	menu.Add("GIT", THISBACK(GitMenu));
}

void GitApp::GitSetup(Bar& menu)
{
	menu.Add("Setup GIT directory", THISBACK1(SetGitDir, ""));
	menu.Add("Setup GIT file", THISBACK1(SetGitFile, ""));
}

void GitApp::GitMenu(Bar& menu)
{
	menu.Add("GIT init repository", GitImg::GitInit(), THISBACK(GitExecuteInitRepository))
		.Help("Initialize directory as a Git repository");
	menu.Separator();
	menu.Add("GIT config", GitImg::GitConfig(), THISBACK(GitExecuteConfig))
		.Help("Configure Git parameters");
	menu.Add("GIT history", GitImg::GitSearch(), THISBACK(GitExecuteHistory))
		.Help("Show GIT repository history");
	menu.Add("GIT update status", GitImg::GitStatus(), THISBACK(GitExecuteUpdate))
		.Help("Show difference status between local and remote branches");
	menu.Separator();
	menu.Add("GIT status", GitImg::GitStatus(), THISBACK(GitExecuteStatus))
		.Help("Status of the branch");
	menu.Add("GIT fetch", GitImg::GitFetch(), THISBACK(GitExecuteFetch))
		.Help("Fetch request");
	menu.Add("GIT merge", GitImg::GitMerge(), THISBACK(GitExecuteMerge))
		.Help("Merge repository ");
	menu.Add("GIT commit", GitImg::GitCommit(), THISBACK(GitExecuteCommit))
		.Help("Commit changes");
	menu.Add("GIT stash", GitImg::GitStash(), THISBACK(GitExecuteStash))
		.Help("Stash changes");
	menu.Add("GIT apply stash", GitImg::GitUnstash(), THISBACK(GitExecuteStashApply))
		.Help("Apply stashed changes");
	menu.Add("GIT push", GitImg::GitPush(), THISBACK(GitExecutePush))
		.Help("Push request");
	menu.Add("GIT branch", GitImg::GitBranch(), THISBACK(GitExecuteBranch))
		.Help("Manage / Create branches");
	menu.Separator();
	menu.Add("GIT diff", GitImg::GitDiff(), THISBACK(GitExecuteDiff))
		.Help("Show difference between current and last commit");
	menu.Add("GIT apply patch", GitImg::GitDiffApply(), THISBACK(GitExecutePatch))
		.Help("Apply GIT patch from the file");
	menu.Separator();
	menu.Add("GIT file history", GitImg::GitSearchHistory(), THISBACK(GitExecuteFileHistory))
	    .Help("Show GIT history of the file");
	menu.Add("GIT file blame", GitImg::GitSearchBlame(), THISBACK(GitExecuteBlame))
		.Help("Show what revision and author last modified each line of a file");
}

void GitApp::GitExecuteInitRepository()
{
	FileSel dir;
	dir.ActiveDir(GetHomeDirectory());
	dir.ExecuteSelectDir("Select directory for a GIT repository");

	String path = dir.Get();
	if (DirectoryExists(AppendFileName(path, ".git"))) {
		Exclamation(Format("Git repository already exists in the '%s'", path));
		return;
	}

	WithGitConfigDropListLayout<TopWindow> gitdir;
	CtrlLayout(gitdir, "Select directory for GIT init");
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
		Vector<String> tmp = git.InitRepository().GetOutput();
		for (int i = 0; i < tmp.GetCount(); ++i)
			cmd.Add(tmp[i]);
		if (git.GetCmdErrorCode())
			cmd.Add(Format("Initialization of the GIT repository in the directory '%s' failed!", path));
	}
}

void GitApp::GitExecuteConfig()
{
	GitConfiguration configuration;

	configuration.Init(gitdirs);
	configuration.Run();
}

void GitApp::GitExecuteStatus()
{
	gitdir.Clear();
	Git git(GetGitDir());

	if (git.GetGitDir().GetCount() > 0) {
		cmd.Clear();
		cmd.Add(Format("GIT status of the repository: %s", git.GetGitDir()));
		cmd.Sync();

		Vector<String> tmp = git.GetStatusVerbose().GetOutput();
		for (int i = 0; i < tmp.GetCount(); ++i)
			cmd.Add(tmp[i]);
		cmd.Add("Status DONE");
	}
}

void GitApp::GitExecuteFetch()
{
	Git git;

	cmd.Clear();
	for (int i = 0; i < gitdirs.GetCount(); ++i) {
		cmd.Add(Format("Fetching data from all repositories in the GIT directory: %s", gitdirs[i]));
		cmd.Sync();

		git.SetGitDir(gitdirs[i]);
		Vector<String> tmp = git.FetchAll().GetOutput();
		for (int j = 0; j < tmp.GetCount(); ++j)
			cmd.Add(tmp[j]);
		cmd.Add("Fetch DONE");
	}
}

void GitApp::GitExecuteMerge()
{
	GitMerging merging;

	merging.Init(gitdirs);
	merging.Run();
}

void GitApp::GitExecuteCommit()
{
	GitCommitting commiting;
	String filepath;

	commiting.WhenRepositoryChange = THISBACK(GitRepositoryChange);
	commiting.WhenLeftDouble = THISBACK(GitGetFilePos);
	commiting.Init(gitdirs, gitdirs.Top());
	commiting.Run();
}

void GitApp::GitStash()
{
	Exclamation("Stash");
}

void GitApp::GitRepositoryChange()
{
	Exclamation("Repository Change invoked");
}

void GitApp::GitGetFilePos(const String& filepath, int filepos)
{
	if (!filepath.IsEmpty())
		Exclamation(DeQtf(Format("File: %s, Pos: %d", filepath, filepos + 1)));
}

void GitApp::GitExecutePush()
{
	Git git;

	cmd.Clear();
	for (int i = 0; i < gitdirs.GetCount(); ++i) {
		cmd.Add(Format("Pushing data to all repositories in the GIT directory: %s", gitdirs[i]));
		cmd.Sync();

		git.SetGitDir(gitdirs[i]);
		Vector<String> tmp = git.PushAll().GetOutput();
		for (int j = 0; j < tmp.GetCount(); ++j)
			cmd.Add(tmp[j]);
		cmd.Add("Push DONE");
	}
}

void GitApp::GitExecuteBranch()
{
	GitBranches branches;

	branches.Init(gitdirs);
	branches.Run();
}

void GitApp::GitExecuteHistory()
{
	GitHistories history;

	history.Init(gitdirs);
	history.Run();
}

void GitApp::GitExecuteDiff()
{
	gitdir.Clear();
	Git git(GetGitDir());
	GitDiffCtrl diffOutput;

	Vector<String> diff = git.GetDiff(String("HEAD")).GetOutput();
	if (git.GetCmdErrorCode()) {
		Exclamation("Execution of GIT diff failed!");
		return;
	}
	diffOutput.Set(diff);

	WithGitSplitterLayout<TopWindow> gitDiff;
	CtrlLayout(gitDiff, "Show difference between current and last commit");
	gitDiff.splitter.Add(diffOutput);
	gitDiff.ok.Hide();
	gitDiff.status.Hide();
	gitDiff.Rejector(gitDiff.close, IDCANCEL);
	gitDiff.close.Cancel().SetLabel("Close");
	gitDiff.WhenClose = gitDiff.Rejector(IDCANCEL);

	gitDiff.RunAppModal();
}

void GitApp::GitExecutePatch()
{
	WithGitPatchLayout<TopWindow> dlg;
	CtrlLayout(dlg, "Select GIT repository and patch file");
	dlg.Acceptor(dlg.ok, IDOK);
	dlg.ok.Ok();
	dlg.Rejector(dlg.cancel, IDCANCEL);
	dlg.cancel.Cancel();

	for (int i = 0; i < gitdirs.GetCount(); ++i)
		dlg.gitdir.Add(gitdirs[i]);
	dlg.gitdir.GoBegin();

	OpenFileButton  openpatchfile;
	openpatchfile.Attach(dlg.patchfile);
	openpatchfile.Title(dlg.patchtext.GetText());

	int c = dlg.Run();
	if (c == IDOK) {
		if (IsGitDir(dlg.gitdir.GetData())) {
			Git git(GetGitRoot(dlg.gitdir.GetData()));
			if (git.IsPatchApplyPending())
				Exclamation("GIT apply patch is pending! Solve the problem first.");
			else {
				if (FileExists(dlg.patchfile.GetData().ToString())) {
					cmd.Clear();
					cmd.Add(Format("Applying GIT patch in the directory: %s", git.GetGitDir()));
					cmd.Sync();

					Vector<String> tmp = git.PatchApply(dlg.patchfile.GetData()).GetOutput();
					for (int i = 0; i < tmp.GetCount(); ++i)
						cmd.Add(tmp[i]);
					cmd.Add(Format("Git patch apply %s", git.GetCmdErrorCode() ? "failed" : "successful"));
				}
				else
					Exclamation(Format("File '%s' doesn't exist!", DeQtf(dlg.patchfile.GetData().ToString())));
			}
		} else
			Exclamation(Format("'%s' is not a GIT repository!", DeQtf(dlg.gitdir.GetData().ToString())));
	}
	openpatchfile.Detach();
}

void GitApp::GitExecuteStash()
{
	Git	git(GetGitDir());

	cmd.Clear();
	cmd.Add(Format("Stashing the changes in the directory: %s", git.GetGitDir()));
	cmd.Sync();

	Vector<String> tmp = git.Stash("").GetOutput();
	for (int i = 0; i < tmp.GetCount(); ++i)
		cmd.Add(tmp[i]);
	cmd.Add("Stash DONE");
}

void GitApp::GitExecuteStashApply()
{
	Git	git(GetGitDir());

	cmd.Clear();
	cmd.Add(Format("Applying stashed changes in the directory: %s", git.GetGitDir()));
	cmd.Sync();

	Vector<String> tmp = git.ProcessStashApply().GetOutput();
	for (int i = 0; i < tmp.GetCount(); ++i)
		cmd.Add(tmp[i]);
	cmd.Add("Stash apply DONE");
}

void GitApp::GitExecuteFileHistory()
{
	gitfile.Clear();
	SetGitFile();

	if (IsGitDir(gitfile)) {
		GitFileHistory filehistory;
		filehistory.ShowFileHistory(gitfile);
		filehistory.Execute();
	}
}

void GitApp::GitExecuteBlame()
{
	gitfile.Clear();
	SetGitFile();

	if (IsGitDir(gitfile)) {
		GitBlame git;
		git.ShowBlame(gitfile);
		git.Execute();
	}
}

void GitApp::GitExecuteUpdate()
{
	Git git;
	git.ProcessUpdateStatus(gitdirs);
}

void GitApp::SetGitDir(const String& path)
{
	if (DirectoryExists(path) && IsGitDir(path))
		gitdir = path;
	else
		SetGitDir();

	Title(gitdir);
}

void GitApp::SetGitDir()
{
	FileSel dir;

	dir.ActiveDir(GetHomeDirectory());
	dir.ExecuteSelectDir("Select a GIT repository");
	if (IsGitDir(dir.Get()))
		gitdir = dir.Get();
	else
		Exclamation(Format("'%s' does not contain a GIT repository!", DeQtf(dir.Get())));

	Title(gitdir);
}

String GitApp::GetGitDir()
{
	if (gitdir.IsEmpty())
		SetGitDir();

	return (gitdir);
}
 
void GitApp::SetGitFile(const String& filepath)
{
	if (FileExists(filepath))
		gitfile = filepath;
	else
		SetGitFile();

	GitExecuteFileHistory();
}

void GitApp::SetGitFile()
{
	FileSel file;

	file.ActiveDir(GetFileDirectory(gitfile));
	file.ExecuteOpen("Select a GIT file");
	if (IsGitDir(file.Get()))
		gitfile = file.Get();
	else
		Exclamation(Format("'%s' is not a part of a GIT repository!", DeQtf(file.Get())));
}

#ifdef flagMAIN
GUI_APP_MAIN
{
	if (!CheckGit())
		return;

	GitApp app;

	Index<String> dirs;

	String dir1 = GetHomeDirectory() << "/Develop/MyApps";
	dirs.Add(dir1);

	String dir2 = GetHomeDirectory() << "/Develop/upp_git.test";
	dirs.Add(dir2);

	app.SetGitDirs(dirs);
	app.Run();
}
#endif

// vim: ts=4
