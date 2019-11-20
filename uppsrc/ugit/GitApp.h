#ifndef _ugit_GitApp_h_
#define _ugit_GitApp_h_

#include <ugit/Git.h>

struct GitApp : Upp::WithGitCmdOutputLayout<Upp::TopWindow> {
  private:
	Upp::MenuBar menubar;
	Upp::ToolBar toolbar;

	Upp::Vector<Upp::String> gitdirs;
	Upp::String gitdir;
	Upp::String gitfile;

	void MainMenu(Upp::Bar& bar);
	void GitSetup(Upp::Bar& menu);
	void GitMenu(Upp::Bar& menu);
	void GitExecuteInitRepository();
	void GitExecuteConfig();
	void GitExecuteStatus();
	void GitExecuteHistory();
	void GitExecuteCommit();
	void GitExecuteFetch();
	void GitExecuteMerge();
	void GitExecutePush();
	void GitExecuteBranch();
	void GitExecuteDiff();
	void GitExecutePatch();
	void GitExecuteStash();
	void GitExecuteStashApply();
	void GitExecuteFileHistory();
	void GitExecuteBlame();
	void GitExecuteUpdate();

	void GitStash();
	void GitRepositoryChange();
	void GitGetFilePos(const Upp::String& filename, int filepos);

  public:
	typedef GitApp CLASSNAME;

	void SetGitDir(const Upp::String& path);
	void SetGitDir();
	Upp::String GetGitDir();
	void SetGitFile(const Upp::String& filepath);
	void SetGitFile();
	void SetGitDirs(Upp::Index<Upp::String>& dirs) { gitdirs = dirs.PickKeys(); };

	GitApp();
};

#endif

// vim: ts=4
